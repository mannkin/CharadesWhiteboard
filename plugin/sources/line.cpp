#include "line.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Line::Line() {
}

Shape* Line::create() {
	return new Line();
}

Shape* Line::clone() {
	return new Line(*this);
}

void Line::paint(QPainter* painter) {
	painter->setPen(getShapeStyle().getPen());
	painter->setBrush(getShapeStyle().getBrush());

	if (getShapePosition().getSize() == 1) {
		painter->drawPoint(getShapePosition().getPoint(0).x(), getShapePosition().getPoint(0).y());
	} else {
		for (int i = 1; i < getShapePosition().getSize(); ++i) {
			painter->drawLine(getShapePosition().getPoint(i - 1).x(), getShapePosition().getPoint(i - 1).y(), getShapePosition().getPoint(i).x(), getShapePosition().getPoint(i).y());
		}
	}

	if (isSelected()) {
		makeBoundingBox();
		getShapeBoundingBox().paint(painter);
	}
}

//TODO: separate methods from switch body
void Line::manipulate(const InputEvent& event) {
	switch (event.type) {
	case InputEvent::Type::MouseButtonDown: {
		const MouseButtonDown& e = static_cast<const MouseButtonDown&>(event);
		getShapePosition().addPoint(QPoint(e.getX(), e.getY()));
		qDebug() << "MouseButtonDown" << e.getX() << e.getY();
		break;
	}

	case InputEvent::Type::MouseMovement: {
		const MouseMovement& e = static_cast<const MouseMovement&>(event);
		getShapePosition().addPoint(QPoint(e.getX(), e.getY()));
		qDebug() << "MouseMovement" << e.getX() << e.getY();
		break;
	}

	case InputEvent::Type::MouseButtonUp: {
		makeBoundingBox();
		break;
	}
	}
}

void Line::makeBoundingBox() {
	int left = getShapePosition().getPoint(0).x();
	int right = getShapePosition().getPoint(0).x();
	int top = getShapePosition().getPoint(0).y();
	int bottom = getShapePosition().getPoint(0).y();

	for (auto it = std::next(getShapePosition().getPoints().begin()); it != getShapePosition().getPoints().end(); ++it) {
		int x = (*it).x();
		int y = (*it).y();

		left = std::min(left, x);
		right = std::max(right, x);
		top = std::min(top, y);
		bottom = std::max(bottom, y);
	}
	setShapeBoundingBox(BoundingBox(left, right, top, bottom));
}

QJsonObject Line::serialize() {
	QJsonObject serializedShape;

	serializedShape["UUID"] = getShapeUuid().toString();
	serializedShape["ShapeType"] = "Line";
	serializedShape["ShapeStyle"] = getShapeStyle().serialize();
	serializedShape["ShapeBoundingBox"] = getShapeBoundingBox().serialize();

	QJsonObject shapeData;
	shapeData["ControlNodesCount"] = static_cast<qint64>(getShapePosition().getSize());

	QJsonArray controlNodesData;
	for (const QPoint& point : getShapePosition().getPoints()) {
		controlNodesData.append(point.x());
		controlNodesData.append(point.y());
	}
	shapeData["ControlNodesData"] = controlNodesData;

	serializedShape["ShapeData"] = getShapePosition().serialize();

	return serializedShape;
}

Shape* Line::deserialize(const QJsonObject& data) {
	setShapeUuid(QUuid::fromString(data["UUID"].toString()));
	setShapeStyle(ShapeStyle::deserialize(data["ShapeStyle"].toObject()));
	setShapeBoundingBox(BoundingBox::deserialize(data["ShapeBoundingBox"].toObject()));

	setShapePosition(ShapePosition::deserialize(data["ShapeData"].toObject()));

	return this;
}
