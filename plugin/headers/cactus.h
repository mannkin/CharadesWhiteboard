#ifndef CACTUS_H
#define CACTUS_H

#include "input_event.h"
#include "shape.h"

class Cactus : public Shape {
public:
	Cactus();

	Shape* create() override;
	Shape* clone() override;

	void paint(QPainter* painter) override;
	void manipulate(const InputEvent& event) override;
	void makeBoundingBox();

	QJsonObject serialize() override;
	Shape* deserialize(const QJsonObject& data) override;

private:
	QPainterPath path;
};

#endif // CACTUS_H
