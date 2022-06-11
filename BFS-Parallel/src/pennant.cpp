#include "../include/pennant.h"
#include "../include/node.h"
#include <iostream>
#include <stack>

Pennant::Pennant(Node* root)
{
    this->root = root;
}
Pennant::~Pennant()
{
    if (this->root != NULL)
    {
        std::stack<Node*> nodes;
        nodes.push(this->root);
        while (nodes.size() > 0)
        {
            Node* curr = nodes.top();
            nodes.pop();
            if (curr->get_left() != NULL)
            {
                nodes.push(curr->get_left());
            }
            if (curr->get_right() != NULL)
            {
                nodes.push(curr->get_right());
            }
            delete curr;
        }
    }
}

Node* Pennant::get_root()
{
    return this->root;
}

Pennant* Pennant::split()
{
    if (this->root->get_left() == NULL)
    {
        return NULL;
    }
    Pennant* y = new Pennant(this->root->get_left());
    this->root->set_left(y->root->get_right());
    y->root->set_right(NULL);
    return y;
}

void Pennant::merge(Pennant* &y)
{
    if (y != NULL)
    {
        y->root->set_right(this->root->get_left());
        this->root->set_left(y->root);
        y->root = NULL;
        delete y;
        y = NULL;
    }
}

void Pennant::FA(Pennant* &x, Pennant* &y, Pennant* &carry)
{
    if (x == NULL && y == NULL && carry == NULL) {
        return;
    }
    if (x != NULL && y == NULL && carry == NULL) {
        return;
    }
    if (x == NULL && y != NULL && carry == NULL) {
        x = y;
        y = NULL;
        return;
    }
    if (x == NULL && y == NULL && carry != NULL) {
        x = carry;
        carry = NULL;
        return;
    }
    if (x != NULL && y != NULL && carry == NULL) {
        x->merge(y);
        carry = x;
        x = NULL;
        return;
    }
    if (x != NULL && y == NULL && carry != NULL) {
        x->merge(carry);
        carry = x;
        x = NULL;
        return;
    }
    if (x == NULL && y != NULL && carry != NULL) {
        y->merge(carry);
        carry = y;
        y = NULL;
        return;
    }
    if (x != NULL && y != NULL && carry != NULL) {
        carry->merge(y);
        return;
    }
}