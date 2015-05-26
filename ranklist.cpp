#include "ranklist.h"
#include <algorithm>
#include <cstdio>
using namespace std;
void RankList::changelist(vector<graph_node> &_node, vector<double> &_w, vector<NodeCtrl*> &_np)
{
    tmp.resize(_node.size());
    for(size_t i=0;i<_node.size();i++){
        tmp[i].name=_node[i].name;
        tmp[i].w=_w[i];
        tmp[i].np = _np[i];
    }
    sort(tmp.begin(), tmp.end());
    for(size_t i=0;i<tmp.size();i++){
        QStringList t;
        QVariant rank(int(i)+1), value(tmp[i].w);
        t<<rank.toString()<<tmp[i].name<<value.toString();
        QTreeWidgetItem *ln = new QTreeWidgetItem(this, t);
        ln->setData(0x0100, 0x0100,int(i));
        this->addTopLevelItem(ln);
    }
    this->header()->resizeSections(QHeaderView::ResizeToContents);
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(NodeChange(QTreeWidgetItem*, QTreeWidgetItem*)));
}
void RankList::NodeChange(QTreeWidgetItem * current, QTreeWidgetItem * previous){
    for(size_t i=0;i<tmp.size();i++){
        if(tmp[i].np)tmp[i].np->setSelected(false);
    }
    if(current&&tmp[current->data(0x0100, 0x0100).toInt()].np)tmp[current->data(0x0100, 0x0100).toInt()].np->setSelected(true);
}


