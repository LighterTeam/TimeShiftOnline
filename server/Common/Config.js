module.exports = {
    AdaptServerPort: 9900, // 唯一 TCP其他服务器
    AdaptServerPort_WS: 9901, // 唯一 WebSocket浏览器使用
    AdaptServerIP: "192.168.1.6",

    GateWayServerPort: 10000, // TCP其他服务器使用
    GateWayServerPort_WS: 20000, // WebSocket浏览器连接的网关地址 20000起
    GateWayServerPlayerIDRule: 1000, // UUID规则.UUID中包含网关信息
    GateWayServerIP: "192.168.1.6",

    HallServerPort: 9001, // 唯一 大厅服务器(用来组玩家房子)
    HallServerIP: "192.168.1.6",

    HTTPServerPort: 9010, // 唯一 游戏的位置
    HTTPServerIP: "192.168.1.6",

    GameServerIDAdapt: 1000, // 游戏服务器规则
    GameServerMaxRoom: 20, // 一个游戏服务器 最多多少个房子

    end: null
};