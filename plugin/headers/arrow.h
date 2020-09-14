#ifndef ARROW_H
#define ARROW_H

#include "input_event.h"
#include "shape.h"

class Arrow : public Shape {
public:
	Arrow();
	Shape* create() override;
	Shape* clone() override;

	void paint(QPainter* painter) override;
	void manipulate(const InputEvent& event) override;
	void makeBoundingBox();

	QJsonObject serialize() override;
	Shape* deserialize(const QJsonObject& data) override;
};

#endif // ARROW_H
