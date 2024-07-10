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
    void addDummyData();
    std::vector<Category> fetchCategoriesAndSources();
    std::vector<Article> fetchArticlesFromDb(const std::string &category, const std::string &source) const;
    bool insertArticle(const Article &article, int sourceId);
    void insertArticleList(const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Article>>> &articles);
    void insertArticleList(const std::vector<Article> &articles, const std::string &category, const std ::string &source);
    int64_t parseRfc822Date(const std::string &datetime);

private:
    void setupDatabase(std::string_view dbName);
    void createCategoryTable();
    void addInitialCategoriesAndSources();
    void createSourceTable();
    void createArticleTable();
    QSqlDatabase db;
};