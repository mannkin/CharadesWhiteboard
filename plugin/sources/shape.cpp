#include "shape.h"

#include "arrow.h"
#include "cactus.h"
#include "dash_line.h"
#include "ellipse.h"
#include "line.h"
#include "rectangle.h"
#include "straight_line.h"
#include "text.h"

Shape* Shape::createShapeByName(const QString& shapeName) {
	if (shapeName == "Line") {
		return new Line();
	} else if (shapeName == "Rectangle") {
		return new Rectangle();
	} else if (shapeName == "Cactus") {
		return new Cactus();
	} else if (shapeName == "Text") {
		return new Text();
	} else if (shapeName == "StraightLine") {
		return new StraightLine();
	} else if (shapeName == "DashLine") {
		return new DashLine();
	} else if (shapeName == "Arrow") {
		return new Arrow();
	} else if (shapeName == "Ellipse") {
		return new Ellipse();
	} else {
		qFatal("Unknown shape name! Application terminating...");
	}
}

void Shape::setShapeStyle(const ShapeStyle& newShapeStyle) {
	shapeStyle = newShapeStyle;
}

ShapeStyle Shape::getShapeStyle() const {
	return shapeStyle;
}

void Shape::setShapePosition(const ShapePosition& newShapePosition) {
	shapePosition = newShapePosition;
}

ShapePosition& Shape::getShapePosition() {
	return shapePosition;
}

void Shape::setShapeUuid(const QUuid& newUuid) {
	uuid = newUuid;
}

QUuid Shape::getShapeUuid() const {
	return uuid;
}

void Shape::setShapeBoundingBox(const BoundingBox& newBoundingBox) {
	boundingBox = newBoundingBox;
}

BoundingBox Shape::getShapeBoundingBox() const {
	return boundingBox;
}

void Shape::select() {
	selected = true;
}

void Shape::deselect() {
	selected = false;
}

bool Shape::isSelected() const {
	return selected;
}
