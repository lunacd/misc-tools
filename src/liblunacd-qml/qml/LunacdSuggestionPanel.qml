import QtQuick
import QtQuick.Layouts

Item {
    property alias model: repeater.model
    property var selectedIndex: undefined

    signal fill(text: string)

    id: suggestionPanel
    height: columnLayout.height
    implicitHeight: columnLayout.height

    ColumnLayout {
        id: columnLayout
        spacing: 0
        anchors.left: parent.left
        anchors.right: parent.right

        Repeater {
            id: repeater
            width: parent.width

            delegate: LunacdSuggestionItem {
                required property int index
                required property string completionText

                id: suggestionItem
                text: completionText
                selected: index === selectedIndex
                Layout.fillWidth: true

                onClicked: () => suggestionPanel.fill(completionText)
            }
        }
    }
}