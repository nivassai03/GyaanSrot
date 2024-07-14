#include "dbmanager.h"
#include <QDebug>
#include <QDateTime>
#include <QSqlError>
#include <QTimeZone>
#include "../utils/Qtdirutil.h"
#include "../utils/util.h"
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

void DbManager::insertArticleList(const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Article>>> &articles)
{
    for (const auto &categoryPair : articles)
    {
        const std::string &categoryName = categoryPair.first;
        for (const auto &sourcePair : categoryPair.second)
        {
            const std::string &sourceName = sourcePair.first;
            const std::vector<Article> &articleList = sourcePair.second;
            insertArticleList(articleList, categoryName, sourceName);
        }
    }
}

void DbManager::insertArticleList(const std::vector<Article> &articles, const std::string &category, const std::string &source)
{
    QSqlQuery query(db);
    query.exec("BEGIN TRANSACTION");
    query.prepare("SELECT Sources.source_id "
                  "FROM Sources "
                  "JOIN Categories ON Sources.category_id = Categories.category_id "
                  "WHERE Sources.source_name = :source AND Categories.category_name = :category ");
    query.bindValue(":source", QString::fromStdString(source));
    query.bindValue(":category", QString::fromStdString(category));
    if (!query.exec())
    {
        qDebug() << "Failed to execute query: " << query.lastError().text();
    }
    int sourceId = -1;
    if (query.next())
    {
        sourceId = query.value(0).toInt();
    }
    if (sourceId != -1)
    {
        for (const auto &article : articles)
        {
            insertArticle(article, sourceId);
        }
    }
    query.exec("END TRANSACTION");
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

std::vector<Article> DbManager::fetchArticlesFromDb(const std::string &category, const std::string &source) const
{
    std::vector<Article> articles;
    QSqlQuery query(db);
    query.prepare("SELECT Articles.title,Articles.description,Articles.url,Articles.img_url,Articles.pub_date,Articles.guid,Articles.img_name "
                  "FROM Articles "
                  "INNER JOIN Sources on Sources.source_id = Articles.source_id "
                  "INNER JOIN Categories on Categories.category_id = Sources.category_id "
                  "WHERE Sources.source_name = :source AND Categories.category_name = :category ");
    query.bindValue(":source", QString::fromStdString(source));
    query.bindValue(":category", QString::fromStdString(category));
    if (!query.exec())
    {
        qDebug() << "Fecth Article From DB Execution Failed: " << query.lastError().text();
        return articles;
    }
    while (query.next())
    {
        const std::string &title = query.value(0).toString().toStdString();
        const std::string &description = query.value(1).toString().toStdString();
        const std::string &url = query.value(2).toString().toStdString();
        const std::string &imgUrl = query.value(3).toString().toStdString();
        const std::chrono::system_clock::time_point &pubDate = fromEpochSeconds(query.value(4).toInt());
        const std::string &guid = query.value(5).toString().toStdString();
        const std::string &imgName = query.value(6).toString().toStdString();
        articles.push_back(Article(title, description, url, imgUrl, pubDate, guid, imgName, source, category));
    }
    return articles;
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
    createIndexes();
    addInitialCategoriesAndSources();
}

void DbManager::createIndexes()
{
    QSqlQuery query(db);
    if (query.exec("CREATE INDEX IF NOT EXISTS idx_articles_date ON Articles (pub_date) "))
    {
        qDebug() << "Date Index Successfull";
    }
    else
    {
        qDebug() << "Date Index Failed: " << query.lastError().text();
    }
    if (query.exec("CREATE INDEX IF NOT EXISTS idx_articles_titles ON Articles (title) "))
    {
        qDebug() << "Title Index Successfull";
    }
    else
    {
        qDebug() << "Title Index Failed: " << query.lastError().text();
    }
}

int64_t DbManager::getTodaysEpochSeconds()
{
    QDate currentDate = QDate::currentDate();
    QDateTime startOfDay(currentDate.startOfDay(QTimeZone::systemTimeZone()));
    return startOfDay.toSecsSinceEpoch();
}
int64_t DbManager::getYesterdaysEpochSeconds()
{
    QDate yesterday = QDate::currentDate().addDays(-1);
    QDateTime startOfYesterday(yesterday.startOfDay(QTimeZone::systemTimeZone()));
    return startOfYesterday.toSecsSinceEpoch();
}

std::vector<Article> DbManager::fetchFilteredAndSortedArticles(const std::string &filter, const std::string &sortOrder, const std::string &category, const std::string &source)
{
    QSqlQuery query(db);
    std::vector<Article> articles;
    int64_t todayStart = getTodaysEpochSeconds();
    int64_t yesterdayStart = getYesterdaysEpochSeconds();
    QString queryString("SELECT Articles.title,Articles.description,Articles.url,Articles.img_url,Articles.pub_date,Articles.guid,Articles.img_name "
                        "FROM Articles "
                        "INNER JOIN Sources on Sources.source_id = Articles.source_id "
                        "INNER JOIN Categories on Categories.category_id = Sources.category_id "
                        "WHERE Sources.source_name = :source AND Categories.category_name = :category ");
    if (filter == "Today")
    {
        queryString += "AND " + QString("pub_date >= %1 ").arg(QString::number(todayStart));
    }
    if (filter == "Yesterday")
    {
        queryString += "AND " + QString("pub_date >= %1 AND pub_date < %2 ").arg(QString::number(yesterdayStart), QString::number(todayStart));
    }
    if (sortOrder == "Date Ascending")
    {
        queryString += "ORDER BY pub_date ASC ";
    }
    if (sortOrder == "Date Descending")
    {
        queryString += "ORDER BY pub_date DESC ";
    }
    if (sortOrder == "Title")
    {
        queryString += "ORDER BY title ";
    }
    query.prepare(queryString);
    query.bindValue(":category", QString::fromStdString(category));
    query.bindValue(":source", QString::fromStdString(source));

    if (!query.exec())
    {
        qDebug() << "Fecth Article From DB Execution Failed: " << query.lastError().text();
        return articles;
    }
    while (query.next())
    {
        const std::string &title = query.value(0).toString().toStdString();
        const std::string &description = query.value(1).toString().toStdString();
        const std::string &url = query.value(2).toString().toStdString();
        const std::string &imgUrl = query.value(3).toString().toStdString();
        const std::chrono::system_clock::time_point &pubDate = fromEpochSeconds(query.value(4).toInt());
        const std::string &guid = query.value(5).toString().toStdString();
        const std::string &imgName = query.value(6).toString().toStdString();
        articles.push_back(Article(title, description, url, imgUrl, pubDate, guid, imgName, source, category));
    }
    return articles;
}

std::vector<Article> DbManager::fetchArticlesFromDB(const std::string &filter, const std::string &sortOrder, const std::string &searchText, const std::string &category, const std::string &source)
{
    QSqlQuery query(db);
    std::vector<Article> articles;
    int64_t todayStart = getTodaysEpochSeconds();
    int64_t yesterdayStart = getYesterdaysEpochSeconds();
    QString queryString("SELECT Articles.title,Articles.description,Articles.url,Articles.img_url,Articles.pub_date,Articles.guid,Articles.img_name "
                        "FROM Articles "
                        "INNER JOIN Sources on Sources.source_id = Articles.source_id "
                        "INNER JOIN Categories on Categories.category_id = Sources.category_id "
                        "WHERE Sources.source_name = :source AND Categories.category_name = :category ");
    if (filter == "" || filter == "Today")
    {
        queryString += "AND " + QString("pub_date >= %1 ").arg(QString::number(todayStart));
    }
    if (filter == "Yesterday")
    {
        queryString += "AND " + QString("pub_date >= %1 AND pub_date < %2 ").arg(QString::number(yesterdayStart), QString::number(todayStart));
    }
    if (!searchText.empty())
    {
        queryString += "AND (Articles.title LIKE :searchText OR Articles.description LIKE :searchText) ";
    }
    if (sortOrder == "Date Ascending")
    {
        queryString += "ORDER BY pub_date ASC ";
    }
    if (sortOrder == "" || sortOrder == "Date Descending")
    {
        queryString += "ORDER BY pub_date DESC ";
    }
    if (sortOrder == "Title")
    {
        queryString += "ORDER BY title ";
    }
    query.prepare(queryString);
    query.bindValue(":category", QString::fromStdString(category));
    query.bindValue(":source", QString::fromStdString(source));
    if (!searchText.empty())
    {
        query.bindValue(":searchText", QString("%%1%").arg(QString::fromStdString(searchText)));
    }
    if (!query.exec())
    {
        qDebug() << "Fecth Article From DB Execution Failed: " << query.lastError().text();
        return articles;
    }
    while (query.next())
    {
        const std::string &title = query.value(0).toString().toStdString();
        const std::string &description = query.value(1).toString().toStdString();
        const std::string &url = query.value(2).toString().toStdString();
        const std::string &imgUrl = query.value(3).toString().toStdString();
        const std::chrono::system_clock::time_point &pubDate = fromEpochSeconds(query.value(4).toInt());
        const std::string &guid = query.value(5).toString().toStdString();
        const std::string &imgName = query.value(6).toString().toStdString();
        articles.push_back(Article(title, description, url, imgUrl, pubDate, guid, imgName, source, category));
    }
    return articles;
}