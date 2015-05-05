#include "mainwindow.h"

#include <QApplication>
#include <QAction>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>
#include <QSplitter>
#include <QStyleFactory>
#include <QToolBar>
#include <QDebug>
#include <QMenuBar>
#include <QTime>
#include <QProgressDialog>
#include <cmath>
#include <cstring>

#include "nodectrl.h"
#include "mainctrl.h"
#include "propertyeditor.h"
#include "relarankgraph/baseedge.h"
#include "relarankgraph/edgearrow.h"
#include "relarankgraph/edgelabel.h"
#include "relarankgraph/node.h"
#include "relarankgraph/nodelabel.h"
#include "relarankgraph/perimeter.h"
#include "relarankgraph/plug.h"
#include "relarankgraph/pluglabel.h"
#include "relarankgraph/scene.h"
#include "relarankgraph/view.h"
using namespace std;

MainWindow::MainWindow(QWidget * parent): QMainWindow(parent)
{
    this->resize(1366, 768);
    // create the main toolbar
    //QMenuBar* mainMenuBar = new QMenuBar(this);
    QToolBar *
    mainToolBar = new QToolBar("ToolBar", this);
    //this->setMenuBar(mainMenuBar);
    //mainToolBar->setStyleSheet("QToolBar {border: 0px;}");
    //mainToolBar->setIconSize(QSize(12,12));
    mainToolBar->setMovable(false);
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addToolBar(Qt::TopToolBarArea, mainToolBar);
    // create global actions
    QAction *
    newNodeAction =
        new QAction(QIcon(":/icons/incoming.png"), tr("&Open"), this);
    newNodeAction->setShortcuts(QKeySequence::Open);
    newNodeAction->setStatusTip(tr("Open the file"));
    mainToolBar->addAction(newNodeAction);
    connect(newNodeAction, SIGNAL(triggered()), this, SLOT(openfile()));
    QAction *
    closeAction =
        new QAction(QIcon(":/icons/outgoing.png"), tr("E&xit"), this);
    closeAction->setStatusTip(tr("Exit"));
    closeAction->setShortcuts(QKeySequence::Close);
    mainToolBar->addAction(closeAction);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));
    QAction *
    aboutAction =
        new QAction(QIcon(":/icons/questionmark.png"), tr("&About"), this);
    aboutAction->setStatusTip(tr("About this application"));
    mainToolBar->addAction(aboutAction);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(displayAbout()));
    // create the status bar
    statusBar();
    // create the RelaRank graph
    relarankScene = new relarank::Scene(this);
    relarankView = new relarank::View(this);
    relarankView->setScene(relarankScene);
    // create the Property Editor
    propertyEditor = new PropertyEditor(this);
    // create the Main Controller
    m_mainCtrl = new MainCtrl(this, relarankScene, propertyEditor);
    // setup the main splitter
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainSplitter->addWidget(propertyEditor);
    m_mainSplitter->addWidget(relarankView);
    m_mainSplitter->setSizes({
        100, 900
    });
    // initialize the GUI
    setCentralWidget(m_mainSplitter);
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if (m_mainCtrl->shutdown()) {
        // if the user accepted, close the application
        //writeSettings();
        event->accept();
    } else {
        // if the user objected, do nothing
        event->ignore();
    }
}

void MainWindow::displayAbout()
{
    QMessageBox aboutBox;
    aboutBox.setWindowTitle("About the Relarank");
    aboutBox.setText("<h3>About this Application</h3>");
    aboutBox.exec();
}

void MainWindow::openfile()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("打开文件"), "",
                                     tr("rel文件(*.rel);;所有文件(*.*)"));
    if (fileName == "")
        return;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("错误"),
                              tr("无法打开 %1").arg(fileName));
        return;
    }
    int n, m;
    QTextStream in(&file);
    node.clear();
    edge.clear();
    in >> n >> m;
    qDebug() << n << " " << m << endl;
    for (int i = 0; i < n; i++) {
        QString name;
        in >> name;
        qDebug() << name;
        node.push_back(graph_node(name));
    }
    for (int i = 0; i < m; i++) {
        int x, y;
        double w;
        in >> x >> y >> w;
        x--, y--;
        qDebug() << x << " " << y << " " << w;
        edge.push_back(graph_edge(x, y, w));
        node[x].insert(x, y, w);
    }
    for (int i = 0; i < n; i++) {
        node[i].sort();
    }
    PageRank *pr = new PageRank(node, edge, pagerank, this);
    pr->start();
    Planarity *pl = new Planarity(node, edge, location, this);
    pl->start();
    clearsta();
    printsta(m_mainCtrl, pl);
}

