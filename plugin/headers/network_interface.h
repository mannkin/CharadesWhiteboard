#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <QHostAddress>
#include <QObject>
#include <memory>

class ConnectionHandleInterface : public QObject {
	Q_OBJECT

public:
	virtual ~ConnectionHandleInterface() = default;
	virtual std::vector<std::uint8_t> readData() const = 0;
	virtual void writeData(const std::vector<std::uint8_t>& data) const = 0;
	virtual void disconnect() const = 0;

signals:
	void disconnected() const;
	void error() const;
	void dataReady() const;
	void bytesWritten(std::size_t bytesWritten) const;
};

class NetworkDeviceInterface : public QObject {
	Q_OBJECT

public:
	virtual ~NetworkDeviceInterface() = default;
	virtual bool listen(const QHostAddress& hostAddress, quint16 port) = 0;
	virtual bool closeServer() = 0;
	virtual bool isListening() const = 0;
	virtual bool connect(const QHostAddress& hostAddress, quint16 port) = 0;
	virtual bool connect(const QString& hostAddressAndPort) = 0;
	virtual bool disconnectAll() = 0;

signals:
	void newConnectionIncoming(ConnectionHandleInterface* connectionHandle);
	void newConnectionOutgoing(ConnectionHandleInterface* connectionHandle);
	void disconnectedAll();
};

#endif // NETWORK_INTERFACE_H
