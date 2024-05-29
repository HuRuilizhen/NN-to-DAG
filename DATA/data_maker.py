import random
import argparse
import networkx as nx

def edgeWeightGenerator(WEIGHT_RANGE):
    return random.randrange(WEIGHT_RANGE[0], WEIGHT_RANGE[1])

def edgeGenerator(NUM_NODES, WEIGHT_RANGE, BIAS=0):
    u = BIAS + random.randrange(0, NUM_NODES)  
    v = BIAS + random.randrange(0, NUM_NODES)
    
    while v == u:
        v = BIAS + random.randrange(0, NUM_NODES)

    w = edgeWeightGenerator(WEIGHT_RANGE)

    if u > v:
        u, v = v, u

    return u, v, w


def edgeString(edge):
    return f"{edge[0]} {edge[1]} {edge[2]}"

def printGraph(NUM_NODES, NUM_EDGES , edges, FILENAME):
    if FILENAME != "":
        with open(FILENAME, "w", encoding="utf8") as f:
            f.write(f"{NUM_NODES} {NUM_EDGES}\n")
            for edge in edges:
                f.write(edgeString(edge)+"\n")
    else:
        print(f"{NUM_NODES} {NUM_EDGES}")
        for edge in edges:
            print(edgeString(edge))


def randomGraphGenerator(NUM_NODES, NUM_EDGES, WEIGHT_RANGE, FILENAME):
    edges = []

    for line in range(NUM_EDGES):
        egde = edgeGenerator(NUM_NODES, WEIGHT_RANGE)
        edges.append(egde)
    
    printGraph(NUM_NODES, NUM_EDGES, edges, FILENAME)

def randWireGroupGenerator(NUM_NODES, NUM_EDGES, WEIGHT_RANGE, BIAS):
    edges = []  
    hasInEdgeNodes = []
    hasOutEdgeNodes = []
        
    for line in range(NUM_EDGES):
        egde = edgeGenerator(NUM_NODES + 1, WEIGHT_RANGE, BIAS)
        edges.append(egde)
        
        if not egde[1] in hasInEdgeNodes:
            hasInEdgeNodes.append(egde[1])
        if not egde[0] in hasOutEdgeNodes:
            hasOutEdgeNodes.append(egde[0])

    for node in range(NUM_NODES + 1):
        node = BIAS + node
        
        if node not in hasInEdgeNodes:
            if BIAS == node:
                continue
            edge = BIAS, node, edgeWeightGenerator(WEIGHT_RANGE)
            edges.append(edge)

        if node not in hasOutEdgeNodes:
            if node == BIAS + NUM_NODES:
                continue
            edge = node, BIAS + NUM_NODES, edgeWeightGenerator(WEIGHT_RANGE)
            edges.append(edge)
    
    return edges        


def randWireGraphGenerator(LIST_RANDWIRE_NODES, LIST_RANDWIRE_EDGES , WEIGHT_RANGE, FILENAME):
    edges = []
    BIAS = 0

    for i in range(len(LIST_RANDWIRE_NODES)):
        NUM_NODES = LIST_RANDWIRE_NODES[i]
        NUM_EDGES = LIST_RANDWIRE_EDGES[i]

        edges.extend(randWireGroupGenerator(NUM_NODES, NUM_EDGES, WEIGHT_RANGE, BIAS))
        BIAS += NUM_NODES
        
    NUM_NODES = BIAS + 1
    NUM_EDGES = len(edges)
    
    printGraph(NUM_NODES, NUM_EDGES, edges, FILENAME)

def fcnnGraphGenerator(LIST_FCNN_NODES, WEIGHT_RANGE, FILENAME):
    edges = []
    BIAS = 0

    for i in range(len(LIST_FCNN_NODES) - 1):
        NUM_CURRENT_NODES = LIST_FCNN_NODES[i]
        NUM_NEXT_NODES = LIST_FCNN_NODES[i + 1]

        for u in range(NUM_CURRENT_NODES):
            for v in range(NUM_NEXT_NODES):
                edge = BIAS + u, BIAS + NUM_CURRENT_NODES + v, edgeWeightGenerator(WEIGHT_RANGE)
                edges.append(edge)

        BIAS += NUM_CURRENT_NODES

    NUM_NODES = sum(LIST_FCNN_NODES)
    NUM_EDGES = len(edges)

    printGraph(NUM_NODES, NUM_EDGES, edges, FILENAME)


