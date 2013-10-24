require("CMAI/cadrs.js");
require("CMAI/Card.js");
require("CMAI/Rule.js");
require("CMAI/AIMain.js");

var UUIDAdapt = 0;
function UserStruct (){
    this.UUID = UUIDAdapt++;
    this.VName = "ROBOT" + this.UUID;
    this.ICON = TSRandom(4);
    this.RoomFlowID = this.UUID;
    log("RoomFlowID = " + this.RoomFlowID);
}

var room = {};
room.ClientArr = {}; //都是UserStruct

//0号UUID默认为玩家
room.ClientArr[0] = new UserStruct;
room.ClientArr[1] = new UserStruct;
room.ClientArr[2] = new UserStruct;

function GetUserInfo(id){
     
	 var string=""+room.ClientArr[id].UUID+","
	 +room.ClientArr[id].VName+","
	 +room.ClientArr[id].ICON+","
	 +room.ClientArr[id].RoomFlowID;
	
	 return string;
}

function TSRandom(Max){
    return parseInt(Math.random()*Max);
}

function ArrayInData(arr, ele){
    for(var iter in arr) {
        if (arr[iter] == ele){
            return true;
        }
    }
    return false;
}

function SendBuffer(UUID, sBuffer) {
    if (UUID != 0) {
        return;
    }
    TS_SendBuffer(sBuffer.BUF);
}

function StartGame(){
    var PacketList = [];
    PacketList[0] = "";
    PacketList[1] = "";
    PacketList[2] = "";
    var AllCard = CARDCards();

    for (var j = 0; j < AllCard.length; j++) {
        var cardList = AllCard[j];
        for (var i = 0; i < cardList.length; i++) {
            PacketList[j] += cardList[i] + ",";
        }
    }

    room.CardList = {};     //当前玩家手持的牌.
    room.CurRole = 0;       //用于标识当前的玩家.
    room.RuleIndex = 0;     //轮询打牌.
    room.nextUser = null;  //下一个打牌的玩家.
    room.upCard = "";       //玩家上次出的牌
    room.newLoop = 0;       //新循环
    room.winLoop = 0;       //准备开始框
    room.stopGame = 0;

    var iLen = Object.keys(room.ClientArr).length;
    if(iLen != 3) {
        log("错误的房间!.房间里面不是3个人. length = " + iLen);
        return;
    }

    var index = 0;
    var isOK = 0;
    for (var iter in room.ClientArr) {
        var iUUID = iter;
        room.CardList[iUUID] = AllCard[index];

        var sPacket = {};
        if (ArrayInData(room.CardList[iUUID], "A3")) {
            isOK = 1;
            room.CurRole = iUUID;
            //room.CurRole = 0;
            sPacket.BUF = "SendCard," + "1," + PacketList[index];
            //SendBuffer(0, sPacket);
        }
        else {
            sPacket.BUF = "SendCard," + "0," + PacketList[index];
        }
        SendBuffer(iUUID, sPacket);
        index++;
    }

    log(JSON.stringify(room.CardList));

    if (isOK <= 0) {
        log("错误的分牌!.牌里面始终没有A3");
    }
}

//AI出牌延迟.不然太快 AIStart龙提供!
var dalteTime = 0;
function TSDraw(){
    if (room.stopGame == 1) {
        return;
    }
    dalteTime ++;
    if (dalteTime > 5)
    {
        dalteTime = 0;
        if (room.CurRole == 0){
            return;
        }

        if (room.upCard.length <= 0){
            var r = AIOneStart(room.CardList[room.CurRole].toString());
            var sBuffer = "Send," + r;
            log("机器人主动出牌:"+sBuffer);
            ClientMsgProcess(sBuffer, room.CurRole);
            return;
        }

        var sBuffer = "";
        log("电脑反应 room.upCard = " + room.upCard.toString());
        log("电脑反应 room.CardList[room.CurRole] = " + room.CardList[room.CurRole].toString());
        var r = AIStart(room.upCard.toString(), room.CardList[room.CurRole].toString());
        log("电脑反应 r =" + r);
        if (r === "") {
            sBuffer = "NoGen";
        }
        else {
            sBuffer = "Send," + r;
        }
        log("电脑反应 出牌 =" + sBuffer);
        log("电脑反应UUID:========================================End! UUID=" + room.CurRole)//电脑反应..
        ClientMsgProcess(sBuffer, room.CurRole);
    }
};

//AI调用.
function ClientMsgProcess(sBuffer, UUID) {

    log(sBuffer);
    //UUID = parseInt(UUID);
    var OrderList = sBuffer.split(',');
    switch (OrderList[0]) {
        case "Send": //出牌
            if (room.stopGame == 0)
                SendCard(UUID, sBuffer);
            break;
        case "NoGen": //不跟
            if (room.stopGame == 0)
                NoGen(UUID);
            break;
        case "AgainReady": //准备下一把
            AgainReady();
            break;
    }
}

