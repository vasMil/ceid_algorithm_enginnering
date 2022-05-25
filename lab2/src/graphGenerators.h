#pragma once

std::pair<Vertex, Vertex> randomGraph(Graph& G, int n_nodes, int m_edges, int min_cost, int max_cost) {
    // Initialize a random number generator
    boost::mt19937 rng;
    // add a seed to it
    rng.seed(uint32_t(time(0)));
    // map the random numbers to a distribution
    boost::uniform_int<> dist(min_cost, max_cost);
    // Use the generator to create a random graph
    boost::generate_random_graph(G, n_nodes, m_edges, rng, false, false);

    // Add random costs on the edges
    CostPMap cost = get(&EdgeInfo::cost, G);
    EdgeIter first, last;
    Edge e;
    for (boost::tie(first, last) = boost::edges(G); first != last; ++first) {
        cost[*first] = dist(rng);
    }
    return std::make_pair(boost::random_vertex(G, rng), boost::random_vertex(G, rng));
}

std::pair<Vertex, Vertex> gridGraph(Graph& G, int rows, int cols, int min_cost, int max_cost) {
    Vertex s, t;
    Edge e;
    bool succ_e;
    int rand_cost;
    CostPMap cost = boost::get(&EdgeInfo::cost, G);

    // Create a random number generator for the edge costs
        // Initialize a random number generator
        boost::mt19937 rng;
        // add a seed to it
        rng.seed(uint32_t(time(0)));
        // Define the distribustion and the range of numbers.
        boost::uniform_int<> dist(min_cost, max_cost);
        boost::uniform_int<> dist_row(0, rows-1);
    // Select a random node from the first column (column index 0)
    int s_row = dist_row(rng);
    // Select a random node from the last column (column index cols-1)
    int t_row = dist_row(rng);
    // Create the grid
    std::vector<Vertex> prev_row(cols);
    std::vector<Vertex> current_row(cols);
    for(int i=0; i < rows; i++) {
        for(int j=0; j < cols; j++) {
            current_row[j] = boost::add_vertex(G);
            // Extract the starting Vertex s
            if(i==s_row && j==0) s = current_row[j];
            // Extract the target Vertex t
            if(i==t_row && j==cols-1) t = current_row[j];
            // Forall the vertices of the current row, connect them with the vertex above
            // except for those in row 0 (which they do not have a row above)
            if(i != 0) {
                rand_cost = dist(rng);
                boost::tie(e, succ_e) = boost::add_edge(prev_row[j], current_row[j], G);
                cost[e] = rand_cost;
                boost::tie(e, succ_e) = boost::add_edge(current_row[j], prev_row[j], G);
                cost[e] = rand_cost;
            }
            // Forall new vertices (of the current row) create an edge connecting them to the vertex
            // created immediately before, except for those in column 0 (which do not have a vertex immediately before)
            if(j != 0) {
                rand_cost = dist(rng);
                boost::tie(e, succ_e) = boost::add_edge(current_row[j], current_row[j-1], G);
                cost[e] = rand_cost;
                boost::tie(e, succ_e) = boost::add_edge(current_row[j-1], current_row[j], G);
                cost[e] = rand_cost;
            }
        }
        prev_row = current_row;
    }
    return std::make_pair(s, t);
}