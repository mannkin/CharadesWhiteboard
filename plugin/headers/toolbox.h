#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QQuickItem>
#include <input_event.h>
#include <string>

#include "local_drawing_client.h"
#include "shape.h"

class Toolbox : public QQuickItem {
	Q_OBJECT
	Q_PROPERTY(QString selectedShape READ getSelectedShape WRITE setSelectedShape NOTIFY selectedShapeChanged)
	Q_PROPERTY(unsigned penWidth READ getPenWidth WRITE setPenWidth NOTIFY penWidthChanged)
	Q_PROPERTY(QColor penColor READ getPenColor WRITE setPenColor NOTIFY penColorChanged)

public:
	Toolbox(QQuickItem* parent = nullptr);
	~Toolbox();

	void setClient(LocalDrawingClient* client);

	void setSelectedShape(QString selectedShapeName);
	QString getSelectedShape();

	void setPenWidth(unsigned penWidth);
	unsigned getPenWidth();

	void setPenColor(QColor penColor);
	QColor getPenColor();

	Q_INVOKABLE void deleteSelectedShape();
	Q_INVOKABLE void clearCanvas();
	Q_INVOKABLE void moveShapeUp();
	Q_INVOKABLE void moveShapeDown();

signals:
	void selectedShapeChanged();
	void penWidthChanged();
	void penColorChanged();

private:
	LocalDrawingClient* client;

	QString selectedShape;
	unsigned penWidth;
	QColor penColor;
};

#endif // TOOLBOX_H
