#ifndef DRAWINGSERVER_H
#define DRAWINGSERVER_H

#include "drawing_client.h"
#include <QObject>
#include <memory>

class DrawingServer : public QObject {
	Q_OBJECT
public:
	DrawingServer(QObject* parent = nullptr) :
	  QObject(parent) {
	}
	virtual ~DrawingServer() = default;
	virtual void registerClient(DrawingClient* client) = 0;

public slots:
	virtual bool receiveEvent(const NetworkEvent& event) = 0;
	virtual bool processEvent(const NetworkEvent& event) = 0;
	virtual void notifyClients(const NetworkEvent& event) = 0;
	virtual void deregisterClient() = 0;

signals:
	void resetServer();
};
#endif // DRAWINGSERVER_H
