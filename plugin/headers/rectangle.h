#ifndef RECT_H
#define RECT_H

#include "input_event.h"
#include "shape.h"

class Rectangle : public Shape {
public:
	Rectangle();

	Shape* create() override;
	Shape* clone() override;

	void paint(QPainter* painter) override;
	void manipulate(const InputEvent& event) override;
	void makeBoundingBox();

	QJsonObject serialize() override;
	Shape* deserialize(const QJsonObject& data) override;
};

#endif // RECT_H
