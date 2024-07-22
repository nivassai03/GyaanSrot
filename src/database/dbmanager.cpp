#include "dbmanager.h"
#include <QDebug>
#include <QDateTime>
#include <QSqlError>
#include <QTimeZone>
#include "../utils/Qtdirutil.h"
#include "../utils/util.h"
#include <QSqlQuery>

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
    createInstalledSourcesTable();
    addInitialCategoriesAndSources();
}

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

void DbManager::createInstalledSourcesTable()
{
    QSqlQuery query(db);
    if (!query.exec("CREATE TABLE IF NOT EXISTS InstalledSources ("
                    "source_id INTEGER PRIMARY KEY,"
                    "category_id INTEGER,"
                    "install_date INTEGER NOT NULL,"
                    "FOREIGN KEY (source_id) REFERENCES Sources(source_id),"
                    "FOREIGN KEY (category_id) REFERENCES Categories(category_id))"))
    {
        qDebug() << "Error Creating Installed Sources Table: " << query.lastError().text();
    }
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

std::vector<Category> DbManager::fetchInstalledCategoriesAndSources()
{
    QSqlQuery categoryQuery(db);
    std::vector<Category> categories;
    if (!categoryQuery.exec("SELECT DISTINCT Categories.category_name,Categories.category_id "
                            "FROM Categories "
                            "INNER JOIN InstalledSources ON Categories.category_id = InstalledSources.category_id "))
    {
        qDebug() << "Failed to fetch installed categories: " << categoryQuery.lastError().text();
    }

    while (categoryQuery.next())
    {
        const std::string &category_name = categoryQuery.value(0).toString().toStdString();
        int categoryId = categoryQuery.value(1).toInt();

        Category category(category_name);

        QSqlQuery sourceQuery(db);
        sourceQuery.prepare("SELECT Sources.source_name,Sources.source_url "
                            "FROM Sources "
                            "INNER JOIN InstalledSources ON Sources.source_id = InstalledSources.source_id "
                            "WHERE InstalledSources.category_id = :categoryId ");

        sourceQuery.bindValue(":categoryId", categoryId);

        if (!sourceQuery.exec())
        {
            qDebug() << "Failed to Fetch Sources: " << sourceQuery.lastError().text();
        }
        while (sourceQuery.next())
        {
            const std::string &source_name = sourceQuery.value(0).toString().toStdString();
            const std::string &source_url = sourceQuery.value(1).toString().toStdString();
            category.addSource(Source(source_name, source_url));
        }
        categories.push_back(category);
    }
    return categories;
}

std::vector<Category> DbManager::fetchNotInstalledCategoriesAndSources()
{
    QSqlQuery query(db);
    QString queryString = "SELECT Categories.category_id,Categories.category_name,Sources.source_name,Sources.source_url "
                          "FROM Sources "
                          "LEFT JOIN InstalledSources ON Sources.source_id = InstalledSources.source_id "
                          "LEFT JOIN Categories ON Categories.category_id = Sources.category_id "
                          "WHERE InstalledSources.source_id IS NULL";

    if (!query.exec(queryString))
    {
        qDebug() << "Fetch Not Installed Sources Failed: " << query.lastError().text();
    }
    QHash<int, Category> categoryMap;
    while (query.next())
    {
        int categoryId = query.value(0).toInt();
        const std::string &category_name = query.value(1).toString().toStdString();
        const std::string &source_name = query.value(2).toString().toStdString();
        const std::string &source_url = query.value(3).toString().toStdString();
        if (!categoryMap.contains(categoryId))
        {
            Category category(category_name);
            categoryMap.insert(categoryId, category);
        }
        Source source(source_name, source_url);
        categoryMap[categoryId].addSource(source);
    }
    const QList<Category> &QCategories = categoryMap.values();
    return std::vector<Category>(QCategories.constBegin(), QCategories.constEnd());
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
std::vector<Article> DbManager::fetchArticlesFromDB(const std::string &filter, const std::string &sortOrder, const std::string &searchText, const std::string &category, const std::string &source)
{
    QSqlQuery query(db);
    std::vector<Article> articles;
    QString queryString("SELECT Articles.title,Articles.description,Articles.url,Articles.img_url,Articles.pub_date,Articles.guid,Articles.img_name "
                        "FROM Articles "
                        "INNER JOIN Sources on Sources.source_id = Articles.source_id "
                        "INNER JOIN Categories on Categories.category_id = Sources.category_id "
                        "WHERE Sources.source_name = :source AND Categories.category_name = :category ");
    if (filter == "" || filter == "Today")
    {
        queryString += "AND " + QString("pub_date >= %1 ").arg(QString::number(getTodaysEpochSeconds()));
    }
    if (filter == "Yesterday")
    {
        queryString += "AND " + QString("pub_date >= %1 AND pub_date < %2 ").arg(QString::number(getYesterdaysEpochSeconds()), QString::number(getTodaysEpochSeconds()));
    }
    if (filter == "This Week")
    {
        queryString += "AND " + QString("pub_date >= %1 ").arg(QString::number(getStartOfWeekEpochSeconds()));
    }
    if (filter == "This Month")
    {
        queryString += "AND " + QString("pub_date >= %1 ").arg(QString::number(getStartOfMonthEpochSeconds()));
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
void DbManager::installSource(const std::string &category, const std::string &source)
{
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT Sources.source_id,Sources.category_id "
                       "FROM Sources "
                       "INNER JOIN Categories on Sources.category_id = Categories.category_id "
                       "WHERE Sources.source_name = :source AND Categories.category_name = :category ");
    checkQuery.bindValue(":source", QString::fromStdString(source));
    checkQuery.bindValue(":category", QString::fromStdString(category));
    if (!checkQuery.exec())
    {
        qDebug() << "Queyring Installed Sources Failed: " << checkQuery.lastError().text();
    }
    if (!checkQuery.next())
    {
        qDebug() << "Source Not Found in Source Table: " << checkQuery.lastError().text();
    }

    int sourceId = checkQuery.value(0).toInt();
    int categoryId = checkQuery.value(1).toInt();

    QSqlQuery installQuery(db);
    installQuery.exec("BEGIN TRANSACTION");
    installQuery.prepare("INSERT OR IGNORE INTO InstalledSources (source_id,category_id,install_date) VALUES (:source_id,:category_id,:install_date)");
    installQuery.bindValue(":source_id", sourceId);
    installQuery.bindValue(":category_id", categoryId);
    installQuery.bindValue(":install_date", (qint64)getCurrentEpochSeconds());
    if (!installQuery.exec())
    {
        qDebug() << "Failed to install Source: " << installQuery.lastError().text();
    }

    installQuery.exec("END TRANSACTION");
}

void DbManager::uninstallSource(const std::string &category, const std::string &source)
{
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT Sources.source_id "
                       "FROM Sources "
                       "INNER JOIN Categories ON Sources.category_id = Categories.category_id "
                       "WHERE Categories.category_name = :category AND Sources.source_name = :source ");

    checkQuery.bindValue(":category", QString::fromStdString(category));
    checkQuery.bindValue(":source", QString::fromStdString(source));

    if (!checkQuery.exec())
    {
        qDebug() << "Failed to fetch Sources: " << checkQuery.lastError().text();
    }
    if (!checkQuery.next())
    {
        qDebug() << "Source not found in Sources: " << checkQuery.lastError().text();
    }
    int sourceId = checkQuery.value(0).toInt();

    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM InstalledSources "
                        "WHERE InstalledSources.source_id = :sourceId ");
    deleteQuery.bindValue(":sourceId", sourceId);
    if (!deleteQuery.exec())
    {
        qDebug() << "Deleting Row Failed: " << deleteQuery.lastError().text();
    }
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

int64_t DbManager::getStartOfWeekEpochSeconds()
{
    QDate currentDate = QDate::currentDate();
    int daysToStartOfWeek = currentDate.dayOfWeek() - 1;
    QDate startOfWeek = currentDate.addDays(-daysToStartOfWeek);
    QDateTime startOfWeekDateTime(startOfWeek.startOfDay(QTimeZone::systemTimeZone()));
    return startOfWeekDateTime.toSecsSinceEpoch();
}
int64_t DbManager::getStartOfMonthEpochSeconds()
{
    QDate currentDate = QDate::currentDate();
    QDate startOfMonth(currentDate.year(), currentDate.month(), 1);
    QDateTime startOfMonthDateTime(startOfMonth.startOfDay(QTimeZone::systemTimeZone()));
    return startOfMonthDateTime.toSecsSinceEpoch();
}

int64_t DbManager::getCurrentEpochSeconds()
{
    return QDateTime::currentDateTime().currentSecsSinceEpoch();
}
