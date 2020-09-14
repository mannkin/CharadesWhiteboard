#include "shape_editor.h"
#include <QJsonDocument>

ShapeEditor::ShapeEditor(QQuickItem* parent) :
  QQuickItem(parent) {
}

ShapeEditor::~ShapeEditor() {
}

void ShapeEditor::setClient(LocalDrawingClient* localDrawingClient) {
	if (localDrawingClient == nullptr) {
		qFatal("Cannot set null client!");
	}

	client = localDrawingClient;
}

void ShapeEditor::setPenWidth(int penWidth) {
	shapeStyle.setPenWidth(penWidth);
	qDebug().noquote().nospace() << "setPenWidth()";
	emit updateGui();
	emit shapeStyleChanged(shapeStyle);
}

int ShapeEditor::getPenWidth() {
	qDebug().noquote().nospace() << "getPenWidth()";
	return shapeStyle.getPenWidth();
}

void ShapeEditor::setPenColor(const QColor& penColor) {
	shapeStyle.setPenColor(penColor);
	qDebug().noquote().nospace() << "setPenColor()";
	emit updateGui();
	emit shapeStyleChanged(shapeStyle);
}

QColor ShapeEditor::getPenColor() {
	qDebug().noquote().nospace() << "getPenColor()";
	return shapeStyle.getPenColor();
}

void ShapeEditor::onShapeSelected() {
	if (client == nullptr) {
		qFatal("Client cannot be null!");
	}

	shapeStyle = client->getSelectedShape()->getShapeStyle();

	emit updateGui();
}
