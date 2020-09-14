#include "text.h"
#include <QDebug>

Text::Text() {
}

Shape* Text::create() {
	return new Text();
}

Shape* Text::clone() {
	return new Text(*this);
}

void Text::paint(QPainter* painter) {
	painter->setPen(getShapeStyle().getPen());
	painter->setBrush(getShapeStyle().getBrush());
	painter->setFont(font);
	painter->drawText(begin.x(), begin.y(), text);
}

void Text::manipulate(const InputEvent& event) {
	switch (event.type) {
	case InputEvent::Type::MouseButtonDown: {
		const MouseButtonDown& e = static_cast<const MouseButtonDown&>(event);
		begin = QPoint(e.getX(), e.getY());
		end = begin;
		break;
	}

	case InputEvent::Type::MouseButtonUp: {
		const MouseButtonUp& e = static_cast<const MouseButtonUp&>(event);
		end = QPoint(e.getX(), e.getY());
		break;
	}

	case InputEvent::Type::MouseMovement: {
		const MouseMovement& e = static_cast<const MouseMovement&>(event);
		end = QPoint(e.getX(), e.getY());
		break;
	}

	default: {}
	}
}

QJsonObject Text::serialize() {
	QJsonObject serializedShape;

	serializedShape["UUID"] = getShapeUuid().toString();
	serializedShape["ShapeType"] = "Text";
	serializedShape["ShapeStyle"] = getShapeStyle().serialize();
	serializedShape["ShapeBoundingBox"] = getShapeBoundingBox().serialize();

	QJsonObject shapeData;
	shapeData["Text"] = text;
	shapeData["TextBeginningX"] = static_cast<qint64>(begin.x());
	shapeData["TextBeginningY"] = static_cast<qint64>(begin.y());
	shapeData["TextEndX"] = static_cast<qint64>(end.x());
	shapeData["TextEndY"] = static_cast<qint64>(end.y());

	serializedShape["ShapeData"] = shapeData;

	return serializedShape;
}

Shape* Text::deserialize(const QJsonObject& data) {
	setShapeUuid(QUuid::fromString(data["UUID"].toString()));
	setShapeStyle(ShapeStyle::deserialize(data["ShapeStyle"].toObject()));
	setShapeBoundingBox(BoundingBox::deserialize(data["ShapeBoundingBox"].toObject()));

	unsigned beginX = data["ShapeData"]["TextBeginningX"].toInt();
	unsigned beginY = data["ShapeData"]["TextBeginningY"].toInt();
	unsigned endX = data["ShapeData"]["TextEndX"].toInt();
	unsigned endY = data["ShapeData"]["TextEndY"].toInt();

	begin = QPoint(beginX, beginY);
	end = QPoint(endX, endY);

	text = data["ShapeData"]["Text"].toString();

	return this;
}