void MainWindow::clearsta()
{
    for (vector < NodeCtrl * >::const_iterator i = nodectrl.begin();
         i != nodectrl.end(); i++) {
        (*i)->remove();
        delete *i;
    }
    nodectrl.clear();
    delete m_mainCtrl;
    delete relarankView;
    delete relarankScene;
    delete propertyEditor;
    delete m_mainSplitter;
    relarankScene = new relarank::Scene(this);
    relarankView = new relarank::View(this);
    relarankView->setScene(relarankScene);
    propertyEditor = new PropertyEditor(this);
    m_mainCtrl = new MainCtrl(this, relarankScene, propertyEditor);
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainSplitter->addWidget(propertyEditor);
    m_mainSplitter->addWidget(relarankView);
    m_mainSplitter->setSizes({
        100, 900
    }
                            );
    setCentralWidget(m_mainSplitter);
}

void MainWindow::printsta(MainCtrl * mainCtrl, Planarity *planar)
{
    QProgressDialog progress_dialog("    Loading graph file...    ", "Cancel", 0, node.size() + edge.size(), this);
    progress_dialog.show();
    qApp->processEvents();
    /*const double PI = 3.1415926535898;
    const double dis = 200.;
    const double ang = 2 * PI / node.size();
    const double R = sqrt(dis*dis/2/(1-cos(ang)))*/
/*
    int *d = new int[node.size()+1], h=0, t=1;d[t]=0;
    int *f = new int[node.size()+1];
    vector<vector<int> >sta;sta.clear();
    for(int i=0;i<=(int)node.size();i++)f[i]=0;
    for(int i=0;i<(int)node.size();i++)if(!f[i]){
        f[i]=1;
        if(sta.empty())sta.push_back(vector<int>());
        sta[0].push_back(i);
        while(h != t){
            h++;
            for (vector < graph_edge >::const_iterator i = node[d[h]].edge.begin(); i != node[d[h]].edge.end(); i++) {
                int y = !f[i->x]?i->x:i->y, x=(f[i->x] == y)?i->y:i->x;
                if(!f[y]){
                    if((int)sta.size() <= f[x])sta.push_back(vector<int>());
                    sta[f[x]].push_back(y);
                    f[y]=f[x]+1;
                    d[++t]=y;
                }
            }
        }
    }
    delete [] d;
    delete [] f;
    double R = dis;
    qsrand(QTime().secsTo(QTime::currentTime()));
    for(vector<vector<int> >::iterator i = sta.begin();i != sta.end(); i++){
        const double ang = 2 * PI / i->size();
        const double angtr = double(qrand()%10000)/20000.0*PI;
        R = max(R+dis, (abs(sin(ang))>1e-3)?sqrt(dis*dis/2/sin(ang)):0);
        for(vector<int>::iterator j = i->begin();j!= i->end();j++){
            progress_dialog.setValue(progress_dialog.value()+1);
            nodectrl.push_back(mainCtrl->createNode(node[*j].name));

            nodectrl.back()->getNodeHandle().setPos(R *cos(ang * (j - i->begin())+angtr), R * sin(ang * (j - i->begin()))+angtr);
            qApp->processEvents();
            if (progress_dialog.wasCanceled()) {
                clearsta();
                return;
            }
        }
    }
*/
    while(!planar->isFinished())qApp->processEvents();
    for (vector < graph_node >::const_iterator i = node.begin(); i != node.end(); i++) {
        progress_dialog.setValue(i - node.begin());
        nodectrl.push_back(mainCtrl->createNode(i->name));
        nodectrl.back()->getNodeHandle().setPos(location[i-node.begin()].first, location[i-node.begin()].second);
        qApp->processEvents();
        /*if (progress_dialog.wasCanceled()) {
            clearsta();
            return;
        }*/
    }
    for (vector < graph_edge >::const_iterator i = edge.begin(); i != edge.end(); i++) {
        progress_dialog.setValue(node.size() + i - edge.begin());
        char tmpw[50];
        sprintf(tmpw, "%.2lf", i->w);
        relarank::PlugHandle x =
            nodectrl[i->x]->addOutgoingPlug("Out to " + node[i->y].name + " Value = " + QString(tmpw));
        relarank::PlugHandle y =
            nodectrl[i->y]->addIncomingPlug("In from " +node[i->x].name + " Value = " + QString(tmpw));
        x.connectPlug(y);
        qApp->processEvents();
        if (progress_dialog.wasCanceled()) {
            //clearsta();
            return;
        }
    }
    progress_dialog.accept();
}

