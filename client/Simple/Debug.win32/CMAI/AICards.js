/**
 * Created with JetBrains WebStorm.
 * User: controlstrength
 * Date: 13-6-3
 * Time: 下午3:09
 * To change this template use File | Settings | File Templates.
 */
//console.log( AI("55","A5,B5,C5,D8,B10,A11,A12,A13"));
//console.log(AI("6789I","A4,A6,B7,A5,B4,B8,B9,A10"))
function AI(Jbuff,MyCard1){
    var OP="";
    var OPP="";
    var Str=[];
    var Strs=[];
    var Spart={};
    var rNum = 0;
    var MyCard=MyCard1.split(",");
    var Split=Jbuff;
    for(var m=0 ;m< Split.length ;m++){
        var K=Split[m];
        if(K==="I"){
            K="10";
            Str.push(K);
        }
        else if(K==="J"){
            K="11"
            Str.push(K);
        }

        else if(K==="Q"){
            K="12"
            Str.push(K);
        }

        else if(K==="K"){
            K="13"
            Str.push(K);
        }

        else if(K==="A"){
            K="14"
            Str.push(K);
        }else if(K==="2"){
            K="15"
            Str.push(K);
        }else{
            Str.push(K);
        }

    }
// console.log(Str)

        for(var iter in  Str){
        var p= Str[iter];
         if(p==="S"){
            Strs.push("A3");
            for(var UP in  MyCard){
                if(MyCard[UP]==="A3"){
                    MyCard.splice(UP,1)
                }
            }
            continue;
        }
        for(var UPiter in  MyCard){
            var TC=MyCard[UPiter];
            var PP=TC.substring(1,TC.length);
            if(UPiter in Spart){
                continue;
            }
            if(PP===p){
                Strs.push(TC);
                Spart[UPiter]=""
               break;
            }

        }
    }
    for(var w=0;w<Strs.length;w++){
        OPP+=Strs[w]+",";
    }
    return OPP.substring(0,OPP.length-1)
}
