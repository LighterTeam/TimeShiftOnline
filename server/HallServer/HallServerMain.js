var tcp = require("../LighterWebEngine/TCP");
var cfg = require("../Common/Config");
var hs = require("./HallSystem");

var HallSystem = new hs.HallSystem();

// 重启流程保护: 通知AS, AS通知GWS, 重新连接HS.
var G_ASSocket = tcp.CreateClient(cfg.AdaptServerPort, cfg.AdaptServerPort,
    function (){
        // 给AS发请求.然后AS发通知给网关.重新连接HS
        var sPacket = {
            MM:"HS_ConnectHall"
        };
        tcp.SendBuffer(G_ASSocket,JSON.stringify(sPacket));
    },

    function (sBuffer){

    }
);

// 启动大厅服务器
tcp.CreateServer(cfg.HallServerPort,
    function() {
        console.log("Timeshift HallTCPServer Success!");
    },

    function(hSocket, sBuffer) {
        var oPacket = JSON.parse(sBuffer);
        switch (oPacket.MM) {
            case "SysOrder": //用户命令行
                console.log(oPacket.MM + ":" +oPacket.Order + ":" + oPacket.UUID);
                HallSystem.ProcessOrder(oPacket.Order, parseInt(oPacket.UUID), hSocket);
                break;
            case "ClientOffLine":
                HallSystem.ClientOffLine(parseInt(oPacket.UUID));
                break;
            case "RegGateWay":
                HallSystem.RegGateWay(oPacket, hSocket);
                break;
            case "RegGameServer":
                HallSystem.RegGameServer(hSocket);
                break;
            case "GS_RemoveRoom":
                HallSystem.GS_RemoveRoom(oPacket);
                break;
        }
    },

    function(hSocket) {
        HallSystem.Disconnect(hSocket);
    },

    function(hSocket) {

    }
);
