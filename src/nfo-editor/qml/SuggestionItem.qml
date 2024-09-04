import QtQuick

Item {
    property alias text: text.text
    property bool selected: false

    signal clicked()

    id: suggestionItem
    height: text.height

    Rectangle {
        anchors.fill: parent
        color: (mouseArea.containsMouse || selected) ? "#f0f0f0" : "white"
    }

    Text {
        id: text
        padding: 6
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: suggestionItem.clicked()
    }
}
