#ifndef LINE_H
#define LINE_H

#include "input_event.h"
#include "shape.h"

class Line : public Shape {

public:
	Line();

	Shape* create() override;
	Shape* clone() override;

	void paint(QPainter* painter) override;
	void manipulate(const InputEvent& event) override;
	void makeBoundingBox();
	QJsonObject serialize() override;
	Shape* deserialize(const QJsonObject& data) override;
};

#endif // LINE_H
