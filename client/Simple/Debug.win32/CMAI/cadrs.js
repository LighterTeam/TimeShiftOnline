/**
 * Created with JetBrains WebStorm.
 * User: controlstrength
 * Date: 13-5-13
 * Time: 下午3:08
 * To change this template use File | Settings | File Templates.
 */

require("CMAI/TSUtil.js");

//console.log(fs);
function CARDCards() {
    var list = [];
    var list1 = [];
    var list2 = [];
    var list3 = [];

    var p = ["A3", "D3", "C3", "B3", "A4", "D4", "C4", "B4", "A5", "D5", "C5", "B5", "A6", "D6", "C6", "B6", "A7",
        "D7", "C7", "B7", "A8", "D8", "C8", "B8", "A9", "D9", "C9", "B9", "A10", "D10", "C10", "B10", "A11", "D11",
        "C11", "B11", "A12", "D12", "C12", "B12", "A13", "D13", "C13", "B13", "A15", "D14", "C14", "B14"];

    for (var i = 0; i < p.length; i++) {
        list.push(p[i]);
    }

    list = shuffle(p);

    for (var i = list.length - 1; i >= 0; i--) {
        if (list.length > 32) {
            list1.push(list[i])
            list.splice(i, 1);

        } else if (list.length <= 32 && list.length > 16) {
            list2.push(list[i])
            list.splice(i, 1);
        } else if (list.length > 0 && list.length <= 16) {
            list3.push(list[i])
            list.splice(i, 1);
        }
    }

    var R = [];
    R.push(list1);
    R.push(list2);
    R.push(list3);
    return R;
}
function shuffle(p) {
    for (var i = p.length - 1; i > 0; i--) {
        var j = TSRandom(i);
        var ci = p[i];
        p[i] = p[j];
        p[j] = ci;
    }
    return p;
}
