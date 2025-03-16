import QtQuick

Item {
    id: removableItem

    property alias text: text.text
    property int padding: 6
    
    signal removed()

    implicitHeight: Math.max(removeButton.height, text.height) + 2 * this.padding
    implicitWidth: text.width + removeButton.height + 2 * this.padding + 2

    Rectangle {
        id: background
        anchors.fill: parent
        color: "white"
        radius: 4
    }

    Text {
        id: text

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: removableItem.padding
    }

    Item {
        id: removeButton

        width: 16
        height: 16
        anchors.left: text.right
        anchors.leftMargin: 2
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            id: buttonBackground
            color: mouseArea.containsMouse ? "#f0f0f0" : "white"
            anchors.fill: parent
            radius: removeButton.height / 2
        }

        Image {
            source: "icons/x.svg"
            anchors.fill: parent
            anchors.leftMargin: 2
            anchors.rightMargin: 2
            anchors.topMargin: 2
            anchors.bottomMargin: 2
        }

        MouseArea {
            id: mouseArea
            hoverEnabled: true
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: () => { removableItem.removed() }
        }
    }
}
