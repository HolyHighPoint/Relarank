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
#include <QTimer>
#include <QProgressDialog>
#include <cmath>
#include <cstring>
#include <algorithm>

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
    QAction *newNodeAction = new QAction(QIcon(":/icons/incoming.png"), tr("&Open"), this);
    newNodeAction->setShortcuts(QKeySequence::Open);
    newNodeAction->setStatusTip(tr("Open the file"));
    mainToolBar->addAction(newNodeAction);
    connect(newNodeAction, SIGNAL(triggered()), this, SLOT(openfile()));
    QAction *saveAction = new QAction(QIcon(":/icons/play.png"), tr("&Save"), this);
    saveAction->setStatusTip(tr("Save"));
    saveAction->setShortcuts(QKeySequence::Save);
    mainToolBar->addAction(saveAction);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(savefile()));
    QAction *closeAction = new QAction(QIcon(":/icons/outgoing.png"), tr("E&xit"), this);
    closeAction->setStatusTip(tr("Exit"));
    closeAction->setShortcuts(QKeySequence::Close);
    mainToolBar->addAction(closeAction);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));
    QAction * aboutAction = new QAction(QIcon(":/icons/questionmark.png"), tr("&About"), this);
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
    ranklist = new RankList(this);
    // create the Node List
    m_mainCtrl = new MainCtrl(this, relarankScene, propertyEditor);
    // setup the main splitter
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainSplitter->addWidget(propertyEditor);
    m_mainSplitter->addWidget(relarankView);
    m_mainSplitter->addWidget(ranklist);
    m_mainSplitter->setSizes({200, 900, 200});
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
    QMessageBox aboutBox(this);
    aboutBox.setToolTip("About this Application");
    aboutBox.setText("<h2>Relarank</h2>"
                     "<p>Relarank is an application for comparing importance degrees in the relationship.</p>"
                     "<h4>By: </h4><p>&nbsp;&nbsp;&nbsp;&nbsp;Jiguanglizipao<br>&nbsp;&nbsp;&nbsp;&nbsp;ccssqq1122233</p>"
                     "<h4>Website: </h4><p><a href=https://github.com/holyhighpoint/relarank>https://github.com/holyhighpoint/relarank</a></p>");
    aboutBox.setIconPixmap(QPixmap(":/icons/zerg.png"));
    aboutBox.exec();
    //aboutBox.about(this, "About this Application", "Relarank is an application for comparing importance degree in the relationship.");
}

void MainWindow::openfile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Files"), "", tr("Rel Files(*.rel);;All Files(*.*)"));
    if (fileName == "") return;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("ERROR!"), tr("Can not open this fils. %1").arg(fileName));
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
    vector<double> sumn;
    sumn.assign(n, 0.0);
    for (int i = 0; i < m; i++) {
        int x, y;
        double w;
        in >> y >> x >> w;
        x--, y--;
        sumn[x]+=w;
        qDebug() << y << " " << x << " " << w;
        edge.push_back(graph_edge(x, y, w));
        node[x].insert(x, y, w);
    }
    file.close();
    for (int i = 0; i < m; i++)edge[i].wt = edge[i].w/sumn[edge[i].x];
    sort(edge.begin(), edge.end());
    pagerank.clear();location.clear();
    PageRank *pr = new PageRank(node, edge, pagerank, this);
    pr->start();
    Planarity *pl = new Planarity(node, edge, location, this);
    if(node.size() <= 500)pl->start();
    clearsta();
    if(node.size() <= 500)printsta(m_mainCtrl, pl, pr);else printsta(pr);
}

void MainWindow::savefile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Files"), "", tr("CSV Files(*.csv);;All Files(*.*)"));
    if(pagerank.empty())return;
    qDebug()<<fileName<<endl;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))return;
    QTextStream out(&file);
    vector<pair<double, QString> > tmp;
    for(size_t i=0;i<node.size();i++)tmp.push_back(make_pair(pagerank[i], node[i].name));
    sort(tmp.begin(), tmp.end());
    out<<"Rank,"<<"Name,"<<"Value"<<endl;
    for(int i=node.size()-1;i>=0;i--)out<<node.size()-i<<",\""<<tmp[i].second<<"\","<<tmp[i].first<<endl;
    file.close();
}

void MainWindow::clearsta()
{
    for (vector < NodeCtrl * >::const_iterator i = nodectrl.begin();
         i != nodectrl.end(); i++) {
        if(*i != NULL)(*i)->remove();
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
    ranklist = new RankList(this);
    m_mainCtrl = new MainCtrl(this, relarankScene, propertyEditor);
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainSplitter->addWidget(propertyEditor);
    m_mainSplitter->addWidget(relarankView);
    m_mainSplitter->addWidget(ranklist);
    m_mainSplitter->setSizes({200, 900, 200});
    setCentralWidget(m_mainSplitter);
}

void MainWindow::printsta(MainCtrl * mainCtrl, Planarity *planarT, PageRank * pagerankT)
{
    QProgressDialog progress_dialog("    Loading graph file...    ", "Cancel", 0, node.size() + edge.size(), this);
    progress_dialog.show();
    qApp->processEvents();
    while(!planarT->isFinished())qApp->processEvents();
    nodectrl.assign(node.size(), NULL);
    for (vector < graph_node >::const_iterator i = node.begin(); i != node.end(); i++) {
        progress_dialog.setValue(i - node.begin());
        nodectrl[i-node.begin()]=mainCtrl->createNode(i->name);
        nodectrl[i-node.begin()]->getNodeHandle().setPos(location[i-node.begin()].first, location[i-node.begin()].second);
        qApp->processEvents();
        if (progress_dialog.wasCanceled()) {
            break;
        }
    }
    while(!pagerankT->isFinished())qApp->processEvents();
    ranklist->changelist(node, pagerank, nodectrl);
    if (progress_dialog.wasCanceled()) {
        return;
    }
    for (vector < graph_edge >::const_iterator i = edge.begin(); i != edge.end(); i++) {
        progress_dialog.setValue(node.size() + i - edge.begin());
        if(!i->flag)continue;
        char tmpw[50];
        sprintf(tmpw, "%.2lf", i->wt);
        relarank::PlugHandle y =
            nodectrl[i->y]->addOutgoingPlug("Out to " + node[i->x].name + " Value = " + QString(tmpw));
        relarank::PlugHandle x =
            nodectrl[i->x]->addIncomingPlug("In from " +node[i->y].name + " Value = " + QString(tmpw));
        y.connectPlug(x);
        qApp->processEvents();
        if (progress_dialog.wasCanceled()) {
            return;
        }
    }
    progress_dialog.accept();
    delete pagerankT;
    delete planarT;
}

void MainWindow::printsta(PageRank * pagerankT)
{
    QProgressDialog progress_dialog("    Loading graph file...    ", "Cancel", 0, node.size()*node.size()*100, this);
    progress_dialog.show();
    qApp->processEvents();
    while(!pagerankT->isFinished()){
        progress_dialog.setValue(progress_dialog.value()+1);
        qApp->processEvents();
        if (progress_dialog.wasCanceled()) {
            return;
        }
    }
    nodectrl.assign(node.size(), NULL);
    ranklist->changelist(node, pagerank, nodectrl);
    progress_dialog.accept();
    delete pagerankT;
}
