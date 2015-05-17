#ifndef PAGERANK_H
#define PAGERANK_H

#include "graph.h"
#include <QThread>

class PageRank : public QThread
{
    Q_OBJECT
public:
    PageRank(std::vector<graph_node> &_node,
                       std::vector<graph_edge> &_edge, std::vector<double> &_pr,
                       QObject *parent = 0)
        : QThread(parent), node(_node), edge(_edge), pr(_pr)
    {
        d.assign(_node.size()+1, 0);
        dis.assign(_node.size(), 0);
        maxdis.assign(_node.size(), 0);
        f.assign(_node.size(), false);
    }
protected:
    void run();
private:
    void spfa(int x);
    const double alpha = 0.85;
    std::vector<graph_node> &node;
    std::vector<graph_edge> &edge;
    std::vector<double> &pr;
    std::vector<int> d, dis, maxdis;
    std::vector<bool> f;
};

#endif // PAGERANK_H
