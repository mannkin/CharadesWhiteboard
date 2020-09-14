#ifndef VIRTUAL_DRAWING_CLIENT_H
#define VIRTUAL_DRAWING_CLIENT_H

#include "drawing_client.h"
#include "network_interface.h"
#include <QDataStream>
#include <QObject>
#include <QTcpSocket>

class VirtualDrawingClient : public DrawingClient {
	Q_OBJECT
public:
	VirtualDrawingClient(std::unique_ptr<ConnectionHandleInterface> connectionHandle, QObject* parent = nullptr);
	~VirtualDrawingClient() override;

public slots:
	void onReceiveEventFromServer(const NetworkEvent& event) override;
	void onSendEventToServer(const NetworkEvent& event) override;
	void onNewDataAvailable();

private:
	std::unique_ptr<ConnectionHandleInterface> connectionHandle;
};

#endif // VIRTUAL_DRAWING_CLIENT_H
