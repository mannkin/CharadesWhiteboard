#include "shape_position.h"
#include <QDebug>
#include <QJsonArray>
#include <QUuid>

ShapePosition::ShapePosition() {
}

ShapePosition::ShapePosition(QVector<QPoint> newPoints) :
  points(newPoints) {
}

//ShapePosition::ShapePosition(const ShapePosition& newShapePosition) :
//  points(newShapePosition.points) {
//}

void ShapePosition::addPoint(QPoint point) {
	qDebug() << "PointAdded" << point.x() << point.y();
	points.push_back(point);
	qDebug() << points.size();
}

void ShapePosition::insertPoint(QPoint point, int i) {
	points.insert(i, point);
}

void ShapePosition::addPoints(QVector<QPoint> newPoints) {
	points = newPoints;
}
QPoint ShapePosition::getPoint(int i) {
	return points[i];
}

QVector<QPoint>& ShapePosition::getPoints() {
	return points;
}

QVector<QPoint> ShapePosition::changePosition(int x, int y) {

	QPoint offset = QPoint(x - points.front().x(), y - points.front().y());
	qDebug() << offset;

	for (auto& i : points) {
		i += offset;
	}

	return points;
}

int ShapePosition::getSize() {
	return points.size();
}

QJsonObject ShapePosition::serialize() const {
	QJsonObject serializedPosition;
	serializedPosition["ControlNodesCount"] = static_cast<qint64>(points.size());

	QJsonArray controlNodesData;
	for (const auto& point : points) {
		controlNodesData.append(point.x());
		controlNodesData.append(point.y());
	}

	serializedPosition["ControlNodesData"] = controlNodesData;

	return serializedPosition;
}

ShapePosition ShapePosition::deserialize(const QJsonObject& data) {

	unsigned controlNodesCount = data["ControlNodesCount"].toInt();
	QJsonArray controlNodesData = data["ControlNodesData"].toArray();
	QVector<QPoint> deserializedPoints;

	for (unsigned i = 0; i < controlNodesCount; ++i) {
		int x = controlNodesData[2 * i].toInt();
		int y = controlNodesData[(2 * i) + 1].toInt();

		deserializedPoints.append(QPoint(x, y));
	}

	return ShapePosition(deserializedPoints);
}
