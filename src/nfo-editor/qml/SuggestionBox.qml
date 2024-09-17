import QtQuick
import QtQuick.Controls

FocusScope {
    id: suggestionBox

    required property bool autocomplete
    property string completionSource
    property alias text: textInput.text

    signal accepted()

    x: rectangle.x
    y: rectangle.y
    width: rectangle.width
    height: rectangle.height
    Component.onCompleted: () => {
        if (autocomplete) {
            bridge.registerCompletionSource(completionSource);
        }
    }
    
    function nextSuggestion() {
        if (suggestionPanel.model.length === 0) {
            suggestionPanel.selectedIndex = undefined;
        }
        if (suggestionPanel.selectedIndex === undefined) {
            suggestionPanel.selectedIndex = 0;
        } else {
            suggestionPanel.selectedIndex += 1;
        }
        suggestionPanel.selectedIndex %= suggestionPanel.model.length;
    }

    function previousSuggestion() {
        if (suggestionPanel.model.length === 0) {
            suggestionPanel.selectedIndex = undefined;
        }
        if (suggestionPanel.selectedIndex === undefined) {
            suggestionPanel.selectedIndex = suggestionBox.model.length - 1;
        } else {
            suggestionPanel.selectedIndex -= 1;
        }
        suggestionPanel.selectedIndex %= suggestionPanel.model.length;
    }

    function acceptSuggestion() {
        if (suggestionPanel.model.length === 0) {
            return;
        }
        if (suggestionPanel.selectedIndex === undefined) {
            textInput.text = suggestionPanel.model[0];
        } else {
            textInput.text = suggestionPanel.model[suggestionPanel.selectedIndex];
        }
        closeSuggestion();
    }

    function closeSuggestion() {
        suggestionPanel.selectedIndex = undefined;
        suggestionPopup.close();
    }

    Rectangle {
        id: rectangle
        radius: 4
        border.color: "black"
        border.width: 1

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 24

        TextInput {
            id: textInput
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 4
            anchors.rightMargin: 4
            focus: true

            Keys.onDownPressed: (event) => {
                suggestionBox.nextSuggestion()
            }
            Keys.onUpPressed: (event) => {
                suggestionBox.previousSuggestion()
            }

            onTextEdited: () => suggestionPopup.open()
            onAccepted: () => {
                suggestionBox.acceptSuggestion();
                suggestionBox.accepted();
            }
            onActiveFocusChanged: () => {
                if (!activeFocus) {
                    suggestionPopup.close()
                }
            }
        }

        Popup {
            id: suggestionPopup
            x: 0
            y: parent.height
            width: parent.width
            height: suggestionPanel.height

            bottomPadding: 0
            leftPadding: 0
            topPadding: 0
            rightPadding: 0

            closePolicy: Popup.NoAutoClose

            background: Rectangle {
                anchors.fill: parent
                color: "white"
            }

            SuggestionPanel {
                function getCompletion(prefix) {
                    if (!autocomplete || prefix.length <= 1) {
                        return []
                    }
                    return bridge.autocomplete(completionSource, prefix)
                }

                id: suggestionPanel
                anchors.fill: parent
                model: getCompletion(textInput.text)
                onFill: (text) => {
                    textInput.text = text;
                    suggestionBox.closeSuggestion();
                }
            }
        }
    }
}