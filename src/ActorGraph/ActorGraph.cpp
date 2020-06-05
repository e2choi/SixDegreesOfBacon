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
        vector<MovieEdge*> movies;
        vector<ActorVert*> actors;
        unordered_map<string, ActorVert*>::iterator actorIter =
            actorList.find(actor);
        if (actorIter == actorList.end()) {
            ActorVert* actor_name = new ActorVert(actor, movies);
            actorList.emplace(actor, actor_name);
            actorIter = actorList.find(actor);
        }
        string newTitle = title + to_string(year);
        unordered_map<string, MovieEdge*>::iterator movieIter =
            movieList.find(newTitle);
        if (movieIter == movieList.end()) {
            MovieEdge* movie1 = new MovieEdge(title, year, actors);
            movieList.emplace(newTitle, movie1);
            movieIter = movieList.find(newTitle);
        } else {
            if (year != (*movieIter).second->year) {
                MovieEdge* movie2 = new MovieEdge(title, year, actors);
                movieList.emplace(newTitle, movie2);
                movieIter = movieList.find(newTitle);
            }
        }
        // ActorVert* newActor;
        // MovieEdge* newMovie;
        // cout << "Input: " << actor << " " << title << " " << year << endl;
        // unordered_map<string, ActorVert*>::iterator actorIter =
        //     actorList.find(actor);
        // if (actorIter == actorList.end()) {
        //     newActor = new ActorVert(actor, movies);
        //     actorIter = actorList.insert(make_pair(title, newActor)).first;
        //     cout << (*actorIter).second->actorName << endl;
        //     cout << "actor if" << endl;
        // }
        // unordered_map<string, MovieEdge*>::iterator movieIter =
        //     movieList.find(title);
        // if (movieIter == movieList.end()) {
        //     newMovie = new MovieEdge(title, year, actors);
        //     movieIter = movieList.insert(make_pair(title, newMovie)).first;
        //     cout << (*movieIter).second->movieName << endl;
        //     cout << "movie if" << endl;
        // } else {
        //     if (year != (*movieIter).second->year) {
        //         newMovie = new MovieEdge(title, year, actors);
        //         movieIter = movieList.insert(make_pair(title,
        //         newMovie)).first; cout << (*movieIter).second->movieName <<
        //         endl; cout << "year if" << endl;
        //     } else {
        //         movieIter = movieList.find(title);
        //         cout << "else" << endl;
        //     }
        // }
        // update actor's movie list and movie's actor list (adjacency matrices)
        // cout << (*actorIter).second->actorName << ": ";
        (*actorIter).second->actorMovies.push_back((*movieIter).second);
        (*movieIter).second->actors.push_back((*actorIter).second);
        // for (int i = 0; i < (*actorIter).second->actorMovies.size(); i++) {
        //     cout << (*actorIter).second->actorMovies[i]->movieName << " ";
        // }
        // cout << endl;
        // cout << (*movieIter).second->movieName << ": ";
        // for (int i = 0; i < (*movieIter).second->actors.size(); i++) {
        //     cout << (*movieIter).second->actors[i]->actorName << " ";
        // }
        // cout << endl;
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
        (*actorListIter).second->visited = false;
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
            curr->visited = true;
            if (curr == actorList[toActor]) {
                break;
            }
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
            // check if toActor node so no arrows after it just (actor)
            if (i == bfsPath.size() - 1) {
                shortestPath += "(" + bfsPath.at(i)->actorName + ")";
                // otherwise print the node and the path after its
            } else {
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
