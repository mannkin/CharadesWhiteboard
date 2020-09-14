#ifndef SHAPE_POSITION_H
#define SHAPE_POSITION_H

#include <QJsonObject>
#include <QPoint>
#include <QVector>

class ShapePosition {
public:
	ShapePosition();
	ShapePosition(QVector<QPoint> newPoints);
//	ShapePosition(const ShapePosition& newShapePosition);

	QJsonObject serialize() const;
	static ShapePosition deserialize(const QJsonObject& data);

	void addPoint(QPoint point);
	void insertPoint(QPoint, int i);
	void addPoints(QVector<QPoint> newPoints);
	QPoint getPoint(int i);
	QVector<QPoint>& getPoints();

	QVector<QPoint> changePosition(int x, int y);

	int getSize();

private:
	QVector<QPoint> points;
};

#endif // SHAPE_POSITION_H
