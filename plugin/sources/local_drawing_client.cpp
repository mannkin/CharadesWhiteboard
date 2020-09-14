#include "local_drawing_client.h"
#include <QDebug>
#include <algorithm>

LocalDrawingClient::LocalDrawingClient(QObject* parent) :
  DrawingClient(parent) {
	QObject::connect(this, &LocalDrawingClient::sendEventToServer, this, &LocalDrawingClient::onSendEventToServer);
}

LocalDrawingClient::~LocalDrawingClient() {
}

std::unique_ptr<Shape>& LocalDrawingClient::getPreview() {
	return shapePreview;
}

void LocalDrawingClient::setPreview(std::unique_ptr<Shape> preview) {
	shapePreview = std::move(preview);
	emit requestUpdate();
}

void LocalDrawingClient::movePreviewToShapesWaitingToAccept() {
	if (shapePreview != nullptr) {
		Shape* lastAddedShape = shapesWaitingToAccept.emplace_back(std::move(shapePreview)).get();
		emit requestUpdate();
		emit sendEventToServer(AddObject(lastAddedShape->clone()));
	}
}

std::deque<std::unique_ptr<Shape>>& LocalDrawingClient::getShapesWaitingToAccept() {
	return shapesWaitingToAccept;
}

std::vector<std::unique_ptr<Shape>>& LocalDrawingClient::getAcceptedShapes() {
	return shapes;
}

void LocalDrawingClient::deleteAllShapes() {
	shapePreview.reset();
	shapesWaitingToAccept.clear();
	shapes.clear();

	emit requestUpdate();
}

Shape* LocalDrawingClient::getSelectedShape() const {
	return selectedShape;
}

void LocalDrawingClient::onReceiveEventFromServer(const NetworkEvent& event) {
	switch (event.type) {
	case NetworkEvent::Type::AddObject: {
		Shape* shape = static_cast<const AddObject&>(event).getShape();
		QUuid shapeUuid = shape->getShapeUuid();

		auto result = std::find_if(shapesWaitingToAccept.begin(), shapesWaitingToAccept.end(), [&shapeUuid](const std::unique_ptr<Shape>& shape) {
			return shape->getShapeUuid() == shapeUuid;
		});

		if (result != shapesWaitingToAccept.end()) {
			qDebug().noquote().nospace() << "Found shape with received UUID.";
			shapes.emplace_back(std::move(*result));
			shapesWaitingToAccept.erase(std::remove(shapesWaitingToAccept.begin(), shapesWaitingToAccept.end(), *result), shapesWaitingToAccept.end());
		} else {
			qDebug().noquote().nospace() << "Shape with received UUID not found.";
			qDebug().noquote().nospace() << "[Received shape]: " << shape->serialize();
			shapes.emplace_back(shape);
		}

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
			return;
		}

		shapes.erase(result);

		break;
	}

	case NetworkEvent::Type::ClearAll: {
		shapes.clear();

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
			break;
		}

		(*result)->setShapeStyle(e.getShapeStyle());
		(*result)->setShapePosition(e.getShapePosition());

		break;
	}
	case NetworkEvent::Type::MoveShape: {
		const MoveShape& e = static_cast<const MoveShape&>(event);
		QUuid shapeUuid = e.shapeUuid;
		QString move = e.move;

		auto result = std::find_if(shapes.begin(), shapes.end(), [&shapeUuid](const std::unique_ptr<Shape>& shape) { return shape->getShapeUuid() == shapeUuid; });
		if (result == shapes.end()) {
			qDebug().noquote().nospace() << "No shape with Uuid: {" << shapeUuid.toString() << "}. Nothing was deleted.";
			return;
		}

		if (move == "Up") {
			std::vector<std::unique_ptr<Shape>>::iterator next = result + 1;
			if (next == shapes.end()) {
				qDebug().noquote().nospace() << "Can't move up.";
			} else {
				std::swap((*result), (*next));
			}
		} else if (move == "Down") {
			std::vector<std::unique_ptr<Shape>>::iterator previous = result - 1;
			if (result == shapes.begin()) {
				qDebug().noquote().nospace() << "Can't move down.";
			} else {
				std::swap((*result), (*previous));
			}
		} else {
			break;
		}

		break;
	}

	default: { break; }
	}

	emit requestUpdate();
}

void LocalDrawingClient::onSendEventToServer(const NetworkEvent& event) {
	Q_UNUSED(event)
	//qDebug().noquote().nospace() << event.serialize();
}

void LocalDrawingClient::deleteSelectedShape() {
	if (!isShapeSelected()) {
		qDebug().noquote().nospace() << "No shape selected! Nothing was deleted.";
		return;
	}

	emit sendEventToServer(DeleteShape(selectedShape->getShapeUuid()));
    selectedShape->deselect();
    selectedShape = nullptr;
}

void LocalDrawingClient::clearCanvas() {
    emit sendEventToServer(ClearAll());
    if (isShapeSelected()) {
        selectedShape->deselect();
        selectedShape = nullptr;
    }
}

void LocalDrawingClient::moveShape(const QString& move) {
	if (selectedShape == nullptr) {
		qDebug().noquote().nospace() << "No shape selected - nothing was moved.";
		return;
	}

	emit sendEventToServer(MoveShape(selectedShape->getShapeUuid(), move));
}

void LocalDrawingClient::onShapeStyleChanged(const ShapeStyle& shapeStyle) {
	selectedShape->setShapeStyle(shapeStyle);
	emit requestUpdate();
	emit sendEventToServer(ShapeModified(*selectedShape));
}

void LocalDrawingClient::onShapePositionChanged() {
	emit sendEventToServer(ShapeModified(*selectedShape));
}

void LocalDrawingClient::select(int x, int y) {
	selectedShape = nullptr;

	for (const auto& shape : shapes) {
		shape->deselect();
	}

	for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
		if ((*it)->getShapeBoundingBox().contains(x, y)) {
			selectedShape = (*it).get();
			selectedShape->select();

			emit shapeSelected();

			break;
		}
	}

	emit requestUpdate();
}

void LocalDrawingClient::changeShapePosition(int x, int y) {
	selectedShape->setShapePosition(selectedShape->getShapePosition().changePosition(x, y));
	emit requestUpdate();
}

bool LocalDrawingClient::isShapeSelected() const {
	return (selectedShape != nullptr);
}
