#include "rectangle.h"
#include <QDebug>

Rectangle::Rectangle() {
}

Shape* Rectangle::create() {
	return new Rectangle();
}

Shape* Rectangle::clone() {
	return new Rectangle(*this);
}

void Rectangle::paint(QPainter* painter) {
	painter->setPen(getShapeStyle().getPen());
	painter->setBrush(getShapeStyle().getBrush());
	painter->drawRect(getShapePosition().getPoint(0).x(), getShapePosition().getPoint(0).y(), getShapePosition().getPoint(1).x() - getShapePosition().getPoint(0).x(), getShapePosition().getPoint(1).y() - getShapePosition().getPoint(0).y());

	if (isSelected()) {
		makeBoundingBox();
		getShapeBoundingBox().paint(painter);
	}
}
void Rectangle::manipulate(const InputEvent& event) {
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
void Rectangle::makeBoundingBox() {
	int left = std::min(getShapePosition().getPoint(0).x(), getShapePosition().getPoint(1).x());
	int right = std::max(getShapePosition().getPoint(0).x(), getShapePosition().getPoint(1).x());
	int top = std::min(getShapePosition().getPoint(0).y(), getShapePosition().getPoint(1).y());
	int bottom = std::max(getShapePosition().getPoint(0).y(), getShapePosition().getPoint(1).y());
	setShapeBoundingBox(BoundingBox(left, right, top, bottom));
}

QJsonObject Rectangle::serialize() {
	QJsonObject serializedShape;
	serializedShape["UUID"] = getShapeUuid().toString();
	serializedShape["ShapeType"] = "Rectangle";
	serializedShape["ShapeStyle"] = getShapeStyle().serialize();
	serializedShape["ShapeBoundingBox"] = getShapeBoundingBox().serialize();
	serializedShape["ShapeData"] = getShapePosition().serialize();

	return serializedShape;
}

Shape* Rectangle::deserialize(const QJsonObject& data) {
	setShapeUuid(QUuid::fromString(data["UUID"].toString()));
	setShapeStyle(ShapeStyle::deserialize(data["ShapeStyle"].toObject()));
	setShapeBoundingBox(BoundingBox::deserialize(data["ShapeBoundingBox"].toObject()));
	setShapePosition(ShapePosition::deserialize(data["ShapeData"].toObject()));
	return this;
}
