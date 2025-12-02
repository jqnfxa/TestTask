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

            Button {
                text: "Load"
                onClicked: fileLoadDialog.open()
            }
            Button {
                text: "Save"
                onClicked: fileSaveDialog.open()
            }
            Button {
                text: "Clear"
                onClicked: hullController.clear()
            }

            Label {
                text: "Gamma:"
                verticalAlignment: Label.AlignVCenter
            }
            SpinBox {
                id: gammaSpinBox
                from: 0.0
                to: 10.0
                editable: true
                stepSize: 1
                value: hullController.gamma
                onValueChanged: hullController.gamma = value
            }

            Button {
                id: addPointButton
                text: "Add Points"
                checkable: true
            }
        }
    }

    Canvas {
        id: canvas
        anchors.fill: parent

        Connections {
            target: hullController
            function onPointsModelChanged() { canvas.requestPaint() }
            function onHullModelChanged() { canvas.requestPaint() }
        }

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, canvas.width, canvas.height);

            ctx.fillStyle = "black";
            var points = hullController.pointsModel;
            for (var i = 0; i < points.length; i++) {
                ctx.beginPath();
                ctx.arc(points[i].x, points[i].y, 3, 0, 2 * Math.PI);
                ctx.fill();
            }

            var hull = hullController.hullModel;
            if (hull.length > 1) {
                ctx.strokeStyle = "blue";
                ctx.lineWidth = 2;
                ctx.beginPath();
                ctx.moveTo(hull[0].x, hull[0].y);
                for (var j = 1; j < hull.length; j++) {
                    ctx.lineTo(hull[j].x, hull[j].y);
                }
                ctx.closePath();
                ctx.stroke();
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (addPointButton.checked) {
                    hullController.addPoint(mouse.x, mouse.y);
                }
            }
        }
    }

    FileDialog {
        id: fileLoadDialog
        title: "Load Points"
        folder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        nameFilters: ["Text files (*.txt)"]
        onAccepted: hullController.loadPointsFromFile(file)
    }

    FileDialog {
        id: fileSaveDialog
        title: "Save Hull"
        folder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        nameFilters: ["Text files (*.txt)"]
        onAccepted: hullController.saveHullToFile(file)
    }
}
