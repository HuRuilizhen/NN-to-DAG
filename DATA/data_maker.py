import random
import sys
import argparse


def edgeGenerator(NUM_NODES, WEIGHT_RANGE):
    u = random.randrange(0, NUM_NODES)
    v = random.randrange(0, NUM_NODES)
    while v == u:
        v = random.randrange(0, NUM_NODES)

    w = random.randrange(0, WEIGHT_RANGE)

    if u > v:
        u, v = v, u

    return f"{u} {v} {w}\n"


def graphGenerator(NUM_NODES, NUM_EDGES, WEIGHT_RANGE, FILENAME):
    with open("Graph.in", "w", encoding="utf8") as f:
        f.write(f"{NUM_NODES} {NUM_EDGES}\n")
        for line in range(NUM_EDGES):
            f.write(edgeGenerator(NUM_NODES, WEIGHT_RANGE))


if __name__ == "__main__":
    NUM_NODES = 20
    NUM_EDGES = 100
    WEIGHT_RANGE = 1000
    RANDOM_SEED = -1
    FILENAME = "graph.in"

    paser = argparse.ArgumentParser()
    paser.add_argument("-NUM_NODES", dest="NUM_NODES", required=False)
    paser.add_argument("-NUM_EDGES", dest="NUM_EDGES", required=False)
    paser.add_argument("-WEIGHT_RANGE", dest="WEIGHT_RANGE", required=False)
    paser.add_argument("-RANDOM_SEED", dest="RANDOM_SEED", required=False)
    paser.add_argument("-FILENMAE", dest="FILENAME", required=False)
    args = paser.parse_args()

    if args.NUM_NODES:
        NUM_NODES = int(args.NUM_NODES)
    if args.NUM_EDGES:
        NUM_EDGES = int(args.NUM_EDGES)
    if args.WEIGHT_RANGE:
        WEIGHT_RANGE = int(args.WEIGHT_RANGE)
    if args.RANDOM_SEED:
        RANDOM_SEED = int(args.RANDOM_SEED)
    if args.FILENAME:
        FILENAME = int(args.FILENAME)
    if RANDOM_SEED != -1:
        random.seed(RANDOM_SEED)

    graphGenerator(NUM_NODES, NUM_EDGES, WEIGHT_RANGE, FILENAME)