function AgainReady(){
    StartGame();
}

//不跟
function NoGen(UUID) {
	var CurUser = room.ClientArr[UUID];
    room.RuleIndex ++;
    if (room.RuleIndex >= 2) {
        room.RuleIndex = 0;
        room.newLoop = 0;
        for (var iter in room.ClientArr) {
            var user = room.ClientArr[iter];
            var sPacket = {};
            sPacket.BUF = "ALL,1";
            SendBuffer(user.UUID, sPacket);
            room.upCard = "";
        }
    }

    var nextUser = findNextPlayer(room, UUID);
    if(nextUser == null){
        log("找到下一个要出牌的玩家 没有找到.");
        return;
    }
    log("找到下一个要出牌的玩家 找到." + nextUser.UUID);
    room.CurRole = nextUser.UUID;
    room.nextUser = nextUser;
    for (var iter in room.ClientArr) {
        var uuid = room.ClientArr[iter].UUID;
        var sPacket = {};
        if (uuid === nextUser.UUID) {
            sPacket.BUF = "Gen,Y," + CurUser.RoomFlowID;
        } else {
            sPacket.BUF = "Gen,N," + CurUser.RoomFlowID;
        }
        SendBuffer(uuid, sPacket);
    }
}

//出牌
function SendCard(UUID, sBuffer){
    var oCard = sBuffer.substring(5, sBuffer.length);
    var oCardList = oCard.split(",");
    var lAllCard = room.CardList[UUID];
    var CurUser = room.ClientArr[UUID];

    log("出牌:" + sBuffer + " UUID:" + UUID);

    var AllCardRule = null;
    if (room.newLoop == 0) {
        var ccUpCard = Cards(oCard);
        AllCardRule = StarShow(ccUpCard, Cards(lAllCard.join(",")));
        var isOK = false;
        for (var m = 0; m < AllCardRule.length; m++) {
            var all = AllCardRule[m];
            if (all == ccUpCard) {
                isOK = true;
                break;
            }
        }
    }
    else {
        var ccUpCard = Cards(room.upCard);
        var ccOCard = Cards(oCard);
        AllCardRule = INShow(ccUpCard);
        for (var m = 0; m < AllCardRule.length; m++) {
            var all = AllCardRule[m];
            if (all == ccOCard) {
                isOK = true;
                break;
            }
        }
    }

    log("可以出牌 isOK = " + isOK);

    if (isOK) { //可以出牌
        room.upCard = oCard;
        room.newLoop = 1;

        //清除掉手中的牌.
        for (var j = 0; j < oCardList.length; j++) {
            for (var i = 0; i < lAllCard.length; i++) {
                if (lAllCard[i] === oCardList[j]) {
                    lAllCard.splice(i, 1);
                    break;
                }
            }
        }

        var nextUser = findNextPlayer(room, UUID);
        if(nextUser == null){
            log("找到下一个要出牌的玩家 没有找到.");
            return;
        }
        log("找到下一个要出牌的玩家 找到." + nextUser.UUID);
        room.nextUser = nextUser;
        room.CurRole = nextUser.UUID;
        room.RuleIndex = 0;
        for (var iter in room.ClientArr) {
            var uuid = room.ClientArr[iter].UUID;
            var sPacket = {};
            if (uuid === nextUser.UUID) {
                sPacket.BUF = "Gen,Y," + CurUser.RoomFlowID + "," + oCard;
            } else {
                sPacket.BUF = "Gen,N," + CurUser.RoomFlowID + "," + oCard;
            }
            SendBuffer(uuid, sPacket);
        }

        //如果玩家手中没有牌了就重新开启一把游戏
        if(Object.keys(lAllCard).length <= 0){
            for(var iter in room.ClientArr){
                var user = room.ClientArr[iter];
                var sPacket = {};
                if (UUID == user.UUID) {
                    sPacket.BUF = "Win,Y," + CurUser.VName;
                }
                else {
                    sPacket.BUF = "Win,N," + CurUser.VName;
                }
                SendBuffer(user.UUID, sPacket);
            }
            room.stopGame = 1;
            return;
        }
    }
    else { //不可以出牌
        var sPacket = {};
        sPacket.BUF = "Gen,NO";
        SendBuffer(UUID, sPacket);
    }
}

function findNextPlayer(room, UUID){
    // 找到下一个要出牌的玩家
    var user = room.ClientArr[UUID];
    var nextUser = null;
    var nextUserFlowID = (user.RoomFlowID + 1) % 3;
    log("user.RoomFlowID : " + nextUserFlowID);
    for (var iter in room.ClientArr) {
        var oUser = room.ClientArr[iter];
        if (oUser.RoomFlowID == nextUserFlowID) {
            nextUser = oUser;
            break;
        }
    }

    if(nextUser == null) {
        return;
    }

    return nextUser;
}



