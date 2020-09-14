#include "ellipse.h"

Ellipse::Ellipse() {
}

Shape* Ellipse::create() {
	return new Ellipse();
}

Shape* Ellipse::clone() {
	return new Ellipse(*this);
}

void Ellipse::paint(QPainter* painter) {
	painter->setPen(getShapeStyle().getPen());
	painter->setBrush(getShapeStyle().getBrush());
	QPointF center(getShapePosition().getPoint(0));
	painter->drawEllipse(center, getShapePosition().getPoint(1).x() - getShapePosition().getPoint(0).x(), getShapePosition().getPoint(1).y() - getShapePosition().getPoint(0).y());

	if (isSelected()) {
		makeBoundingBox();
		getShapeBoundingBox().paint(painter);
	}
}

void Ellipse::manipulate(const InputEvent& event) {
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

void Ellipse::makeBoundingBox() {
	QPoint begin = getShapePosition().getPoint(0);
	QPoint end = getShapePosition().getPoint(1);

	int left = std::min(begin.x(), end.x());
	int right = std::max(begin.x(), end.x());
	int top = std::min(begin.y(), end.y());
	int bottom = std::max(begin.y(), end.y());

	if (end.x() - begin.x() > 0) {
		left = std::min(right, end.x() - 2 * (end.x() - begin.x()));
	} else {
		right = std::max(left, end.x() - 2 * (end.x() - begin.x()));
	}

	if (end.y() - begin.y() > 0) {
		top = std::min(bottom, end.y() - 2 * (end.y() - begin.y()));
	} else {
		bottom = std::max(top, end.y() - 2 * (end.y() - begin.y()));
	}

	setShapeBoundingBox(BoundingBox(left, right, top, bottom));
}

QJsonObject Ellipse::serialize() {
	QJsonObject serializedShape;

	serializedShape["UUID"] = getShapeUuid().toString();
	serializedShape["ShapeType"] = "Ellipse";
	serializedShape["ShapeStyle"] = getShapeStyle().serialize();
	serializedShape["ShapeBoundingBox"] = getShapeBoundingBox().serialize();
	serializedShape["ShapeData"] = getShapePosition().serialize();

	return serializedShape;
}

Shape* Ellipse::deserialize(const QJsonObject& data) {
	setShapeUuid(QUuid::fromString(data["UUID"].toString()));
	setShapeStyle(ShapeStyle::deserialize(data["ShapeStyle"].toObject()));
	setShapeBoundingBox(BoundingBox::deserialize(data["ShapeBoundingBox"].toObject()));
	setShapePosition(ShapePosition::deserialize(data["ShapeData"].toObject()));

	return this;
}
