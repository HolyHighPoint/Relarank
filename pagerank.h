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

    }
protected:
    void run();
private:
    const double alpha = 0.85;
    std::vector<graph_node> &node;
    std::vector<graph_edge> &edge;
    std::vector<double> &pr;
};

/*class RenderThread : public QThread
{
    Q_OBJECT

signals:
    void notify(int);

public:
    RenderThread(std::vector<graph_node> &_node,
                 std::vector<graph_edge> &_edge, std::vector<NodeCtrl*> &_nodectrl,
                 MainCtrl* &_mainCtrl, QObject *parent = 0)
        : node(_node), edge(_edge), nodectrl(_nodectrl), mainCtrl(_mainCtrl), QThread(parent)
    {
    }

protected:
    void run();
private:
    std::vector<graph_node> &node;
    std::vector<graph_edge> &edge;
    std::vector<NodeCtrl*> &nodectrl;
    MainCtrl* &mainCtrl;
};*/

#endif // PAGERANK_H
