#include "pagerank.h"
#include <QDebug>
#include <algorithm>
#include <cmath>
using namespace std;
void PageRank::spfa(int x){
    int h=0,t=1;d[1] = x, f[x] = true;
    for(size_t i=0;i<dis.size();i++)dis[i] = 1000000000;dis[x]=0;
    while(h != t){
        h = h%dis.size()+1;
        for(size_t k=0;k<node[d[h]].edge.size();k++){
            if((dis[node[d[h]].edge[k].y] > dis[node[d[h]].edge[k].x]+node[d[h]].edge[k].w)){
                dis[node[d[h]].edge[k].y] = dis[node[d[h]].edge[k].x]+node[d[h]].edge[k].w;
                if(!f[node[d[h]].edge[k].y]){
                    d[t = t%dis.size()+1] = node[d[h]].edge[k].y;
                    f[node[d[h]].edge[k].y] = true;
                }
            }
        }
        f[d[h]] = false;
    }
}

void PageRank::run()
{
    double diff = 1;
    std::vector<double> old_pr;
    size_t num = node.size();
    if (num == 0)return;
    pr.assign(num, 0);pr[0] = 1;

    for(int num_iter = 0; diff > 1e-7 && num_iter < 1000000; num_iter++) {
        double sum = 0;
        double noout = 0;
        for (size_t k = 0; k < pr.size(); k++) {
            sum += pr[k];
            if ((int)node[k].edge.size() == 0)noout += pr[k];
        }
        if (num_iter == 0)old_pr = pr;else {
            for (size_t i = 0; i < pr.size(); i++) {
                old_pr[i] = pr[i] / sum;
                pr[i]=0;
            }
        }
        double delta0 = alpha * noout / num;
        double delta1 = (1 - alpha) / num;
        diff = 0;
        for(size_t i = 0; i < edge.size(); i++){
            pr[edge[i].y] += edge[i].wt*old_pr[edge[i].x];
        }
        for (size_t i = 0; i < num; i++) {
            pr[i] = pr[i]*alpha + (delta0 + delta1);
            diff += fabs(pr[i] - old_pr[i]*sum);
        }
    }

    double sum = 0;
    int mdis=0;
    for (size_t k = 0; k < pr.size(); k++){
        spfa(k);
        for(size_t i=0;i<dis.size();i++)maxdis[k]=(dis[i]>1000000000)?maxdis[i]:max(maxdis[k], dis[i]);
        mdis=max(mdis, maxdis[k]);
        qDebug()<<k<<": "<<maxdis[k];
    }

    for (size_t k = 0; k < pr.size(); k++)sum += pr[k];
    for (size_t i = 0; i < pr.size(); i++){
        pr[i] = pr[i] / sum * node.size();
        const double e = 2.718281828459;
        pr[i] += 2/(1+pow(e, -(double(mdis)/maxdis[i]-2)));
        qDebug()<<i<<": "<<2/(1+pow(e, -(double(mdis)/maxdis[i]-2)));
    }
    sum=0;
    for (size_t k = 0; k < pr.size(); k++)sum += pr[k];
    for (size_t i = 0; i < pr.size(); i++){
        pr[i] = pr[i] / sum * node.size();
        qDebug()<<i<<": "<<pr[i];
    }
    qDebug()<<node.size();
}

