#ifndef WHITEBOARD_QUICK_CANVAS_H
#define WHITEBOARD_QUICK_CANVAS_H

#include <QPainter>
#include <QtQuick/QQuickPaintedItem>
#include <memory>

#include "local_drawing_client.h"
#include "shape.h"

class WhiteboardQuickCanvas : public QQuickPaintedItem {
	Q_OBJECT

public:
	WhiteboardQuickCanvas(QQuickItem* parent = nullptr);
	void paint(QPainter* painter) override;
	void setClient(LocalDrawingClient* client);

public slots:
	void update();

private:
	LocalDrawingClient* client = nullptr;
};

#endif // WHITEBOARD_QUICK_CANVAS_H
