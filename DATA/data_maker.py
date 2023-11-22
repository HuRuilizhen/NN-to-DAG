import random

NUM_NODES = 10000
NUM_EDGES = 100000

WEIGHT_RANGE = 1000


def edgeGenerator():
    u = random.randrange(0, NUM_NODES)
    v = random.randrange(0, NUM_NODES)
    w = random.randrange(0, WEIGHT_RANGE)

    if u > v:
        u, v = v, u

    return f"{u} {v} {w}\n"


with open("Graph.in", "w", encoding="utf8") as f:
    f.write(f"{NUM_NODES} {NUM_EDGES}\n")
    for line in range(NUM_EDGES):
        f.write(edgeGenerator())
