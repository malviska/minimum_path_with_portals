#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/config.hpp>
#include <iostream>
#include <random>
#include <cmath>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <unordered_set>
#include <iterator>

using namespace boost;

#define VERTICES 50000
#define CHANCE_OF_PATH 0.05
#define CHANCE_OF_PORTAL 0.01
#define QUADRANT 1000000

struct EdgeTrait {
  double weight;
  bool portal;
};

struct VertexTrait {
  double x;
  double y;

};

typedef adjacency_list<vecS, vecS, directedS, VertexTrait, EdgeTrait> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_iterator VertexIterator;
typedef std::pair<int, int> EdgePair;

void clkDiff(struct timespec t1, struct timespec t2,
                   struct timespec * res)
// Descricao: calcula a diferenca entre t2 e t1, que e armazenada em res
// Entrada: t1, t2
// Saida: res
{
  if (t2.tv_nsec < t1.tv_nsec){
    // ajuste necessario, utilizando um segundo de tv_sec
    res-> tv_nsec = 1000000000+t2.tv_nsec-t1.tv_nsec;
    res-> tv_sec = t2.tv_sec-t1.tv_sec-1;
  } else {
    // nao e necessario ajuste
    res-> tv_nsec = t2.tv_nsec-t1.tv_nsec;
    res-> tv_sec = t2.tv_sec-t1.tv_sec;
  }
}

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
  struct timespec inittp, endtp, restp;
  int retp;

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<> dis(0.0, 1.0);
  int numVertices = std::stoi(argv[1]);
  int numPaths = std::stoi(argv[2]);
  int numPortals = std::stoi(argv[3]);
  double quadrant = QUADRANT;
  //std::normal_distribution<> normalPath(chanceOfEdge*numVertices, std::sqrt(chanceOfEdge*numVertices));
  //std::normal_distribution<> normalPortal(chanceOfPortal*numVertices, std::sqrt(chanceOfPortal*numVertices));
  std::uniform_real_distribution<> quad(-quadrant, quadrant);
  Graph graph;
  std::vector<Vertex> vrts;

  for(int i = 0; i< numVertices; i++){
    vrts.push_back(add_vertex({quad(gen), quad(gen)}, graph));
  }
  // std::vector<int> visited(numVertices, 0);
  // int val = 0;
  // for(int i = 0; i< (int) numVertices*0.7; i++){
  //   int aleatory_vertex = std::rand() % numVertices -1;
  //   if(visited[aleatory_vertex] == 1){
  //     continue;
  //   }
  //   visited[aleatory_vertex] = 1;
  //   double weight = euclideanDistance(graph[vrts[val]], graph[vrts[aleatory_vertex]]);
  //   add_edge(vrts[val], vrts[aleatory_vertex], EdgeTrait{weight}, graph);
  //   val = aleatory_vertex;
  //   euclideanPaths++;
  // }
  // double weight = euclideanDistance(graph[vrts[val]], graph[vrts[numVertices - 1]]);
  // add_edge(vrts[val], vrts[numVertices - 1], EdgeTrait{weight}, graph);
  std::vector<std::pair<int,int>> links;
  for(int i = 0; i<numVertices-2; i++){
    for(int j = i+1; j< numVertices-1; j++){
      links.push_back({i,j});
    }
  }
  //std::cout<<"checkpoint"<<links.size()<<'\n';

  for(int i = 0; i< numPortals; i++){
    //clock_t time0 = clock();
    int randomIndex = std::rand() % links.size();
    std::vector<std::pair<int,int>>::iterator it = links.begin();
    //clock_t time1 = clock();
    std::advance(it, randomIndex);
    //clock_t time2 = clock();
    double weight = 0;
    add_edge(vrts[it->first], vrts[it->second], EdgeTrait{weight}, graph);
    //clock_t time3 = clock();
    links.erase(it);
    //clock_t time4 = clock();
   // std::cout<<time1-time0<<' '<<time2-time1<<' '<<time3 - time2 <<' '<<time4 - time3<<std::endl;
  }

  //std::cout<<"checkpoint\n";

  for(int i = 0; i< numPaths; i++){
    int randomIndex = std::rand() % links.size();
    int k = 0;
    auto it = links.begin();
    std::advance(it, randomIndex);
    double weight = euclideanDistance(graph[vrts[it->first]], graph[vrts[it->second]]);
    add_edge(vrts[it->first], vrts[it->second], EdgeTrait{weight}, graph);
    links.erase(it);
  }

  //std::cout<<"checkpoint\n";
  auto weightmap = boost::get(&EdgeTrait::weight, graph);
  std::vector<double> distances(num_vertices(graph), std::numeric_limits<double>::infinity());
  std::vector<Vertex> p(num_vertices(graph));
  //std::cout<<"checkpoint\n";
  retp = clock_gettime(CLOCK_MONOTONIC, &inittp);
  dijkstra_shortest_paths(graph, vertex(0, graph), predecessor_map(boost::make_iterator_property_map(
                            p.begin(), get(boost::vertex_index, graph))).distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, graph))).weight_map(weightmap));
  retp = clock_gettime(CLOCK_MONOTONIC, &endtp);
  clkDiff(inittp, endtp, &restp);
  //"list,%d,%d,%d,%ld.%.9ld,%.4lf,%ld.%.9ld,%.4lf,%ld,%ld\n", n, m, k, dijkstra_sec, dijkstra_nsec, dijkstra_distance, a_star_sec, a_star_nsec, a_star_distance, allocs, memoryUsage
  double finalDistance = distances[distances.size() - 1] == std::numeric_limits<double>::infinity()-1000 ? -1 : distances[distances.size() - 1];
  std::cout<<"boost,"<<numVertices<< "," << numPaths << "," << numPortals << "," << restp.tv_sec << "." << restp.tv_nsec << "," << finalDistance <<'\n';
  // int vertx = numVertices -1;
  // while(vertx != 0 ){
  //   std::cout<<p[vertx]<<" ";
  //   vertx = p[vertx];
  // }
  printGraph(graph, numVertices, numPortals, numPaths, "./tests/graph.txt");

  return 0;
  
}