
#ifndef MOVIEEDGE_HPP
#define MOVIEEDGE_HPP

#include <string>
#include <vector>

#include "ActorVertex.hpp"

using namespace std;

class ActorVert;

// Class for creating an edge between 2 actors based on a movie ,year, and
// actors

class MovieEdge {
  public:
    string movieName;
    vector<ActorVert*> actors;
    unsigned int year;

    /* The constructor that creates a new edge */
    MovieEdge(string movie, unsigned int year, vector<ActorVert*> actors)
        : movieName(movie), year(year), actors(actors) {}
};

#endif