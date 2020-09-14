#include "whiteboard_quick_canvas.h"

WhiteboardQuickCanvas::WhiteboardQuickCanvas(QQuickItem* parent) :
  QQuickPaintedItem(parent) {
}

// Paint() is thread-safe: http://doc.qt.io/qt-5/qquickpainteditem.html#paint
void WhiteboardQuickCanvas::paint(QPainter* painter) {
	if (client == nullptr) {
		qDebug() << "Client is nullptr.";
		return;
	}

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setRenderHint(QPainter::HighQualityAntialiasing);

	const std::vector<std::unique_ptr<Shape>>& shapes = client->getAcceptedShapes();
	for (const auto& shape : shapes) {
		shape.get()->paint(painter);
	}

	const std::deque<std::unique_ptr<Shape>>& shapesWaitingToAccept = client->getShapesWaitingToAccept();
	for (const auto& shape : shapesWaitingToAccept) {
		shape.get()->paint(painter);
	}

	std::unique_ptr<Shape>& preview = client->getPreview();
	if (preview != nullptr) {
		preview->paint(painter);
	}
}

void WhiteboardQuickCanvas::setClient(LocalDrawingClient* client) {
	if (client == nullptr) {
		qFatal("Client pointer cannot be null!");
	}

	this->client = client;
	QObject::connect(this->client, &LocalDrawingClient::requestUpdate, this, &WhiteboardQuickCanvas::update);
}

void WhiteboardQuickCanvas::update() {
	QQuickPaintedItem::update();
}
