#include "network_event.h"
#include "arrow.h"
#include "cactus.h"
#include "dash_line.h"
#include "ellipse.h"
#include "line.h"
#include "rectangle.h"
#include "shape.h"
#include "straight_line.h"
#include "text.h"

AddObject::AddObject(Shape* shape) :
  shape(shape) {
	type = NetworkEvent::Type::AddObject;
}

QByteArray AddObject::serialize() const {
	QJsonObject json;

	json["EventType"] = "AddObject";
	json["Shape"] = shape->serialize();

	return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

NetworkEvent* AddObject::deserialize(const QByteArray& data) {
	QJsonObject json = QJsonDocument::fromJson(data).object();
	QJsonObject shapeData = json["Shape"].toObject();
	QString shapeName = shapeData["ShapeType"].toString();

	shape = Shape::createShapeByName(shapeName);

	shape->deserialize(json["Shape"].toObject());

	return this;
}

Shape* AddObject::getShape() const {
	return shape;
}

ClearAll::ClearAll() {
	type = NetworkEvent::Type::ClearAll;
}

QByteArray ClearAll::serialize() const {
	return QJsonDocument(QJsonObject{{"EventType", "ClearAll"}}).toJson(QJsonDocument::Compact);
}

NetworkEvent* ClearAll::deserialize(const QByteArray& data) {
	Q_UNUSED(data);

	return this;
}

DeleteShape::DeleteShape() {
	type = NetworkEvent::Type::DeleteShape;
}

DeleteShape::DeleteShape(const QUuid& shapeUuid) {
	type = NetworkEvent::Type::DeleteShape;

	this->shapeUuid = shapeUuid;
}

QByteArray DeleteShape::serialize() const {
	QJsonObject json;

	json["EventType"] = "DeleteShape";
	json["ShapeUUID"] = shapeUuid.toString();

	return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

NetworkEvent* DeleteShape::deserialize(const QByteArray& data) {
	QJsonObject json = QJsonDocument::fromJson(data).object();
	shapeUuid = QUuid::fromString(json["ShapeUUID"].toString());

	return this;
}

QUuid DeleteShape::getShapeUuid() const {
	return shapeUuid;
}

ShapeModified::ShapeModified() {
	type = NetworkEvent::Type::ShapeModified;
}

ShapeModified::ShapeModified(Shape& shape) :
  shapeUuid(shape.getShapeUuid()),
  shapeStyle(shape.getShapeStyle()),
  shapePosition(shape.getShapePosition()) {
	type = NetworkEvent::Type::ShapeModified;
}

QByteArray ShapeModified::serialize() const {
	QJsonObject serializedEvent;

	serializedEvent["EventType"] = "ShapeModified";
	serializedEvent["ShapeUUID"] = shapeUuid.toString();
	serializedEvent["ShapeStyle"] = shapeStyle.serialize();
	serializedEvent["ShapeData"] = shapePosition.serialize();

	return QJsonDocument(serializedEvent).toJson(QJsonDocument::Compact);
}

NetworkEvent* ShapeModified::deserialize(const QByteArray& data) {
	QJsonObject json = QJsonDocument::fromJson(data).object();

	shapeUuid = QUuid::fromString(json["ShapeUUID"].toString());
	shapeStyle = ShapeStyle::deserialize(json["ShapeStyle"].toObject());
	shapePosition = ShapePosition::deserialize(json["ShapeData"].toObject());

	return this;
}

QUuid ShapeModified::getShapeUuid() const {
	return shapeUuid;
}

ShapeStyle ShapeModified::getShapeStyle() const {
	return shapeStyle;
}

ShapePosition ShapeModified::getShapePosition() const {
	return shapePosition;
}

MoveShape::MoveShape() {
	type = NetworkEvent::Type::MoveShape;
}

MoveShape::MoveShape(const QUuid& shapeUuid, const QString& move) {
	type = NetworkEvent::Type::MoveShape;

	this->shapeUuid = shapeUuid;
	this->move = move;
}

QByteArray MoveShape::serialize() const {
	QJsonObject json;

	json["EventType"] = "MoveShape";
	json["ShapeUUID"] = shapeUuid.toString();
	json["Move"] = move;

	return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

NetworkEvent* MoveShape::deserialize(const QByteArray& data) {
	QJsonObject json = QJsonDocument::fromJson(data).object();

	shapeUuid = QUuid::fromString(json["ShapeUUID"].toString());
	move = json["Move"].toString();

	return this;
}
