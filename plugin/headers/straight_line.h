#ifndef STRAIGHT_LINE_H
#define STRAIGHT_LINE_H

#include "input_event.h"
#include "shape.h"

class StraightLine : public Shape {
public:
	StraightLine();

	Shape* create() override;
	Shape* clone() override;

	void paint(QPainter* painter) override;
	void manipulate(const InputEvent& event) override;
	void makeBoundingBox();

	QJsonObject serialize() override;
	Shape* deserialize(const QJsonObject& data) override;
};

#endif // STRAIGHT_LINE_H
