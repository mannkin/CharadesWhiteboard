#include "dash_line.h"
#include <QDebug>
DashLine::DashLine() {
}

Shape* DashLine::create() {
	return new DashLine();
}

Shape* DashLine::clone() {
	return new DashLine(*this);
}
void DashLine::paint(QPainter* painter) {
	QPen pen = getShapeStyle().getPen();
	pen.setStyle(Qt::DashLine);
	painter->setPen(pen);
	painter->setBrush(getShapeStyle().getBrush());
	painter->drawLine(getShapePosition().getPoint(0), getShapePosition().getPoint(1));

	if (isSelected()) {
		makeBoundingBox();
		getShapeBoundingBox().paint(painter);
	}
}

void DashLine::manipulate(const InputEvent& event) {
	switch (event.type) {
	case InputEvent::Type::MouseButtonDown: {
		const MouseButtonDown& e = static_cast<const MouseButtonDown&>(event);
		getShapePosition().insertPoint(QPoint(e.getX(), e.getY()), 0);
		getShapePosition().insertPoint(QPoint(e.getX(), e.getY()), 1);
		break;
	}

	case InputEvent::Type::MouseMovement: {
		const MouseMovement& e = static_cast<const MouseMovement&>(event);
		getShapePosition().insertPoint(QPoint(e.getX(), e.getY()), 1);
		break;
	}

	case InputEvent::Type::MouseButtonUp: {
		makeBoundingBox();
		break;
	}
	}
}

void DashLine::makeBoundingBox() {
	int left = std::min(getShapePosition().getPoint(0).x(), getShapePosition().getPoint(1).x());
	int right = std::max(getShapePosition().getPoint(0).x(), getShapePosition().getPoint(1).x());
	int top = std::min(getShapePosition().getPoint(0).y(), getShapePosition().getPoint(1).y());
	int bottom = std::max(getShapePosition().getPoint(0).y(), getShapePosition().getPoint(1).y());
	setShapeBoundingBox(BoundingBox(left, right, top, bottom));
}

QJsonObject DashLine::serialize() {
	QJsonObject serializedShape;

	serializedShape["UUID"] = getShapeUuid().toString();
	serializedShape["ShapeType"] = "DashLine";
	serializedShape["ShapeStyle"] = getShapeStyle().serialize();
	serializedShape["ShapeBoundingBox"] = getShapeBoundingBox().serialize();
	serializedShape["ShapeData"] = getShapePosition().serialize();

	return serializedShape;
}

Shape* DashLine::deserialize(const QJsonObject& data) {
	setShapeUuid(QUuid::fromString(data["UUID"].toString()));
	setShapeStyle(ShapeStyle::deserialize(data["ShapeStyle"].toObject()));
	setShapeBoundingBox(BoundingBox::deserialize(data["ShapeBoundingBox"].toObject()));
	setShapePosition(ShapePosition::deserialize(data["ShapeData"].toObject()));

	return this;
}
