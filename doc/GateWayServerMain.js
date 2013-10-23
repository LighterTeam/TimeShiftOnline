var tcp = require("../LighterWebEngine/TCP");
var cfg = require("../Common/Config");
var log = require('../Common/TSLog');
var ByteBuffer = require('ByteBuffer');
TSLog = log.create(log.INFO, 'GW.log');

/////////////////////////////////////////////////////////
// 向AdapterServer请求UUID和端口号
var hASSocket = tcp.CreateClient(cfg.AdaptServerPort, "",
    function(){
        TSLog.info("Timeshift CreateClient AdaptServer Success!");
        var sPacket = {};
        sPacket["MM"] = "GW_GetUuidPort"; //请求UUID Port
        tcp.SendBuffer(hASSocket, JSON.stringify(sPacket));
    },

    function(sBuffer){
        var oPacket = JSON.parse(sBuffer);
        switch(oPacket.MM){
            case "GW_GetUuidPort":
                var iPORT = parseInt(oPacket.PORT);
                var iUUID = parseInt(oPacket.UUID);
                RunServer_WS(iPORT, iUUID);
                RunServer(iPORT%cfg.GateWayServerPort+cfg.GateWayServerPort, iUUID);
                break;
            case "HS_ConnectHall":
                //重新连接大厅
                TSLog.info("重新连接大厅. 成功!");
                G_HallSocket = tcp.CreateClient(cfg.HallServerPort, cfg.HallServerIP,
                    HallConnectSuccess,
                    HallMessageRoute,
                    MessageLongBuffer
                );
                break;
        };
    }
);


/////////////////////////////////////////////////////////
// ws服务器启动完成.监听消息
var G_GateWay = null;
var G_HallSocket = null;
var G_GateWayTCP = null;

var G_ClientNumber = 0;
var G_ClientUUID = 0;
var G_PoolClientSocket = {};

var G_PoolUUIDInGameServer = {};

// 断线重连
var G_ReConnect = {};
var G_ReConnectName = {};

