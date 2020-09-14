#include "input_event.h"

MouseButtonDown::MouseButtonDown(int x, int y, Qt::MouseButtons button) :
  x(x),
  y(y),
  button(button) {
	type = InputEvent::Type::MouseButtonDown;
}

int MouseButtonDown::getX() const {
	return x;
}

int MouseButtonDown::getY() const {
	return y;
}

Qt::MouseButtons MouseButtonDown::getButtons() const {
	return button;
}

MouseButtonUp::MouseButtonUp(int x, int y, Qt::MouseButtons button) :
  x(x),
  y(y),
  button(button) {
	type = InputEvent::Type::MouseButtonUp;
}

int MouseButtonUp::getX() const {
	return x;
}

int MouseButtonUp::getY() const {
	return y;
}

Qt::MouseButtons MouseButtonUp::getButtons() const {
	return button;
}

MouseMovement::MouseMovement(int x, int y) :
  x(x),
  y(y) {
	type = InputEvent::Type::MouseMovement;
}

int MouseMovement::getX() const {
	return x;
}

int MouseMovement::getY() const {
	return y;
}
