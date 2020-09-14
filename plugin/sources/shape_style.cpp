#include "shape_style.h"

ShapeStyle::ShapeStyle() {
	shapePen.setCapStyle(Qt::RoundCap);
	shapePen.setJoinStyle(Qt::RoundJoin);
	shapeBrush.setStyle(Qt::SolidPattern);
}

ShapeStyle::ShapeStyle(const QPen& pen, const QBrush& brush) :
  shapePen(pen),
  shapeBrush(brush) {
}

void ShapeStyle::setPenWidth(int width) {
	shapePen.setWidth(width);
}

int ShapeStyle::getPenWidth() const {
	return shapePen.width();
}

void ShapeStyle::setPenColor(const QColor& color) {
	shapePen.setColor(color);
	// TODO: brush color
	shapeBrush.setColor(color);
}

QColor ShapeStyle::getPenColor() const {
	return shapePen.color();
}

void ShapeStyle::setPenStyle(Qt::PenStyle penStyle) {
	shapePen.setStyle(penStyle);
}

Qt::PenStyle ShapeStyle::getPenStyle() const {
	return shapePen.style();
}

void ShapeStyle::setPenCapStyle(Qt::PenCapStyle penCapStyle) {
	shapePen.setCapStyle(penCapStyle);
}

Qt::PenCapStyle ShapeStyle::getPenCapStyle() const {
	return shapePen.capStyle();
}

void ShapeStyle::setPenJoinStyle(Qt::PenJoinStyle penJoinStyle) {
	shapePen.setJoinStyle(penJoinStyle);
}

Qt::PenJoinStyle ShapeStyle::getPenJoinStyle() const {
	return shapePen.joinStyle();
}

void ShapeStyle::setBrushColor(const QColor& color) {
	shapeBrush.setColor(color);
}

QColor ShapeStyle::getBrushColor() const {
	return shapeBrush.color();
}

void ShapeStyle::setBrushStyle(Qt::BrushStyle brushStyle) {
	shapeBrush.setStyle(brushStyle);
}

Qt::BrushStyle ShapeStyle::getBrushStyle() const {
	return shapeBrush.style();
}

QJsonObject ShapeStyle::serialize() const {
	QJsonObject serializedShapeStyle;

	QJsonObject serializedShapePen;
	serializedShapePen["Width"] = shapePen.width();
	serializedShapePen["Color"] = shapePen.color().name();
	serializedShapePen["PenStyle"] = shapePen.style();
	serializedShapePen["CapStyle"] = shapePen.capStyle();
	serializedShapePen["JoinStyle"] = shapePen.joinStyle();

	QJsonObject serializedShapeBrush;
	serializedShapeBrush["Color"] = shapeBrush.color().name();
	serializedShapeBrush["BrushStyle"] = shapeBrush.style();

	serializedShapeStyle["PenStyle"] = serializedShapePen;
	serializedShapeStyle["BrushStyle"] = serializedShapeBrush;

	return serializedShapeStyle;
}

ShapeStyle ShapeStyle::deserialize(const QJsonObject& data) {
	if (!data["PenStyle"].isObject()) {
		qFatal("Received data is not valid! Could not read \"PenStyle\" section.");
	}

	if (!data["BrushStyle"].isObject()) {
		qFatal("Received data is not valid! Could not read \"BrushStyle\" section.");
	}

	QPen deserializedPen;
	QJsonObject penStyle = data["PenStyle"].toObject();

	deserializedPen.setWidth(penStyle["Width"].toInt());
	deserializedPen.setColor(QColor(penStyle["Color"].toString()));
	deserializedPen.setStyle(static_cast<Qt::PenStyle>(penStyle["PenStyle"].toInt()));
	deserializedPen.setCapStyle(static_cast<Qt::PenCapStyle>(penStyle["CapStyle"].toInt()));
	deserializedPen.setJoinStyle(static_cast<Qt::PenJoinStyle>(penStyle["JoinStyle"].toInt()));

	QBrush deserializedBrush;
	QJsonObject brushStyle = data["BrushStyle"].toObject();

	deserializedBrush.setColor(QColor(brushStyle["Color"].toString()));
	deserializedBrush.setStyle(static_cast<Qt::BrushStyle>(brushStyle["BrushStyle"].toInt()));

	return ShapeStyle(deserializedPen, deserializedBrush);
}

QPen ShapeStyle::getPen() const {
	return shapePen;
}

QBrush ShapeStyle::getBrush() const {
	return shapeBrush;
}
