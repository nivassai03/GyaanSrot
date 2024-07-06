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
    bool insertArticle(const Article &article, int sourceId);
    void insertArticles(const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Article>>> &articles);
    int64_t parseRfc822Date(const std::string &datetime);

private:
    void setupDatabase(std::string_view dbName);
    void createCategoryTable();
    void addInitialCategoriesAndSources();
    void createSourceTable();
    void createArticleTable();
    QSqlDatabase db;
};