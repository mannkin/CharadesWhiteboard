#include "toolbox.h"

Toolbox::Toolbox(QQuickItem* parent) :
  QQuickItem(parent) {
}

Toolbox::~Toolbox() {
}

void Toolbox::setClient(LocalDrawingClient* localDrawingClient) {
	if (localDrawingClient == nullptr) {
		qFatal("Client cannot be null");
	}

	client = localDrawingClient;
}

void Toolbox::setSelectedShape(QString selectedShape) {
	qDebug().noquote().nospace() << "Selected shape name: " << selectedShape;
	this->selectedShape = selectedShape;
}

QString Toolbox::getSelectedShape() {
	return selectedShape;
}

void Toolbox::setPenWidth(unsigned penWidth) {
	qDebug().noquote().nospace() << "Pen width: " << penWidth;
	this->penWidth = penWidth;
}

unsigned Toolbox::getPenWidth() {
	return penWidth;
}

void Toolbox::setPenColor(QColor penColor) {
	qDebug().noquote().nospace() << "Pen color: " << penColor.toRgb();
	this->penColor = penColor;
}

QColor Toolbox::getPenColor() {
	return penColor;
}

void Toolbox::deleteSelectedShape() {
	client->deleteSelectedShape();
}

void Toolbox::clearCanvas() {
	client->clearCanvas();
}

void Toolbox::moveShapeUp() {
	qDebug() << "Up";
	client->moveShape("Up");
}

void Toolbox::moveShapeDown() {
	qDebug() << "Down";
	client->moveShape("Down");
}
