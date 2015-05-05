#include "planarity.h"
#include <QDebug>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/ref.hpp>

using namespace std;
using namespace boost;


static void planarity(std::vector<graph_node> &node,
                      std::vector<graph_edge> &edge, std::vector < std::pair<int, int> > &location)
{
    struct coord_t
    {
      std::size_t x;
      std::size_t y;
    };

    typedef adjacency_list< vecS, vecS, undirectedS, property<vertex_index_t, int>, property<edge_index_t, int> > graph;
    typedef std::vector< std::vector< graph_traits<graph>::edge_descriptor > > embedding_storage_t;
    typedef boost::iterator_property_map< embedding_storage_t::iterator, property_map<graph, vertex_index_t>::type > embedding_t;

    graph g((int)node.size()), gt;
    g.clear();

    // Create the planar embedding
    embedding_storage_t embedding_storage(num_vertices(g));
    for(size_t i=0;i<edge.size();i++){
        gt=g;
        add_edge(edge[i].x, edge[i].y, g);
        embedding_storage = embedding_storage_t(num_vertices(g));
        if(!boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = &embedding_storage[0])){
            g=gt;
            if(edge[i].wt < 0.3)edge[i].flag = false;
        }
    }

    embedding_storage = embedding_storage_t(num_vertices(g));
    graph_traits<graph>::edge_iterator ei, ei_end;
    graph_traits<graph>::edges_size_type edge_count = 0;
    property_map<graph, edge_index_t>::type e_index = get(edge_index, g);
    for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)put(e_index, *ei, edge_count++);
    qDebug() << "has " << num_edges(g) << " edges.";
    qDebug()<<boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = &embedding_storage[0]);
    make_biconnected_planar(g, &embedding_storage[0]);
    edge_count = 0;
    for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)put(e_index, *ei, edge_count++);
    qDebug() << "has " << num_edges(g) << " edges.";
    qDebug()<<boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = &embedding_storage[0]);
    make_maximal_planar(g, &embedding_storage[0]);
    edge_count = 0;
    for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)put(e_index, *ei, edge_count++);
    qDebug() << "has " << num_edges(g) << " edges.";
    qDebug()<<boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = &embedding_storage[0]);

    embedding_t embedding(embedding_storage.begin(), get(vertex_index,g));

    // Find a canonical ordering
    std::vector<graph_traits<graph>::vertex_descriptor> ordering;ordering.clear();
    planar_canonical_ordering(g, embedding, std::back_inserter(ordering));

    //Set up a property map to hold the mapping from vertices to coord_t's
    typedef std::vector< coord_t > straight_line_drawing_storage_t;
    typedef boost::iterator_property_map< straight_line_drawing_storage_t::iterator, property_map<graph, vertex_index_t>::type >straight_line_drawing_t;
    straight_line_drawing_storage_t straight_line_drawing_storage(num_vertices(g));
    straight_line_drawing_t straight_line_drawing(straight_line_drawing_storage.begin(), get(vertex_index,g));

    // Compute the straight line drawing
    qDebug()<<boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = embedding);
    chrobak_payne_straight_line_drawing( g, embedding, ordering.begin(), ordering.end(), straight_line_drawing );

    graph_traits<graph>::vertex_iterator vi, vi_end;
    location.clear();
    pair<int, int> mi, ma, mid;
    for(tie(vi,vi_end) = vertices(g); vi != vi_end; vi++){
        coord_t coord(get(straight_line_drawing,*vi));
        location.push_back(make_pair(coord.x*60, coord.y*60));
        mi.first = min(location.back().first, mi.first), mi.second = min(location.back().second, mi.second);
        ma.first = max(location.back().first, ma.first), ma.second = max(location.back().second, ma.second);
    }
    mid = make_pair((mi.first+ma.first)>>1, (mi.second+ma.second)>>1);
    for(size_t i=0; i<node.size(); i++){
        location[i].first-=mid.first;
        location[i].second-=mid.second;
        qDebug() << i << " -> (" << location[i].first << ", " << location[i].second << ")";
    }

    if (is_straight_line_drawing(g, straight_line_drawing))
        qDebug() << "Is a plane drawing.";
      else
        qDebug() << "Is not a plane drawing.";

    return;
}

void Planarity::run(){
    planarity(node,edge,location);
}
