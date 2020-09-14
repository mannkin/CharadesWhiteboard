#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include <QInputEvent>

class InputEvent {
public:
	enum class Type {
		MouseButtonDown,
		MouseButtonUp,
		MouseMovement
	} type;
};

class MouseButtonDown : public InputEvent {
public:
	MouseButtonDown(int x, int y, Qt::MouseButtons button);

	int getX() const;
	int getY() const;
	Qt::MouseButtons getButtons() const;

private:
	int x;
	int y;
	Qt::MouseButtons button;
};

class MouseButtonUp : public InputEvent {
public:
	MouseButtonUp(int x, int y, Qt::MouseButtons button);

	int getX() const;
	int getY() const;
	Qt::MouseButtons getButtons() const;

private:
	int x;
	int y;
	Qt::MouseButtons button;
};

class MouseMovement : public InputEvent {
public:
	MouseMovement(int x, int y);

	int getX() const;
	int getY() const;

private:
	int x;
	int y;
};

#endif // INPUT_EVENT_H
