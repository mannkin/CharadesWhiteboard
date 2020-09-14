#ifndef LOCALDRAWINGSERVER_H
#define LOCALDRAWINGSERVER_H

#include "drawing_client.h"
#include "drawing_server.h"
#include "shape.h"
#include <QObject>
#include <QTcpSocket>
#include <memory>

class LocalDrawingServer : public DrawingServer {
	Q_OBJECT
public:
	explicit LocalDrawingServer(QObject* parent = nullptr);
	~LocalDrawingServer() override = default;

	void registerClient(DrawingClient* client) override;

public slots:
	bool receiveEvent(const NetworkEvent& event) override;
	bool processEvent(const NetworkEvent& event) override;
	void notifyClients(const NetworkEvent& event) override;
	void deregisterClient() override;

private:
	std::vector<std::unique_ptr<Shape>> shapes;
	std::vector<DrawingClient*> clients;
};
#endif // LOCALDRAWINGSERVER_H
