#ifndef WHITEBOARD_INPUT_BACKEND_H
#define WHITEBOARD_INPUT_BACKEND_H

#include <QDebug>
#include <QObject>

#include "input_event.h"

class WhiteboardInputBackend : public QObject {
	Q_OBJECT

public:
	WhiteboardInputBackend();
	~WhiteboardInputBackend();
public slots:
	void onPressedEvent(int x, int y, Qt::MouseButtons button);
	void onReleasedEvent(int x, int y, Qt::MouseButtons button);
	void onPositionChangedEvent(int x, int y);

signals:
	void pressedEvent(int x, int y, Qt::MouseButtons button);
	void releasedEvent(int x, int y, Qt::MouseButtons button);
	void positionChangedEvent(int x, int y);

	void inputEvent(const InputEvent& event);
};

#endif // WHITEBOARD_INPUT_BACKEND_H
