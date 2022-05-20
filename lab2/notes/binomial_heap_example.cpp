// In Coliru: https://coliru.stacked-crooked.com/a/b7ea797e74d4b0ad
#include <boost/heap/binomial_heap.hpp>
#include <iostream>

struct node
{
    int id;

    node(int i) {
        this->id = i;    
    }
};

struct compare_node
{
    bool operator()(const node* n1, const node* n2) const
    {
        return n1->id > n2->id;
    }
};

typedef boost::heap::binomial_heap<node*, boost::heap::compare<compare_node>> NodeBH;
typedef NodeBH::handle_type NBH_handle_t;

int main()
{
    NodeBH heap;
    std::vector<NBH_handle_t> handles(4);
    node* n2 = new node(2);
    handles[0] = heap.push(new node(3));
    handles[1] = heap.push(n2);
    handles[2] = heap.push(new node(1));
    handles[3] = heap.push(new node(4));
    
    // Decrease the value of a node id, so the priority of that node increases
    n2->id = 0;
    heap.increase(handles[1]);
    
    while(!heap.empty()) {
        std::cout << heap.top()->id << "\n";
        heap.pop();
    }
}