#include "whiteboard_input_backend.h"

WhiteboardInputBackend::WhiteboardInputBackend() {
	QObject::connect(this, &WhiteboardInputBackend::pressedEvent, this, &WhiteboardInputBackend::onPressedEvent);
	QObject::connect(this, &WhiteboardInputBackend::releasedEvent, this, &WhiteboardInputBackend::onReleasedEvent);
	QObject::connect(this, &WhiteboardInputBackend::positionChangedEvent, this, &WhiteboardInputBackend::onPositionChangedEvent);
}

WhiteboardInputBackend::~WhiteboardInputBackend() {
}

void WhiteboardInputBackend::onPressedEvent(int x, int y, Qt::MouseButtons button) {
	emit inputEvent(MouseButtonDown(x, y, button));
}

void WhiteboardInputBackend::onReleasedEvent(int x, int y, Qt::MouseButtons button) {
	emit inputEvent(MouseButtonUp(x, y, button));
}

void WhiteboardInputBackend::onPositionChangedEvent(int x, int y) {
	emit inputEvent(MouseMovement(x, y));
}
