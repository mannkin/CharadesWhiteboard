#include "straight_line.h"

StraightLine::StraightLine() {
}

Shape* StraightLine::create() {
	return new StraightLine();
}

Shape* StraightLine::clone() {
	return new StraightLine(*this);
}
void StraightLine::paint(QPainter* painter) {
	painter->setPen(getShapeStyle().getPen());
	painter->setBrush(getShapeStyle().getBrush());
	painter->drawLine(getShapePosition().getPoint(0), getShapePosition().getPoint(1));

	if (isSelected()) {
		makeBoundingBox();
		getShapeBoundingBox().paint(painter);
	}
}

void StraightLine::manipulate(const InputEvent& event) {
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
void StraightLine::makeBoundingBox() {
	int left = std::min(getShapePosition().getPoint(0).x(), getShapePosition().getPoint(1).x());
	int right = std::max(getShapePosition().getPoint(0).x(), getShapePosition().getPoint(1).x());
	int top = std::min(getShapePosition().getPoint(0).y(), getShapePosition().getPoint(1).y());
	int bottom = std::max(getShapePosition().getPoint(0).y(), getShapePosition().getPoint(1).y());
	setShapeBoundingBox(BoundingBox(left, right, top, bottom));
}

QJsonObject StraightLine::serialize() {
	QJsonObject serializedShape;

	serializedShape["UUID"] = getShapeUuid().toString();
	serializedShape["ShapeType"] = "StraightLine";
	serializedShape["ShapeStyle"] = getShapeStyle().serialize();
	serializedShape["ShapeBoundingBox"] = getShapeBoundingBox().serialize();
	serializedShape["ShapeData"] = getShapePosition().serialize();
	return serializedShape;
}

Shape* StraightLine::deserialize(const QJsonObject& data) {
	setShapeUuid(QUuid::fromString(data["UUID"].toString()));
	setShapeStyle(ShapeStyle::deserialize(data["ShapeStyle"].toObject()));
	setShapeBoundingBox(BoundingBox::deserialize(data["ShapeBoundingBox"].toObject()));
	setShapePosition(ShapePosition::deserialize(data["ShapeData"].toObject()));

	return this;
}
