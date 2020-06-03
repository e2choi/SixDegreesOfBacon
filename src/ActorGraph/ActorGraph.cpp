#include "ActorGraph.hpp"

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

ActorGraph::ActorGraph() {}

/* Build the actor graph from dataset file.
 * Each line of the dataset file must be formatted as:
 * ActorName <tab> MovieName <tab> Year
 * Two actors are connected by an undirected edge if they have worked in a movie
 * before.
 */
bool ActorGraph::buildGraph(istream& is) {
    bool readHeader = false;

    while (is) {
        string s;
        if (!getline(is, s)) break;

        // skip the header of the file
        if (!readHeader) {
            readHeader = true;
            continue;
        }

        // read each line of the dataset to get the movie actor relation
        istringstream ss(s);
        vector<string> record;
        while (ss) {
            string str;
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        // if format is wrong, skip current line
        if (record.size() != 3) {
            continue;
        }

        // extract the information
        string actor(record[0]);
        string title(record[1]);
        int year = stoi(record[2]);

        // TODO: we have an actor/movie relationship to build the graph

        /* process actor then title info */
        // check if actor is in actorList if so we go to it
        ActorVert* actorPtr;
        bool actorFound = false;
        for (unordered_map<string, ActorVert*>::iterator iter =
                 actorList.begin();
             iter != actorList.end(); iter++) {
            if (actor == (*iter).first) {
                actorPtr = actorList.at(actor);
                actorFound = true;
            }
        }
        // if not in actorList then we must insert actor
        if (!actorFound) {
            vector<MovieEdge*> actorsMovies;
            actorPtr = new ActorVert(actor, actorsMovies);
            actorList.emplace(actor, actorPtr);
        }

        /* process movie info */
        MovieEdge* moviePtr;
        bool movieFound = false;
        for (unordered_map<string, MovieEdge*>::iterator iter =
                 movieList.begin();
             iter != movieList.end(); iter++) {
            /* if movie title exists in mapping then check if same year so
             * already exist or if different year then doesnt exist */
            if (title == (*iter).first) {
                // if same year then movie already exists so go to it
                if (year == (*iter).second->year) {
                    moviePtr = movieList.at((*iter).second->movie);
                    movieFound = true;
                }
            }
            // if movie title not found or different year then create new movie
            // and insert to movieList
        }
        if (!movieFound) {
            vector<Actor*> moviesActors;
            moviePtr = new MovieEdge(title, year, moviesActors);
            movieList.emplace(title, moviePtr);
        }
        // update actor's movie list and movie's actor list (adjacency matrices)
        actorPtr->actorMovies.push_back(moviePtr);
        moviePtr->actors.push_back(actorPtr);
    }

    // if failed to read the file, clear the graph and return
    if (!is.eof()) {
        // TODO: delete the graph
        ~ActorGraph();
        return false;
    }

    return true;
}

/* TODO */
void ActorGraph::BFS(const string& fromActor, const string& toActor,
                     string& shortestPath) {
    // check if fromActor and toActor are in actorList first
    bool fromActorInList = false;
    bool toActorInList = false;
    queue<ActorVert*> q;
    for (unordered_map<string, ActorVert*>::iterator iter = actorList.begin();
         iter != actorList.end(); iter++) {
        if (fromActor == (*iter).first) {
            fromActorInList = true;
        }
        if (toActor == (*iter).first) {
            toActorInList = true;
        }
    }
    // if fromActor and toActor in list then perform bfs
    if (fromActorInList && toActorInList) {
        // bfs alg
        // source node at fromActor
        ActorVert* source = actorList.at(fromActor);
        source->distance = 0;
        q.push(source);

    }
    // else fromActor and toActor not in list so
    else {
        shortestPath = "";
        return;
    }
}

/* TODO */
ActorGraph::~ActorGraph() {}