//客户端服务器. 转发客户端包到服务器
function RunServer_WS(iPORT, iUUID) {
    // 创建客户端服务器
    G_GateWay = tcp.CreateServer(iPORT,
        function() {
            TSLog.info("GateWay CreateServer WebSocket Init Success! Port:" + iPORT + " UUID:" + iUUID);
            TSLog = log.create(log.INFO, 'GW' + iUUID + '.log');
            var sPacket = {};
            sPacket.MM = "GW_RegGateWay"; //客户端自动连接网关操作
            sPacket.IP = cfg.GateWayServerIP;
            sPacket.Port = G_GateWay.address().port;
            tcp.SendBuffer(hASSocket,JSON.stringify(sPacket));

            G_GateWay.UUID = iUUID;
            G_GateWay.PORT = G_GateWay.address().port;
            G_GateWay.IP = cfg.GateWayServerIP;

            //连接大厅
            G_HallSocket = tcp.CreateClient(cfg.HallServerPort, cfg.HallServerIP,
                HallConnectSuccess,
                HallMessageRoute,
                MessageLongBuffer
            );
        },

        // 客户端消息转发: 如果玩家在大厅.则转发到大厅. 如果玩家在游戏.则转发到游戏服
        function(hSocket, sBuffer) {
            var TransPacket = sBuffer.split(',');
            if (TransPacket[0] == "ReConnect") {
                var iUUID = parseInt(TransPacket[1]);
                if ( iUUID in G_ReConnect) {
                    hSocket.UUID = iUUID;
                    G_PoolClientSocket[hSocket.UUID] = hSocket;
                    var msgArr = G_ReConnect[iUUID];
                    for (var iter in msgArr) {
                        tcp.SendBuffer(hSocket, iter);
                    }
                    delete G_ReConnect[iUUID];
                    TSLog.info("客户端重新连接成功 网关客户数:" + G_ClientNumber + " UUID:" + hSocket.UUID);

                    var sPacket = {};
                    sPacket.BUF = "ReConnect";
                    tcp.SendBuffer(hSocket, sPacket.BUF);
                }
                return;
            } else if (TransPacket[0] == "Connect"){ // 客户端登陆成功.分配UUID给客户端.
                G_ClientNumber ++;
                G_ClientUUID++;
                hSocket.UUID = G_ClientUUID * cfg.GateWayServerPlayerIDRule + G_GateWay.UUID;
                G_PoolClientSocket[hSocket.UUID] = hSocket;
                SendALLCline(hSocket.UUID,"OK");
                TSLog.info("客户端登陆成功 网关客户数:" + G_ClientNumber + " UUID:" + hSocket.UUID);
                return;
            }

            var oPacket = {};
            oPacket.MM = "ClientOrder";
            oPacket.BUF = sBuffer;
            oPacket.UUID = hSocket.UUID;

            TSLog.info("sBuffer:" + sBuffer);
            // 取消断线重连操作
            var omsg = sBuffer.split(",");
            if (omsg[0] == "Verify" || omsg[0] == "Login" || omsg[0] == "GuestLogin"){
                hSocket.NAME = omsg[1];
				TSLog.info("取消断线重连操作 Verify:" + hSocket.NAME);
                if (omsg[1] in G_ReConnectName) {
                    TSLog.info("取消断线重连操作 UUID:" + G_ReConnectName[omsg[1]].UUID + " Name:" + omsg[1]);
                    clearTimeout(G_ReConnectName[omsg[1]].TimeID);
                    timeFun(G_ReConnectName[omsg[1]].UUID);
                    delete G_ReConnectName[omsg[1]];
                }
            }

            if (omsg[0] == "Regist" || omsg[0] == "Login" || omsg[0] == "GuestLogin"){
                oPacket.BUF += "," + hSocket.remoteAddress;
                TSLog.debug("追加IP hSocket.remoteAddress:" + oPacket.BUF + " UUID:" + hSocket.UUID);
            }

            // 通过去hSocket.UUID 找对应的Game服Socket.然后把数据转发过.
            if (hSocket.UUID in G_PoolUUIDInGameServer) {
                tcp.SendBuffer(G_PoolUUIDInGameServer[hSocket.UUID],JSON.stringify(oPacket));
                return;
            }

            // 如果没有路由到GameServer. 则进入Hall路由
            if(G_HallSocket != null){
                tcp.SendBuffer(G_HallSocket,JSON.stringify(oPacket));
            }
        },

        // 客户端断开连接操作:  通知大厅玩家下线
        function(hSK) {
            var iUUID = hSK.UUID;
            G_ReConnect[iUUID] = [];

            TSLog.info("客户端异常断开! 启动断线重连 UUID:" + hSK.UUID + " NAME:" + hSK.NAME);
            if (hSK.NAME == null) {
                timeFun(hSK.UUID)
            }

            // 如果该玩家在GameServer.那么通知GS服.该玩家断线.
            if (hSK.UUID in G_PoolUUIDInGameServer) {
                var sPacket = {
                    MM:"GW_ReConnectWnd", //断线重连
                    UUID:hSK.UUID
                };
                tcp.SendBuffer(G_PoolUUIDInGameServer[hSK.UUID],JSON.stringify(sPacket));
            }

            if ("NAME" in hSK){
                G_ReConnectName[hSK.NAME] = {};
                G_ReConnectName[hSK.NAME].TimeID = setTimeout(timeFun, 0*1000, hSK.UUID);
                G_ReConnectName[hSK.NAME].UUID = hSK.UUID;
            }
        },

        function(hSocket) {
        },

        function(hSocket, byteBuffer) {
            bb = byteBuffer[0];
            var hS = null;
            if (hSocket.UUID in G_PoolUUIDInGameServer) {
                hS = G_PoolUUIDInGameServer[hSocket.UUID];
            } else if (G_HallSocket != null){
                //hS = G_HallSocket;
            }
            bb[2] = 66; //76 //改成BB模式
            if (hS != null) {
                tcp.SendBufferByteArr(hS, bb, hSocket.UUID);
            }
        }
    );
};

function timeFun(iUUID) {
    G_ClientNumber --;
    SendALLCline(iUUID,"NO")
    TSLog.info("客户端断开连接操作! 网关客户数:" + G_ClientNumber + " UUID:" + iUUID);
    delete G_PoolClientSocket[iUUID];

    // 通知大厅玩家断开链接
    var sPacket = {
        MM:"GW_LeaveGame",
        UUID:iUUID
    };
    tcp.SendBuffer(G_HallSocket,JSON.stringify(sPacket));

    // 如果该玩家在GameServer.那么通知GS服.该玩家断线.
    if (iUUID in G_PoolUUIDInGameServer) {
        sPacket = {
            MM:"GW_LeaveGame",
            UUID:iUUID
        };
        tcp.SendBuffer(G_PoolUUIDInGameServer[iUUID],JSON.stringify(sPacket));
    }
}

