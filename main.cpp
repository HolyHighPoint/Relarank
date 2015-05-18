#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
using namespace std;
int main(int argc, char *argv[])
{
    // create application
    QApplication app(argc, argv);
    app.setOrganizationName("Holy High Point");
    app.setOrganizationDomain("github.com/holyhighpoint");
    app.setApplicationName("Relarank");
    app.setWindowIcon(QIcon(":/icons/Relarank.png"));
    //if(true){
        // create the main window and enter the main execution loop
        MainWindow window;
        window.show();
        return app.exec();
    /*}else{
        vector < graph_node > node;
        vector < graph_edge > edge;
        vector < double > pagerank;
        QString filename(argv[1]);
        QFile file(filename);
        QTextStream in(&file);
        node.clear();
        edge.clear();
        int n, m;
        in >> n >> m;
        for (int i = 0; i < n; i++) {
            QString name;
            in >> name;
            node.push_back(graph_node(name));
        }
        vector<double> sumn;
        sumn.assign(n, 0.0);
        for (int i = 0; i < m; i++) {
            int x, y;
            double w;
            in >> y >> x >> w;
            x--, y--;
            sumn[x]+=w;
            edge.push_back(graph_edge(x, y, w));
            node[x].insert(x, y, w);
        }
        for (int i = 0; i < m; i++)edge[i].wt = edge[i].w/sumn[edge[i].x];
        sort(edge.begin(), edge.end());
        PageRank *pr = new PageRank(node, edge, pagerank, NULL);
        pr->start();
        while(!pr->isFinished());
        delete pr;
        vector<pair<double, QString>> tmp;
        for(size_t i=0;i<node.size();i++)tmp.push_back(make_pair(pagerank[i], node[i].name));
        sort(tmp.begin(), tmp.end());
        FILE *fo = fopen(argv[2], "w");
        QTextStream out(fo, QIODevice::WriteOnly);
        for(int i=node.size()-1;i>=0;i--)out<<"Rank:"<<n-i<<"    Name:"<<tmp[i].second<<"    Value:"<<tmp[i].first<<endl;
        return 0;
    }*/
}
