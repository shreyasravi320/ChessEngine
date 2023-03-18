# Web based GUI for my chess engine

# Packages
from flask import Flask
from flask import render_template
from flask import request
import subprocess
from subprocess import Popen, PIPE
import chess

app = Flask(__name__)

# Define index route
@app.route('/')
def root():
    return render_template('ion.html')

@app.route('/move', methods=['POST'])
def move():
    fen = request.form.get('fen')
    
    p = Popen(['./engine/ion'], shell=True, stdout=PIPE, stdin=PIPE)

    fen += '\n'
    x = bytes(fen, 'UTF-8')
    p.stdin.write(x)
    p.stdin.flush()

    best_move = p.stdout.readline()
    best_move = best_move.strip().decode()

    line = p.stdout.readline()
    line = line.strip().decode()

    moves = line.split()
    moves = [move.rstrip('\x00') for move in moves]

    b = chess.Board(fen)
    moves = b.variation_san([chess.Move.from_uci(move) for move in moves])

    moves = moves.split()
    if ('...' in moves[0]):
        moves[0] = moves[0][moves[0].rfind('.') + 1:]
        moves = [moves[i] for i in range(len(moves)) if i % 3 != 1]

    else:
        moves = [moves[i] for i in range(len(moves)) if i % 3 != 0]
        
    line = ' '.join(moves)

    score = p.stdout.readline()
    score = score.strip().decode()
    score = str(float(score) / 100)

    time = p.stdout.readline()
    time = time.strip().decode()

    nodes = p.stdout.readline()
    nodes = nodes.strip().decode()

    fen = p.stdout.readline()
    fen = fen.strip().decode()
    fen += " 0 1"

    return { 'fen': fen, 'best_move': best_move, 'line': line, 'score': score, 'time': time, 'nodes': nodes }

if __name__ == '__main__':
    
    # Start HTTP server
    app.run(debug=True, threaded=True)
