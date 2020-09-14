#include "network_device.h"

#include <QAbstractSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtGlobal>

ConnectionHandle::ConnectionHandle(QTcpSocket* socket) :
  socket(socket) {
	if (socket == nullptr) {
		qFatal("[FATAL ERROR]: New \"ConnectionHandle\" socket cannot be null pointer!");
	}

	QObject::connect(this->socket, &QTcpSocket::disconnected, this, &ConnectionHandle::disconnected);
	QObject::connect(this->socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ConnectionHandle::error);
	QObject::connect(this->socket, &QTcpSocket::readyRead, this, &ConnectionHandle::dataReady);
	QObject::connect(this->socket, &QTcpSocket::bytesWritten, this, &ConnectionHandle::bytesWritten);
}

std::vector<uint8_t> ConnectionHandle::readData() const {
	if (!isSocketValid()) {
		qFatal("[FATAL ERROR]: Cannot read data from null socket!");
	}

	std::vector<std::uint8_t> dataRead;
	std::int64_t bytesReadLastTime = 0;
	std::uint8_t buffer[1024] = {0};

	while ((bytesReadLastTime = socket->read(reinterpret_cast<char*>(buffer), 1024)) > 0) {
		dataRead.insert(dataRead.end(), buffer, buffer + bytesReadLastTime);
	}

	if (bytesReadLastTime == -1) {
		qFatal("Error occured while trying to read incoming data!");
	}

	return dataRead;
}

void ConnectionHandle::writeData(const std::vector<uint8_t>& data) const {
	if (!isSocketValid()) {
		qFatal("[FATAL ERROR]: Cannot write data to null socket!");
	}

	std::int64_t bytesWrittenToSocket = socket->write(reinterpret_cast<const char*>(data.data()), data.size());
	socket->waitForBytesWritten();

	if (bytesWrittenToSocket == -1) {
		qFatal("Error occured while trying to write data to socket");
	}

	emit bytesWritten(bytesWrittenToSocket);
}

void ConnectionHandle::disconnect() const {
	if (!isSocketValid()) {
		qFatal("Cannot disconnect null socket!");
	}

	socket->disconnectFromHost();
	if (socket->state() != QAbstractSocket::SocketState::UnconnectedState) {
		socket->waitForDisconnected();
	}

	emit disconnected();
}

bool ConnectionHandle::isSocketValid() const {
	return (socket != nullptr);
}

NetworkDevice::NetworkDevice() {
	try {
		server = std::unique_ptr<QTcpServer, QObjectDeleter>(new QTcpServer(nullptr));
	} catch (const std::bad_alloc& e) {
		qFatal("[EXCEPTION]: %s", e.what());
	}

	QObject::connect(server.get(), &QTcpServer::newConnection, this, &NetworkDevice::onNewConnectionPending);
}

NetworkDevice::NetworkDevice(const QHostAddress& hostAddress, quint16 port) :
  NetworkDevice() {
	listen(hostAddress, port);
}

bool NetworkDevice::listen(const QHostAddress& hostAddress, quint16 port) {
	if (server == nullptr) {
		qFatal("Cannot listen on null server!");
	}

	server->close();
	if (!server->listen(hostAddress, port)) {
		qDebug().noquote().nospace() << "Error while trying to listen on " << hostAddress.toString().toLatin1().constData() << ":" << port << "!";
		qDebug().noquote().nospace() << "No incoming connections will be handled.";
		return false;
	}

	qDebug().noquote().nospace() << "Server is now listening for pending connections on " << hostAddress.toString() << ":" << port << ".";
	return true;
}

bool NetworkDevice::closeServer() {
	if (server == nullptr) {
		qFatal("[FATAL ERROR]: Cannot stop null server!");
	}

	server->close();

	return true;
}

bool NetworkDevice::isListening() const {
	if (server == nullptr) {
		qFatal("[FATAL ERROR]: Cannot check if null server is listening!");
	}

	return server->isListening();
}

bool NetworkDevice::connect(const QHostAddress& hostAddress, quint16 port) {
	try {
		std::unique_ptr<QTcpSocket, QObjectDeleter> socket(new QTcpSocket(nullptr));

		qDebug().noquote().nospace() << "Connecting to " << hostAddress.toString() << ":" << port;
		socket->connectToHost(hostAddress.toString(), port, QIODevice::ReadWrite, QAbstractSocket::NetworkLayerProtocol::AnyIPProtocol);
		if (!socket->waitForConnected()) {
			qDebug().noquote().nospace() << "Error while attempting to connect!";
			return false;
		}

		QTcpSocket* lastAddedSocket = activeConnections.emplace_back(std::move(socket)).get();
		QObject::connect(lastAddedSocket, &QTcpSocket::disconnected, [this, lastAddedSocket]() {
			onDisconnected(lastAddedSocket);
		});

		qDebug().noquote().nospace() << "New connection estabilished - " << lastAddedSocket;

		emit newConnectionOutgoing(new ConnectionHandle(lastAddedSocket));
		emit activeConnectionsChanged();
	} catch (const std::bad_alloc& e) {
		qFatal("[EXCEPTION]: %s", e.what());
	}

	return true;
}

bool NetworkDevice::connect(const QString& hostAddressAndPort) {
	// TODO: Change hard-coded port number
	return connect(QHostAddress(hostAddressAndPort), 55555);
}

bool NetworkDevice::disconnectAll() {
	while (!activeConnections.empty()) {
		activeConnections.back()->disconnectFromHost();
	}

	emit disconnectedAll();

	return true;
}

int NetworkDevice::getActiveConnections() const {
	return activeConnections.size();
}

void NetworkDevice::onNewConnectionPending() {
	if (server == nullptr) {
		qFatal("[FATAL ERROR]: Cannot accept new connection when server is null!");
	}

	if (server->hasPendingConnections()) {
		std::unique_ptr<QTcpSocket, QObjectDeleter> socket(server->nextPendingConnection());

		QTcpSocket* lastAddedSocket = activeConnections.emplace_back(std::move(socket)).get();
		QObject::connect(lastAddedSocket, &QTcpSocket::disconnected, [this, lastAddedSocket]() {
			onDisconnected(lastAddedSocket);
		});

		qDebug().noquote().nospace() << "New connection estabilished - " << lastAddedSocket;

		emit newConnectionIncoming(new ConnectionHandle(lastAddedSocket));
		emit activeConnectionsChanged();
	}
}

void NetworkDevice::onDisconnected(QTcpSocket* socket) {
	if (socket == nullptr) {
		qFatal("[FATAL ERROR]: Cannot handle disconnected null socket!");
	}

	auto result = std::find_if(activeConnections.begin(), activeConnections.end(), [socket](const std::unique_ptr<QTcpSocket, QObjectDeleter>& currentSocket) {
		return (currentSocket.get() == socket);
	});

	if (result == activeConnections.end()) {
		qFatal("[FATAL ERROR]: No socket to delete found!");
	}

	activeConnections.erase(result);
	emit activeConnectionsChanged();

	if (activeConnections.size() == 0) {
		emit disconnectAll();
	}

	qDebug().noquote().nospace() << "Connection closed.";
}
