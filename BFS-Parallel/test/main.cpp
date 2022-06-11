#include "../include/node.h"
#include "../include/pennant.h"
#include "../include/bag.h"
#include "../include/graph.h"
#include <iostream>
#include <time.h>
#include <omp.h>

using namespace std;

void read_graph(Graph &g);

void print_pennant(Node* root);

int main()
{
    freopen("graph_1000000_nodes.txt", "r", stdin);
    Graph g(1000000);
    read_graph(g);

    omp_set_num_threads(4);

    double start = omp_get_wtime();
    g.bag_pbfs(43437);
    double end = omp_get_wtime();
    std::cout << "parallel: " << (end - start) << std::endl;

    start = omp_get_wtime();
    g.bfs(43437);
    end = omp_get_wtime();
    std::cout << "sequencial: " << (end - start) << std::endl;

    return 0;
}

void read_graph(Graph &g)
{
    int a, b;
    while (cin >> a >> b)
    {
        if (a==b) continue;
        g.add_edge(a, b);
        g.add_edge(b, a);
    }
}

void print_pennant(Node* root)
{
    if (root == NULL)
    {
        return;
    }
    cout << root->get_id() << " ";
    print_pennant(root->get_left());
    print_pennant(root->get_right());
}