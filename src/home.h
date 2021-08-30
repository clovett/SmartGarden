const char* homePage = "<!DOCTYPE HTML>  \r\n\
<html>  \r\n\
<style>  \r\n\
body { background-color: #1E1E1E; color: #D4D4D4;}  \r\n\
button { min-width:80px; min-height:30px; }  \r\n\
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
function slope() { sendCommand('slope'); }  \r\n\
function info() { sendCommand('i'); }  \r\n\
function status() { sendCommand('status'); }  \r\n\
</script>  \r\n\
<body>  \r\n\
<h3>Smart Garden</h3>  \r\n\
<button onclick='read()'>Read</button>  \r\n\
&nbsp;  \r\n\
<button onclick='slope()'>Slope</button>  \r\n\
&nbsp;  \r\n\
<button onclick='info()'>Info</button>  \r\n\
&nbsp;  \r\n\
<button onclick='status()'>Status</button>  \r\n\
<br/><br/><br/>  \r\n\
<span class='big' id='result'></span>  \r\n\
</body>  \r\n\
  \r\n\
</html>  \r\n\
";
