var queue = {};
queue.get = function(){
  if(queue.head){
    var item = queue.head;
    if(item.next){
      queue.head = item.next;
    }else{
      queue.head = null;
      queue.tail = null;
    }
    return item;
  }else{
    return null;
  }
};
queue.add = function(item){
  if(!queue.tail){
    queue.tail = item;
    if(!queue.head){
      queue.head = item;
    }
  }else{
    queue.tail.next = item;
    queue.tail = item;
  }
};


var initStage = function(){
  for(var i=0;i<=640;i+=64){
    ctx.moveTo(0,i);
    ctx.lineTo(640,i);
    ctx.stroke();
    ctx.moveTo(i,0);
    ctx.lineTo(i,640);
    ctx.stroke();
  }
  
  ctx.font="10px Arial";
  ctx.fillStyle = "red";
  for(var i=0;i<=10;i++){
    for(var j=0;j<=10;j++){
      ctx.fillText((i-4)+","+(6-j),(i*64)+1,(j*64)-2);
    }
  }
};


var drawimg = function(img,x,y){
  ctx.drawImage(img,(x+4)*64,(5-y)*64);
};

var rt = false;
var drawScene = function(x,y,dir){
    c.height=c.height;
    for(var i=0;i<scene.a.length;i++){
      drawimg($("#a")[0],scene.a[i].x,scene.a[i].y);
    }
    for(var i=0;i<scene.m.length;i++){
      drawimg($("#m")[0],scene.m[i].x,scene.m[i].y);
    }
    ctx.save();
    ctx.translate((x+4)*64+32,(5-y)*64+32); 
    ctx.rotate(dir);
    ctx.translate(-32, -32);
    if(rt){
      ctx.drawImage($("#rt")[0], 0, 0);
    }else{
      ctx.drawImage($("#t")[0], 0, 0);
    }
    ctx.restore();
    initStage();
};
