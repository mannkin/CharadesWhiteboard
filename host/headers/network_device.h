#ifndef NETWORK_DEVICE_H
#define NETWORK_DEVICE_H

#include "network_interface.h"
#include <memory>

#include <QTcpServer>
#include <QTcpSocket>

class ConnectionHandle : public ConnectionHandleInterface {
	Q_OBJECT

public:
	explicit ConnectionHandle(QTcpSocket* socket);
	virtual ~ConnectionHandle() override = default;

	virtual std::vector<std::uint8_t> readData() const override;
	virtual void writeData(const std::vector<std::uint8_t>& data) const override;
	virtual void disconnect() const override;

private:
	bool isSocketValid() const;

private:
	QTcpSocket* socket = nullptr;
};

class NetworkDevice : public NetworkDeviceInterface {
	Q_OBJECT
	Q_PROPERTY(int activeConnections READ getActiveConnections NOTIFY activeConnectionsChanged)

public:
	explicit NetworkDevice();
	NetworkDevice(const QHostAddress& hostAddress, quint16 port);
	virtual ~NetworkDevice() override = default;

	virtual bool listen(const QHostAddress& hostAddress, quint16 port) override;
	virtual bool closeServer() override;
	virtual bool isListening() const override;
	virtual bool connect(const QHostAddress& hostAddress, quint16 port) override;
	Q_INVOKABLE virtual bool connect(const QString& hostAddressAndPort) override;
	Q_INVOKABLE virtual bool disconnectAll() override;

	int getActiveConnections() const;

signals:
	void activeConnectionsChanged() const;

private slots:
	void onNewConnectionPending();
	void onDisconnected(QTcpSocket* socket);

private:
	struct QObjectDeleter {
		inline void operator()(QObject* object) {
			if (object != nullptr) {
				object->deleteLater();
				qDebug().noquote().nospace() << "Deleting " << object << "...";
			} else {
				qFatal("Attempted to delete non-existent QObject! Aborting...");
			}
		}
	};

private:
	std::unique_ptr<QTcpServer, QObjectDeleter> server = nullptr;
	std::vector<std::unique_ptr<QTcpSocket, QObjectDeleter>> activeConnections;
};

#endif // NETWORK_DEVICE_H
