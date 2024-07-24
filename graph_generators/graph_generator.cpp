#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/config.hpp>
#include <iostream>
#include <random>
#include <cmath>
#include <fstream>
using namespace boost;

#define VERTICES 1000000
#define CHANCE_OF_PATH 0.000003
#define CHANCE_OF_PORTAL 0.0000005
#define QUADRANT 1000000

struct EdgeTrait {
  double weight;
  bool portal;
};

struct VertexTrait {
  double x;
  double y;

};

typedef adjacency_list<vecS, vecS, undirectedS, VertexTrait, EdgeTrait> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_iterator VertexIterator;
typedef std::pair<int, int> EdgePair;

void printGraph(const Graph& graph, int vrts, int portals, int paths, const std::string& filename){
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return;
  }
  file<<vrts<<' '<<paths<<' '<<portals<<'\n';
  std::pair<VertexIterator, VertexIterator> itr = vertices(graph);
  for(VertexIterator it = itr.first; it != itr.second; ++it){
    Vertex v = *it;
    file<<graph[v].x <<' ' <<graph[v].y <<'\n';
  }
  auto weightmap = boost::get(&EdgeTrait::weight, graph);
  BGL_FORALL_EDGES(e, graph, Graph){
    double weight = weightmap[e];
    if(weight == 0){
      continue;
    }
    Vertex u = source(e, graph);
    Vertex v = target(e, graph);
    file<<u<<' '<<v<<'\n';
  }
  BGL_FORALL_EDGES(e, graph, Graph){
    double weight = weightmap[e];
    if(weight > 0){
      continue;
    }
    Vertex u = source(e, graph);
    Vertex v = target(e, graph);
    file<<u<<' '<<v<<'\n';
  }

  file<<portals+paths<<' '<<portals<<'\n';

}

double euclideanDistance(const VertexTrait& v1, const VertexTrait& v2){
  return std::sqrt(std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2));
}

int main(int argc, char* argv[]) {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<> dis(0.0, 1.0);
  int numVertices = VERTICES;
  double chanceOfEdge = CHANCE_OF_PATH;
  double chanceOfPortal = CHANCE_OF_PORTAL;
  double quadrant = QUADRANT;
  std::normal_distribution<> normalPath(chanceOfEdge*numVertices, std::sqrt(chanceOfEdge*numVertices));
  std::normal_distribution<> normalPortal(chanceOfPortal*numVertices, std::sqrt(chanceOfPortal*numVertices));
  std::uniform_real_distribution<> quad(-quadrant, quadrant);
  Graph graph;
  std::vector<Vertex> vrts;

  for(int i = 0; i< numVertices; i++){
    vrts.push_back(add_vertex({quad(gen), quad(gen)}, graph));
  }
  int euclideanPaths = 0;
  int portals = 0;

  // for (int i = 0; i< numVertices-1; ++i){
  //   double weight = euclideanDistance(graph[vrts[i]], graph[vrts[i+1]]);
  //   add_edge(vrts[i], vrts[i+1], EdgeTrait{weight}, graph);
  //   euclideanPaths++;
  // }

  for(int i = 0; i< numVertices; i++){
    int random_value = std::round(normalPath(gen));
    //std::cout<<"paths: "<<random_value<<std::endl;
    std::vector<int> neightbours(numVertices, 0);
    neightbours[i] = 1;
    int k = 0;
    while(k<random_value){
      int aleatory_vertex = std::rand() % numVertices;
      if(neightbours[aleatory_vertex] == 1){
        continue;
      }
      neightbours[aleatory_vertex] = 1;
      double weight = euclideanDistance(graph[vrts[i]], graph[vrts[aleatory_vertex]]);
      add_edge(vrts[i], vrts[aleatory_vertex], EdgeTrait{weight}, graph);
      euclideanPaths++;
      k++;
    }
    random_value = std::round(normalPortal(gen));
    //std::cout<<"portals: "<<random_value<<std::endl;
    k = 0;
    while(k<random_value){
      int aleatory_vertex = std::rand() % numVertices;
      if(neightbours[aleatory_vertex] == 1){
        continue;
      }
      neightbours[aleatory_vertex] = 1;
      double weight = 0;
      add_edge(vrts[i], vrts[aleatory_vertex], EdgeTrait{weight}, graph);
      portals++;
      k++;
    }
  }
  // for(int i = 0; i< numVertices; i++){
  //   for(int j = 0; j< numVertices; j++){
  //     if (i == j){
  //       continue;
  //     }
  //     double randomProbability = dis(gen);
  //     if (randomProbability < chanceOfEdge){
  //       double weight = euclideanDistance(graph[vrts[i]], graph[vrts[j]]);
  //       add_edge(vrts[i], vrts[j], EdgeTrait{weight}, graph);
  //       euclideanPaths++;
  //       continue;
  //     }
  //     randomProbability = dis(gen);
  //     if(randomProbability < chanceOfPortal){
  //       double weight = 0;
  //       add_edge(vrts[i], vrts[j], EdgeTrait{weight}, graph);
  //       portals++;
  //       continue;
  //     }
  //   }
  // }
  auto weightmap = boost::get(&EdgeTrait::weight, graph);
  std::vector<double> distances(num_vertices(graph), std::numeric_limits<double>::infinity());
  std::vector<Vertex> p(num_vertices(graph));
  std::cout<<"checkpoint\n";
  dijkstra_shortest_paths(graph, vertex(0, graph), predecessor_map(boost::make_iterator_property_map(
                            p.begin(), get(boost::vertex_index, graph))).distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, graph))).weight_map(weightmap));
  std::cout<<distances[distances.size() - 1]<<'\n';
  printGraph(graph, numVertices, portals, euclideanPaths, "./tests/graph.txt");

  return 0;
  
}