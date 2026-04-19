/*****************************************************************
   ESP8266 SMART SOIL MOISTURE MONITOR
   Premium Apple UI + Project Details Popup

   Board: NodeMCU ESP8266
   Sensor: Soil Moisture Sensor

   WiFi Name : Smart_Soil_Monitor
   Password  : 12345678

   Open Browser:
   http://192.168.4.1
*****************************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

/*---------------- WIFI ----------------*/
const char* ssid = "Smart_Soil_Monitor";
const char* pass = "12345678";

/*---------------- SENSOR ----------------*/
#define SENSOR_PIN A0

int dryValue = 1024;
int wetValue = 350;

int rawADC = 0;
int moisture = 0;

/*---------------- TIMER ----------------*/
unsigned long lastRead = 0;
const unsigned long interval = 2000;

/*---------------- HTML ----------------*/
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Smart Soil Monitor</title>

<style>
*{margin:0;padding:0;box-sizing:border-box;font-family:system-ui,-apple-system,sans-serif}
body{
background:linear-gradient(135deg,#dff7e6,#edf7ff,#ffffff);
min-height:100vh;
padding:18px;
display:flex;
justify-content:center;
align-items:center;
color:#111;
}
.wrap{width:100%;max-width:430px;animation:fade .8s ease}
@keyframes fade{
0%{opacity:0;transform:scale(.94)}
100%{opacity:1;transform:scale(1)}
}
.card{
background:rgba(255,255,255,.72);
backdrop-filter:blur(15px);
border-radius:28px;
padding:22px;
box-shadow:0 20px 60px rgba(0,0,0,.12);
}
.top{
display:flex;
justify-content:space-between;
margin-bottom:16px;
font-weight:700;
font-size:14px;
opacity:.7;
}
.hero{
padding:18px;
border-radius:24px;
background:rgba(255,255,255,.55);
text-align:center;
box-shadow:0 12px 35px rgba(0,0,0,.07);
}
.big{
font-size:60px;
font-weight:800;
letter-spacing:-2px;
}
.sub{opacity:.65;margin-top:4px}
.gw{display:flex;justify-content:center;margin:18px 0}
.g{
width:180px;height:180px;border-radius:50%;
background:conic-gradient(#34c759 0deg,#34c759 0deg,#eaeaea 0deg);
display:flex;justify-content:center;align-items:center;
transition:all .8s cubic-bezier(.4,0,.2,1);
box-shadow:0 14px 40px rgba(52,199,89,.22);
}
.gi{
width:138px;height:138px;border-radius:50%;
background:#fff;
display:flex;justify-content:center;align-items:center;
flex-direction:column;
}
.grid{
display:grid;
grid-template-columns:1fr 1fr;
gap:12px;
margin-top:14px;
}
.box{
background:rgba(255,255,255,.55);
padding:14px;
border-radius:18px;
box-shadow:0 10px 28px rgba(0,0,0,.06);
}
.box h4{
font-size:12px;
opacity:.55;
margin-bottom:6px;
}
.box p{
font-size:18px;
font-weight:800;
}
.full{grid-column:1/3}

.btn{
width:100%;
margin-top:14px;
padding:14px;
border:none;
border-radius:18px;
background:#111;
color:#fff;
font-weight:700;
font-size:15px;
transition:.15s;
}
.btn:active{transform:scale(.95)}

.footer{
text-align:center;
font-size:12px;
opacity:.55;
margin-top:10px;
}

/* popup */
.modal{
position:fixed;
top:0;left:0;
width:100%;height:100%;
background:rgba(0,0,0,.45);
display:none;
justify-content:center;
align-items:center;
padding:14px;
z-index:99;
}
.popup{
max-width:520px;
width:100%;
max-height:90vh;
overflow:auto;
background:#fff;
border-radius:24px;
padding:22px;
box-shadow:0 20px 60px rgba(0,0,0,.25);
}
.popup h2{
margin-bottom:10px;
font-size:24px;
}
.popup h3{
margin-top:14px;
margin-bottom:6px;
font-size:17px;
color:#34c759;
}
.popup p,.popup li{
font-size:14px;
line-height:1.6;
opacity:.85;
}
.close{
margin-top:16px;
background:#34c759;
}
</style>
</head>

<body>

<div class="wrap">
<div class="card">

<div class="top">
<div>🌱 Soil Moisture Monitoring</div>
<div>ESP8266</div>
</div>

<div class="hero">
<div class="big"><span id="moist">0</span>%</div>
<div class="sub" id="status">Loading...</div>

<div class="gw">
<div class="g" id="gauge">
<div class="gi">
💧
<div id="raw">ADC 0</div>
</div>
</div>
</div>
</div>

<div class="grid">
<div class="box">
<h4>Uptime</h4>
<p id="up">0s</p>
</div>

<div class="box">
<h4>Signal</h4>
<p id="rssi">AP</p>
</div>

<div class="box full">
<h4>Sensor Health</h4>
<p id="health">Checking...</p>
</div>
</div>

<button class="btn" onclick="openPop()">Project Details</button>

<div class="footer">http://192.168.4.1</div>

</div>
</div>

<!-- Popup -->
<div class="modal" id="modal">
<div class="popup">

<h2>🌱 Project Details</h2>

<h3>Title</h3>
<p>Smart Wireless Soil Moisture Monitoring System using ESP8266</p>

<h3>Group Members</h3>
<ol>
<li><b>Aman Sayyad</b> (Lead & Core Developer)</li>
<li>Smruti Desai</li>
<li>Riddhi Parmar</li>
<li>Aryan Jivani</li>
</ol>

<h3>Electrical Apparatus</h3>
<ul>
<li>ESP8266 NodeMCU</li>
<li>Soil Moisture Sensor Module</li>
<li>Jumper Wires</li>
<li>USB Cable</li>
<li>Mobile / Laptop</li>
</ul>

<h3>Working Principle</h3>
<p>
The soil moisture sensor detects water content in soil by measuring conductivity.
Wet soil gives lower resistance while dry soil gives higher resistance.
The analog value is read by ESP8266 ADC pin A0. The controller converts this value into moisture percentage and displays it live on web dashboard.
</p>

<h3>Performance Result</h3>
<p>
The system provides fast real-time monitoring with stable wireless access.
Moisture percentage updates every 2 seconds and helps identify dry, normal or wet soil condition accurately.
</p>

<h3>Applications</h3>
<ul>
<li>Smart Irrigation Systems</li>
<li>Agriculture Monitoring</li>
<li>Garden Plant Care</li>
<li>Greenhouse Automation</li>
<li>Water Saving Projects</li>
</ul>

<button class="btn close" onclick="closePop()">Close</button>

</div>
</div>

<script>
const $=e=>document.getElementById(e);

function openPop(){ $('modal').style.display='flex'; }
function closePop(){ $('modal').style.display='none'; }

async function load(){
try{
let r=await fetch('/data');
let d=await r.json();

$('moist').innerText=d.moisture;
$('raw').innerText='ADC '+d.raw;
$('up').innerText=d.uptime;
$('rssi').innerText=d.rssi+' dBm';
$('health').innerText=d.health;
$('status').innerText=d.status;

let deg=d.moisture*3.6;
$('gauge').style.background=
'conic-gradient(#34c759 '+deg+'deg,#eaeaea '+deg+'deg)';
}
catch(e){
$('status').innerText='Offline';
}
}
load();
setInterval(load,2000);
</script>

</body>
</html>
)rawliteral";

/*---------------- SENSOR ----------------*/
void readSensor() {
  rawADC = analogRead(SENSOR_PIN);
  moisture = map(rawADC, dryValue, wetValue, 0, 100);
  moisture = constrain(moisture, 0, 100);
}

/*---------------- ROUTES ----------------*/
void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleData() {

  String status = "Dry Soil";
  if (moisture > 35) status = "Normal";
  if (moisture > 70) status = "Wet Soil";

  String health = "Good";
  if (rawADC <= 5 || rawADC >= 1023) health = "Check Sensor";

  String json = "{";
  json += "\"raw\":" + String(rawADC) + ",";
  json += "\"moisture\":" + String(moisture) + ",";
  json += "\"uptime\":\"" + String(millis()/1000) + "s\",";
  json += "\"rssi\":-40,";
  json += "\"health\":\"" + health + "\",";
  json += "\"status\":\"" + status + "\"";
  json += "}";

  server.send(200,"application/json",json);
}

/*---------------- SETUP ----------------*/
void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);

  server.on("/", handleRoot);
  server.on("/data", handleData);

  server.begin();
}

/*---------------- LOOP ----------------*/
void loop() {

  server.handleClient();

  if (millis() - lastRead >= interval) {
    lastRead = millis();
    readSensor();
  }
}