# Exemple per executar: 
#   python simulate.py Stonks Stonks Stonks2 Stonks2
# Sortida:
#   Stonks won with seeds: 43913 70491 41637 65191
# 
#   Stonks2 won with seeds: 39428 20517 29384 42524 24253 54463
# 
#   Stonks won 4 times 40.00%
#   Stonks2 won 6 times 60.00%

import subprocess
import random
import os
import sys

N_PARTIDES = 10

def play(p1, p2, p3, p4):
    players = set()
    players.add(p1)
    players.add(p2)
    players.add(p3)
    players.add(p4)
    wins = {}
    total = 0
    seeds = {}
    for p in players:
        wins[p] = 0
        seeds[p] = set()
    for times in range(N_PARTIDES):
        print(f"{100.0*times/N_PARTIDES:2.2f}%")
        seed = random.randint(1,99999)
        command = f"./Game -s {seed} {p1} {p2} {p3} {p4} --input default.cnf --output out.res"
        res = subprocess.run(list(command.split(" ")), capture_output=True)
        for line in res.stderr.decode().split("\n"):
            if "got top score" in line:
                for p in players:
                    if p in line.split():
                        wins[p] += 1
                        total += 1
                        seeds[p].add(seed)
    print("100%\n\n")
    for p in sorted(players):
        print(f"{p} won with seeds: {' '.join([str(x) for x in seeds[p]])}\n")
    print('\n'.join([f"{p} won {wins[p]} times {100.0*wins[p]/total:2.2f}%" for p in sorted(players)]))

if __name__ == '__main__':
    assert(len(sys.argv) >= 5)
    play(*sys.argv[1:])