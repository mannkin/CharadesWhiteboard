#include "virtual_drawing_client.h"
#include "network_event.h"
#include "network_interface.h"

VirtualDrawingClient::VirtualDrawingClient(std::unique_ptr<ConnectionHandleInterface> connection, QObject* parent) :
  DrawingClient(parent),
  connectionHandle(std::move(connection)) {
	QObject::connect(this, &VirtualDrawingClient::sendEventToServer, this, &VirtualDrawingClient::onSendEventToServer);
	QObject::connect(connectionHandle.get(), &ConnectionHandleInterface::dataReady, this, &VirtualDrawingClient::onNewDataAvailable);
	QObject::connect(connectionHandle.get(), &ConnectionHandleInterface::disconnected, this, &VirtualDrawingClient::disconnected);
}

VirtualDrawingClient::~VirtualDrawingClient() {
}

void VirtualDrawingClient::onSendEventToServer(const NetworkEvent& event) {
	Q_UNUSED(event);
}

void VirtualDrawingClient::onNewDataAvailable() {
	static std::vector<std::uint8_t> buffer;
	QJsonDocument json;

	std::vector<std::uint8_t> dataRead = connectionHandle->readData();
	buffer.insert(buffer.end(), dataRead.begin(), dataRead.end());

	if (!((json = QJsonDocument::fromJson(QByteArray::fromRawData(reinterpret_cast<const char*>(buffer.data()), buffer.size()))).isNull())) {
		QString eventType = json.object()["EventType"].toString();

		if (eventType == "AddObject") {
			AddObject event(nullptr);
			event.deserialize(json.toJson(QJsonDocument::Compact));
			emit sendEventToServer(event);
		}

		if (eventType == "DeleteShape") {
			DeleteShape event;
			event.deserialize(json.toJson(QJsonDocument::Compact));
			emit sendEventToServer(event);
		}

		if (eventType == "ClearAll") {
			emit sendEventToServer(ClearAll());
		}

		if (eventType == "ShapeModified") {
			ShapeModified event;
			event.deserialize(json.toJson(QJsonDocument::Compact));
			emit sendEventToServer(event);
		}

		if (eventType == "MoveShape") {
			MoveShape event;
			event.deserialize(json.toJson(QJsonDocument::Compact));
			emit sendEventToServer(event);
		}

		buffer.clear();
	}
}

void VirtualDrawingClient::onReceiveEventFromServer(const NetworkEvent& event) {
	QByteArray serializedEvent = event.serialize();
	connectionHandle->writeData(std::vector<std::uint8_t>(serializedEvent.begin(), serializedEvent.end()));
}
