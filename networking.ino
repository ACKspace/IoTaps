#define TCP_FLAGS_FIN_V 1 //as declared in net.h
#define TCP_FLAGS_ACK_V 0x10 //as declared in net.h

const char FrontPage1[] PROGMEM =
"HTTP/1.0 200 OK\r\n"
"Content-Type: text/html\r\n"
"\r\n"
"<html><head><script>\r\n"
"var fissaState = 0;\r\n"
"setInterval(function () {loadXMLDoc()}, 500);\r\n"
"function loadXMLDoc(){\r\n"
"var xmlhttp;\r\n"
"xmlhttp=new XMLHttpRequest();\r\n"
"xmlhttp.onreadystatechange=function(){\r\n"
"if (xmlhttp.readyState==4 && xmlhttp.status==200){\r\n"
"var res = xmlhttp.responseText.split(\"\\r\\n\");\r\n"
"res.pop();\r\n"
"fissaState = res[4];\r\n";

const char FrontPage2[] PROGMEM =
"if(res[3] == 1){\r\n"
"document.getElementById(\"ts\").innerHTML= \"Tap open\";document.getElementById(\"ts\").style.background = \"#FFCC00\";\r\n"
"}\r\n"
"else{\r\n"
"document.getElementById(\"ts\").innerHTML= \"Tap dicht\";document.getElementById(\"ts\").style.background = \"#D32B2B\";\r\n"
"}\r\n"
"if(res[4] == 1){\r\n"
"document.getElementById(\"fl\").innerHTML= \"Stop Fissa\";document.getElementById(\"fs\").innerHTML= \"Fissa\";document.getElementById(\"fs\").style.background = \"#00FF00\";\r\n"
"}\r\n";

const char FrontPage3[] PROGMEM =
"else{\r\n"
"document.getElementById(\"fl\").innerHTML= \"Start Fissa\";\r\n"
"document.getElementById(\"fs\").innerHTML= \"Geen Fissa :(\";\r\n"
"document.getElementById(\"fs\").style.background = \"#D32B2B\";\r\n"
"}\r\n"
"document.getElementById(\"tapt\").innerHTML=res[0]+\" &deg;C\";\r\n"
"document.getElementById(\"ptt\").innerHTML=res[1]+\" &deg;C\";\r\n"
"document.getElementById(\"ltt\").innerHTML=res[2]+\" &deg;C\";\r\n"
"}\r\n";

const char FrontPage4[] PROGMEM =
"}\r\n"
"xmlhttp.open(\"GET\",\"/raw.txt?t=\" + Math.random(),true);\r\n"
"xmlhttp.send();\r\n"
"}\r\n"
"function changeFissa(){\r\n"
"var xmlhttp;\r\n"
"xmlhttp=new XMLHttpRequest();\r\n"
"xmlhttp.onreadystatechange=function(){\r\n"
"if (xmlhttp.readyState==4 && xmlhttp.status==200){\r\n"
"loadXMLDoc();\r\n"
"}\r\n"
"}\r\n"
"if(fissaState==1){xmlhttp.open(\"GET\",\"/fm0?t=\" + Math.random(),true);}\r\n"
"else{xmlhttp.open(\"GET\",\"/fm1?t=\" + Math.random(),true);};\r\n";

const char FrontPage5[] PROGMEM =
"xmlhttp.send();\r\n"
"}\r\n"
"</script><style>\r\n"
"td{text-align: center; font-size: 24PT; width: 50%}\r\n"
"</style></head>\r\n"
"<body onload=\"loadXMLDoc()\">\r\n"
"<table style=\"width: 100%; max-width: 512px;\">\r\n"
"<tr><td colspan=2><h1><a onclick=\"changeFissa()\" href=\"javascript:void(0);\" id=\"fl\">Start Fissa</a></h1></td></tr>\r\n"
"<tr><td id=\"ts\">loading</td><td id=\"fs\">loading</td>";

const char FrontPage6[] PROGMEM =
"</tr>\r\n</table><h2>Tap:</h2><h1 id=\"tapt\"></h1><h2>Partytent:</h2><h1 id=\"ptt\"></h1><h2>Legertent:</h2><h1 id=\"ltt\"></h1></body></html>";

    
const char DataReadPage[] PROGMEM =
"HTTP/1.0 200 OK\r\n"
"Content-Type: text/plain\r\n"
"\r\n";

