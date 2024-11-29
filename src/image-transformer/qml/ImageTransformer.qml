import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.platform

import com.lunacd.qml
import com.lunacd.imageTransformer

ApplicationWindow
{
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Image Transformer")

    Connections {
        target: ImageTransformerQtBridge
        function onTransformCompleted() {
            statusText.text = "Finished! Please check the converted directory next to you images.";
            console.log("Completed");
        }
    }

    Rectangle {
        id: page
        anchors.fill: parent
        color: "lightgray"

        ColumnLayout {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 8
            anchors.leftMargin: 8
            anchors.rightMargin: 8

            LunacdLabeledInput {
                id: targetWidthInput
                label: "Target image width"
                Layout.fillWidth: true
            }

            LunacdButton {
                text: "Add Image"
                onClicked: fileDialog.visible = true;
            }

            Repeater {
                id: repeater
                model: ListModel { }
                Text {
                    required property string fileName
                    text: fileName
                }
            }

            LunacdButton {
                text: "Convert"
                onClicked: () => {
                    const images = [];
                    for (let i =  0; i < repeater.model.count; i++) {
                        images.push(repeater.model.get(i).file);
                    }
                    ImageTransformerQtBridge.transformImages(images, targetWidthInput.text);
                    statusText.text = "Running conversion. Please wait.";
                }
            }

            Text {
                id: statusText
                text: ""
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose where to save to"
        fileMode: FileDialog.OpenFiles
        onAccepted: () => {
            for (const chosenFile of files) {
                const fileName = chosenFile.toString().split("/").pop();
                repeater.model.append({"file": chosenFile, "fileName": fileName});
            }
        }
    }
}