import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

FocusScope {
    property alias text: suggestionBox.text
    property alias label: label.text
    required property bool autocomplete
    property string completionSource
    property list<string> items: []

    id: labeledListInput
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

            SuggestionBox {
                id: suggestionBox        

                Layout.fillWidth: true

                autocomplete: labeledListInput.autocomplete
                completionSource: labeledListInput.completionSource
                focus: true

                onAccepted: () => addItem()
            }

            CustomButton {
                icon: "icons/plus.svg"
                onClicked: () => addItem()
            }
        }

        Flow {
            id: flow
            spacing: 4
            Layout.fillWidth: true

            Repeater {
                model: items

                RemovableItem {
                    required property string modelData
                    required property int index
                    text: modelData
                    onRemoved: () => { items.splice(index, 1) }
                }
            }
        }
    }
}