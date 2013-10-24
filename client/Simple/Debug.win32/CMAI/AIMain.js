/**
 * Created with JetBrains WebStorm.
 * User: controlstrength
 * Date: 13-5-31
 * Time: 下午3:54
 * To change this template use File | Settings | File Templates.
 */
require("./CMAI/Rule.js");
require("./CMAI/Card.js");
require("./CMAI/AICards.js");

function ArrInArr(ArrT, ArrO){
    var inner = {};
    var rNum = 0;
    for(var iter in ArrT){
        var t = ArrT[iter];
        for(var subIter in ArrO) {
            var o = ArrO[subIter];
            if(subIter in inner) {
                continue;
            }
            if(t === o){
                inner[subIter] = "";
                rNum ++
                break;
            }
        }
    }
    if(ArrT.length == rNum) {
        return true;
    }
    return false;
}


function AIStart(UserCard1,MyCard1){
    var St=[];
    var SV=[];
    var IS_OK=[];
    var PH="";
    var PP;

    var UserCard=Cards(UserCard1); //88
    var MyCard=Cards(MyCard1);
    var UserCarsArry=INShow(UserCard);
    for(var q=0 ; q<MyCard.length ; q++){
        var MyCad=MyCard[q];
        IS_OK.push(MyCad);
    }

    for(var i=0 ; i<UserCarsArry.length ; i++){
        var type=UserCarsArry[i];
        for(var j=0 ; j<type.length ; j++){
            St.push(type[j]);
        }
        var S_Arry=ArrInArr(St,IS_OK)
        if(S_Arry===true){
            SV=St;
            break;
        }else{
            St=[];
        }
    }
    for(var i=0;i<SV.length;i++){
        PH+=SV[i];
    }
    PP=AI(PH,MyCard1);
    return PP;
}

function AIOneStart(MyCard1){
    var SVN=[];
    var TC_All=[];
    var MyCard=Cards(MyCard1);
    var Bool=false;
    var Spart = {};
    for(var d=0;d<MyCard.length;d++){
        var My=MyCard[d];
        SVN.push(My);
        if(My==="S"){
            Bool=true;
        }
    }
    if(Bool===true){
        for (var iter in CMRule) {
            if(CMRule[iter][2]){
                var OP=ArrInArr(CMRule[iter][2],SVN);
                if(OP===true){
                    for(var m=0;m<CMRule[iter][2].length;m++){
                        var MK=CMRule[iter][2][m];
                        if(MK==="S"){
                            Spart[CMRule[iter][1]]=CMRule[iter][2];
                            break;
                        }
                    }
                }
            }
        }
    }else{
        for (var iter in CMRule) {
            if(CMRule[iter][2]){
                var OP=ArrInArr(CMRule[iter][2],SVN);
                if(OP===true){
                    Spart[CMRule[iter][1]]=CMRule[iter][2];
                }
            }
        }
    }

    for(var iter in Spart){
        TC_All.push(parseInt(iter))
    }
    return AI(Spart[Math.max.apply(null,TC_All)],MyCard1);
}
