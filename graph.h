#ifndef GRAPH
#define GRAPH
#include <vector>
#include <QString>
#include <algorithm>
class graph_edge{
public:
    int x, y;
    double w;
    graph_edge(int _x=0, int _y=0, double _w=0.0)
        :x(_x), y(_y), w(_w){

    }
    bool operator<(const graph_edge &b){
        if(x < b.x)return true;if(x > b.x)return false;
        if(y < b.y)return true;if(y > b.y)return false;
        if(w < b.w)return true;if(w > b.w)return false;
        return false;
    }
};
class graph_node{
public:
    QString name;
    double x, y;
    std::vector<graph_edge> edge;
    graph_node(QString _name):name(_name){
        edge.clear();
        x=0.0, y=0.0;
    }
    std::vector<graph_edge>::iterator insert(graph_edge x){
        edge.push_back(x);
        return --edge.end();
    }
    std::vector<graph_edge>::iterator insert(int x, int y, double w){
        edge.push_back(graph_edge(x, y, w));
        return --edge.end();
    }
    void sort(){
        std::sort(edge.begin(), edge.end());
    }
    int find(const graph_edge &x){
        return std::lower_bound(edge.begin(), edge.end(), x) - edge.begin();
    }
};

#endif // GRAPH
