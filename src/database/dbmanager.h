#pragma once
#include <QSqlDatabase>
#include <string_view>
#include <chrono>
#include "article.h"
#include "category.h"

class DbManager
{
public:
    DbManager(std::string_view dbName);
    std::vector<Category> fetchCategoriesAndSources();
    std::vector<Category> fetchInstalledCategoriesAndSources();
    std::vector<Category> fetchNotInstalledCategoriesAndSources();
    std::vector<Article> fetchArticlesFromDB(const std::string &filter, const std::string &sortOrder, const std::string &searchText, const std::string &category, const std::string &source);
    bool insertArticle(const Article &article, int sourceId);
    void insertArticleList(const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Article>>> &articles);
    void insertArticleList(const std::vector<Article> &articles, const std::string &category, const std ::string &source);
    void installSource(const std::string &category, const std::string &source);
    void uninstallSource(const std::string &category, const std::string &source);

private:
    void setupDatabase(std::string_view dbName);
    void createCategoryTable();
    void addInitialCategoriesAndSources();
    void createSourceTable();
    void createArticleTable();
    void createInstalledSourcesTable();
    void createIndexes();
    int64_t parseRfc822Date(const std::string &datetime);
    int64_t getTodaysEpochSeconds();
    int64_t getYesterdaysEpochSeconds();
    int64_t getStartOfWeekEpochSeconds();
    int64_t getStartOfMonthEpochSeconds();
    int64_t getCurrentEpochSeconds();
    QSqlDatabase db;
};