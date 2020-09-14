#ifndef TEXT_H
#define TEXT_H

#include "input_event.h"
#include "shape.h"

class Text : public Shape {
public:
	Text();

	Shape* create() override;
	Shape* clone() override;

	void paint(QPainter* painter) override;
	void manipulate(const InputEvent& event) override;

	QJsonObject serialize() override;
	Shape* deserialize(const QJsonObject& data) override;

private:
	QFont font;
	QString text = "Default text";
	QPoint begin;
	QPoint end;
};

#endif // TEXT_H
