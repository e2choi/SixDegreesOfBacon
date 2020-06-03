#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <ActorVertex.hpp>
#include <MovieEdge.hpp>
#include <iostream>
#include <unordered_map>

using namespace std;

class ActorGraph {
  protected:
    // TODO: add data structures used in actor graph
    unordered_map<string, MovieEdge*> movieList;
    unordered_map<string, ActorVert*> actorList;

  public:
    /* TODO */
    ActorGraph();

    /* TODO */
    bool buildGraph(istream& is);

    /* TODO */
    void BFS(const string& fromActor, const string& toActor,
             string& shortestPath);

    /* TODO */
    ~ActorGraph();
};

#endif  // ACTORGRAPH_HPP
