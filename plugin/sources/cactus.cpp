#include "cactus.h"

#include <QDebug>

Cactus::Cactus() {
}

Shape* Cactus::create() {
	return new Cactus();
}

Shape* Cactus::clone() {
	return new Cactus(*this);
}

void Cactus::paint(QPainter* painter) {

	QPoint begin = getShapePosition().getPoint(0);
	QPoint end = getShapePosition().getPoint(1);

	painter->setPen(getShapeStyle().getPen());
	painter->setBrush(getShapeStyle().getBrush());

	painter->drawEllipse(QPoint(begin.x(), end.y()), (end.x() - begin.x()) / 2, (end.x() - begin.x()) / 2);
	painter->drawEllipse(end, (end.x() - begin.x()) / 2, (end.x() - begin.x()) / 2);
	painter->drawEllipse(QPoint(begin.x() + (end.x() - begin.x()) / 2, begin.y()), (end.x() - begin.x()) / 2, (end.x() - begin.x()) / 2);
	painter->drawRect(begin.x(), begin.y(), end.x() - begin.x(), end.y() - begin.y());

	if (isSelected()) {
		makeBoundingBox();
		getShapeBoundingBox().paint(painter);
	}
}

//TODO: separate methods from switch body
void Cactus::manipulate(const InputEvent& event) {
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

void Cactus::makeBoundingBox() {
	QPoint begin = getShapePosition().getPoint(0);
	QPoint end = getShapePosition().getPoint(1);

	int left = std::min(begin.x(), end.x());
	int right = std::max(begin.x(), end.x());
	int top = std::min(begin.y(), end.y());
	int bottom = std::max(begin.y(), end.y());

	int rad = std::abs(end.x() - begin.x());

	if (end.x() - begin.x() > 0) {
		left = std::min(right, end.x() - 3 * rad / 2);
		right = std::max(left, end.x() + rad / 2);
	} else {
		left = std::min(right, end.x() - rad / 2);
		right = std::max(left, end.x() + 3 * rad / 2);
	}

	if (end.y() - begin.y() > 0) {
		top = std::min(bottom, begin.y() - rad / 2);
		bottom = std::max(top, end.y() + rad / 2);
	} else {
		top = std::min(bottom, end.y() - rad / 2);
		bottom = std::max(top, begin.y() + rad / 2);
	}

	setShapeBoundingBox(BoundingBox(left, right, top, bottom));
}
QJsonObject Cactus::serialize() {
	QJsonObject serializedShape;

	serializedShape["UUID"] = getShapeUuid().toString();
	serializedShape["ShapeType"] = "Cactus";
	serializedShape["ShapeStyle"] = getShapeStyle().serialize();
	serializedShape["ShapeBoundingBox"] = getShapeBoundingBox().serialize();
	serializedShape["ShapeData"] = getShapePosition().serialize();

	return serializedShape;
}

Shape* Cactus::deserialize(const QJsonObject& data) {
	setShapeUuid(QUuid::fromString(data["UUID"].toString()));
	setShapeStyle(ShapeStyle::deserialize(data["ShapeStyle"].toObject()));
	setShapeBoundingBox(BoundingBox::deserialize(data["ShapeBoundingBox"].toObject()));
	setShapePosition(ShapePosition::deserialize(data["ShapeData"].toObject()));

	return this;
}
