#ifndef BAG_H
#define BAG_H

#include "pennant.h"

class Bag
{
private:
    // Pennant** backbone;
    // int largest_nonempty_index;
    int r;
public:
    Pennant** backbone;
    int largest_nonempty_index;
    
    Bag(int r);
    Bag();
    ~Bag();

    void init(int r);
    void clear();

    bool is_empty();
    int size();
    void print();

    void insert(int v);
    void insert(Pennant* &vertices);

    Bag* split();
    void merge(Bag* y);
};

#endif