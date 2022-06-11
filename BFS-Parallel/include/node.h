#ifndef NODE_H
#define NODE_H

class Node
{
private:
    int id;
    Node* left;
    Node* right;
public:
    Node(int id);
    ~Node();

    int get_id();

    Node* get_left();
    Node* get_right();

    void set_left(Node* left);
    void set_right(Node* right);
};

#endif