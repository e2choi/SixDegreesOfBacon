#ifndef ACTORVERTEX_HPP
#define ACTORVERTEX_HPP

#include <string>
#include <vector>

#include "MovieEdge.hpp"

#define MAX_ACTORS 412528

using namespace std;

class MovieEdge;

/* Class defines the vertex of an Actor */
class ActorVert {
  public:
    const string actorName;  // name of vertex with actor's name
    ActorVert* previous;     // points to prev node
    MovieEdge* movie;        // movie object
    int distance;            // distance from starting node
    bool visited;
    vector<MovieEdge*> actorMovies;  // list of outgoing edges for an actor

    // constructor
    ActorVert(const string& name, vector<MovieEdge*> moviesList)
        : actorName(name),
          actorMovies(moviesList),
          distance(MAX_ACTORS),
          visited(false) {}
};

#endif