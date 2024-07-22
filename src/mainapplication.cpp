#include "mainapplication.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QTextBrowser>
#include <QMenuBar>
#include <QApplication>
#include <QToolBar>
#include <QSplitter>
#include <QProgressDialog>

void MainApplication::reloadNewArticles(const std::vector<Article> &articles)
{
    feedList->loadArticles(articles);
}

MainApplication::MainApplication(QWidget *parent, const std::string &dbName)
    : QMainWindow(parent),
      manager(dbName),
      fetcher(new QtFeedFetcher()),
      parser(new QtFeedParser()),
      installDialog(new SourceInstallDialog(this)),
      fid(new QtFeedImageDownloader()),
      util(new QtDirUtil())
{

    util->initApplicationDirs();
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

    // QMenuBar *menuBar = new QMenuBar(this);
    // setMenuBar(menuBar);
    // QMenu *fileMenu = menuBar->addMenu("File");
    // QAction *installSources = fileMenu->addAction("Install Sources");

    QToolBar *toolBar = new QToolBar(this);
    installSourcesAction = new QAction(QIcon::fromTheme("list-add"), "Install Sources", this);
    toolBar->addAction(installSourcesAction);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addToolBar(toolBar);

    setWindowTitle("Gyaan Srot");
    resize(1200, 800);

    // connections
    connect(sourceTree, &SourceTree::sourceClicked, this, &MainApplication::sourceSelected);
    connect(feedList->filterSelector, &FilterSelector::filterChanged, this, &MainApplication::onFilterChanged);
    connect(feedList->filterSelector, &FilterSelector::sortChanged, this, &MainApplication::onSortChanged);
    connect(feedList->filterSelector, &FilterSelector::searchTextChanged, this, &MainApplication::onSearchTextChanged);
    connect(feedList->filterSelector, &FilterSelector::searchTextCleared, this, &MainApplication::onSearchTextCleared);
    connect(installSourcesAction, &QAction::triggered, this, &MainApplication::onInstallSourcesAction);
    connect(installDialog, &SourceInstallDialog::installSources, this, &MainApplication::onInstallSources);
    connect(installDialog, &SourceInstallDialog::uninstallSources, this, &MainApplication::onUninstallSources);
}

void MainApplication::fetchAndStoreArticlesFromSources(const std::vector<Category> &installedSources)
{
    int count = 0;
    int noOfSourcesToInstall = 0;
    for (const auto &category : installedSources)
    {
        for (const auto &source : category.getSources())
        {
            noOfSourcesToInstall++;
        }
    }
    QProgressDialog progress(this);
    progress.setWindowTitle("Fetching Articles");
    progress.setLabelText("<b> Fetching Articles... </b>");
    progress.setCancelButtonText("Cancel");
    progress.setRange(0, noOfSourcesToInstall);
    progress.setMinimumSize(QSize(300, 100));
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setWindowIcon(QIcon::fromTheme("emblem-downloads"));

    for (const auto &category : installedSources)
    {
        for (const auto &source : category.getSources())
        {
            if (progress.wasCanceled())
            {
                break;
            }
            progress.setValue(count++);
            const std::vector<Article> &articleList = parser->getArticleList(fetcher->getFeedXmlData(source.getSourceUrl()), category.getCategoryName(), source.getSourceName());
            fid->downloadAllImages(articleList, category.getCategoryName(), source.getSourceName());
            manager.insertArticleList(articleList, category.getCategoryName(), source.getSourceName());
        }
    }
    progress.setValue(installedSources.size());
}

void MainApplication::initInitialArticleData()
{
    categories = manager.fetchInstalledCategoriesAndSources();
    fetchAndStoreArticlesFromSources(categories);
}

void MainApplication::refreshSourceTree(const std::vector<Category> &installedSources)
{
    sourceTree->setCategoryList(installedSources);
    sourceTree->update();
}

void MainApplication::refreshSourceInstallDialog(const std::vector<Category> &installedSources, const std::vector<Category> &avialableSources)
{
    installDialog->makeAvailableList(avialableSources);
    installDialog->makeInstalledList(installedSources);
    installDialog->update();
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

void MainApplication::onInstallSourcesAction()
{
    installDialog->makeAvailableList(manager.fetchNotInstalledCategoriesAndSources());
    installDialog->makeInstalledList(manager.fetchInstalledCategoriesAndSources());
    installDialog->exec();
}

void MainApplication::onInstallSources(const QList<QPair<QString, QString>> &sourcesToInstall)
{
    for (const auto &[category, source] : sourcesToInstall)
    {
        manager.installSource(category.toStdString(), source.toStdString());
    }
    const std::vector<Category> &installedSources = manager.fetchInstalledCategoriesAndSources();
    const std::vector<Category> &availableSources = manager.fetchNotInstalledCategoriesAndSources();
    refreshSourceInstallDialog(installedSources, availableSources);
    refreshSourceTree(installedSources);
    fetchAndStoreArticlesFromSources(installedSources);
}

void MainApplication::onUninstallSources(const QList<QPair<QString, QString>> &sourcesToUninstall)
{
    for (const auto &[category, source] : sourcesToUninstall)
    {
        manager.uninstallSource(category.toStdString(), source.toStdString());
    }
    const std::vector<Category> &installedSources = manager.fetchInstalledCategoriesAndSources();
    const std::vector<Category> &availableSources = manager.fetchNotInstalledCategoriesAndSources();
    refreshSourceInstallDialog(installedSources, availableSources);
    refreshSourceTree(installedSources);
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
    const std::vector<Article> &newArticleList = manager.fetchArticlesFromDB(filter.toStdString(), sortOrder, "", category, source);
    reloadNewArticles(newArticleList);
}
void MainApplication::onSortChanged(const QString &sortOrder)
{

    const std::string &filter = feedList->getFilter().toStdString();
    const std::string &category = sourceTree->getCurrentCategory().toStdString();
    const std::string &source = sourceTree->getCurrentSource().toStdString();
    const std::vector<Article> &newArticleList = manager.fetchArticlesFromDB(filter, sortOrder.toStdString(), "", category, source);
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