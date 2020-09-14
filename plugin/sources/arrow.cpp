#include "arrow.h"
#include <QLineF>
#include <cmath>

Arrow::Arrow() {
}

Shape* Arrow::create() {
	return new Arrow();
}

Shape* Arrow::clone() {
	return new Arrow(*this);
}

void Arrow::paint(QPainter* painter) {

	qreal arrowHeadSize = 20;
	QLineF line(getShapePosition().getPoint(0), getShapePosition().getPoint(1));
	double angle = std::atan2(-line.dy(), line.dx());

	QPointF arrowHeadPoint1 = line.p2() - QPointF(sin(angle + M_PI / 3) * arrowHeadSize, cos(angle + M_PI / 3) * arrowHeadSize);
	QPointF arrowHeadPoint2 = line.p2() - QPointF(sin(angle + M_PI - M_PI / 3) * arrowHeadSize, cos(angle + M_PI - M_PI / 3) * arrowHeadSize);
	QPointF arrowHead[3] = {line.p2(), arrowHeadPoint1, arrowHeadPoint2};

	painter->setPen(getShapeStyle().getPen());
	painter->setBrush(getShapeStyle().getBrush());

	painter->drawLine(line);
	painter->drawPolygon(arrowHead, 3);

	if (isSelected()) {
		makeBoundingBox();
		getShapeBoundingBox().paint(painter);
	}
}

void Arrow::manipulate(const InputEvent& event) {
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

void Arrow::makeBoundingBox() {
	int left = std::min(getShapePosition().getPoint(0).x(), getShapePosition().getPoint(1).x());
	int right = std::max(getShapePosition().getPoint(0).x(), getShapePosition().getPoint(1).x());
	int top = std::min(getShapePosition().getPoint(0).y(), getShapePosition().getPoint(1).y());
	int bottom = std::max(getShapePosition().getPoint(0).y(), getShapePosition().getPoint(1).y());
	setShapeBoundingBox(BoundingBox(left, right, top, bottom));
}

QJsonObject Arrow::serialize() {
	QJsonObject serializedShape;

	serializedShape["UUID"] = getShapeUuid().toString();
	serializedShape["ShapeType"] = "Arrow";
	serializedShape["ShapeStyle"] = getShapeStyle().serialize();
	serializedShape["ShapeBoundingBox"] = getShapeBoundingBox().serialize();
	serializedShape["ShapeData"] = getShapePosition().serialize();

	return serializedShape;
}

Shape* Arrow::deserialize(const QJsonObject& data) {
	setShapeUuid(QUuid::fromString(data["UUID"].toString()));
	setShapeStyle(ShapeStyle::deserialize(data["ShapeStyle"].toObject()));
	setShapeBoundingBox(BoundingBox::deserialize(data["ShapeBoundingBox"].toObject()));
	setShapePosition(ShapePosition::deserialize(data["ShapeData"].toObject()));

	return this;
}
