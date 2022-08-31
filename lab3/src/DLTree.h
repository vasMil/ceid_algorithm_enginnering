#pragma once
#include "aliases.h"

template<typename NodeContent_t>
struct Node {
    Node* parent;
    NodeContent_t content;
    std::vector<Node<NodeContent_t>* > children;
    
    Node() {
        this->content = -1;
        this->parent = NULL;
        children = std::vector<Node<NodeContent_t>* >();
    }

    Node(NodeContent_t content) {
        this->parent = NULL;
        this->content = content;
        children = std::vector<Node<NodeContent_t>* >();
    }
    
    Node(Node* parent, NodeContent_t content) {
        this->parent = parent;
        this->content = content;
        children = std::vector<Node<NodeContent_t>* >();
    }
};


template<typename NodeContent_t>
class DLTree {
    // https://stackoverflow.com/questions/610245/where-and-why-do-i-have-to-put-the-template-and-typename-keywords
    typedef typename std::list<std::pair<Node<NodeContent_t>*, Node<NodeContent_t>* > > DLEdge;

    private:
        Node<NodeContent_t>* root;

        DLEdge _edges;
        bool is_empty;

        void getDescendantNodes(Node<NodeContent_t>* child, std::queue<Node<NodeContent_t>* >& Q) {
            auto num_of_children = child->children.size();
            if(num_of_children == 0) {
                Q.push(child);
                return;
            }
            for (auto i = 0; i < num_of_children; i++) {
                getDescendantNodes(child->children[i], Q);
            }
            Q.push(child);
            return;
        }

    public:

        DLTree() {
            this->root = NULL;
            this->is_empty = true;
        }

        DLTree(NodeContent_t rootContent) {
            this->root = new Node<NodeContent_t>(rootContent);
            this->is_empty = false;
        }

        ~DLTree() {
            pruneChild(this->root);
            this->is_empty = true;
        }

        Node<NodeContent_t>* setRoot(NodeContent_t rootContent) {
            this->root = new Node<NodeContent_t>(rootContent);
            this->is_empty = false;
            return root;
        }

        Node<NodeContent_t>* addChild(Node<NodeContent_t>* parent, NodeContent_t childContent) {
            if (parent == NULL) {
                throw std::invalid_argument("Parent cannot be NULL!");
            }
            Node<NodeContent_t>* child = new Node<NodeContent_t>(parent, childContent);
            _edges.push_back(std::make_pair(parent, child));
            parent->children.push_back(child);
            return child;
        }

        void removeChild(Node<NodeContent_t>*& child) {
            if(child == root) {
                this->is_empty = true;
                this->root = NULL;
            }
            if (child == NULL) return;

            auto parent = child->parent;
            if (parent != NULL) {
                // Erase the child from parents children
                auto cit = parent->children.begin(); auto cend = parent->children.end();
                for ( ; cit != cend; cit++) {
                    if (*cit == child) {
                        cit = parent->children.erase(cit);
                        break;
                    }
                }
            }

            // Delete all the edges that contain that Node
            auto eit = _edges.begin(); auto eend = _edges.end();
            while(eit != eend) {
                if((*eit).first == child) {
                    // If child is the source then the targets parent
                    // should be updated to NULL
                    (*eit).second->parent = NULL;
                    (*eit).first = NULL;
                }
                if ((*eit).second == child) {
                    eit = _edges.erase(eit);
                }
                else {
                    eit++;
                }
            }
            // Delete the Node
            delete child;
            // Assign NULL to the input pointer
            child = NULL;
        }

        void pruneChild(Node<NodeContent_t>*& child) {
            if(child == root) {
                this->is_empty = true;
                this->root = NULL;
            }
            if (child == NULL) return;

            std::queue<Node<NodeContent_t>* > childrenQ;
            Node<NodeContent_t>* temp;
            auto parent = child->parent;

            // Erase the child from parents children
            if(parent != NULL) {
                auto cit = parent->children.begin(); auto cend = parent->children.end();
                for ( ; cit != cend; cit++) {
                    if (*cit == child) {
                        cit = parent->children.erase(cit);
                        break;
                    }
                }
            }
            getDescendantNodes(child, childrenQ);

            // Prune the children
            while(!childrenQ.empty()) {
                temp = childrenQ.front();
                childrenQ.pop();
                // Fix the _edges list
                auto eit = _edges.begin(); auto eend = _edges.end();
                while(eit != eend) {
                    // Set NULL the pointer that points to the node to be deleted
                    // and set the parent of the edges target to NULL;
                    if ((*eit).first == temp) {
                        (*eit).first = NULL;
                        (*eit).second->parent = NULL;
                    }

                    // Delete the edge where temp is the target
                    if ((*eit).second == temp) {
                        eit = _edges.erase(eit);
                    }
                    else {
                        eit++;
                    }
                }
                delete temp;
            }
            child = NULL;
            return;
        }

        std::pair<typename DLEdge::iterator, typename DLEdge::iterator > edges() {
            return std::make_pair(_edges.begin(), _edges.end());
        }

        std::pair<typename DLEdge::reverse_iterator, typename DLEdge::reverse_iterator > redges() {
            return std::make_pair(_edges.rbegin(), _edges.rend());
        }

        bool empty() {
            if (is_empty) {
                return true;
            }
            return false;
        }
};

