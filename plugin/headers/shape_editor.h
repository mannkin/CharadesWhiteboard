#ifndef SHAPE_EDITOR_H
#define SHAPE_EDITOR_H

#include "local_drawing_client.h"
#include "shape.h"
#include <QQuickItem>

class ShapeEditor : public QQuickItem {
	Q_OBJECT
	Q_PROPERTY(int penWidth READ getPenWidth WRITE setPenWidth NOTIFY updateGui)
	Q_PROPERTY(QColor penColor READ getPenColor WRITE setPenColor NOTIFY updateGui)
public:
	ShapeEditor(QQuickItem* parent = nullptr);
	~ShapeEditor();

	void setClient(LocalDrawingClient* client);

	void setPenWidth(int penWidth);
	int getPenWidth();

	void setPenColor(const QColor& penColor);
	QColor getPenColor();

public slots:
	void onShapeSelected();

signals:
	void penWidthChanged();
	void penColorChanged();
	void updateGui();
	void shapeStyleChanged(const ShapeStyle& shapeStyle);

private:
	LocalDrawingClient* client = nullptr;
	ShapeStyle shapeStyle;
};

#endif // SHAPE_EDITOR_H
