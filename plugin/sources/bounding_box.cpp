#include "bounding_box.h"

#include <QDebug>

BoundingBox::BoundingBox() :
  BoundingBox(0, 0, 0, 0) {
}

BoundingBox::BoundingBox(int left, int right, int top, int bottom) {
	setLeft(left);
	setRight(right);
	setTop(top);
	setBottom(bottom);

	boundingBoxBrush.setColor("transparent");
	boundingBoxBrush.setStyle(Qt::SolidPattern);

	boundingBoxPen.setBrush(boundingBoxBrush);
	boundingBoxPen.setColor(QColor(0, 0, 0));
	boundingBoxPen.setWidth(1);
	boundingBoxPen.setCapStyle(Qt::RoundCap);
	boundingBoxPen.setStyle(Qt::DashLine);
}

void BoundingBox::paint(QPainter* painter) const {
	if (painter == nullptr) {
		qDebug().noquote().nospace() << "[" << __PRETTY_FUNCTION__ << "]: QPainter pointer is null!";
		return;
	}

	painter->setPen(boundingBoxPen);
	painter->setBrush(boundingBoxBrush);
	painter->drawRect(boundingBoxRectangle);
}

bool BoundingBox::contains(int x, int y) const {
	return boundingBoxRectangle.contains(x, y);
}

void BoundingBox::setLeft(int left) {
	boundingBoxRectangle.setLeft(left);
}

int BoundingBox::getLeft() const {
	return boundingBoxRectangle.left();
}

void BoundingBox::setRight(int right) {
	boundingBoxRectangle.setRight(right);
}

int BoundingBox::getRight() const {
	return boundingBoxRectangle.right();
}

void BoundingBox::setTop(int top) {
	boundingBoxRectangle.setTop(top);
}

int BoundingBox::getTop() const {
	return boundingBoxRectangle.top();
}

void BoundingBox::setBottom(int bottom) {
	boundingBoxRectangle.setBottom(bottom);
}

int BoundingBox::getBottom() const {
	return boundingBoxRectangle.bottom();
}

QJsonObject BoundingBox::serialize() const {
	QJsonObject serializedBoundingBox;

	serializedBoundingBox["Left"] = getLeft();
	serializedBoundingBox["Right"] = getRight();
	serializedBoundingBox["Top"] = getTop();
	serializedBoundingBox["Bottom"] = getBottom();

	return serializedBoundingBox;
}

BoundingBox BoundingBox::deserialize(const QJsonObject& data) {
	int left = data["Left"].toInt();
	int right = data["Right"].toInt();
	int top = data["Top"].toInt();
	int bottom = data["Bottom"].toInt();

	return BoundingBox(left, right, top, bottom);
}

QRect BoundingBox::changePosition(int x, int y) {
	QPoint offset(x, y);

	boundingBoxRectangle.moveBottomLeft(offset);

	return boundingBoxRectangle;
}