//服务器. 转发服务器包到客户端
function RunServer(iPORT, iUUID) {
    G_GateWayTCP = tcp.CreateServer(iPORT,
        function () {
            TSLog.info("GateWay CreateServer TCP Init Success! Port:" + iPORT + " UUID:" + iUUID);
            G_GateWayTCP.UUID = iUUID;
            G_GateWayTCP.PORT = iPORT;
            G_GateWayTCP.IP = cfg.GateWayServerIP;
        },

        function (hSocket, sBuffer) {
            var oPacket = JSON.parse(sBuffer);
            if (oPacket.MM == null) {
                // 玩家消息路由
                var iUUID = oPacket.UUID;
                if (!(iUUID in G_PoolClientSocket)){
                    TSLog.error("网关获取了 错误的玩家UUID:" + iUUID + " G_GateWay.UUID:" + G_GateWay.UUID + ":" + oPacket.BUF);
                    return;
                }

                if (iUUID in G_ReConnect){
                    G_ReConnect[iUUID].push(oPacket.BUF);
                    return;
                }

                tcp.SendBuffer(G_PoolClientSocket[iUUID], oPacket.BUF);
                return;
            }

            // 系统消息处理
            switch(oPacket.MM) {
                case "GS_RouteToGameServer":
                    var room = oPacket.Room;
                    for (var iUUID in room.ClientArr){
                        G_PoolUUIDInGameServer[iUUID] = hSocket;
                        hSocket.ClientArr.push(iUUID);

                        if (iUUID in G_PoolClientSocket) {
                            TSLog.info("EnterGame! 通知客户端开始游戏! 该玩家已经路由到游戏服 UUID:" + iUUID);
                            var sPacket = "EnterGame";
                            tcp.SendBuffer(G_PoolClientSocket[iUUID], sPacket);
                        }
                    }
                    return;
                case "GS_LeaveGame":
                    var iUUID = oPacket.UUID;
                    delete G_PoolUUIDInGameServer[iUUID];
                    return;
            }
        },

        function (hSocket) {
            for (var i = 0 ; i < hSocket.ClientArr.length; i++) {
                if (hSocket.ClientArr[i] in G_PoolUUIDInGameServer){
                    delete G_PoolUUIDInGameServer[hSocket.ClientArr[i]];
                }
            }
        },

        function (hSocket) {
            hSocket.ClientArr = [];
        },
        function (hSocket, sBuffer) {
            var iUUID = sBuffer[1];
            TSLog.trace("Server MessageLongBuffer UUID:" + iUUID);
            if (!(iUUID in G_PoolClientSocket)){
                TSLog.error("Server MessageLongBuffer 网关获取了 错误的玩家UUID:" + iUUID + " G_GateWay.UUID:" + G_GateWay.UUID);
                return;
            }
            sBuffer[0][2] = 76; //66
            var hSocket = G_PoolClientSocket[iUUID];
            tcp.SendBufferByteArrToClient(hSocket, sBuffer[0]);
        }
    );
};

///////////////////////////////////////////////
//大厅消息处理

//大厅连接成功
function HallConnectSuccess() {
    var iUUID = G_GateWayTCP.UUID;
    var sIP = G_GateWayTCP.IP;
    var iPort = G_GateWayTCP.PORT;
    var sPacket = {
        MM:"RegGateWay",
        UUID:iUUID,
        IP: sIP,
        PORT: iPort
    };
    tcp.SendBuffer(G_HallSocket, JSON.stringify(sPacket));
};

//大厅消息路由, 大厅消息发送给客户端.
function HallMessageRoute(sBuffer) {
    var oPacket = JSON.parse(sBuffer);
    var iUUID = oPacket.UUID;
    if (!(iUUID in G_PoolClientSocket)){
        TSLog.error("网关获取了 错误的玩家UUID:" + iUUID + " G_GateWay.UUID:" + G_GateWay.UUID + " Buf:" + oPacket.BUF);
        return;
    }
    TSLog.info(oPacket.BUF);
    var hSocket = G_PoolClientSocket[iUUID];
    tcp.SendBuffer(hSocket, oPacket.BUF);
};

function MessageLongBuffer(sBuffer) {
    var iUUID = sBuffer[1];
    TSLog.trace("MessageLongBuffer UUID:" + iUUID);
    if (!(iUUID in G_PoolClientSocket)){
        TSLog.error("MessageLongBuffer 网关获取了 错误的玩家UUID:" + iUUID + " G_GateWay.UUID:" + G_GateWay.UUID);
        return;
    }
    sBuffer[0][2] = 76; //66
    var hSocket = G_PoolClientSocket[iUUID];
    tcp.SendBufferByteArrToClient(hSocket, sBuffer[0]);
}


function SendALLCline(id,type){
    var PSocket = tcp.CreateClient(cfg.Nub_Port, cfg.VB_IP,
        function(){
            sPacket="";
            if(type=="OK"){
                sPacket="Number,"+id+","+"OK" ;
            }else if(type=="NO"){
                sPacket="Number,"+id+","+"NO" ;
            }
            tcp.SendBuffer(PSocket, sPacket);
        },
        function(BUFF){

        }
    );

}



