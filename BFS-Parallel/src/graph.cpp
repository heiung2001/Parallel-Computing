#include "..\include\graph.h"
#include "..\include\bag.h"
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <omp.h>

#define MIN_SET 25

int f = 7;

Graph::Graph(int V)
{
    this->V = V;
    this->adj = new std::vector<int>[this->V];
}
Graph::~Graph() { }

void Graph::add_edge(int v, int w)
{
    this->adj[v].push_back(w);
}

void Graph::bfs(int s)
{
    std::vector<int> path(V, -1);
    std::vector<int>::iterator iter;
    std::queue<int> q;
    
    path[s] = s;
    q.push(s);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (iter = this->adj[u].begin(); iter != this->adj[u].end(); iter++)
        {
            if (path[*iter] == -1)
            {
                q.push(*iter);
                path[*iter] = u;
            }
        }
    }
    this->result(path, s, f);
}
void Graph::parallel_bfs(int s)
{
    std::vector<int> path(V, -1);
    std::vector<int> L, L2;
    
    path[s] = s;
    L.push_back(s);

    while (!L.empty())
    {
        #pragma omp parallel for
        for (int i = 0; i < L.size(); i++)
        {
            #pragma omp critical
            {
                for (auto iter = this->adj[L[i]].begin(); iter != this->adj[L[i]].end(); iter++)
                {
                    if (path[*iter] == -1)
                    {
                        path[*iter] = L[i];
                        L2.push_back(*iter);
                    }
                }
            }
        }
        L.clear();
        L = L2;
        L2.clear();
    }
    this->result(path, s, f);
}

void Graph::process_level_bag(Bag* &L, Bag* &L2, std::vector<int> &path)
{
    if (L->size() > MIN_SET)
	{
		Bag* y = L->split();
		Bag* L21 = new Bag();
#pragma omp parallel sections
		{
#pragma omp section
			{
				process_level_bag(y, L21, path);
			}
#pragma omp section
			{
				process_level_bag(L, L2, path);
			}
		}
		L2->merge(L21);
	}
	else
	{
		// XỬ LÝ TUẦN TỰ
		std::stack<Node *> nodes;
		for (int i = 0; i <= L->largest_nonempty_index; i++)
		{
			if (L->backbone[i] != NULL)
			{
				nodes.push(L->backbone[i]->get_root());
				while (nodes.size() > 0)
				{
					Node* current = nodes.top();
					nodes.pop();
					if (current->get_left() != NULL)
					{
						nodes.push(current->get_left());
					}
					if (current->get_right() != NULL)
					{
						nodes.push(current->get_right());
					}
					for (std::vector<int>::iterator it = this->adj[current->get_id()].begin(); it != this->adj[current->get_id()].end(); it++)
					{
						if (path[*it] == -1)
						{
							L2->insert(*it);
							path[*it] = current->get_id();
						}
					}
				}
			}
		}
	}
}
void Graph::bag_pbfs(int s)
{
    std::vector<int> path(V, -1);
    Bag* L = new Bag();

    L->insert(s);
    path[s] = s;

    while (!L->is_empty())
    {
        Bag* L2 = new Bag();
        this->process_level_bag(L, L2, path);
        L->clear();
        L = L2;
    }
    this->result(path, s, f);
}

void Graph::result(std::vector<int> path, int s, int f)
{
    if (path[f] == -1)
    {
        std::cout << "Not found path!" << std::endl;
        return;
    }

    while (f != s)
    {
        std::cout << f << "<- ";
        f = path[f];
    }
    std::cout << s << std::endl;
}
