#include "mainapplication.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QTextBrowser>
#include <QMenuBar>
#include <QToolBar>
#include <QSplitter>

void MainApplication::reloadNewArticles(const std::vector<Article> &articles)
{
    feedList->loadArticles(articles);
}

MainApplication::MainApplication(QWidget *parent, const std::string &dbName)
    : QMainWindow(parent),
      manager(dbName),
      fetcher(new QtFeedFetcher()),
      parser(new QtFeedParser()),
      fid(new QtFeedImageDownloader()),
      util(new QtDirUtil())
{
    fetchData();

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 5, 0, 0);
    mainLayout->setSpacing(0);
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    sourceTree = new SourceTree(this);
    sourceTree->setCategoryList(categories);
    sourceTree->setFixedWidth(300);

    feedList = new FeedList(this);
    feedList->loadArticles(std::vector<Article>());
    feedList->setMinimumWidth(600);

    webview = new WebView(this);
    webview->setMinimumWidth(600);
    connect(feedList, &FeedList::articleClicked, this, &MainApplication::openArticleInWebView);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(feedList);
    splitter->addWidget(webview);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 3);

    contentLayout->addWidget(sourceTree, 1);
    contentLayout->addWidget(splitter, 2);

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

    // connections
    connect(sourceTree, &SourceTree::sourceClicked, this, &MainApplication::updateFeedList);
}
void MainApplication::fetchData()
{
    categories = manager.fetchCategoriesAndSources();
    for (auto category : categories)
    {
        for (auto source : category.getSources())
        {
            const std::vector<Article> &articleList = parser->getArticleList(fetcher->getFeedXmlData(source.getSourceUrl()), category.getCategoryName(), source.getSourceName());
            fid->downloadAllImages(articleList, category.getCategoryName(), source.getSourceName());
            articles[category.getCategoryName()][source.getSourceName()] = articleList;
            manager.insertArticleList(articleList, category.getCategoryName(), source.getSourceName());
            sources.push_back(source);
        }
    }
}

void MainApplication::updateFeedList(const std::string &category, const std::string &source)
{
    const std::vector<Article> &newArticleList = articles[category][source];
    reloadNewArticles(newArticleList);
}

void MainApplication::openArticleInWebView(const QString &url)
{
    webview->loadArticle(url);
}