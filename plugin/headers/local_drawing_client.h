#ifndef LOCALDRAWINGCLIENT_H
#define LOCALDRAWINGCLIENT_H

#include "drawing_client.h"
#include "line.h"
#include "network_event.h"
#include "shape.h"
#include <QObject>
#include <memory>
#include <queue>

class LocalDrawingClient : public DrawingClient {
	Q_OBJECT
public:
	LocalDrawingClient(QObject* parent = nullptr);
	~LocalDrawingClient() override;

	std::unique_ptr<Shape>& getPreview();
	void setPreview(std::unique_ptr<Shape> preview);
	void movePreviewToShapesWaitingToAccept();

	bool isShapeSelected() const;
	void select(int x, int y);
	Shape* getSelectedShape() const;
	void deleteSelectedShape();

	void changeShapePosition(int x, int y);

	void clearCanvas();

	void moveShape(const QString& move);

	std::deque<std::unique_ptr<Shape>>& getShapesWaitingToAccept();
	std::vector<std::unique_ptr<Shape>>& getAcceptedShapes();

	void deleteAllShapes();

signals:
	void requestUpdate();
	void shapeSelected();
	void shapeDeselected();

public slots:
	void onReceiveEventFromServer(const NetworkEvent& event) override;
	void onSendEventToServer(const NetworkEvent& event) override;

public slots:
	void onShapeStyleChanged(const ShapeStyle& shapeStyle);
	void onShapePositionChanged();

protected:
	std::unique_ptr<Shape> shapePreview;
	std::deque<std::unique_ptr<Shape>> shapesWaitingToAccept;
	std::vector<std::unique_ptr<Shape>> shapes;

	Shape* selectedShape = nullptr;
};

#endif // LOCALDRAWINGCLIENT_H