def erGraphGenerator(NUM_NODES, ER_PROBABILITY, WEIGHT_RANGE, FILENAME):
    edges = []
    G = nx.erdos_renyi_graph(NUM_NODES, ER_PROBABILITY)

    for u, v in G.edges():
        if u > v:
            u, v = v, u
        edges.append((u, v, edgeWeightGenerator(WEIGHT_RANGE)))

    NUM_EDGES = len(edges)
    printGraph(NUM_NODES, NUM_EDGES, edges, FILENAME)

def baGraphGenerator(NUM_NODES, BA_INITIAL_NODES, WEIGHT_RANGE, FILENAME):
    edges = []
    G = nx.barabasi_albert_graph(NUM_NODES, BA_INITIAL_NODES)

    for u, v in G.edges():
        if u > v:
            u, v = v, u
        edges.append((u, v, edgeWeightGenerator(WEIGHT_RANGE)))
        
    NUM_EDGES = len(edges)
    printGraph(NUM_NODES, NUM_EDGES, edges, FILENAME)
    
def wsGraphGenerator(NUM_NODES, WS_K, WS_PROBABILITY, WEIGHT_RANGE, FILENAME):
    edges = []
    G = nx.watts_strogatz_graph(NUM_NODES, WS_K, WS_PROBABILITY)

    for u, v in G.edges():
        if u > v:
            u, v = v, u
        edges.append((u, v, edgeWeightGenerator(WEIGHT_RANGE)))
        
    NUM_EDGES = len(edges)
    printGraph(NUM_NODES, NUM_EDGES, edges, FILENAME)

