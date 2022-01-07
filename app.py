# Web based GUI for my chess engine

# Packages
from flask import Flask
from flask import render_template

app = Flask(__name__)

# Define index route
@app.route('/')
def root():
    return render_template('ion.html')

if __name__ == '__main__':
    
    # Start HTTP server
    app.run(debug=True, threaded=True)
