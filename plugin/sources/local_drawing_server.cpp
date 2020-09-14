#include "local_drawing_server.h"

LocalDrawingServer::LocalDrawingServer(QObject* parent) :
  DrawingServer(parent) {
}

void LocalDrawingServer::registerClient(DrawingClient* client) {
	if (client == nullptr) {
		qWarning("FILE %s FUNCTION %s LINE %d\nERROR: Client is nullptr!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	} else {
		DrawingClient* lastAddedClient = clients.emplace_back(client);

		QObject::connect(lastAddedClient, &DrawingClient::sendEventToServer, this, &LocalDrawingServer::receiveEvent);
		QObject::connect(lastAddedClient, &DrawingClient::disconnected, this, &LocalDrawingServer::deregisterClient);

		qDebug().noquote().nospace() << "Client " << lastAddedClient << " registered.";
	}
}

void LocalDrawingServer::deregisterClient() {
}

bool LocalDrawingServer::receiveEvent(const NetworkEvent& event) {
	qDebug().noquote().nospace() << "[" << QObject::sender() << "]: " << event.serialize() << "\n";

	switch (event.type) {
	case NetworkEvent::Type::AddObject: {
		const AddObject& e = static_cast<const AddObject&>(event);
		Shape* lastAddedShape = shapes.emplace_back(e.getShape()).get();
		notifyClients(AddObject(lastAddedShape->clone()));
		break;
	}

	case NetworkEvent::Type::DeleteShape: {
		const DeleteShape& e = static_cast<const DeleteShape&>(event);
        QUuid shapeUuid = e.getShapeUuid();

        auto result = std::find_if(shapes.begin(), shapes.end(), [&shapeUuid](const std::unique_ptr<Shape>& shape) {
            return shape->getShapeUuid() == shapeUuid;
        });

        if (result == shapes.end()) {
            qDebug().noquote().nospace() << "No shape with Uuid: {" << shapeUuid.toString() << "}. Nothing was deleted.";
            return false;
        }

        shapes.erase(result);

        notifyClients(DeleteShape(shapeUuid));

		break;
	}

	case NetworkEvent::Type::ClearAll: {
        shapes.clear();
		notifyClients(ClearAll());
		break;
	}

	case NetworkEvent::Type::ShapeModified: {
		const ShapeModified& e = static_cast<const ShapeModified&>(event);
		QUuid shapeUuid = e.getShapeUuid();

		auto result = std::find_if(shapes.begin(), shapes.end(), [&shapeUuid](const std::unique_ptr<Shape>& shape) {
			return shapeUuid == shape->getShapeUuid();
		});

		if (result == shapes.end()) {
			qDebug().noquote().nospace() << "No shape with UUID: {" << shapeUuid.toString() << "}.";
		}

		(*result)->setShapeStyle(e.getShapeStyle());
		(*result)->setShapePosition(e.getShapePosition());

		notifyClients(e);

		break;
	}
	case NetworkEvent::Type::MoveShape: {
		const MoveShape& e = static_cast<const MoveShape&>(event);
		notifyClients(MoveShape(e.shapeUuid, e.move));
		break;
	}

	default: { break; }
	}

	return true;
}

bool LocalDrawingServer::processEvent(const NetworkEvent& event) {
	Q_UNUSED(event);
	return true;
}

void LocalDrawingServer::notifyClients(const NetworkEvent& event) {
	for (auto& client : clients) {
		client->onReceiveEventFromServer(event);
	}
}
