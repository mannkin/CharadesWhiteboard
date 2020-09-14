import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import Whiteboard 1.0

Item {
	readonly property Toolbox model : model;

	Toolbox {
		id: model;
		Component.onCompleted: { model.selectedShape = "Line"; }
	}

	Rectangle {
		id: backgroundRectangle;
		anchors.fill: parent;
		color: "#A9A9A9"
	}

	Column {
		id: toolsLayout;
		anchors.left: parent.left;
		anchors.right: parent.right;
		spacing: 2;

		RoundButton {
			id: selectionTool;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Selection tool")
			icon.name: "selection-tool";
			icon.source: "round_touch_app_black_18dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.selectedShape = "Select"; }
		}

		RoundButton {
			id: deleteShapeButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Delete selected shape")
			icon.name: "delete";
			icon.source: "baseline_close_black_36dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.deleteSelectedShape() }
		}

		RoundButton {
			id: moveShapeUp;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Move selected shape up")
			icon.name: "moveUp";
			icon.source: "baseline_arrow_upward_black_36dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.moveShapeUp(); }
		}

		RoundButton {
			id: moveShapeDown;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Move selected shape down")
			icon.name: "moveDown";
			icon.source: "baseline_arrow_downward_black_36dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.moveShapeDown(); }
		}
		

		RoundButton {
			id: clearCanvasButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Clear canvas")
			icon.name: "clear";
			icon.source: "baseline_delete_forever_black_36dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.clearCanvas(); }
		}

		RoundButton {
			id: colorPickerButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Color picker")
			icon.name: "color-picker";
			icon.source: "baseline_color_lens_black_36dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { colorDialog.open(); }
		}

		RoundButton {
			id: widthPickerButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Pen width")
			icon.name: "pen-width";
			icon.source: "baseline_edit_black_36dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { penWidthPopup.open(); }
		}

		RoundButton {
			id: lineButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Pencil")
			icon.name: "pencil";
			icon.source: "baseline_brush_black_36dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.selectedShape = qsTr("Line"); }
		}
		
		RoundButton {
			id: ellipseButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Ellipse")
			icon.name: "ellipse"
			icon.source: "baseline_radio_button_unchecked_black_18dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.selectedShape = qsTr("Ellipse"); }
		}
		

		RoundButton {
			id: rectButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Rectangle")
			icon.name: "rectangle";
			icon.source: "baseline_crop_3_2_black_36dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.selectedShape = qsTr("Rectangle"); }
		}

		RoundButton {
			id: cactusButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Cactus")
			icon.name: "cactus";
			icon.source: "baseline_accessible_black_36dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.selectedShape = qsTr("Cactus"); }
		}

		RoundButton {
			id: straightLineButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("StraightLine")
			icon.name: "line";
			icon.source: "baseline_remove_black_18dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.selectedShape = qsTr("StraightLine"); }
		}

		RoundButton {
			id: dashLineButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Dash Line")
			icon.name: "dashline"
			icon.source: "baseline_more_vert_black_18dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.selectedShape = qsTr("DashLine"); }
		}

		RoundButton {
			id: arrowButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Arrow")
			icon.name: "arrow"
			icon.source: "baseline_call_made_black_18dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.selectedShape = qsTr("Arrow"); }
		}


		RoundButton {
			id: textButton;
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Text")
			icon.name: "clear";
			icon.source: "baseline_text_fields_black_36dp";
			anchors.horizontalCenter: parent.horizontalCenter
			background: Rectangle {
				radius: 10
				border.color: "black"
			}

			onClicked: { model.selectedShape = qsTr("Text"); }
		}

		Popup {
			id: penWidthPopup

			SpinBox {
				id: penWidthSpinBox;
				editable: true;
				from: 1;
				to: 50;
				value: 1;

				Component.onCompleted: { model.penWidth = value; }
				onValueModified: { model.penWidth = value; }
			}
		}

		ColorDialog {
			id: colorDialog
			modality: Qt.WindowModal
			title: "Choose a color"
			color: "black"

			Component.onCompleted: { model.penColor = colorDialog.color; }
			onAccepted: { model.penColor = colorDialog.color; }
		}
	}
}

