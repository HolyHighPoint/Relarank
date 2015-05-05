#include "pagerank.h"
#include <QDebug>

void PageRank::run()
{
    double diff = 1;
    std::vector<double> old_pr;
    size_t num = node.size();
    if (num == 0)return;
    pr.assign(num, 0);pr[0] = 1;

    for(int num_iter = 0; diff > 1e-7 && num_iter < 10000; num_iter++) {

        double sum = 0;
        double noout = 0;

        for (size_t k = 0; k < pr.size(); k++) {
            sum += pr[k];
            if ((int)node[k].edge.size() == 0)noout += pr[k];
        }

        if (num_iter == 0)old_pr = pr;else {
            for (size_t i = 0; i < pr.size(); i++) {
                old_pr[i] = pr[i] / sum;
            }
        }

        sum = 1;
        double delta0 = alpha * noout / num;
        double delta1 = (1 - alpha) * sum / num;
        diff = 0;

        for(size_t i = 0; i < edge.size(); i++){
            pr[edge[i].y] += edge[i].w*old_pr[edge[i].x];
        }
        for (size_t i = 0; i < num; i++) {
            pr[i] = pr[i]*alpha + delta0 + delta1;
            diff += fabs(pr[i] - old_pr[i]);
        }
    }
    double sum = 0;
    for (size_t k = 0; k < pr.size(); k++)sum += pr[k];
    for (size_t i = 0; i < pr.size(); i++){
        pr[i] = pr[i] / sum;
        qDebug()<<i<<": "<<pr[i];
    }
    qDebug()<<sum/sum;
}

