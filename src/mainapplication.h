#pragma once
#include <QObject>
#include <QMainWindow>
#include "gui/feedlist.h"
#include <QWidget>
#include <QTabWidget>
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
    std::vector<Category> categories;
    void openArticleInWebView(const QString &url);
    void initInitialArticleData();
    void onFilterChanged(const QString &filter);
    void onSortChanged(const QString &sortOrder);
    void onSearchTextChanged(const QString &searchText);
    void onSearchTextCleared();

public slots:
    void sourceSelected(const QString &category, const QString &source);

public:
    void reloadNewArticles(const std::vector<Article> &articleList);
    MainApplication(QWidget *parent = nullptr, const std::string &dbName = "data.db");
};