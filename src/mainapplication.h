#pragma once
#include <QObject>
#include <QMainWindow>
#include "gui/feedlist.h"
#include <QWidget>
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
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Article>>> articles;
    std::vector<Category> categories;
    std::vector<Source> sources;
    void fetchData();
    FeedFetcher *fetcher;
    SourceTree *sourceTree;
    WebView *webview;
    FeedParser *parser;
    FeedImageDownloader *fid;
    DbManager manager;
    DirUtil *util;
    void openArticleInWebView(const QString &url);

public slots:
    void updateFeedList(const std::string &category, const std::string &source);

public:
    void reloadNewArticles(const std::vector<Article> &articleList);
    MainApplication(QWidget *parent = nullptr, const std::string &dbName = "data.db");
};