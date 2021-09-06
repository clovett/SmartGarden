from flask import Flask, render_template

app = Flask(__name__)

@app.route("/")
def home():
    return render_template('home.htm')

app.run(debug = True, host="192.168.1.119")
