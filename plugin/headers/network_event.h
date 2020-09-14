#ifndef NETWORK_EVENT_H
#define NETWORK_EVENT_H

#include <QByteArray>
#include <QString>

#include "shape.h"

class NetworkEvent {
public:
	virtual QByteArray serialize() const = 0;
	virtual NetworkEvent* deserialize(const QByteArray& data) = 0;
	virtual ~NetworkEvent() = default;

	enum class Type {
		AddObject,
		DeleteShape,
		ClearAll,
		ShapeModified,
		MoveShape,
		AcceptChanges,
		RejectChanges
	} type;
};

class AddObject : public NetworkEvent {
public:
	AddObject(Shape* shape = nullptr);

	QByteArray serialize() const override;
	NetworkEvent* deserialize(const QByteArray& data) override;

	Shape* getShape() const;

private:
	Shape* shape = nullptr;
};

class DeleteShape : public NetworkEvent {
public:
	DeleteShape();
	DeleteShape(const QUuid& shapeUuid);

	QByteArray serialize() const override;
	NetworkEvent* deserialize(const QByteArray& data) override;

	QUuid getShapeUuid() const;

private:
	QUuid shapeUuid;
};

class ClearAll : public NetworkEvent {
public:
	ClearAll();

	QByteArray serialize() const override;
	NetworkEvent* deserialize(const QByteArray& data) override;
};

class ShapeModified : public NetworkEvent {
public:
	ShapeModified();
	ShapeModified(Shape& shape);

	QByteArray serialize() const override;
	NetworkEvent* deserialize(const QByteArray& data) override;

	QUuid getShapeUuid() const;
	ShapeStyle getShapeStyle() const;
	ShapePosition getShapePosition() const;

private:
	QUuid shapeUuid;
	ShapeStyle shapeStyle;
	ShapePosition shapePosition;
};

class MoveShape : public NetworkEvent {
public:
	MoveShape();
	MoveShape(const QUuid& shapeUuid, const QString& move);

	QByteArray serialize() const override;
	NetworkEvent* deserialize(const QByteArray& data);

	QUuid shapeUuid;
	QString move;
};

#endif // NETWORK_EVENT_H
