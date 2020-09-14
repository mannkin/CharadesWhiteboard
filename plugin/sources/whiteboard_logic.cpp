#include "whiteboard_logic.h"
#include "network_interface.h"

WhiteboardLogic::WhiteboardLogic() {
	drawingClient = std::make_unique<LocalDrawingClient>();
	if (drawingClient == nullptr) {
		qFatal("FILE %s FUNCTION %s LINE %d\n[FATAL ERROR]: Local client is nullptr, allocation by 'new operator' failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	} else {
		qDebug().noquote().nospace() << "Local drawing client created.";
	}

	QObject::connect(this, &WhiteboardLogic::shapePositionChanged, drawingClient.get(), &LocalDrawingClient::onShapePositionChanged);

	drawingServer = std::make_unique<LocalDrawingServer>();
	if (drawingClient == nullptr) {
		qFatal("FILE %s FUNCTION %s LINE %d\n[FATAL ERROR]: Local client is nullptr, allocation by 'new operator' failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	} else {
		qDebug().noquote().nospace() << "Local drawing server created.";
	}

	drawingServer->registerClient(drawingClient.get());
	qDebug() << "\"WhiteboardLogic\" created.";
}

WhiteboardLogic::~WhiteboardLogic() {
}

void WhiteboardLogic::setInputBackend(WhiteboardInputBackend* inputBackend) {
	if (inputBackend == nullptr) {
		qFatal("FILE %s FUNCTION %s LINE %d\n[FATAL ERROR] :canvas will be assigned nullptr ", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	this->inputBackend = inputBackend;
	qDebug() << "Input backend pointer was written.";

	QObject::connect(this->inputBackend, &WhiteboardInputBackend::inputEvent, this, &WhiteboardLogic::onInputEvent);
}

WhiteboardInputBackend* WhiteboardLogic::getInputBackend() {
	qDebug() << "Input backend pointer was read.";
	return inputBackend;
}

void WhiteboardLogic::setCanvas(WhiteboardQuickCanvas* canvas) {
	if (canvas == nullptr) {
		qFatal("FILE %s FUNCTION %s LINE %d\n[FATAL ERROR]: canvas will be assigned nullptr ", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	this->canvas = canvas;
	qDebug() << "Whiteboard canvas pointer was written.";

	this->canvas->setClient(drawingClient.get());
}

WhiteboardQuickCanvas* WhiteboardLogic::getCanvas() {
	qDebug() << "Whiteboard canvas pointer was read.";
	return canvas;
}

void WhiteboardLogic::setNetworkInterface(NetworkDeviceInterface* networkInterface) {
	if (networkInterface == nullptr) {
		qFatal("FILE %s FUNCTION %s LINE %d\n[FATAL ERROR]: Network interface pointer will be assigned nullptr!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	this->networkInterface = networkInterface;
	qDebug() << "Network pointer was written";

	QObject::connect(networkInterface, &NetworkDeviceInterface::newConnectionIncoming, this, &WhiteboardLogic::onNewConnectionIncoming);
	QObject::connect(networkInterface, &NetworkDeviceInterface::newConnectionOutgoing, this, &WhiteboardLogic::onNewConnectionOutgoing);
	QObject::connect(networkInterface, &NetworkDeviceInterface::disconnectedAll, this, &WhiteboardLogic::onDisconnected);
}

NetworkDeviceInterface* WhiteboardLogic::getNetworkInterface() {
	qDebug() << "Network pointer was read.";
	return networkInterface;
}

void WhiteboardLogic::setToolbox(Toolbox* toolbox) {
	if (toolbox == nullptr) {
		qFatal("[FILE %s, FUNCTION %s, LINE %d]\n[FATAL ERROR]: Network interface pointer will be assigned nullptr!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	this->toolbox = toolbox;
	qDebug() << "Toolbox pointer was written.";

	this->toolbox->setClient(drawingClient.get());
}

Toolbox* WhiteboardLogic::getToolbox() {
	qDebug() << "Toolbox pointer was read.";
	return toolbox;
}

void WhiteboardLogic::setShapeEditor(ShapeEditor* shapeEditor) {
	if (shapeEditor == nullptr) {
		qFatal("[FILE %s, FUNCTION %s, LINE %d]\n[FATAL ERROR]: Network interface pointer will be assigned nullptr!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	this->shapeEditor = shapeEditor;
	qDebug() << "ShapeEditor pointer was written.";

	this->shapeEditor->setClient(this->drawingClient.get());

	QObject::connect(drawingClient.get(), &LocalDrawingClient::shapeSelected, this->shapeEditor, &ShapeEditor::onShapeSelected);
	QObject::connect(shapeEditor, &ShapeEditor::shapeStyleChanged, drawingClient.get(), &LocalDrawingClient::onShapeStyleChanged);
}

ShapeEditor* WhiteboardLogic::getShapeEditor() {
	qDebug() << "ShapeEditor pointer was read.";
	return shapeEditor;
}

std::unique_ptr<Shape> WhiteboardLogic::getNewShape() {
	QString shapeName = toolbox->getSelectedShape();

	return std::unique_ptr<Shape>(Shape::createShapeByName(shapeName));
}

void WhiteboardLogic::onInputEvent(const InputEvent& event) {
	switch (event.type) {
	case InputEvent::Type::MouseButtonDown: {
		const MouseButtonDown& e = static_cast<const MouseButtonDown&>(event);

		switch (e.getButtons()) {
		case Qt::LeftButton: {
			switch (drawingClient->isShapeSelected()) {
			case true: {

				drawingClient->changeShapePosition(e.getX(), e.getY());
				break;
			}
			case false: {
				if (toolbox == nullptr) {
					qFatal("Toolbox pointer is null!");
				}

				drawingClient->setPreview(getNewShape());

				// TODO: unique_ptr vs naked pointer
				std::unique_ptr<Shape> shape = std::move(drawingClient->getPreview());

				ShapeStyle shapeStyle = shape->getShapeStyle();
				shapeStyle.setPenColor(toolbox->getPenColor());
				shapeStyle.setPenWidth(toolbox->getPenWidth());
				shape->setShapeStyle(shapeStyle);

				shape->manipulate(event);
				drawingClient->setPreview(std::move(shape));
				break;
			}
			}
			break;
		}
		case Qt::RightButton: {
			drawingClient->select(e.getX(), e.getY());
			break;
		}
		}

		break;
	}

	case InputEvent::Type::MouseMovement: {

		switch (drawingClient->isShapeSelected()) {
		case true: {
			const MouseMovement& e = static_cast<const MouseMovement&>(event);
			drawingClient->changeShapePosition(e.getX(), e.getY());
			break;
		}
		case false: {
			break;
		}
		}
		std::unique_ptr<Shape> shape = std::move(drawingClient->getPreview());
		if (shape != nullptr) {
			shape->manipulate(event);
		}
		drawingClient->setPreview(std::move(shape));
		break;
	}

	case InputEvent::Type::MouseButtonUp: {
		const MouseButtonUp& e = static_cast<const MouseButtonUp&>(event);
		switch (e.getButtons()) {
		case Qt::LeftButton: {
			break;
		}
		case Qt::RightButton: {
			break;
		}
		}
		switch (drawingClient->isShapeSelected()) {
		case true: {
			emit shapePositionChanged();
			break;
		}
		case false: {
			std::unique_ptr<Shape> shape = std::move(drawingClient->getPreview());
			if (shape != nullptr) {
				shape->manipulate(event);
			}
			drawingClient->setPreview(std::move(shape));

			drawingClient->movePreviewToShapesWaitingToAccept();
			break;
		}
		}
		break;
	}

	default: {
		qFatal("Event type unknown!");
	}
	}
}

void WhiteboardLogic::onNewConnectionIncoming(ConnectionHandleInterface* connectionHandle) {
	qDebug().noquote().nospace() << "New connection incoming.";
	std::unique_ptr<ConnectionHandleInterface> connection(connectionHandle);

	switch (state) {
	case State::DISCONNECTED: {
		state = State::SERVER;

		VirtualDrawingClient* client = virtualDrawingClients.emplace_back(new VirtualDrawingClient(std::move(connection))).get();
		qDebug().noquote().nospace() << "New client connected - " << client;

		drawingClient->deleteAllShapes();
		drawingServer->registerClient(client);

		break;
	}

	case State::CLIENT: {
		connectionHandle->disconnect();

		drawingClient->deleteAllShapes();
		drawingServer.reset(new LocalDrawingServer());
		drawingServer->registerClient(drawingClient.get());

		break;
	}

	case State::SERVER: {
		VirtualDrawingClient* client = virtualDrawingClients.emplace_back(new VirtualDrawingClient(std::move(connection))).get();
		qDebug().noquote().nospace() << "New client connected - " << client;

		drawingServer->registerClient(client);

		break;
	}
	}

	qDebug().noquote().nospace() << "Current state: " << getCurrentState();
}

void WhiteboardLogic::onNewConnectionOutgoing(ConnectionHandleInterface* connectionHandle) {
	qDebug().noquote().nospace() << "New connection outgoing.";
	std::unique_ptr<ConnectionHandleInterface> connection(connectionHandle);

	state = State::CLIENT;
	drawingServer.reset(new VirtualDrawingServer(std::move(connection)));

	drawingClient->deleteAllShapes();
	drawingServer->registerClient(drawingClient.get());

	qDebug().noquote().nospace() << "Current state: " << getCurrentState();
}

void WhiteboardLogic::onDisconnected() {
	state = State::DISCONNECTED;

	drawingServer.reset(new LocalDrawingServer());

	drawingClient->deleteAllShapes();
	drawingServer->registerClient(drawingClient.get());

	qDebug().noquote().nospace() << "Current state: " << getCurrentState();
}

QString WhiteboardLogic::getCurrentState() const {
	switch (state) {
	case State::DISCONNECTED: {
		return "DISCONNECTED";
	}
	case State::CLIENT: {
		return "CLIENT";
	}
	case State::SERVER: {
		return "SERVER";
	}
	}

	return "INVALID_STATE";
}
