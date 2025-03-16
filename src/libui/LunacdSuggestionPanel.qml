pragma ComponentBehavior: Bound

import QtQuick

Item {
    property alias model: repeater.model
    property var selectedIndex: undefined

    signal fill(text: string)

    id: suggestionPanel
    height: columnLayout.height
    implicitHeight: columnLayout.height

    Column {
        id: columnLayout
        spacing: 0
        anchors.left: parent.left
        anchors.right: parent.right

        Repeater {
            id: repeater
            anchors.left: parent.left
            anchors.right: parent.right

            delegate: LunacdSuggestionItem {
                required property int index
                required property string completionText

                id: suggestionItem
                text: completionText
                selected: index === suggestionPanel.selectedIndex
                anchors.left: parent.left
                anchors.right: parent.right

                onClicked: () => suggestionPanel.fill(completionText)
            }
        }
    }
}
