import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import com.task3.controllers 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    title: "Task3"

    HullController {
        id: hullController
        onComputationFinished: canvas.requestPaint()
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            Button { text: "Load"; onClicked: fileLoadDialog.open() }
            Button { text: "Save"; onClicked: fileSaveDialog.open() }
            Button { text: "Clear"; onClicked: { hullController.clear(); view.resetView(); } }
            Label { text: "Gamma:"; verticalAlignment: Text.AlignVCenter }
            SpinBox {
                id: gammaSpinBox
                from: 0.0; to: 10.0; editable: true; stepSize: 1
                value: hullController.gamma
                onValueChanged: hullController.gamma = value
            }
            Button { id: addPointButton; text: "Add Points"; checkable: true }
        }
    }

    Item {
        id: view
        anchors.fill: parent
        clip: true

        property real viewScale: 1.0
        property real viewOffsetX: 0.0
        property real viewOffsetY: 0.0

        function resetView() {
            view.viewScale = 1.0;
            view.viewOffsetX = 0.0;
            view.viewOffsetY = 0.0;
            canvas.requestPaint();
        }

        function fitView() {
            var points = hullController.pointsModel;
            if (points.length === 0) { resetView(); return; }
            var minX = points[0].x, maxX = points[0].x, minY = points[0].y, maxY = points[0].y;
            for (var i = 1; i < points.length; i++) {
                minX = Math.min(minX, points[i].x);
                maxX = Math.max(maxX, points[i].x);
                minY = Math.min(minY, points[i].y);
                maxY = Math.max(maxY, points[i].y);
            }
            var bboxWidth = maxX - minX;
            var bboxHeight = maxY - minY;
            if (bboxWidth < 1) bboxWidth = 100;
            if (bboxHeight < 1) bboxHeight = 100;
            var totalWidth = bboxWidth * 1.2;
            var totalHeight = bboxHeight * 1.2;
            var scaleX = view.width / totalWidth;
            var scaleY = view.height / totalHeight;

            view.viewScale = Math.min(scaleX, scaleY);
            var contentCenterX = minX + bboxWidth / 2;
            var contentCenterY = minY + bboxHeight / 2;
            view.viewOffsetX = (view.width / 2) - (contentCenterX * view.viewScale);
            view.viewOffsetY = (view.height / 2) - (contentCenterY * view.viewScale);

            canvas.requestPaint();
        }

        Canvas {
            id: canvas
            anchors.fill: parent

            Connections {
                target: hullController
                function onPointsModelChanged() {
                    view.fitView();
                }
                function onHullModelChanged() {
                    canvas.requestPaint();
                }
            }

            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, canvas.width, canvas.height);
                ctx.save();

                ctx.translate(view.viewOffsetX, view.viewOffsetY);
                ctx.scale(view.viewScale, view.viewScale);
                var invScale = 1.0 / view.viewScale;

                ctx.fillStyle = "black";
                var points = hullController.pointsModel;
                for (var i = 0; i < points.length; i++) {
                    ctx.beginPath();
                    ctx.arc(points[i].x, points[i].y, 3 * invScale, 0, 2 * Math.PI);
                    ctx.fill();
                }

                var hull = hullController.hullModel;
                if (hull.length > 1) {
                    ctx.strokeStyle = "blue";
                    ctx.lineWidth = 2 * invScale;
                    ctx.beginPath();
                    ctx.moveTo(hull[0].x, hull[0].y);
                    for (var j = 1; j < hull.length; j++) {
                        ctx.lineTo(hull[j].x, hull[j].y);
                    }
                    ctx.closePath();
                    ctx.stroke();
                }
                ctx.restore();
            }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            property point lastMousePos;

            onPressed: { lastMousePos = Qt.point(mouse.x, mouse.y); }
            onPositionChanged: {
                if (pressedButtons & Qt.LeftButton) {
                    view.viewOffsetX += (mouse.x - lastMousePos.x);
                    view.viewOffsetY += (mouse.y - lastMousePos.y);
                    lastMousePos = Qt.point(mouse.x, mouse.y);
                    canvas.requestPaint();
                }
            }
            onWheel: {
                var zoomFactor = 1.1;
                var oldScale = view.viewScale;
                var newScale = (wheel.angleDelta.y > 0) ? oldScale * zoomFactor : oldScale / zoomFactor;
                view.viewScale = newScale;

                view.viewOffsetX = mouse.x - (mouse.x - view.viewOffsetX) * (newScale / oldScale);
                view.viewOffsetY = mouse.y - (mouse.y - view.viewOffsetY) * (newScale / oldScale);
                canvas.requestPaint();
            }
            onClicked: {
                if (addPointButton.checked && mouse.button === Qt.RightButton) {
                    var canvasX = (mouse.x - view.viewOffsetX) / view.viewScale;
                    var canvasY = (mouse.y - view.viewOffsetY) / view.viewScale;
                    hullController.addPoint(canvasX, canvasY);
                }
            }
        }
    }

    FileDialog {
        id: fileLoadDialog
        title: "Load Points"; fileMode: FileDialog.OpenFile
        folder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        nameFilters: ["Text files (*.txt)"]
        onAccepted: hullController.loadPointsFromFile(file)
    }

    FileDialog {
        id: fileSaveDialog
        title: "Save Hull"; fileMode: FileDialog.SaveFile
        folder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        nameFilters: ["Text files (*.txt)"]
        onAccepted: hullController.saveHullToFile(file)
    }
}
