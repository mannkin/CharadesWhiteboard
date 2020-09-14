import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import Whiteboard 1.0

Item {

	property alias network: mainLogic.networkInterface

	MouseArea {
		id: userInputArea;
//		anchors.fill: parent
		anchors.left: toolbox.right
		anchors.right:  shapeEditor.left
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		acceptedButtons: Qt.LeftButton | Qt.RightButton

		onPressed: { inputBackend.pressedEvent(mouseX, mouseY, pressedButtons); }
		onReleased: { inputBackend.releasedEvent(mouseX, mouseY, pressedButtons); }
		onPositionChanged: { inputBackend.positionChangedEvent(mouseX, mouseY); }
	}

	WhiteboardInputBackend {
		id: inputBackend;
	}

	WhiteboardQuickCanvas {
		id: canvas;
		anchors.left: toolbox.right
		anchors.right:  shapeEditor.left
		anchors.top: parent.top
		anchors.bottom: parent.bottom
	}

	WhiteboardLogic {
		id: mainLogic;
		inputBackend: inputBackend;
		canvas: canvas;
		toolbox: toolbox.model;
		shapeEditor: shapeEditor.model;
		networkInterface: networkDevice;
	}

	Toolbar {
		id: toolbox
		anchors.left: parent.left
		height: parent.height
		width: 40
	}

	Editorbar {
		id: shapeEditor
		anchors.right: parent.right
		height: parent.height
		width: 200
	}

}