void handleNetworking()
{
  word pos = ether.packetLoop(ether.packetReceive());
  // check if valid tcp data is received
  if (pos) {
    char* data = (char *) Ethernet::buffer + pos;
    if (strncmp("GET /raw", data, 8) == 0) {
      // Create data readout thingy here
      ether.httpServerReplyAck(); //Send ack to the request

      // Content generation
      memcpy_P(ether.tcpOffset(), DataReadPage, sizeof DataReadPage); // Put header in buffer
      uint16_t totalSize = sizeof DataReadPage;
      unsigned char * bufferP = ether.tcpOffset();
      char convertBuffer[10];

      // Add beer pump temperature to the buffer
      dtostrf(tapSensorTemp, 1, 2, convertBuffer);
      memcpy(ether.tcpOffset() + (totalSize - 1), convertBuffer, strlen(convertBuffer));
      totalSize += strlen(convertBuffer);
      bufferP[totalSize-1] = '\r';
      bufferP[totalSize] = '\n';
      totalSize += 2;

      // Add OPT1 temperature to the buffer
      dtostrf(optSensor1Temp, 1, 2, convertBuffer);
      memcpy(ether.tcpOffset() + (totalSize - 1), convertBuffer, strlen(convertBuffer));
      totalSize += strlen(convertBuffer);
      bufferP[totalSize-1] = '\r';
      bufferP[totalSize] = '\n';
      totalSize += 2;

      // Add OPT2 temperature to the buffer
      dtostrf(optSensor2Temp, 1, 2, convertBuffer);
      memcpy(ether.tcpOffset() + (totalSize - 1), convertBuffer, strlen(convertBuffer));
      totalSize += strlen(convertBuffer);
      bufferP[totalSize-1] = '\r';
      bufferP[totalSize] = '\n';
      totalSize += 2;

      //Add beer pump switch state
      bufferP[totalSize-1] = (int)bpState + 48;
      bufferP[totalSize] = '\r';
      bufferP[totalSize+1] = '\n';
      totalSize += 3;
      
      //Add fissa mode state
      bufferP[totalSize-1] = (int)fissaMode + 48;
      bufferP[totalSize] = '\r';
      bufferP[totalSize+1] = '\n';
      totalSize += 3;
      
      // Send HTTP reply
      ether.httpServerReply_with_flags(totalSize - 1, TCP_FLAGS_ACK_V | TCP_FLAGS_FIN_V);
    }
    else
    {
      if (strncmp("GET /fm1", data, 8) == 0)
      {
        fissaMode = TRUE;
      }
      else if (strncmp("GET /fm0", data, 8 ) == 0)
      {
        fissaMode = FALSE;
      }
      ether.httpServerReplyAck(); // send ack to the request

      memcpy_P(ether.tcpOffset(), FrontPage1, sizeof FrontPage1); // send first packet and not send the terminate flag
      ether.httpServerReply_with_flags(sizeof FrontPage1 - 1,TCP_FLAGS_ACK_V);

      memcpy_P(ether.tcpOffset(), FrontPage2, sizeof FrontPage2); // send first packet and not send the terminate flag
      ether.httpServerReply_with_flags(sizeof FrontPage2 - 1,TCP_FLAGS_ACK_V);

      memcpy_P(ether.tcpOffset(), FrontPage3, sizeof FrontPage3); // send first packet and not send the terminate flag
      ether.httpServerReply_with_flags(sizeof FrontPage3 - 1,TCP_FLAGS_ACK_V);

      memcpy_P(ether.tcpOffset(), FrontPage4, sizeof FrontPage4); // send first packet and not send the terminate flag
      ether.httpServerReply_with_flags(sizeof FrontPage4 - 1,TCP_FLAGS_ACK_V);

      memcpy_P(ether.tcpOffset(), FrontPage5, sizeof FrontPage5); // send first packet and not send the terminate flag
      ether.httpServerReply_with_flags(sizeof FrontPage5 - 1,TCP_FLAGS_ACK_V);
      
      memcpy_P(ether.tcpOffset(), FrontPage6, sizeof FrontPage6);
      ether.httpServerReply_with_flags(sizeof FrontPage6 - 1, TCP_FLAGS_ACK_V | TCP_FLAGS_FIN_V);
    }
  }
}