if __name__ == "__main__":
    
    """ ARGS DEFAULT SETTING """
    NUM_NODES = 20
    NUM_EDGES = 100
    WEIGHT_MEAN = 500
    WEIGHT_DEV = 100
    WEIGHT_RANGE = []
    RANDOM_SEED = -1
    FILENAME = ""
    MODE = "Random"
    MODE_LIST = ["Random", "RandWire", "FCNN", "ER", "BA", "WS"]
    LIST_RANDWIRE_NODES = [50, 50, 50]
    LIST_RANDWIRE_EDGES = [100, 100, 100]
    LIST_FCNN_NODES = [20, 20, 20]
    ER_PROBABILITY = 0.5
    BA_INITIAL_NODES = 20
    WS_K = 10
    WS_PROBABILITY = 0.5
    
    paser = argparse.ArgumentParser()
    paser.add_argument("-NUM_NODES", dest="NUM_NODES", required=False, type=int)
    paser.add_argument("-NUM_EDGES", dest="NUM_EDGES", required=False, type=int)
    paser.add_argument("-WEIGHT_MEAN", dest="WEIGHT_MEAN", required=False, type=int)
    paser.add_argument("-WEIGHT_DEV", dest="WEIGHT_DEV", required=False, type=int)
    paser.add_argument("-RANDOM_SEED", dest="RANDOM_SEED", required=False, type=int)
    paser.add_argument("-FILENAME", dest="FILENAME", required=False, type=str)
    paser.add_argument(
        "-MODE", dest="MODE", required=False, type=str, choices=MODE_LIST
    )

    paser.add_argument(
        "-LIST_RANDWIRE_GROUP_NODES",
        dest="LIST_RANDWIRE_GROUP_NODES",
        required=False,
        type=lambda s: list(map(int, s.split(","))),
    )
    paser.add_argument(
        "-LIST_RANDWIRE_GROUP_EDGES",
        dest="LIST_RANDWIRE_GROUP_EDGES",
        required=False,
        type=lambda s: list(map(int, s.split(","))),
    )


    paser.add_argument(
        "-LIST_FCNN_GROUP_NODES",
        dest="LIST_FCNN_GROUP_NODES",
        required=False,
        type=lambda s: list(map(int, s.split(","))),
    )

    paser.add_argument(
        "-ER_PROBABILITY",
        dest="ER_PROBABILITY",
        required=False,
        type=float,
    )

    paser.add_argument(
        "-BA_INITIAL_NODES",
        dest="BA_INITIAL_NODES",
        required=False,
        type=int,
    )
    
    paser.add_argument(
        "-WS_K",
        dest="WS_K",
        required=False,
        type=int,
    )
    paser.add_argument(
        "-WS_PROBABILITY",
        dest="WS_PROBABILITY",
        required=False,
        type=float,
    )

    args = paser.parse_args()
    
    

    """ GLOBAL: BASIC SETTING """
    if args.RANDOM_SEED:
        RANDOM_SEED = int(args.RANDOM_SEED)
    if RANDOM_SEED != -1:
        random.seed(RANDOM_SEED)
    if args.FILENAME:
        FILENAME = args.FILENAME

    """ GLOBAL: WEIGHT RANGE SETTING """
    if args.WEIGHT_MEAN:
        WEIGHT_MEAN = int(args.WEIGHT_MEAN)
    if args.WEIGHT_DEV:
        WEIGHT_DEV = int(args.WEIGHT_DEV)
    WEIGHT_DEV = min(WEIGHT_MEAN, WEIGHT_DEV)
    WEIGHT_RANGE = [WEIGHT_MEAN - WEIGHT_DEV, WEIGHT_MEAN + WEIGHT_DEV]

    """ RANDOM MODE: NUM NODES AND NUM EDGES SETTING  """
    if args.NUM_NODES:
        NUM_NODES = int(args.NUM_NODES)
    if args.NUM_EDGES:
        NUM_EDGES = int(args.NUM_EDGES)

    """ RANDOMWIRE MODE: NUM NODES AND NUM EDGES PER GROUP SETTING  """
    if args.LIST_RANDWIRE_GROUP_NODES:
        LIST_RANDWIRE_NODES = args.LIST_RANDWIRE_GROUP_NODES
    if args.LIST_RANDWIRE_GROUP_EDGES:
        LIST_RANDWIRE_EDGES = args.LIST_RANDWIRE_GROUP_EDGES

    """ FCNN MODE: NUM NODES PER LAYER SETTING  """
    if args.LIST_FCNN_GROUP_NODES:
        LIST_FCNN_NODES = args.LIST_FCNN_GROUP_NODES
        
    """ ER MODE: NUM NODES AND PROBABILITY SETTING  """
    if args.ER_PROBABILITY:
        ER_PROBABILITY = float(args.ER_PROBABILITY)
        
    """ BA MODE: NUM NODES AND INITIAL NODES SETTING  """
    if args.BA_INITIAL_NODES:
        BA_INITIAL_NODES = int(args.BA_INITIAL_NODES)
        
    """ WS MODE: NUM NODES, K AND PROBABILITY SETTING  """
    if args.WS_K:
        WS_K = int(args.WS_K)
    if args.WS_PROBABILITY:
        WS_PROBABILITY = float(args.WS_PROBABILITY)

    """ MODE CHOICE """
    if args.MODE:
        MODE = args.MODE
    if MODE == "Random" or MODE not in MODE_LIST:
        randomGraphGenerator(NUM_NODES, NUM_EDGES, WEIGHT_RANGE, FILENAME)
    elif MODE == "RandWire":
        randWireGraphGenerator(
            LIST_RANDWIRE_NODES,
            LIST_RANDWIRE_EDGES,
            WEIGHT_RANGE,
            FILENAME,
        )
    elif MODE == "FCNN":
        fcnnGraphGenerator(LIST_FCNN_NODES, WEIGHT_RANGE, FILENAME)
    elif MODE == "ER":
        erGraphGenerator(NUM_NODES, ER_PROBABILITY, WEIGHT_RANGE, FILENAME)
    elif MODE == "BA":
        baGraphGenerator(NUM_NODES, BA_INITIAL_NODES, WEIGHT_RANGE, FILENAME)
    elif MODE == "WS":
        wsGraphGenerator(NUM_NODES, WS_K, WS_PROBABILITY, WEIGHT_RANGE, FILENAME)
