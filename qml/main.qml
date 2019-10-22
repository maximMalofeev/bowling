import QtQuick 2.12
import Qt.labs.platform 1.1
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Game 1.0


ApplicationWindow {
    id: root
    visible: true
    minimumWidth: 450
    width: 450
    height: 800
    minimumHeight: 800
    title: qsTr("Bowling")

    footer: ToolBar{
        ToolButton{
            anchors.horizontalCenter: parent.horizontalCenter
            text: game.playersNumber > 0 ? "Next" : "Open File"
            onClicked: {
                if(game.playersNumber > 0){
                    game.next()
                }else{
                    fileDialog.open()
                }
            }
        }
    }

    ColumnLayout{
        id: playersLayout
        anchors{
            top: parent.top
            left: parent.left
            bottom: parent.bottom
            right: bowlingLine.left
        }

        width: 200
        Repeater{
            model: game.playersNumber

            Page{
                id: playerPage
                property int score: 0
                property int player: modelData
                Layout.preferredHeight: parent.height / game.playersNumber
                Layout.fillWidth: true
                header: Label{
                    text: "Player " + (player + 1) + " Score: " + playerPage.score
                    horizontalAlignment: Qt.AlignHCenter
                    font.underline: true
                }

                ScrollView{
                    Column{
                        anchors.fill: parent
                        anchors.leftMargin: 5

                        Repeater{
                            // number of frames
                            model: 10
                            Label{
                                id: frameLabel
                                property int frame: modelData
                                visible: game.currentFrame > modelData
                            }
                        }
                    }
                }

                Connections{
                    target: game
                    onScoreChanged:{
                        playerPage.score = game.score(modelData)
                    }
                }
            }
        }
    }
    BowlingLane{
        id: bowlingLine
        height: parent.height
        anchors.right: parent.right
        anchors.rightMargin: 10
    }

    Dialog {
        id: dialog
        anchors.centerIn: parent
        title: "Title"
        standardButtons: Dialog.Ok
        modal: true

        onAccepted: {
            Qt.quit()
        }
    }

    FileDialog {
          id: fileDialog
          title: "Please choose a file"
          onAccepted: {
              game.setInput(file)
          }
      }

    Connections{
        target: game

        onStepMade:{
            bowlingLine.run(player, pins)
        }
        onGameOver:{
            var winner = 0;
            for(var i = 1; i < game.playersNumber; i++){
                console.log("Gema score p ", i-1, game.score(i - 1))
                console.log("Gema score p ", i, game.score(i))
                if(game.score(i - 1) < game.score(i)){
                    winner = i;
                }
            }

            dialog.title = "Game over. Player " + (winner + 1) + " win"
            dialog.open()
        }
        onRulesBroken:{
            dialog.title = "Rules broken"
            dialog.open()
        }
    }
}
