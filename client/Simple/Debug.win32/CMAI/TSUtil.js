function TSRandom(Max){
    return parseInt(Math.random()*Max);
}

function TSReverse(srcString){
    var temp = [];
    for(var i=srcString.length-1;i>-1;i--){
        temp.push(srcString.charAt(i));
    }
    return temp.join("").toString();
}