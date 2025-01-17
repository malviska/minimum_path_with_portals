import networkx as nx
import random
import math
import sys

def euclideanDistance(pos1, pos2):
  return math.sqrt(math.pow(pos1[0] - pos2[0], 2) + math.pow(pos1[1] - pos2[1], 2))

def generate_random_connected_graph(n, width, height):
  #while True:
  G:nx.Graph = nx.gnp_random_graph(n, 0.000001)  # Generate random graph
    # if nx.is_connected(G):  # Ensure the graph is connected
    #   break

    # Assign random coordinates in a 2D plane
  pos = {i: (random.uniform(-width, width), random.uniform(-height, height)) for i in G.nodes()}
  nx.set_node_attributes(G, pos, 'pos')
  
  G.remove_node(0)
  G.remove_node(n-1)
  G.add_node(0,pos = (random.uniform(-width, width), random.uniform(-height, height)))
  G.add_node(n-1, pos = (random.uniform(-width, width), random.uniform(-height, height)))
  connectionsToEnd = random.randint(round(n/2), n-1)
  init = 0
  final = n -1
  visited = []
  for i in range(connectionsToEnd):
    v = random.randint(1, n-2)
    if(v in visited):
      continue
    G.add_edge(init, v)
    visited.append(v)
    init = v
  G.add_edge(v, final)
  portalCount = 0
  pathsCount = 0
  for u, v in G.edges():
    portal = random.choices([True, False], weights=[0.2, 0.8])[0]
    position1 = G.nodes[v]['pos']
    position2 =  G.nodes[u]['pos']
    if(portal):
      portalCount += 1
      G[u][v]['weight'] = 0
      G[v][u]['weight'] = 0
    else:
      pathsCount += 1
      G[u][v]['weight'] = euclideanDistance(position1, position2)
      G[v][u]['weight'] = euclideanDistance(position1, position2)
  with open('./tests/graphtest2.txt', 'w') as file:
    file.write(f'{n} {pathsCount} {portalCount}\n')
    for node in sorted(G.nodes):
      pos = G.nodes[node]['pos']
      file.write(f'{pos[0]} {pos[1]}\n')
    for u, v in G.edges:
      if G[u][v]['weight'] > 0:
        if u < v:
          file.write(f'{u} {v}\n')
        else:
          file.write(f'{v} {u}\n')
    for u, v in G.edges:
      if G[u][v]['weight'] == 0:
        if u < v:
          file.write(f'{u} {v}\n')
        else:
          file.write(f'{v} {u}\n')
    file.write(f'{width*height} {portalCount}')

  return G

n = int(sys.argv[1])
G = generate_random_connected_graph(n, 100000, 100000)
#shortest_path_length = nx.shortest_path_length(G, 0, n-1, 'weight')
# shortest_path = nx.shortest_path(G, 0, n-1, 'weight')
# print(shortest_path_length)
# print(shortest_path)
# pos = nx.get_node_attributes(G, 'pos')

# Visualization (requires matplotlib)
# import matplotlib.pyplot as plt
# nx.draw(G, pos, with_labels=False)
# plt.show()