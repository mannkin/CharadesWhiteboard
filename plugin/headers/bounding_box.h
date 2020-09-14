#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <QJsonObject>
#include <QPainter>
#include <QRect>

class BoundingBox {
public:
	BoundingBox();
	BoundingBox(int left, int right, int top, int bottom);

	void paint(QPainter* painter) const;

	bool contains(int x, int y) const;

	void setLeft(int left);
	int getLeft() const;

	void setRight(int right);
	int getRight() const;

	void setTop(int top);
	int getTop() const;

	void setBottom(int bottom);
	int getBottom() const;

	QJsonObject serialize() const;
	static BoundingBox deserialize(const QJsonObject& data);

	QRect changePosition(int x, int y);

private:
	QRect boundingBoxRectangle;
	QPen boundingBoxPen;
	QBrush boundingBoxBrush;
};

#endif // BOUNDING_BOX_H
