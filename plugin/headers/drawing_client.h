#ifndef DRAWINGCLIENT_H
#define DRAWINGCLIENT_H

#include <QObject>

#include "network_event.h"

class DrawingClient : public QObject {
	Q_OBJECT
public:
	DrawingClient(QObject* parent = nullptr) :
	  QObject(parent) {
	}
	virtual ~DrawingClient() = default;

signals:
	void sendEventToServer(const NetworkEvent& event);
	void disconnected();

public slots:
	virtual void onReceiveEventFromServer(const NetworkEvent& event) = 0;
	virtual void onSendEventToServer(const NetworkEvent& event) = 0;
};

#endif // DRAWINGCLIENT_H
