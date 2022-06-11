#ifndef GRAPH_H
#define GRAPH_H

#include "bag.h"
#include <vector>

class Graph
{
private:
    std::vector<int>* adj;
    int V;
    
public:
    Graph(int V);
    ~Graph();

    void add_edge(int v, int w);

    void bfs(int s);
    void parallel_bfs(int s);

    void process_level_bag(Bag* &frontier, Bag* &new_frontier, std::vector<int> &path);
    void bag_pbfs(int s);

    void result(std::vector<int> path, int s, int f);
};

#endif