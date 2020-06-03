
#ifndef MOVIEEDGE_HPP
#define MOVIEEDGE_HPP

#include <string>
#include <vector>

#include "ActorVertex.hpp"

using namespace std;

class Actor;

// Class for creating an edge between 2 actors based on a movie ,year, and
// actors

class MovieEdge {
  public:
    string movie;
    vector<Actor*> actors;
    unsigned int year;

    /* The constructor that creates a new edge */
    MovieEdge(string movie, unsigned int year, vector<Actor*> actors)
        : movie(movie), year(year), actors(actors) {}
};

#endif