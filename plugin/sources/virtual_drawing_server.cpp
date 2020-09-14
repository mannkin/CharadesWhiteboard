#include "virtual_drawing_server.h"
#include <QByteArray>

VirtualDrawingServer::VirtualDrawingServer(std::unique_ptr<ConnectionHandleInterface> connection, QObject* parent) :
  DrawingServer(parent),
  connectionHandle(std::move(connection)) {
	QObject::connect(connectionHandle.get(), &ConnectionHandleInterface::disconnected, this, &VirtualDrawingServer::deregisterClient);
	QObject::connect(connectionHandle.get(), &ConnectionHandleInterface::dataReady, this, &VirtualDrawingServer::onNewDataAvailable);
}

void VirtualDrawingServer::registerClient(DrawingClient* client) {
	if (client == nullptr) {
		qWarning("FILE %s FUNCTION %s LINE %d\nERROR: Client is nullptr!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	} else {

		this->client = client;

		QObject::connect(this->client, &DrawingClient::sendEventToServer, this, &VirtualDrawingServer::receiveEvent);
		qDebug() << "Client registered." << this->client;
	}
}

bool VirtualDrawingServer::receiveEvent(const NetworkEvent& event) {
	QByteArray serializedEvent = event.serialize();
	std::vector<std::uint8_t> rawEventData(serializedEvent.begin(), serializedEvent.end());
	connectionHandle->writeData(rawEventData);

	return true;
}

bool VirtualDrawingServer::processEvent(const NetworkEvent& event) {
	Q_UNUSED(event)
	return true;
}

void VirtualDrawingServer::notifyClients(const NetworkEvent& event) {
	client->onReceiveEventFromServer(event);
}

void VirtualDrawingServer::deregisterClient() {
	//emit resetServer();
}

void VirtualDrawingServer::onNewDataAvailable() {
	static std::vector<std::uint8_t> buffer;
	QJsonDocument json;

	std::vector<std::uint8_t> dataRead = connectionHandle->readData();
	buffer.insert(buffer.end(), dataRead.begin(), dataRead.end());

	if (!((json = QJsonDocument::fromJson(QByteArray::fromRawData(reinterpret_cast<const char*>(buffer.data()), buffer.size()))).isNull())) {
		QString eventType = json.object()["EventType"].toString();

		if (eventType == "AddObject") {
			AddObject event(nullptr);
			event.deserialize(json.toJson(QJsonDocument::Compact));
			notifyClients(event);
		}

		if (eventType == "DeleteShape") {
			DeleteShape event;
			event.deserialize(json.toJson(QJsonDocument::Compact));
			notifyClients(event);
		}

		if (eventType == "ClearAll") {
			notifyClients(ClearAll());
		}

		if (eventType == "ShapeModified") {
			ShapeModified event;
			event.deserialize(json.toJson(QJsonDocument::Compact));
			notifyClients(event);
		}

		if (eventType == "MoveShape") {
			MoveShape event;
			event.deserialize(json.toJson(QJsonDocument::Compact));
			notifyClients(event);
		}

		buffer.clear();
	}
}
