import QtQuick

Item {
    property alias text: textElem.text
    property bool selected: false

    signal clicked()

    id: suggestionItem
    height: textElem.height
    implicitHeight: textElem.implicitHeight
    implicitWidth: textElem.implicitWidth
    
    Component.onCompleted: () => {
        console.log(suggestionItem.width)
        console.log(suggestionItem.height)
        console.log(suggestionItem.x)
        console.log(suggestionItem.y)
    }

    Rectangle {
        anchors.fill: parent
        color: (mouseArea.containsMouse || suggestionItem.selected) ? "#f0f0f0" : "white"
    }


    Text {
        id: textElem
        anchors.left: parent.left
        anchors.right: parent.right
        padding: 8
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: suggestionItem.clicked()
    }
}
