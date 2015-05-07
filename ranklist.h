#ifndef RANKLIST_H
#define RANKLIST_H

#include <QTreeWidget>
#include <QHeaderView>
#include "nodectrl.h"
#include "graph.h"
#include <vector>

class RankList : public QTreeWidget
{
    Q_OBJECT
public:
    RankList(QWidget *parent = 0)
        :QTreeWidget(parent)
    {
        QStringList headers;
        headers << tr("Rank") << tr("Name") << tr("Value");
        this->setColumnCount(2);
        this->setHeaderLabels(headers);
        this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
    void changelist(std::vector<graph_node> &_node, std::vector<double> &_w, std::vector<NodeCtrl*> &_np);
public slots:
    void NodeChange(QTreeWidgetItem * current, QTreeWidgetItem * previous);
private:
    struct TmpNode
    {
        double w;
        QString name;
        NodeCtrl * np;
        bool operator < (const TmpNode &a){
            return w > a.w;
        }
    };
    std::vector<TmpNode> tmp;
};

#endif // RANKLIST_H
