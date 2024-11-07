import QtQuick
import QtQuick.Layouts

Item {
    id: button

    property alias text: text.text
    property alias icon: icon.source

    signal clicked()

    QtObject {
        id: internal
        readonly property int padding: 4
        readonly property int itemSize: 16
    }

    implicitWidth: Math.max(internal.itemSize + 2 * internal.padding, row.width)
    implicitHeight: internal.itemSize + 2 * internal.padding

    Rectangle {
        anchors.fill: parent
        color: mouseArea.containsMouse ? "#f0f0f0" : "white"
        border.color: button.focus ? "blue" : "black"
        border.width: 1
        radius: 4
    }

    RowLayout {
        id: row
        spacing: 2

        anchors.top: parent.top
        anchors.bottom: parent.bottom

        // Using rectangles for padding for the lack of horizontal anchors
        Rectangle {
            width: internal.padding
        }

        Image {
            id: icon
            sourceSize: Qt.size(internal.itemSize, internal.itemSize)
            width: internal.itemSize
            height: internal.itemSize
            Layout.alignment: Qt.AlignVCenter
            visible: source.toString() !== ""
        }

        Text {
            id: text
            visible: text !== ""
        }

        Rectangle {
            width: internal.padding
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: button.clicked()
    }
}