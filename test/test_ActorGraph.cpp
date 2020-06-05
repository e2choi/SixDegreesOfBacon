#include <gtest/gtest.h>

#include "ActorGraph.hpp"

using namespace std;
using namespace testing;

TEST(ActorGraphTests, TwoActorsOneMovie) {
    string from = "Actor 1";
    string to = "Actor 2";
    string through = "Awesome Movie\t2020";

    stringstream ss;
    ss << "Actor/Actress	Movie	Year" << endl;
    ss << from << "\t" << through << endl;
    ss << to << "\t" << through << endl;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    ASSERT_TRUE(isBuilt);
}

// TODO: add more tests for actor graph
TEST(ActorGraphTest1, TwoActorsOneMovie) {
    stringstream ss;
    ss << "Actor/Actress    Movie   Year" << endl;
    ss << "Kevin Bacon\tX-Men: First Class\t2011" << endl;
    ss << "James McAvoy\tX-Men: First Class\t2011" << endl;
    ss << "James McAvoy\tX-Men: Apocalypse\t2016" << endl;
    ss << "James McAvoy\tGlass\t2019" << endl;
    ss << "Michael Fassbender\tX-Men: First Class\t2011" << endl;
    ss << "Michael Fassbender\tX-men: Apocalypse\t2016" << endl;
    ss << "Michael Fassbender\tAlien: Covenant\t2017" << endl;
    ss << "Samuel L. Jackson\tGlass\t2019" << endl;
    ss << "Samuel L. Jackson\tAvengers: Endgame\t2019" << endl;
    ss << "Robert Downey Jr.\tAvengers: Endgame\t2019" << endl;
    ss << "Robert Downey Jr.\tSpider-Man: Homecoming\t2017" << endl;
    ss << "Tom Holland\tSpider-Man: Homecoming\t2017" << endl;
    ss << "Tom Holland\tThe Current War\t2017" << endl;
    ss << "Katherine Waterston\tAlien: Covenant\t2017" << endl;
    ss << "Katherine Waterston\tThe Current War\t2017" << endl;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    string shortestPath = "";
    graph.BFS("James McAvoy", "Kevin Bacon", shortestPath);
    string out = "(James McAvoy)--[X-Men: First Class#@2011]-->(Kevin Bacon)";
    ASSERT_EQ(out, shortestPath);
    ASSERT_TRUE(isBuilt);
}

TEST(ActorGraphTest2, TwoActorsOneMovie) {
    stringstream ss;
    ss << "Actor/Actress    Movie   Year" << endl;
    ss << "Kevin Zegers\tFrozen\t2010" << endl;
    ss << "Emma Bell\tFrozen\t2010" << endl;
    ss << "Emma Bell\tFinal Destination 5\t2011" << endl;
    ss << "Miles Fisher\tFinal Destination 5\t2011" << endl;
    ss << "Miles Fisher\tBelieve Me\t2014" << endl;
    ss << "Johanna Braddy\tBelieve Me\t2014" << endl;
    ss << "Kristen Bell\tFrozen\t2013" << endl;
    ss << "Idina Menzel\tFrozen\t2013" << endl;
    ss << "Idina Menzel\tUncut Gems\t2019" << endl;
    ss << "Adam Sandler\tUncut Gems\t2019" << endl;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    string shortestPath1 = "";
    graph.BFS("Kevin Zegers", "Johanna Braddy", shortestPath1);
    string out1 =
        "(Kevin Zegers)--[Frozen#@2010]-->(Emma Bell)--[Final Destination "
        "5#@2011]-->(Miles Fisher)--[Believe Me#@2014]-->(Johanna Braddy)";
    string shortestPath2 = "";
    graph.BFS("Kristen Bell", "Adam Sandler", shortestPath2);
    string out2 =
        "(Kristen Bell)--[Frozen#@2013]-->(Idina Menzel)--[Uncut "
        "Gems#@2019]-->(Adam Sandler)";
    ASSERT_TRUE(isBuilt);
    ASSERT_EQ(out1, shortestPath1);
    ASSERT_EQ(out2, shortestPath2);
}