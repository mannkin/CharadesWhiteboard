#ifndef WHITEBOARD_LOGIC_H
#define WHITEBOARD_LOGIC_H

#include "local_drawing_client.h"
#include "local_drawing_server.h"
#include "network_interface.h"
#include "shape.h"
#include "shape_editor.h"
#include "toolbox.h"
#include "virtual_drawing_client.h"
#include "virtual_drawing_server.h"
#include "whiteboard_input_backend.h"
#include "whiteboard_quick_canvas.h"
#include <QDebug>
#include <QObject>
#include <QString>
#include <memory>
#include <vector>

class WhiteboardLogic : public QObject {
	Q_OBJECT
	Q_PROPERTY(WhiteboardInputBackend* inputBackend READ getInputBackend WRITE setInputBackend NOTIFY inputBackendChanged)
	Q_PROPERTY(NetworkDeviceInterface* networkInterface READ getNetworkInterface WRITE setNetworkInterface NOTIFY networkInterfaceChanged)
	Q_PROPERTY(WhiteboardQuickCanvas* canvas READ getCanvas WRITE setCanvas NOTIFY canvasChanged)
	Q_PROPERTY(Toolbox* toolbox READ getToolbox WRITE setToolbox NOTIFY toolboxChanged)
	Q_PROPERTY(ShapeEditor* shapeEditor READ getShapeEditor WRITE setShapeEditor NOTIFY shapeEditorChanged)

private:
	enum class State {
		DISCONNECTED,
		CLIENT,
		SERVER
	};

public:
	WhiteboardLogic();
	~WhiteboardLogic();

	void setInputBackend(WhiteboardInputBackend* inputBackend);
	WhiteboardInputBackend* getInputBackend();

	void setCanvas(WhiteboardQuickCanvas* canvas);
	WhiteboardQuickCanvas* getCanvas();

	void setNetworkInterface(NetworkDeviceInterface* value);
	NetworkDeviceInterface* getNetworkInterface();

	void setToolbox(Toolbox* value);
	Toolbox* getToolbox();

	void setShapeEditor(ShapeEditor* value);
	ShapeEditor* getShapeEditor();

	std::unique_ptr<Shape> getNewShape();

signals:
	void inputBackendChanged();
	void canvasChanged();
	void networkInterfaceChanged();
	void toolboxChanged();
	void shapeEditorChanged();
	void shapePositionChanged();
	void clear();

private slots:
	void onInputEvent(const InputEvent& event);
	void onNewConnectionIncoming(ConnectionHandleInterface* connectionHandle);
	void onNewConnectionOutgoing(ConnectionHandleInterface* connectionHandle);
	void onDisconnected(); // TODO: Is this line necessary

private:
	QString getCurrentState() const;

private:
	WhiteboardInputBackend* inputBackend = nullptr;
	WhiteboardQuickCanvas* canvas = nullptr;
	NetworkDeviceInterface* networkInterface = nullptr;
	Toolbox* toolbox = nullptr;
	ShapeEditor* shapeEditor = nullptr;

	std::unique_ptr<DrawingServer> drawingServer;
	std::unique_ptr<LocalDrawingClient> drawingClient;
	std::vector<std::unique_ptr<VirtualDrawingClient>> virtualDrawingClients;

	State state = State::DISCONNECTED;
};

#endif // WHITEBOARD_LOGIC_H
