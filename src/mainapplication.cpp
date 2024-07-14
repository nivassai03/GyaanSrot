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
    initInitialArticleData();
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
    feedList->filterSelector->setEnabled(false);
    feedList->setMinimumWidth(600);

    // WebView widget
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
    connect(sourceTree, &SourceTree::sourceClicked, this, &MainApplication::sourceSelected);
    connect(feedList->filterSelector, &FilterSelector::filterChanged, this, &MainApplication::onFilterChanged);
    connect(feedList->filterSelector, &FilterSelector::sortChanged, this, &MainApplication::onSortChanged);
    connect(feedList->filterSelector, &FilterSelector::searchTextChanged, this, &MainApplication::onSearchTextChanged);
    connect(feedList->filterSelector, &FilterSelector::searchTextCleared, this, &MainApplication::onSearchTextCleared);
}

void MainApplication::initInitialArticleData()
{
    categories = manager.fetchCategoriesAndSources();
    for (const auto &category : categories)
    {
        for (const auto &source : category.getSources())
        {
            const std::vector<Article> &articleList = parser->getArticleList(fetcher->getFeedXmlData(source.getSourceUrl()), category.getCategoryName(), source.getSourceName());
            fid->downloadAllImages(articleList, category.getCategoryName(), source.getSourceName());
            manager.insertArticleList(articleList, category.getCategoryName(), source.getSourceName());
        }
    }
}

void MainApplication::sourceSelected(const QString &category, const QString &source)
{
    if (!category.isEmpty() && !source.isEmpty())
    {
        feedList->filterSelector->setEnabled(true);
    }
    else
    {
        feedList->filterSelector->setEnabled(false);
    }
    const std::string &filter = feedList->getFilter().toStdString();
    const std::string &sortOrder = feedList->getSortOrder().toStdString();
    const std::string &searchText = feedList->getSearchText().toStdString();
    const std::vector<Article> &newArticleList = manager.fetchArticlesFromDB(filter, sortOrder, searchText, category.toStdString(), source.toStdString());
    reloadNewArticles(newArticleList);
}

void MainApplication::openArticleInWebView(const QString &url)
{
    webview->loadArticle(url);
}

void MainApplication::onFilterChanged(const QString &filter)
{
    const std::string &sortOrder = feedList->getSortOrder().toStdString();
    const std::string &category = sourceTree->getCurrentCategory().toStdString();
    const std::string &source = sourceTree->getCurrentSource().toStdString();
    const std::vector<Article> &newArticleList = manager.fetchFilteredAndSortedArticles(filter.toStdString(), sortOrder, category, source);
    reloadNewArticles(newArticleList);
}
void MainApplication::onSortChanged(const QString &sortOrder)
{

    const std::string &filter = feedList->getFilter().toStdString();
    const std::string &category = sourceTree->getCurrentCategory().toStdString();
    const std::string &source = sourceTree->getCurrentSource().toStdString();
    const std::vector<Article> &newArticleList = manager.fetchFilteredAndSortedArticles(filter, sortOrder.toStdString(), category, source);
    reloadNewArticles(newArticleList);
}

void MainApplication::onSearchTextChanged(const QString &searchText)
{

    const std::string &filter = feedList->getFilter().toStdString();
    const std::string &sortOrder = feedList->getSortOrder().toStdString();
    const std::string &category = sourceTree->getCurrentCategory().toStdString();
    const std::string &source = sourceTree->getCurrentSource().toStdString();
    const std::vector<Article> &newArticleList = manager.fetchArticlesFromDB(filter, sortOrder, searchText.toStdString(), category, source);
    reloadNewArticles(newArticleList);
}

void MainApplication::onSearchTextCleared()
{
    const std::string &filter = feedList->getFilter().toStdString();
    const std::string &sortOrder = feedList->getSortOrder().toStdString();
    const std::string &category = sourceTree->getCurrentCategory().toStdString();
    const std::string &source = sourceTree->getCurrentSource().toStdString();
    const std::vector<Article> &newArticleList = manager.fetchArticlesFromDB(filter, sortOrder, "", category, source);
    reloadNewArticles(newArticleList);
}