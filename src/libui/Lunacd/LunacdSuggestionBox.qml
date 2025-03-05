import QtQuick
import QtQuick.Controls

FocusScope {
    id: suggestionBox

    required property bool autocomplete
    property string completionSource
    property alias text: textInput.text

    signal accepted

    x: rectangle.x
    y: rectangle.y
    width: rectangle.width
    height: rectangle.height
    Component.onCompleted: () => {
        if (autocomplete) {
            LunacdAutocomplete.registerCompletionSource(completionSource);
        }
    }

    function nextSuggestion() {
        if (suggestionPanel.model.count === 0) {
            suggestionPanel.selectedIndex = undefined;
        }
        if (suggestionPanel.selectedIndex === undefined) {
            suggestionPanel.selectedIndex = 0;
        } else {
            suggestionPanel.selectedIndex += 1;
        }
        suggestionPanel.selectedIndex %= suggestionPanel.model.count;
    }

    function previousSuggestion() {
        if (suggestionPanel.model.count === 0) {
            suggestionPanel.selectedIndex = undefined;
        }
        if (suggestionPanel.selectedIndex === undefined) {
            suggestionPanel.selectedIndex = suggestionPanel.model.count - 1;
        } else {
            suggestionPanel.selectedIndex -= 1;
        }
        suggestionPanel.selectedIndex %= suggestionPanel.model.count;
    }

    function acceptSuggestion() {
        if (suggestionPanel.model.count === 0) {
            return;
        }
        if (suggestionPanel.selectedIndex === undefined) {
            textInput.text = suggestionPanel.model.get(0).completionText;
        } else {
            textInput.text = suggestionPanel.model.get(suggestionPanel.selectedIndex).completionText;
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

            Keys.onDownPressed: event => {
                suggestionBox.nextSuggestion();
            }
            Keys.onUpPressed: event => {
                suggestionBox.previousSuggestion();
            }

            onTextEdited: () => {
                suggestionPanel.updateCompletion(textInput.text);
            }
            onAccepted: () => {
                suggestionBox.acceptSuggestion();
                suggestionBox.accepted();
            }
            onActiveFocusChanged: () => {
                if (!activeFocus) {
                    suggestionPopup.close();
                }
            }
        }

        Popup {
            id: suggestionPopup
            x: 0
            y: parent.height
            width: parent.width
            height: suggestionPanel.implicitHeight

            bottomPadding: 0
            leftPadding: 0
            topPadding: 0
            rightPadding: 0

            closePolicy: Popup.NoAutoClose

            LunacdSuggestionPanel {
                id: suggestionPanel
                function updateCompletion(prefix) {
                    suggestionPanel.model.clear();
                    if (!suggestionBox.autocomplete || prefix.length <= 1) {
                        return;
                    }

                    const completions = LunacdAutocomplete.autocomplete(suggestionBox.completionSource, prefix);
                    completions.forEach(completion => {
                        suggestionPanel.model.append({
                            "completionText": completion
                        });
                    });
                    if (completions.length > 0) {
                        suggestionPopup.open();
                    }
                }
                anchors.left: parent.left
                anchors.right: parent.right

                model: ListModel {}
                onFill: text => {
                    textInput.text = text;
                    suggestionBox.closeSuggestion();
                }
            }
        }
    }
}
