#pragma once
#include "utils.h"

#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/graph/templates/shortest_path.h>

void boostGraphToLeda(
    const Graph& BG, leda::graph& LG, 
    const Vertex& boost_s, leda::node& leda_s,
    const Vertex& boost_t, leda::node& leda_t,
    const CostPMap& boost_cost, leda::edge_array<int>& leda_cost
    ) {
        std::vector<leda::node> leda_nodes(boost::num_vertices(BG));
        std::vector<leda::edge> leda_edges(boost::num_edges(BG));

        int i = 0;
        VertexIter vit, vit_end;
        for(boost::tie(vit, vit_end) = boost::vertices(BG); vit != vit_end; vit++) {
            leda_nodes[i] = LG.new_node();
            if(*vit == boost_s) leda_s = leda_nodes[i];
            if(*vit == boost_t) leda_t = leda_nodes[i];
            i++;
        }

        EdgeIter eit, eit_end;
        Vertex bv, bu;
        leda::edge le;
        i = 0;
        for(boost::tie(eit, eit_end) = boost::edges(BG); eit != eit_end; eit++) {
            bu = boost::source(*eit, BG);
            bv = boost::target(*eit, BG);
            // Since bu and bv are vertex descriptors on a vecS, they are indexes.
            // Thus I may use them on to index the parallel vector containing LEDAs nodes.
            leda_edges[i] = LG.new_edge(leda_nodes[bu], leda_nodes[bv]);
            i++;
        }

        leda_cost.init(LG);
        i = 0;
        for(boost::tie(eit, eit_end) = boost::edges(BG); eit != eit_end; eit++) {
            leda_cost[leda_edges[i]] = boost_cost[*eit];
            i++;
        }
        return;
}


/* TIME LEDA */
struct DurationsLeda {
    std::chrono::duration<double, std::milli> time_leda;
    std::string typeOfGraph;
    int numOfNodes;
    int numOfEdges;

    DurationsLeda(std::string typeOfGraph) {
        this->typeOfGraph = typeOfGraph;
    }

    void save_into_csv(std::fstream& outfile) {
        // Create the output file if it does not exist and format it accordingly
        createFileIfNotExists(outfile);
        // Write the measurment - add a newline at the end
        outfile << typeOfGraph << "," << numOfNodes << "," << numOfEdges << "," << time_leda.count() << std::endl;
    }

    private:
    void createFileIfNotExists(std::fstream& outfile) {
        outfile.seekg(0);
        // If file is not empty
        if(outfile.peek() != EOF) {
            // Get back at the end of the file
            outfile.seekg(std::ios::end);
            return;
        }
        // Clear the flags (I only care about eof flag)
        outfile.clear();
        // Get at the beggining of the file
        outfile.seekg(0);
        // Else create and setup the first line as (type, n, leda, myImpl) - add a newline at the end
        outfile << "type_of_graph,number_of_nodes,number_of_edges,time_LEDA" << std::endl;
    }
};

void timeLeda(std::fstream& ledaCsv, const Graph& BG, const Vertex& bs, const Vertex& bt, const CostPMap& boost_cost, std::string typeOfGraph) {
    DurationsLeda dur_leda(typeOfGraph);
    dur_leda.numOfNodes = boost::num_vertices(BG);
    dur_leda.numOfEdges = boost::num_edges(BG);
    leda::graph LG;
    leda::edge_array<int> leda_cost(LG);
    leda::node ls, lt;
    boostGraphToLeda(BG, LG, bs, ls, bt, lt, boost_cost, leda_cost);
    leda::node_array<leda::edge> leda_pred(LG);
    auto t0 = std::chrono::high_resolution_clock::now();
    leda::DIJKSTRA_T(LG, ls, lt, leda_cost, leda_pred);
    auto t1 = std::chrono::high_resolution_clock::now();
    dur_leda.time_leda = t1 - t0;
    dur_leda.save_into_csv(ledaCsv);
}


/* LEDA UTILS */
void printLedaGraph(const leda::graph& G, const leda::edge_array<int> cost, std::string site = "graphviz") {
    std::cout << "Printing Graph..." << std::endl;
    std::string sep, label_start, label_end;
    if(site == "graphviz") {
        sep = " -> ";
        label_start = " label=[";
        label_end = "]";
    }
    else {
        sep = " ";
        label_start = " ";
        label_end = "";
    }
    leda::edge e;
    leda::node s, t;
    if(site == "csacademy") {
        forall_nodes(s, G) {
            std::cout << s->id() << std::endl;
        }
    } 
    forall_edges(e, G) {
        s = G.source(e);
        t = G.target(e);
        std::cout << s->id() << sep << t->id() << label_start << cost[e] << label_end << std::endl;
    }
}


/* LEDA TEST */
void testBoostToLeda() {
    Graph BG;
    CostPMap boost_cost = boost::get(&EdgeInfo::cost, BG);
    PredPMap boost_pred = boost::get(&NodeInfo::pred, BG);
    DistPMap boost_dist = boost::get(&NodeInfo::dist, BG);
    Vertex bs, bt;
    boost::tie(bs, bt) = gridGraph(BG, 4, 8, 1, 100);
    std::cout << "bs = " << bs << ", bt = " << bt << std::endl;
    leda::graph LG;
    leda::edge_array<int> leda_cost(LG);
    leda::node ls, lt;
    boostGraphToLeda(BG, LG, bs, ls, bt, lt, boost_cost, leda_cost);
    leda::node_array<leda::edge> leda_pred(LG);
    leda::DIJKSTRA_T(LG, ls, lt, leda_cost, leda_pred);
    std::cout << "ls = ";
    LG.print_node(ls);
    std::cout<< ", lt = ";
    LG.print_node(lt);
    std::cout << std::endl;
    leda::node temp = lt;
    LG.print_node(temp);
    while(temp != ls) {
        LG.print_node(LG.source(leda_pred[temp]));
        temp = LG.source(leda_pred[temp]);
    }
    std::cout << std::endl;
    int boost_cnt = 0;
    Dijkstra_SP(BG, bs, bt, boost_cost, boost_pred, boost_dist, boost_cnt, GraphOper::getInstance());
    Vertex boost_temp = bt;
    while(boost_temp != bs) {
        std::cout << boost_temp << ", ";
        boost_temp = boost::source(boost_pred[boost_temp], BG);
    }
    std::cout << bs << std::endl;

    // boost::write_graphviz(std::cout, BG, boost::default_writer(), boost::make_label_writer(boost_cost));
    // printLedaGraph(LG, leda_cost);
}