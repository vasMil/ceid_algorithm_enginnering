#include "aliases.h"

template<typename NodeContent_t>
struct Node {
    Node* parent;
    NodeContent_t content;
    std::vector<Node*> children;
    
    Node() { }

    Node(NodeContent_t content) {
        this->parent = NULL;
        this->content = content;
    }
    
    Node(Node* parent, NodeContent_t content) {
        this->parent = parent;
        this->content = content;
    }
};

template<typename NodeContent_t>
struct DLTree {
        Node<NodeContent_t> root;

        DLTree() { }

        DLTree(NodeContent_t rootContent) {
            this->root.content = rootContent;
        }
        
        Node<NodeContent_t>* addChild(Node<NodeContent_t>& parent, NodeContent_t childContent) {
            Node<NodeContent_t> child = Node<NodeContent_t>(parent, childContent);
            parent->children.push_back(child);
            return &child;
        }
};
