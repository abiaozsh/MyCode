"use strict";
(function (WIN, $) {

var TimeFn = null;
var mouseDown = false;
var mouseType = "0";

$(document).ready(function(){
  $("#MainImg").bind("click",function(e){
    clearTimeout(TimeFn);
    TimeFn = setTimeout(function(){
      if(mouseDown){
        mouseDown = false;
        MouseAction(e,"1");
      }
      else
      {
        MouseAction(e,"5");
      }
    },1000);
  });
  $("#MainImg").bind("dblclick",function(e){
    clearTimeout(TimeFn);
    TimeFn = setTimeout(function(){
      MouseAction(e,"2");
    },1000);
  });
  $("#MainImg").bind("mousedown",function(e){
    clearTimeout(TimeFn);
    mouseDown = true;
    TimeFn = setTimeout(function(){
      MouseAction(e,"4");
      mouseDown = false;
    },1000);
    e.preventDefault();
  });
  $("#MainImg").bind("mousemove",function(e){
    e.preventDefault();
  });
  $("#MainImg").bind("contextmenu",function(e){
    clearTimeout(TimeFn);
    TimeFn = setTimeout(function(){
      MouseAction(e,"3");
    },1000);
    e.preventDefault();
  });
  
  $("#Refresh").bind("click",function(e){
    Refresh();
  });

  $(".J_KeyButton").each(function(i,item){
    $(item).bind("click",function(){
      var key = $(item).attr('data-val');
      KeyAction($("#key1").val(),key);
    });
  });
  
  $("#J_Copykey").bind("click",function(e){
    KeyAction("VK_CONTROL","VK_C");
  });
  
  $("#J_Cutkey").bind("click",function(e){
    KeyAction("VK_CONTROL","VK_X");
  });
  
  $("#J_Pastkey").bind("click",function(e){
    KeyAction("VK_CONTROL","VK_V");
  });
  
  $("#SendKey").bind("click",function(e){
    KeyAction($("#key1").val(),$("#key2").val());
  });
  
  $("#SendQuality").bind("click",function(e){
    $.ajax("setQuality", {
        method: "POST",
        data: {quality:$("#J_Quality").val()},
        success: function(xhr, response){
            //var rs = $.parseJSON(response);
            setTimeout(function(){
              Refresh();
            },100);
        },
        error: function(){
            alert("erroras");
        }
    });
  });
  
  $("#SendClipboard").bind("click",function(e){
    $.ajax("ClipBoardAction", {
        method: "POST",
        data: {val:$("#J_ClipBoard").val()},
        success: function(xhr, response){
            //var rs = $.parseJSON(response);
        },
        error: function(){
            alert("erroras");
        }
    });
  });
  
  $(".J_MouseButton").each(function(i,item){
    $(item).bind("click",function(){
      $(".J_MouseButton").each(function(i2,item2){
        $(item2).removeClass("btnon");
        $(item2).addClass("btnoff");
      });
      $(item).removeClass("btnoff");
      $(item).addClass("btnon");
      mouseType = $(item).attr('data-val');
      console.log(mouseType);
    });
  });
  
});


function KeyAction(key1,key2){
  console.log(key1+","+key2);
  $.ajax("KeyAction", {
      method: "POST",
      data: {key1:key1,key2:key2},
      success: function(xhr, response){
          //var rs = $.parseJSON(response);
          setTimeout(function(){
            Refresh();
          },1000);
      },
      error: function(){
          alert("erroras");
      }
  });

}

function MouseAction(e,type){
  var pos = getMouse(e);
  {
    var txt = "";
    switch(type){
		case "1":txt = ("single click");break;
		case "2":txt = ("double click");break;
		case "3":txt = ("right click");break;
		case "4":txt = ("drag");break;
		case "5":txt = ("drop");break;
    }
    console.log(txt+","+pos.x+","+pos.y);
  }

  if(mouseType!="0"){
    type = mouseType;
  }
  $.ajax("MouseAction", {
      method: "POST",
      data: {x:pos.x,y:pos.y,action:type},
      success: function(xhr, response){
          //var rs = $.parseJSON(response);
          setTimeout(function(){
            Refresh();
          },1000);
      },
      error: function(){
          alert("erroras");
      }
  });
}

function Refresh(){
  var timestamp = new Date().getTime();
  $("#MainImg").attr("src", "Screen.jpg" + '?'+ timestamp);
}
function getMouse(e){
	var pos = {x:0,y:0};
  var posx,posy;
	var ev=(!e)?window.event:e;//IE:Moz
	if (ev.pageX){//Moz
		posx=ev.pageX;//+window.pageXOffset;
		posy=ev.pageY;//+window.pageYOffset;
		pos.x = posx-$("#MainImg").offset().left;
		pos.y = posy-$("#MainImg").offset().top;
	}
	else if(ev.clientX){//IE
		posx=ev.clientX+document.body.scrollLeft;
		posy=ev.clientY+document.body.scrollTop;
		pos.x = posx-$("#MainImg").offset().left;
		pos.y = posy-$("#MainImg").offset().top;
	}
	return pos;
}

})(window, jQuery);
