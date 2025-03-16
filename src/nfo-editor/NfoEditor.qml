import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.platform

import libui

ApplicationWindow
{
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("NFO Editor")

    function clearFields() {
        title.text = ""
        studio.text = ""
        actors.items = []
        actors.text = ""
        tags.items = []
        tags.text = ""
    }

    Rectangle {
        id: page
        anchors.fill: parent
        color: "lightgray"

        ColumnLayout {
            id: column
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 8
            spacing: 8

            LunacdLabeledInput {
                id: title
                
                label: "Title"
                Layout.fillWidth: true
                focus: true
                KeyNavigation.tab: studio

                autocomplete: false
            }

            LunacdLabeledInput {
                id: studio

                label: "Studio"
                Layout.fillWidth: true
                KeyNavigation.tab: actors

                autocomplete: true
                completionSource: "studio"
            }

            NfoEditorLabeledListInput {
                id: actors

                label: "Actors"
                Layout.fillWidth: true
                KeyNavigation.tab: tags

                autocomplete: true
                completionSource: "actor"
            }

            NfoEditorLabeledListInput {
                id: tags

                label: "Tags"
                Layout.fillWidth: true

                autocomplete: true
                completionSource: "tag"
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                LunacdButton {
                    text: "Clear"
                    onClicked: () => {
                        window.clearFields();
                    }
                }

                LunacdButton {
                    text: "Save"
                    onClicked: () => {
                        fileDialog.visible = true;
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose where to save to"
        fileMode: FileDialog.SaveFile
        onAccepted: () => {
            NfoEditorQtBridge.saveToXml(currentFile, title.text.trim(), studio.text.trim(), actors.items, tags.items); // qmllint disable unqualified

            // Save completion to file
            LunacdAutocomplete.addCompletionCandidate("studio", studio.text.trim());
            for (let actor of actors.items) {
                LunacdAutocomplete.addCompletionCandidate("actor", actor);
            }
            for (let tag of tags.items) {
                LunacdAutocomplete.addCompletionCandidate("tag", tag);
            } 
            LunacdAutocomplete.exportCompletionData();

            window.clearFields();
        }
    }
}
