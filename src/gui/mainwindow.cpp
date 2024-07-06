#include "mainwindow.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include "../gui/feedlist.h"
#include <QStackedWidget>
#include <QTextBrowser>
#include <QMenuBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *contentLayout = new QHBoxLayout();

    QTreeWidget *treeWidget = new QTreeWidget(this);
    treeWidget->setHeaderHidden(true);
    QTreeWidgetItem *newsWorld = new QTreeWidgetItem(treeWidget, QStringList() << "News World");
    newsWorld->addChild(new QTreeWidgetItem(newsWorld, QStringList() << "The Hindu"));
    newsWorld->addChild(new QTreeWidgetItem(newsWorld, QStringList() << "New York Times"));

    QTreeWidgetItem *newsIndia = new QTreeWidgetItem(treeWidget, QStringList() << "News India");
    newsIndia->addChild(new QTreeWidgetItem(newsIndia, QStringList() << "The Hindu"));
    newsIndia->addChild(new QTreeWidgetItem(newsIndia, QStringList() << "Live Mint"));
    newsIndia->addChild(new QTreeWidgetItem(newsIndia, QStringList() << "First Post"));

    // QListWidget *articleListWidget = new QListWidget(this);

    FeedList *feedList = new FeedList(this);
    std::vector<Article> articles{
        Article("title1", "description1", "url1", "imgurl1"),
        Article("title2", "description2", "url2", "imgurl2"),
        Article("title3", "description3", "url3", "imgurl3"),
        Article("title1", "description1", "url1", "imgurl1"),
        Article("title2", "description2", "url2", "imgurl2"),
        Article("title3", "description3", "url3", "imgurl3"),
        Article("title1", "description1", "url1", "imgurl1"),
        Article("title2", "description2", "url2", "imgurl2"),
        Article("title3", "description3", "url3", "imgurl3"),
        Article("title1", "description1", "url1", "imgurl1"),
        Article("title2", "description2", "url2", "imgurl2"),
        Article("title3", "description3", "url3", "imgurl3"),
        Article("title1", "description1", "url1", "imgurl1"),
        Article("title2", "description2", "url2", "imgurl2"),
    };
    feedList->loadArticles(articles);

    QStackedWidget *stackedWidget = new QStackedWidget(this);
    QTextBrowser *welcomePage = new QTextBrowser(this);
    welcomePage->setHtml("<h1>Welcome to RSS Reader</h1><p>This is a simple RSS reader application.</p>");
    stackedWidget->addWidget(welcomePage);

    contentLayout->addWidget(treeWidget, 1);
    // contentLayout->addWidget(articleListWidget, 2);
    contentLayout->addWidget(feedList, 2);
    contentLayout->addWidget(stackedWidget, 3);

    mainLayout->addLayout(contentLayout);

    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    QMenu *fileMenu = menuBar->addMenu("File");
    QAction *installSources = fileMenu->addAction("Install Sources");

    QToolBar *toolBar = new QToolBar(this);
    addToolBar(toolBar);
    toolBar->addAction(installSources);

    setWindowTitle("Gyaan Srot");
    resize(1200, 800);
}
