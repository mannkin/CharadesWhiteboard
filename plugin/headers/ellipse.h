#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "input_event.h"
#include "shape.h"

class Ellipse : public Shape {
public:
	Ellipse();
	Shape* create() override;
	Shape* clone() override;

	void paint(QPainter* painter) override;
	void manipulate(const InputEvent& event) override;
	void makeBoundingBox();

	QJsonObject serialize() override;
	Shape* deserialize(const QJsonObject& data) override;
};

#endif // ELLIPSE_H
