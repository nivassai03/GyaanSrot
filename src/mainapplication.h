#pragma once
#include <QObject>
#include <QMainWindow>
#include "gui/feedlist.h"
#include <QWidget>
#include <QTabWidget>
#include <QAction>
#include <vector>
#include <unordered_map>
#include <string_view>
#include "database/article.h"
#include "database/category.h"
#include "database/dbmanager.h"
#include "database/source.h"
#include "network/Qtfeedfetcher.h"
#include "network/Qtfeedimagedownloader.h"
#include "parser/Qtfeedparser.h"
#include "utils/Qtdirutil.h"
#include "gui/sourcetree.h"
#include "gui/webview.h"
#include "gui/sourceinstalldialog.h"

class MainApplication : public QMainWindow
{
    Q_OBJECT

private:
    FeedList *feedList;
    FeedFetcher *fetcher;
    SourceTree *sourceTree;
    WebView *webview;
    FeedParser *parser;
    FeedImageDownloader *fid;
    DbManager manager;
    DirUtil *util;
    QAction *installSourcesAction;
    SourceInstallDialog *installDialog;
    std::vector<Category> categories;
    void openArticleInWebView(const QString &url);
    void initInitialArticleData();
    void refreshSourceTree(const std::vector<Category> &installedSources);
    void refreshSourceInstallDialog(const std::vector<Category> &installedSources, const std::vector<Category> &availableSources);
    void fetchAndStoreArticlesFromSources(const std::vector<Category> &installedSources);

private slots:
    void onInstallSourcesAction();
    void sourceSelected(const QString &category, const QString &source);
    void onInstallSources(const QList<QPair<QString, QString>> &sourcesToInstall);
    void onUninstallSources(const QList<QPair<QString, QString>> &sourcesToUninstall);
    void onFilterChanged(const QString &filter);
    void onSortChanged(const QString &sortOrder);
    void onSearchTextChanged(const QString &searchText);
    void onSearchTextCleared();

public:
    void reloadNewArticles(const std::vector<Article> &articleList);
    MainApplication(QWidget *parent = nullptr, const std::string &dbName = "data.db");
};