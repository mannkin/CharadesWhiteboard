#ifndef DASH_LINE_H
#define DASH_LINE_H

#include "input_event.h"
#include "shape.h"

class DashLine : public Shape {
public:
	DashLine();
	Shape* create() override;
	Shape* clone() override;

	void paint(QPainter* painter) override;
	void manipulate(const InputEvent& event) override;
	void makeBoundingBox();

	QJsonObject serialize() override;
	Shape* deserialize(const QJsonObject& data) override;
};

#endif // DASH_LINE_H
