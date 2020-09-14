#ifndef VIRTUALDRAWINGSERVER_H
#define VIRTUALDRAWINGSERVER_H

#include "drawing_server.h"
#include "network_interface.h"
#include <QDataStream>
#include <QTcpSocket>

class VirtualDrawingServer : public DrawingServer {
public:
	VirtualDrawingServer(std::unique_ptr<ConnectionHandleInterface> connectionHandle, QObject* parent = nullptr);
	~VirtualDrawingServer() override = default;
	void registerClient(DrawingClient* client) override;

public slots:
	bool receiveEvent(const NetworkEvent& event) override;
	bool processEvent(const NetworkEvent& event) override;
	void notifyClients(const NetworkEvent& event) override;
	void deregisterClient() override;
	void onNewDataAvailable();

private:
	std::unique_ptr<ConnectionHandleInterface> connectionHandle;
	DrawingClient* client;
};

#endif // VIRTUALDRAWINGSERVER_H
