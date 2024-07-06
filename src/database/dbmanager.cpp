#include "dbmanager.h"
#include <QDebug>
#include <QDateTime>
#include <QSqlError>
#include "../utils/Qtdirutil.h"
#include <QSqlQuery>

void DbManager::setupDatabase(std::string_view dbName)
{
    QtDirUtil util;
    QString dbPath = QString::fromStdString(util.getAppDataDirFullPath().append("/").append(dbName));
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
}

void DbManager::addInitialCategoriesAndSources()
{

    QSqlQuery query(db);
    if (!query.exec("INSERT OR IGNORE INTO Categories (category_name) VALUES"
                    "('News India'),"
                    "('News World')"))
    {
        qDebug() << "Error Inserting Categories: " << query.lastError().text();
    }

    if (!query.exec("INSERT OR IGNORE INTO Sources (source_id,source_name,source_url,category_id) VALUES "
                    "(1,'Mint','https://www.livemint.com/rss/news',1),"
                    "(2,'The Hindu','https://www.thehindu.com/news/national/feeder/default.rss',1),"
                    "(3,'The Hindu','https://www.thehindu.com/news/international/feeder/default.rss',2)"))
    {
        qDebug() << "Error Inserting Sources: " << query.lastError().text();
    }
}

void DbManager::createCategoryTable()
{
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS Categories("
               "category_id INTEGER PRIMARY KEY,"
               "category_name TEXT UNIQUE)");
}

void DbManager::createSourceTable()
{

    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS Sources ("
               "source_id INTEGER PRIMARY KEY,"
               "source_name TEXT,"
               "source_url TEXT UNIQUIE,"
               "category_id INTEGER,"
               "FOREIGN KEY (category_id) REFERENCES Categories(category_id))");
}

int64_t DbManager::parseRfc822Date(const std::string &datetime)
{
    QDateTime dateTime = QDateTime::fromString(QString::fromStdString(datetime), Qt::RFC2822Date);
    if (!dateTime.isValid())
    {
        return QDateTime::currentDateTime().toSecsSinceEpoch();
    }
    return dateTime.toSecsSinceEpoch();
}

void DbManager::createArticleTable()
{
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS Articles ("
               "article_id INTEGER PRIMARY KEY,"
               "title TEXT,"
               "description TEXT,"
               "url TEXT UNIQUE,"
               "img_url TEXT,"
               "pub_date INTEGER,"
               "guid TEXT UNIQUE,"
               "img_name TEXT,"
               "source_id INTEGER,"
               "FOREIGN KEY (source_id) REFERENCES Sources(source_id))");
}

bool DbManager::insertArticle(const Article &article, int sourceId)
{
    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO Articles (title,description,url,img_url,pub_date,guid,img_name,source_id)"
                  "VALUES (:title,:description,:url,:img_url,:pub_date,:guid,:img_name,:source_id)");

    query.bindValue(":title", QString::fromStdString(article.getTitle()));
    query.bindValue(":description", QString::fromStdString(article.getDescription()));
    query.bindValue(":url", QString::fromStdString(article.getArticleUrl()));
    query.bindValue(":img_url", QString::fromStdString(article.getImageUrl()));
    query.bindValue(":pub_date", static_cast<qint64>(article.getPubDateEpochSecs()));
    query.bindValue(":guid", QString::fromStdString(article.getGuid()));
    query.bindValue(":img_name", QString::fromStdString(article.getImageName()));
    query.bindValue(":source_id", sourceId);
    return query.exec();
}

void DbManager::insertArticles(const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Article>>> &articles)
{

    qDebug() << "Writing Article to Db...\n";
    QSqlQuery query(db);
    query.exec("BEGIN TRANSACTION");

    for (const auto &categoryPair : articles)
    {
        const std::string &categoryName = categoryPair.first;
        for (const auto &sourcePair : categoryPair.second)
        {
            const std::string &sourceName = sourcePair.first;
            const std::vector<Article> &articleList = sourcePair.second;

            query.prepare("SELECT Sources.source_id "
                          "FROM Sources "
                          "JOIN Categories ON Sources.category_id = Categories.category_id "
                          "WHERE Sources.source_name = :source AND Categories.category_name = :category ");
            query.bindValue(":source", QString::fromStdString(sourceName));
            query.bindValue(":category", QString::fromStdString(categoryName));
            qDebug() << query.isValid() << "\n";
            if (!query.exec())
            {
                qDebug() << "Category: " << categoryName << " Source: " << sourceName << "Failed to execute query: " << query.lastError().text();
            }
            int sourceId = -1;
            if (query.next())
            {
                sourceId = query.value(0).toInt();
            }
            if (sourceId != -1)
            {
                for (const auto &article : articleList)
                {
                    insertArticle(article, sourceId);
                }
            }
        }
    }
    query.exec("END TRANSACTION");
    qDebug() << "Article Written to Db...\n";
}

std::vector<Category> DbManager::fetchCategoriesAndSources()
{
    std::vector<Category> categories;
    QSqlQuery query;
    query.exec("SELECT category_id,category_name FROM Categories");
    while (query.next())
    {
        int categoryId = query.value(0).toInt();
        std::string categoryName = query.value(1).toString().toStdString();
        Category category(categoryName);

        QSqlQuery sourceQuery;
        sourceQuery.prepare("SELECT source_name,source_url FROM Sources WHERE category_id = :category_id");
        sourceQuery.bindValue(":category_id", categoryId);
        sourceQuery.exec();
        while (sourceQuery.next())
        {
            std::string sourceName = sourceQuery.value(0).toString().toStdString();
            std::string sourceUrl = sourceQuery.value(1).toString().toStdString();

            Source source(sourceName, sourceUrl);
            category.addSource(source);
        }
        categories.push_back(category);
    }
    return categories;
}

void DbManager::addDummyData()
{
    QSqlQuery query;
    query.prepare("insert into articles (id,title,description,url)"
                  "values (:id,:title,:description,:url)");
    for (int i = 0; i < 20; i++)
    {
        query.bindValue(":id", i);
        query.bindValue(":title", QString("title").append(QString::number(i)));
        query.bindValue(":description", QString("description").append(QString::number(i)));
        query.bindValue(":url", QString("url").append(QString::number(i)));
        query.exec();
    }
}

DbManager::DbManager(std::string_view dbName)
{
    setupDatabase(dbName);
    if (!db.open())
    {
        qDebug() << "Error! Connection with databasae failed\n";
    }
    else
    {
        qDebug() << "Database connection succeeded\n";
    }
    createCategoryTable();
    createSourceTable();
    createArticleTable();
    addInitialCategoriesAndSources();
}