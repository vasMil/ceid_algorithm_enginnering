template <typename CostPropertyMap>
void randomGraph(Graph& G, CostPropertyMap& cost, int n_nodes, int m_edges, int min_cost, int max_cost) {
    // Initialize a random number generator
    boost::mt19937 rng;
    // add a seed to it
    rng.seed(uint32_t(time(0)));
    // Define the distribustion and the range of numbers.
    boost::uniform_int<> dist(min_cost, max_cost);

    // Use the generator to create a random graph
    boost::generate_random_graph(G, n_nodes, m_edges, rng, false, false);

    // Add random costs on the edges
    EdgeIter first, last;
    for (boost::tie(first, last) = edges(G); first != last; ++first) {
        cost[*first] = dist(rng);
    }
}