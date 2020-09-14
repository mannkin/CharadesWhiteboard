#include "drawing_server.h"

DrawingServer::DrawingServer(QObject* parent) :
  QObject(parent) {
}

void DrawingServer::registerClient(DrawingClient* client) {
	if (client == nullptr) {
		qFatal("Dupa");
	}

	RegisteredClient* lastAddedClient = clients.emplace_back(std::make_unique<RegisteredClient>(client, clients.size())).get();

	QObject::connect(lastAddedClient->client, &DrawingClient::sendEventToServer, this, &DrawingServer::receiveEvent);
	QObject::connect(lastAddedClient->client, &DrawingClient::disconnected, this, &DrawingServer::removeClient);
}

bool DrawingServer::receiveEvent(const EventBase& event) {
	qDebug().noquote().nospace() << event.serialize();

	return true;
}

bool DrawingServer::processEvent(Event event) {
	Q_UNUSED(event);
	return true;
}

void DrawingServer::removeClient() {
	/*auto del = sender();

	clients.erase(std::remove(clients.begin(), clients.end(), del), clients.end());

	qDebug() << "Client removed.";*/
}

DrawingServer::RegisteredClient::RegisteredClient(DrawingClient* client, quint8 id) :
  client(client),
  id(id) {
	qDebug().noquote().nospace() << "New client connected: {client: " << client << ", id: " << id << "}";
}
