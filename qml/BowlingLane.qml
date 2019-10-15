import QtQuick 2.12
import QtQuick.Controls 2.12

Page {
    property var pinsArray: [pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8, pin9, pin10]

    property int player: 0

    id: root
    width: 200
    height: 500
    header: Label{
        text: "Player " + player
        horizontalAlignment: Qt.AlignHCenter
    }

    function run(pl, count){
        if(pl !== player){
            player = pl
            newPlayer()
        }
        // dirty hack for 10th round bonuses
        var allInvisible = true
        for(var i = 0; i < 10; i++){
            if(pinsArray[i].visible === true){
                allInvisible = false
            }
        }
        if(allInvisible){
            newPlayer()
        }

        ball.visible = true
        ball.x = root.width / 2 - ball.width / 2
        ball.y = root.height - ball.height
        ballAnimation.start()
        ballAnimation.knockCount = count
    }

    function newPlayer(){
        for(var i = 0; i < 10; i++){
            pinsArray[i].visible = true
        }
    }

    function knockDown(count){
        for(var i = 0; count !== 0; i++){
            if(pinsArray[i].visible === true){
                pinsArray[i].visible = false
                count = count - 1
            }
        }
    }

    Rectangle{
        anchors.fill: parent
        color: "tan"
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        Rectangle{
            id: ball
            width: 50
            height: width
            radius: 25
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#0000ff"
                }

                GradientStop {
                    position: 1
                    color: "#000000"
                }
            }
            x: parent.width / 2 - width / 2
            y: parent.height - height
            visible: false

            NumberAnimation{
                property int knockCount: 0
                id: ballAnimation
                target: ball
                property: "y"
                to: 120
                duration: 700
                onFinished: {
                    ball.visible = false
                    if(knockCount > 0){
                        boomAnimation.start()
                        knockDown(knockCount)
                    }
                }
            }
        }

        Image{
            id: boom
            width: parent.width
            height: width
            source: "qrc:/boom.png"
            visible: true
            opacity: 0
            z: 1
            SequentialAnimation{
                id: boomAnimation
                NumberAnimation{
                    target: boom
                    property: "opacity"
                    to: 1
                    duration: 300
                }
                NumberAnimation{
                    target: boom
                    property: "opacity"
                    to: 0
                    duration: 300
                }
            }
        }

        Rectangle {
            id: pin7
            x: 30
            y: 30
            width: 20
            height: 20
            color: "#ffffff"
            radius: 10
        }

        Rectangle {
            id: pin8
            x: 70
            y: 30
            width: 20
            height: 20
            color: "#ffffff"
            radius: 10
        }

        Rectangle {
            id: pin5
            x: 90
            y: 56
            width: 20
            height: 20
            color: "#ffffff"
            radius: 10
        }

        Rectangle {
            id: pin9
            x: 110
            y: 30
            width: 20
            height: 20
            color: "#ffffff"
            radius: 10
        }

        Rectangle {
            id: pin10
            x: 150
            y: 30
            width: 20
            height: 20
            color: "#ffffff"
            radius: 10
        }

        Rectangle {
            id: pin1
            x: 90
            y: 100
            width: 20
            height: 20
            color: "#ffffff"
            radius: 10
        }

        Rectangle {
            id: pin2
            x: 70
            y: 75
            width: 20
            height: 20
            color: "#ffffff"
            radius: 10
        }

        Rectangle {
            id: pin3
            x: 110
            y: 75
            width: 20
            height: 20
            color: "#ffffff"
            radius: 10
        }

        Rectangle {
            id: pin4
            x: 48
            y: 56
            width: 20
            height: 20
            color: "#ffffff"
            radius: 10
        }

        Rectangle {
            id: pin6
            x: 131
            y: 56
            width: 20
            height: 20
            color: "#ffffff"
            radius: 10
        }
    }
}
