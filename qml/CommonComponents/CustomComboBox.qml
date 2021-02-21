import QtQuick 2.12
import QtQuick.Controls 2.12

ComboBox {
    id: control
    model: ["First", "Second", "Third"]

    delegate: ItemDelegate {
        width: control.width
        contentItem: Text {
            text: model.username
            color: "#cbe368"
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        highlighted: control.highlightedIndex === index
    }

    indicator: Text {
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 8
        text: popup.opened ? "⮝" : "⮟"
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        color: "#cbe368"
    }

    contentItem: Text {
        leftPadding: 0
        rightPadding: -control.indicator.width - control.spacing

        text: control.displayText
        font: control.font
        color: "#cbe368"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 140
        implicitHeight: 20
        border.color: "#cbe368"
        border.width: control.visualFocus ? 2 : 1
        radius: 2
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: "#cbe368"
            radius: 2
        }
    }
}
