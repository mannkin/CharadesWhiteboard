import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 1.4
import Whiteboard 1.0

Window {
	visible: true;
	width: 640;
	height: 480;
	title: qsTr("Test host")

	Rectangle {
		id: gui
        width: 640
		height: 25
		color: "#A9A9A9"
		border.color: 'grey'
		border.width: 1

		states: [
			State {
				name: "disconnected"
				when: networkDevice.activeConnections == 0;
				PropertyChanges { target: addressInput; placeholderText: "Enter host name or ip address" }
				PropertyChanges { target: addressInput; textColor: "black"; }
				PropertyChanges { target: addressInput; readOnly: false; }
				PropertyChanges { target: connectButton; text:"Connect"; }
			},
			State {
				name: "connected";
				when: networkDevice.activeConnections > 0;
				PropertyChanges { target: addressInput; text: "Connection estabilished." }
				PropertyChanges { target: addressInput; textColor: "gray"; }
				PropertyChanges { target: addressInput; readOnly: true; }
				PropertyChanges { target: connectButton; text:"Disconnect"; }
			}
		]

		TextField{
			id: addressInput
			width: 500
			height:25
			textColor: "black"
			placeholderText: "IP address..."
			anchors.margins: 20
		}

		Button{
			id: connectButton
			width: 100
			anchors.left: addressInput.right
			anchors.margins: 20

			onClicked: {
				if(gui.state == "connected") {
					networkDevice.disconnectAll();
				} else if(gui.state == "disconnected") {
					networkDevice.connect(addressInput.text);
				}
			}
		}
	}

	Whiteboard {
		id: whiteboard;
		width: parent.width
		height: parent.height - gui.height
		anchors.top: gui.bottom
	}
}
