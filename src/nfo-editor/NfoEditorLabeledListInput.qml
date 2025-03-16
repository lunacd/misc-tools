pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts

import libui

FocusScope {
    id: labeledListInput
    property alias text: suggestionBox.text
    property alias label: label.text
    required property bool autocomplete
    property string completionSource
    property list<string> items: []
    implicitHeight: label.height + column.height + column.anchors.topMargin

    function addItem() {
        let trimmedText = suggestionBox.text.trim();
        suggestionBox.text = "";
        if (trimmedText.length === 0 || items.includes(trimmedText)) {
            return;
        }
        items.push(trimmedText);
    }

    Text {
        id: label
    }

    ColumnLayout {
        id: column
        anchors.top: label.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 4

        RowLayout {
            id: row
            spacing: 4
            Layout.fillWidth: true

            LunacdSuggestionBox {
                id: suggestionBox

                Layout.fillWidth: true

                autocomplete: labeledListInput.autocomplete
                completionSource: labeledListInput.completionSource
                focus: true

                onAccepted: () => labeledListInput.addItem()
            }

            LunacdButton {
                icon: "icons/plus.svg"
                onClicked: () => labeledListInput.addItem()
            }
        }

        Flow {
            id: flow
            spacing: 4
            Layout.fillWidth: true

            Repeater {
                model: labeledListInput.items

                NfoEditorRemovableItem {
                    required property string modelData
                    required property int index
                    text: modelData
                    onRemoved: () => {
                        labeledListInput.items.splice(index, 1);
                    }
                }
            }
        }
    }
}
