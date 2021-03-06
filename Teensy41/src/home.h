const char* homePage = "<!DOCTYPE HTML>  \r\n\
<html>  \r\n\
  <meta name='viewport' content='width=device-width, minimum-scale=1.0, maximum-scale=1.0' />  \r\n\
<style>  \r\n\
body { background-color: #1E1E1E; color: #D4D4D4;}  \r\n\
button { min-width:80px; min-height:30px;  -webkit-appearence: button; padding: 5px;}  \r\n\
.big { font-size:48pt; border-style: solid; border-width: 1px; border-color: #D4D4D4; padding: 10px; }  \r\n\
</style>  \r\n\
<script>  \r\n\
function sendCommand(cmd) {  \r\n\
  var xhr = new XMLHttpRequest();  \r\n\
  xhr.open('GET', '/' + cmd, true);  \r\n\
  xhr.onload = function(e) {  \r\n\
    if (this.status == 200) {  \r\n\
      document.getElementById('result').innerHTML = this.responseText;  \r\n\
    }  \r\n\
  };  \r\n\
  xhr.send();  \r\n\
}  \r\n\
function read() { sendCommand('r'); }  \r\n\
function info() { sendCommand('i'); }  \r\n\
function status() { sendCommand('status'); }  \r\n\
</script>  \r\n\
<body>  \r\n\
<h1>Smart Garden</h1>  \r\n\
<span>  \r\n\
<button onclick='read()'>Read</button>  \r\n\
&nbsp;  \r\n\
<button onclick='info()'>Info</button>  \r\n\
&nbsp;  \r\n\
<button onclick='status()'>Status</button>  \r\n\
</span>  \r\n\
<br/><br/><br/>  \r\n\
<span class='big' id='result'>0.000</span>  \r\n\
</body>  \r\n\
  \r\n\
</html>  \r\n\
";
