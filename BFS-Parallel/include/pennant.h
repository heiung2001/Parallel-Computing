#ifndef PENNANT_H
#define PENNANT_H

#include "node.h"

class Pennant
{
private:
    Node* root;
public:
    Pennant(Node* root);
    ~Pennant();

    Node* get_root();

    Pennant* split();
    void merge(Pennant* &y);

    static void FA(Pennant* &x, Pennant* &y, Pennant* &carry);
};

#endif