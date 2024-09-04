import QtQuick
import QtQuick.Layouts

Item {
    property alias model: repeater.model
    // Using var so that this could be undefined, indicating none is selected
    property var selectedIndex: undefined

    signal fill(text: string)

    id: suggestionPanel
    height: columnLayout.height

    ColumnLayout {
        id: columnLayout
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 0

        Repeater {
            id: repeater

            SuggestionItem {
                required property string modelData
                required property int index

                id: suggestionItem
                text: modelData
                Layout.fillWidth: true
                selected: index === selectedIndex

                onClicked: () => suggestionPanel.fill(modelData)
            }
        }
    }
}