#include "..\include\bag.h"
#include <iostream>
#include <math.h>
#include <algorithm>

#define NORMAL 20

Bag::Bag()
{
    this->init(NORMAL);
}
Bag::Bag(int r)
{
    this->init(r);
}
Bag::~Bag()
{
    this->clear();
    delete[] this->backbone;
}

void Bag::init(int r)
{
    this->backbone = new Pennant*[r]();
    this->r = r;
    this->largest_nonempty_index = -1;
}
void Bag::clear()
{
    for (int i = 0; i <= this->largest_nonempty_index; i++)
    {
        delete this->backbone[i];
        this->backbone[i] = NULL;
    }
    this->largest_nonempty_index = -1;
}

bool Bag::is_empty()
{
    return (this->largest_nonempty_index < 0);
}
int Bag::size()
{
    int num = 0;
    for (int i = 0; i <= this->largest_nonempty_index; i++)
    {
        if (this->backbone[i] != NULL)
        {
            num += pow(2, i);
        }
    }
    return num;
}
void Bag::print()
{
    std::cout << "Bag: { ";
    for (int i = 0; i < this->r; i++)
    {
        std::cout << (this->backbone[i] != NULL)*pow(2, i) << " ";
    }
    std::cout << "}" << std::endl;
}

void Bag::insert(int v)
{
    Node* vertex = new Node(v);
    Pennant* tree = new Pennant(vertex);
    this->insert(tree);
}
void Bag::insert(Pennant* &tree)
{
    int i = 0;
    
    while (this->backbone[i] != NULL)
    {
        this->backbone[i]->merge(tree);
        tree = this->backbone[i];
        this->backbone[i] = NULL;
        i++;
    }

    this->backbone[i] = tree;
    if (i > this->largest_nonempty_index)
    {
        this->largest_nonempty_index = i;
    }
}

Bag* Bag::split()
{
    auto res = new Bag(this->r);
    auto head = this->backbone[0];
    this->backbone[0] = NULL;

    for (auto i = 1; i <= this->largest_nonempty_index; i++)
    {
        if (this->backbone[i] != NULL)
        {
            res->backbone[i-1] = this->backbone[i]->split();
            this->backbone[i-1] = this->backbone[i];
            this->backbone[i] = NULL;
        }
    }

    if (this->largest_nonempty_index != 0)
    {
        int new_last_idx = this->largest_nonempty_index - 1;
        this->largest_nonempty_index = new_last_idx;
        res->largest_nonempty_index = new_last_idx;
    }

    if (head != NULL)
    {
        this->insert(head);
    }

    return res;
}
void Bag::merge(Bag* y)
{
    int max_nonempty_index = std::max(y->largest_nonempty_index, this->largest_nonempty_index);
    Pennant* carry = NULL;

    for (auto i = 0; i <= max_nonempty_index; i++)
    {
        Pennant::FA(this->backbone[i], y->backbone[i], carry);
    }

    if (carry != NULL)
    {
        max_nonempty_index += 1;
        this->backbone[max_nonempty_index] = carry;
    }

    this->largest_nonempty_index = max_nonempty_index;
}