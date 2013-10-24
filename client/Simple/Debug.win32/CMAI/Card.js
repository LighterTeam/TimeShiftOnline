/**
 * Created with JetBrains WebStorm.
 * User: controlstrength
 * Date: 13-5-13
 * Time: 下午2:05
 * To change this template use File | Settings | File Templates.
 */

function Cards(Jbuff){
    var OP="";
    var OPP="";
    var Str=[];
    var Strs=[];
    var Split=Jbuff.split(",");
    for(var m=0 ;m< Split.length ;m++){
        var K=Split[m];
        if(K==="A10"){
            K="AI";
            Str.push(K);
        }
        else if(K==="B10"){
            K="BI"
            Str.push(K);
        }
        else if(K==="C10"){
            K="CI"
            Str.push(K);
        }
        else if(K==="D10"){
            K="DI"
            Str.push(K);
        }
        else if(K==="A11"){
            K="AJ"
            Str.push(K);
        }
        else if(K==="B11"){
            K="BJ"
            Str.push(K);
        }
        else if(K==="C11"){
            K="CJ"
            Str.push(K);
        }
        else if(K==="D11"){
            K="DJ"
            Str.push(K);
        }
        else if(K==="A12"){
            K="AQ"
            Str.push(K);
        }
        else if(K==="B12"){
            K="BQ"
            Str.push(K);
        }
        else if(K==="C12"){
            K="CQ"
            Str.push(K);
        }
        else if(K==="D12"){
            K="DQ"
            Str.push(K);
        }
        else if(K==="A13"){
            K="AK"
            Str.push(K);
        }
        else if(K==="B13"){
            K="BK"
            Str.push(K);
        }
        else if(K==="C13"){
            K="CK"
            Str.push(K);
        }
        else if(K==="D13"){
            K="DK"
            Str.push(K);
        }
        else if(K==="C14"){
            K="CA"
            Str.push(K);
        }else if(K==="B14"){
            K="BA"
            Str.push(K);
        }else if(K==="D14"){
            K="DA"
            Str.push(K);
        }else if(K==="A15"){
            K="A2"
            Str.push(K);
        }else{
            Str.push(K);
        }

    }

    for(var j=0;j<Str.length;j++){
        OP+=  Str[j]+",";
    }
    OP=OP.substring(0,OP.length-1)
    var TC=OP.split(",");
    for(var r=0;r<TC.length;r++){
        var HH=TC[r].substring(1,2);
        if(TC[r]==="A3"){
            TC[r]="S";
            Strs.push(TC[r]);
        }else{
            Strs.push(HH);
        }

    }
    for(var w=0;w<Strs.length;w++){
        OPP+=Strs[w];
    }
    return OPP

}
function AllCard(Jbuff){

    var OP="";
    var OPP="";
    var Str=[];
    var Strs=[];
    var Split=Jbuff;
    for(var m=0 ;m< Split.length ;m++){
        var K=Split[m];
        if(K==="A10"){
            K="AI";
            Str.push(K);
        }
        else if(K==="B10"){
            K="BI"
            Str.push(K);
        }
        else if(K==="C10"){
            K="CI"
            Str.push(K);
        }
        else if(K==="D10"){
            K="DI"
            Str.push(K);
        }
        else if(K==="A11"){
            K="AJ"
            Str.push(K);
        }
        else if(K==="B11"){
            K="BJ"
            Str.push(K);
        }
        else if(K==="C11"){
            K="CJ"
            Str.push(K);
        }
        else if(K==="D11"){
            K="DJ"
            Str.push(K);
        }
        else if(K==="A12"){
            K="AQ"
            Str.push(K);
        }
        else if(K==="B12"){
            K="BQ"
            Str.push(K);
        }
        else if(K==="C12"){
            K="CQ"
            Str.push(K);
        }
        else if(K==="D12"){
            K="DQ"
            Str.push(K);
        }
        else if(K==="A13"){
            K="AK"
            Str.push(K);
        }
        else if(K==="B13"){
            K="BK"
            Str.push(K);
        }
        else if(K==="C13"){
            K="CK"
            Str.push(K);
        }
        else if(K==="D13"){
            K="DK"
            Str.push(K);
        }
        else if(K==="C14"){
            K="CA"
            Str.push(K);
        }else if(K==="B14"){
            K="BA"
            Str.push(K);
        }else if(K==="D14"){
            K="DA"
            Str.push(K);
        }else if(K==="A15"){
            K="A2"
            Str.push(K);
        }else{
            Str.push(K);
        }

    }

    for(var j=0;j<Str.length;j++){
        OP+=  Str[j]+",";
    }
    OP=OP.substring(0,OP.length-1)
    var TC=OP.split(",");
    for(var r=0;r<TC.length;r++){
        var HH=TC[r].substring(1,2);
        if(TC[r]==="A3"){
            TC[r]="S";
            Strs.push(TC[r]);
        }else{
            Strs.push(HH);
        }

    }
    for(var w=0;w<Strs.length;w++){
        OPP+=Strs[w];
    }
    return OPP
}
