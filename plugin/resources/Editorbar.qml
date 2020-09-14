import Whiteboard 1.0
import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3

Item {
	readonly property ShapeEditor model: model;

	ShapeEditor {
		id: model
	}

	Rectangle {
		id: backgroundRectangle
		anchors.fill: parent
		color: "#A9A9A9"
	}

	Column {
		id: optionsLayout;
		anchors.left: parent.left;
		anchors.right: parent.right;
		spacing: 2;

		RowLayout {
			id: penWidthOptions;
			anchors.left: parent.left;
			anchors.right: parent.right;

			Label {
				id: penWidthLabel
				text: "Pen width: "
				Layout.leftMargin: 5.0;
			}

			Slider {
				id: penWidthSlider;
				from: 1.0;
				to: 50.0;
				stepSize: 1.0;
				snapMode: Slider.SnapOnRelease;
				value: model.penWidth;
				onValueChanged: { model.penWidth = value; }
				Layout.fillWidth: true;

				background: Rectangle {
					x: penWidthSlider.leftPadding
					y: penWidthSlider.topPadding + penWidthSlider.availableHeight / 2 - height / 2
					implicitWidth: 200
					implicitHeight: 4
					width: penWidthSlider.availableWidth
					height: implicitHeight
					radius: 2
					color: "#bdbebf"

					Rectangle {
						width: penWidthSlider.visualPosition * parent.width
						height: parent.height
						color: "#21be2b"
						radius: 2
					}
				}

				handle: Rectangle {
					x: penWidthSlider.leftPadding + penWidthSlider.visualPosition * (penWidthSlider.availableWidth - width)
					y: penWidthSlider.topPadding + penWidthSlider.availableHeight / 2 - height / 2
					implicitWidth: 16
					implicitHeight: 16
					radius: 8
					color: penWidthSlider.pressed ? "#f0f0f0" : "#f6f6f6"
					border.color: "#bdbebf"
				}
			}
		}
		RowLayout {
			id: penColorOptions;
			anchors.left: parent.left;
			anchors.right: parent.right;

			Label {
				id: penColorLabel;
				text: "Pen color: "
				Layout.leftMargin: 5.0;
			}

			RoundButton {
				id: deleteShapeButton;
				ToolTip.visible: hovered;
				ToolTip.text: qsTr("Change shape color");

				background: Rectangle {
					radius: 10;
					border.color: "black";
					color: model.penColor;
				}

				Layout.rightMargin: 5.0;
				Layout.fillWidth: true;

				onClicked: { colorDialog.open(); }
			}
		}
	}

	ColorDialog {
		id: colorDialog;
		modality: Qt.WindowModal;
		title: "Choose a color";
		color: "black";

		onAccepted: { model.penColor = colorDialog.color; }
	}
}

