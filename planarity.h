#ifndef PLANARITY_H
#define PLANARITY_H

#include "graph.h"
#include <QThread>

class Planarity : public QThread
{
    Q_OBJECT
public:
    Planarity(std::vector<graph_node> &_node,
                       std::vector<graph_edge> &_edge, std::vector < std::pair<int, int> > &_location,
                       QObject *parent = 0)
        : QThread(parent), node(_node), edge(_edge), location(_location)
    {

    }
protected:
    void run();
private:
    std::vector<graph_node> &node;
    std::vector<graph_edge> &edge;
    std::vector < std::pair<int, int> > &location;
};


#endif // PLANARITY_H
