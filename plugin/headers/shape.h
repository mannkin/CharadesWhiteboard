#ifndef SHAPE_H
#define SHAPE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QUuid>
#include <memory>

#include "bounding_box.h"
#include "input_event.h"
#include "shape_position.h"
#include "shape_style.h"
class Shape {
public:
	virtual ~Shape() = default;

	virtual Shape* create() = 0;
	virtual Shape* clone() = 0;

	virtual void paint(QPainter* painter) = 0;
	virtual void manipulate(const InputEvent& event) = 0;
	//virtual void makeBoundingBox() = 0;

	virtual QJsonObject serialize() = 0;
	virtual Shape* deserialize(const QJsonObject& data) = 0;

	static Shape* createShapeByName(const QString& shapeName);

	void setShapeStyle(const ShapeStyle& newShapeStyle);
	ShapeStyle getShapeStyle() const;

	void setShapeUuid(const QUuid& newUuid);
	QUuid getShapeUuid() const;

	void setShapeBoundingBox(const BoundingBox& newBoundingBox);
	BoundingBox getShapeBoundingBox() const;

	void setShapePosition(const ShapePosition& newShapePosition);
	ShapePosition& getShapePosition();

	bool isSelected() const;
	void select();
	void deselect();

private:
	QUuid uuid = QUuid::createUuid();
	ShapeStyle shapeStyle;
	ShapePosition shapePosition;
	BoundingBox boundingBox;
	bool selected = false;
};

#endif // SHAPE_H
