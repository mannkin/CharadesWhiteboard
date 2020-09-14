#ifndef SHAPE_STYLE_H
#define SHAPE_STYLE_H

#include <QBrush>
#include <QJsonObject>
#include <QPen>

class ShapeStyle {
public:
	ShapeStyle();
	ShapeStyle(const QPen& pen, const QBrush& brush);

	QJsonObject serialize() const;
	static ShapeStyle deserialize(const QJsonObject& data);

	QPen getPen() const;
	QBrush getBrush() const;

	void setPenWidth(int width);
	int getPenWidth() const;

	void setPenColor(const QColor& color);
	QColor getPenColor() const;

	void setPenStyle(Qt::PenStyle penStyle);
	Qt::PenStyle getPenStyle() const;

	void setPenCapStyle(Qt::PenCapStyle penCapStyle);
	Qt::PenCapStyle getPenCapStyle() const;

	void setPenJoinStyle(Qt::PenJoinStyle penJoinStyle);
	Qt::PenJoinStyle getPenJoinStyle() const;

	void setBrushColor(const QColor& color);
	QColor getBrushColor() const;

	void setBrushStyle(Qt::BrushStyle brushStyle);
	Qt::BrushStyle getBrushStyle() const;

private:
	QPen shapePen;
	QBrush shapeBrush;
};

#endif // SHAPE_STYLE_H
