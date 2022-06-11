#include "../include/node.h"
#include <iostream>

Node::Node(int id)
{
    this->id = id;
    this->left   = NULL;
    this->right  = NULL;
}

Node::~Node()
{

}

int Node::get_id()
{
    return this->id;
}

Node* Node::get_left()
{
    return this->left;
}
Node* Node::get_right()
{
    return this->right;
}

void Node::set_left(Node* left)
{
    this->left = left;
}
void Node::set_right(Node* right)
{
    this->right = right;
}