#include <qqml.h>

#include "shape_editor.h"
#include "whiteboard_input_backend.h"
#include "whiteboard_logic.h"
#include "whiteboard_plugin.h"
#include "whiteboard_quick_canvas.h"

void WhiteboardPlugin::registerTypes(const char* uri) {
	qDebug() << "Registering QML types...";

	qDebug() << "Registering \"WhiteboardLogic\"...";
	if (qmlRegisterType<WhiteboardLogic>(uri, 1, 0, "WhiteboardLogic") < 0) {
		qFatal("FILE %s FUNCTION %s LINE %d\nERROR: Registering of \"WhiteboardLogic\" failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	qDebug() << "Registering \"WhiteboardQuickCanvas\"...";
	if (qmlRegisterType<WhiteboardQuickCanvas>(uri, 1, 0, "WhiteboardQuickCanvas") < 0) {
		qFatal("FILE %s FUNCTION %s LINE %d\nERROR: Registering of \"WhiteboardQuickCanvas\" failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	qDebug() << "Registering\" WhiteboardInputBackend\"...";
	if (qmlRegisterType<WhiteboardInputBackend>(uri, 1, 0, "WhiteboardInputBackend") < 0) {
		qFatal("FILE %s FUNCTION %s LINE %d\nERROR: Registering of \"WhiteboardInputBackend\" failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	qDebug() << "Registering \"Toolbox\"...";
	if (qmlRegisterType<Toolbox>(uri, 1, 0, "Toolbox") < 0) {
		qFatal("FILE %s FUNCTION %s LINE %d\nERROR: Registering of \"Toolbox\" failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	qDebug() << "Registering \"Toolbar.qml\"...";
	if (qmlRegisterType(QUrl("qrc:/Toolbar.qml"), uri, 1, 0, "Toolbar") < 0) {
		qFatal("FILE %s FUNCTION %s LINE %d\nERROR: Registering of \"Toolbar.qml\" failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	qDebug() << "Registering \"ShapeEditor\"...";
	if (qmlRegisterType<ShapeEditor>(uri, 1, 0, "ShapeEditor") < 0) {
		qFatal("FILE %s FUNCTION %s LINE %d\nERROR: Registering of \"ShapeEditor\" failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	qDebug() << "Registering \"Editorbar.qml\"...";
	if (qmlRegisterType(QUrl("qrc:/Editorbar.qml"), uri, 1, 0, "Editorbar") < 0) {
		qFatal("FILE %s FUNCTION %s LINE %d\nERROR: Registering of \"Editorbar.qml\" failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	qDebug() << "Registering \"Whiteboard.qml\"...";
	if (qmlRegisterType(QUrl("qrc:/Whiteboard.qml"), uri, 1, 0, "Whiteboard") < 0) {
		qFatal("FILE %s FUNCTION %s LINE %d\nERROR: Registering of \"Whiteboard.qml\" failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}
}
