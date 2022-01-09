import os
import serial
import time
import sys
from flask import Flask, render_template


port = None
port_name = "/dev/ttyAMA0"

# port_name = os.getenv("SERIAL_PORT")
# if port_name is None:
#    print("Please configure SERIAL_PORT environment variable")
#    sys.exit(1)
# web_addr = os.getenv("WEB_ADDRESS")
# if web_addr is None:
#    print("Please configure WEB_ADDRESS environment variable")
#    sys.exit(1)

def readline(port):
    buffer = []
    while True:
        if port.in_waiting > 0:
            ch = ord(port.read(1))
            if ch == 13:
                return bytearray(buffer).decode('utf-8')
            else:
                buffer += [ch]

        else:
            time.sleep(0.01)


def sendcommand(command):
    print(command)
    global port
    if port is None:
        port = serial.Serial(port_name, baudrate=115200, timeout=3.0)
    port.write((command + "\r").encode())
    return readline(port)


app = Flask(__name__)


@app.route("/")
def home():
    return render_template('home.htm')


@app.route("/calibrate")
def calibrate():
    return render_template('calibrate.htm')


@app.route("/r")
def read():
    result = sendcommand('r')
    return result


@app.route("/i")
def info():
    result = sendcommand('i')
    return result


@app.route("/status")
def status():
    result = sendcommand('status')
    return result


@app.route("/mid")
def mid():
    result = sendcommand('cal,mid,7')
    return result


@app.route("/low")
def low():
    result = sendcommand('cal,low,4')
    return result


@app.route("/high")
def high():
    result = sendcommand('cal,high,10')
    return result

@app.route("/hello")
def hello():
    result = sendcommand('cal,low,4')
    return result

if __name__ == '__main__':
    app.run(debug=True)
