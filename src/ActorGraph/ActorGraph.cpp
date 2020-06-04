#include "ActorGraph.hpp"

#include <algorithm>
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
                    moviePtr = movieList.at(title);
                    movieFound = true;
                }
            }
            // if movie title not found or different year then create new movie
            // and insert to movieList
        }
        if (!movieFound) {
            vector<ActorVert*> moviesActors;
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
        this->~ActorGraph();
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
    // clear out values computed for each node and exit
    for (unordered_map<string, ActorVert*>::iterator actorListIter =
             actorList.begin();
         actorListIter != actorList.end(); actorListIter++) {
        (*actorListIter).second->distance = 0;
        (*actorListIter).second->previous = nullptr;
        (*actorListIter).second->movie = nullptr;
    }
    for (unordered_map<string, ActorVert*>::iterator iter = actorList.begin();
         iter != actorList.end(); iter++) {
        if (fromActor == ((*iter).second->actorName)) {
            fromActorInList = true;
        }
        if (toActor == (*iter).second->actorName) {
            toActorInList = true;
        }
    }
    // if fromActor and toActor in list then perform bfs
    if (fromActorInList && toActorInList) {
        // bfs alg
        // source node at fromActor
        ActorVert* source = actorList.at(fromActor);
        source->distance = 0;
        source->visited = false;
        q.push(source);
        // populate queue w neighbors for each node in queue
        while (!q.empty()) {
            ActorVert* curr = q.front();
            q.pop();
            // traverse all movie edges of q
            for (vector<MovieEdge*>::iterator movieIter =
                     curr->actorMovies.begin();
                 movieIter != curr->actorMovies.end(); ++movieIter) {
                // traverse actors of each movie and push them to queue b/c
                // means neighbors
                for (vector<ActorVert*>::iterator actorIter =
                         (*movieIter)->actors.begin();
                     actorIter != (*movieIter)->actors.end(); ++actorIter) {
                    ActorVert* neighbor = actorList[(*actorIter)->actorName];
                    // check if visisted yet if not then add to queue and mark
                    // as visisted
                    if (!neighbor->visited) {
                        neighbor->previous = curr;
                        neighbor->movie = *movieIter;
                        neighbor->distance = curr->distance + 1;
                        neighbor->visited = true;
                        q.push(neighbor);
                    }
                }
            }
        }

        // done traversing tree now need to compute path
        vector<ActorVert*> bfsPath;
        ActorVert* currNode = actorList.at(toActor);
        bfsPath.push_back(currNode);
        // compute path using previous pointer to traverse nodes
        while (currNode != actorList.at(fromActor)) {
            if (currNode->previous != nullptr) {
                currNode = currNode->previous;
                bfsPath.push_back(currNode);
            }
            // if no null ptr then no path back to fromActor
            else {
                break;
            }
        }
        // print out final format start from end because we created path
        // backwards
        reverse(bfsPath.begin(), bfsPath.end());

        for (unsigned int i = 0; i < bfsPath.size(); i++) {
            // cout << bfsPath.at(i)->actorName << endl;
            // check if toActor node so no arrows after it just (actor)
            if (i == bfsPath.size() - 1) {
                cout << "test1" << endl;
                shortestPath += "(" + bfsPath.at(i)->actorName + ")";
                // otherwise print the node and the path after its
            } else {
                cout << "test2" << endl;
                shortestPath += "(" + bfsPath.at(i)->actorName + ")";
                shortestPath +=
                    "--[" + bfsPath.at(i + 1)->movie->movieName + "#@" +
                    to_string(bfsPath.at(i + 1)->movie->year) + "]-->";
            }
        }
    }
    // else fromActor and toActor not in list so
    else {
        shortestPath = "";
    }
}

/* TODO */
ActorGraph::~ActorGraph() {
    for (unordered_map<string, ActorVert*>::iterator actorDestIter =
             actorList.begin();
         actorDestIter != actorList.end(); actorDestIter++) {
        delete (*actorDestIter).second;
    }
    for (unordered_map<string, MovieEdge*>::iterator movieDestIter =
             movieList.begin();
         movieDestIter != movieList.end(); movieDestIter++) {
        delete (*movieDestIter).second;
    }
}
