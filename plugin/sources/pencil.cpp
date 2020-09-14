#include "pencil.h"

Pencil::Pencil() {
}

std::unique_ptr<Shape> Pencil::createNewShape() {
	shapes = Tool::shaper;
	if (shapes == 0) {
		std::unique_ptr<Line> shape = std::make_unique<Line>();
		currentShape = shape.get();
		return shape;
	} else if (shapes == 1) {
		std::unique_ptr<Rectangle> shape = std::make_unique<Rectangle>();
		currentShape = shape.get();
		return shape;
	} else if (shapes == 2) {
		std::unique_ptr<Text> shape = std::make_unique<Text>();
		currentShape = shape.get();
		return shape;
	} else if (shapes == 3) {
		std::unique_ptr<Cactus> shape = std::make_unique<Cactus>();
		currentShape = shape.get();
		return shape;
	} else {
		std::unique_ptr<Line> shape = std::make_unique<Line>();
		currentShape = shape.get();
		return shape;
	}

	//	Q_ASSERT_X(shape != nullptr, __PRETTY_FUNCTION__, " \"line\" is nullptr, allocation failed!");
	//	if (shape == nullptr) {
	//		qFatal("FILE %s FUNCTION %s LINE %d\n ERROR: \"line\" is nullptr, allocation failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	//	}

	//	currentShape = shape.get();
	//	return shape;
}

void Pencil::dispatchEvent(const Event& event) {
	currentShape->manipulate(event);
}
