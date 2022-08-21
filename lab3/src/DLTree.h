#include "aliases.h"

template<typename NodeContent_t>
struct Node {
    Node* parent;
    NodeContent_t content;
    std::vector<Node<NodeContent_t>* > children;
    
    Node() { }

    Node(NodeContent_t content) {
        this->parent = NULL;
        this->content = content;
    }
    
    Node(Node* parent, NodeContent_t content) {
        this->parent = parent;
        this->content = content;
    }

    ~Node() {
        for(int i = 0; i < children.size(); i++) {
            delete children[i];
        }
    }
};

template<typename NodeContent_t>
class DLTree {
    // https://stackoverflow.com/questions/610245/where-and-why-do-i-have-to-put-the-template-and-typename-keywords
    typedef typename std::vector<std::pair<Node<NodeContent_t>*, Node<NodeContent_t>*> >::iterator EdgeIter;

    private:
        std::vector<std::pair<Node<NodeContent_t>*, Node<NodeContent_t>* > >  _edges;
        bool is_empty;

    public:
        Node<NodeContent_t> root;

        DLTree() {
            this->is_empty = true;
        }

        DLTree(NodeContent_t rootContent) {
            this->root.content = rootContent;
            this->is_empty = false;
        }
        
        Node<NodeContent_t>* addChild(Node<NodeContent_t>* parent, NodeContent_t childContent) {
            if (parent == NULL) {
                this->root.content = childContent;
                this->is_empty = false;
                return &root;
            }
            Node<NodeContent_t>* child = new Node<NodeContent_t>(parent, childContent);
            parent->children.push_back(child);
            _edges.push_back(std::make_pair(parent, child));
            return child;
        }

        std::pair<EdgeIter, EdgeIter> edges() {
            return std::make_pair(_edges.begin(), _edges.end());
        }

        bool empty() {
            if (is_empty) {
                return true;
            }
            return false;
        }
};
