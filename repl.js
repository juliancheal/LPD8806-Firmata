var firmata = require("firmata");
var repl = require("repl");

var serialport = "/dev/tty.usbserial-A600aeYe";
var board = new firmata.Board(serialport, function (error) {
if (error) {
  console.log(error);
  return;
}

  console.log('Connected to ' + serialport);
  console.log('Firmware: ' +
              board.firmware.name + '-' +
              board.firmware.version.major + '.' +
              board.firmware.version.minor
              );

  var context = repl.start("RGB>").context;
  //    context.sses = sendSysExString;
  context.sendPixel = sendPixel;
  context.sendPackedPixel = sendPackedPixel;
  context.getPixelColor = getPixelColor;
  context.animation = animation;
  context.rainbow = rainbow;
  context.animation2 = animation2;

  board.on('string', function(data) {
    var message = '';

    for (var index = 0, length = data.length; index < length; index += 2) {
      message += String.fromCharCode(
          ((data.charCodeAt(index+1) & 0x7F) << 7) +
          (data.charCodeAt(index)   & 0x7F)
          );
    }

    if (messageHandler != null) messageHandler(message);
  });
});

function messageHandler(message) {
  console.log(message);
}

// sets a pixel using a packed rgb value. Easiest way to supply this is to send it as a hex value using 0xRRGGBB format.
var sendPackedPixel = function (rgb, pos) {
  msg = "{p:" + pos + ",c:" + rgb + "}";
  console.log(msg);
  sendSysExString(msg);
}

// sets a pixel colour at a particular position
var sendPixel = function(r, g, b, pos) {
  // msg = "{r:" + r +  ",g:" + g + ",b:" + b + ",p:" + pos + "}";
  var rgb = (g<<16) + (r<<8) + b
  msg = "{p:" + pos + ",c:" + rgb + "}";
  console.log(msg);
  sendSysExString(msg);
}

var getPixelColor = function(pos) {
  msg = "{g:" + pos + "}";
  sendSysExString(msg);
}

var animation = function(type, wait, r, g, b) {
  r = r || 0;
  g = g || 0;
  b = b || 0;
  // msg = "{r:" + r + ",g:" + g + ",b:" + b + ",t:" + type + ",w:" + wait + "}";
  var rgb = (g<<16) + (r<<8) + b
  msg = "{t:" + type + ",c:" + rgb + ",w:" + wait + "}";
  console.log(msg);
  sendSysExString(msg);
}

var animation2 = function(type, wait, rgb) {
  rgb = rgb || 0;
  // msg = "{r:" + r + ",g:" + g + ",b:" + b + ",t:" + type + ",w:" + wait + "}";
  msg = "{t:" + type + ",c:" + rgb + ",w:" + wait + "}";
  console.log(msg);
  sendSysExString(msg);
}

// sends an actual sysex string message
var sendSysExString = function(message) {
  var START_SYSEX = 0xF0;
  var STRING_DATA = 0x71;
  var END_SYSEX   = 0xF7;

  var buffer = new Buffer(message + '\0', 'utf8');
  var data   = [];

  data.push(START_SYSEX);
  data.push(STRING_DATA);

  for (var index = 0, length = buffer.length; index < length; index ++) {
    data.push(buffer[index] & 0x7F);
    data.push((buffer[index] >> 7) & 0x7F);
  }
  data.push(END_SYSEX);
  board.sp.write(data);
}

var rainbow = function(wait) {
  var i=0;
  var j=0;
   
  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < 32; i++) {
      sendPackedPixel(i, Wheel( (i + j) % 384));
    }  
    // strip.show();   // write all the pixels out
    // delay(wait);
  }
}

var Wheel = function(WheelPos)
{
  var r = 0;
  var g = 0;
  var b = 0;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  // return(strip.Color(r,g,b));
  var rgb = (g<<16) + (r<<8) + b
  return rgb
}
