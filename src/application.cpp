#include "application.h"
#include "utils/util.h"
#include "database/article.h"
#include "database/dbmanager.h"
#include "database/category.h"

Application::Application(QObject *parent)
    : QObject(parent),
      fetcher(new QtFeedFetcher(this)),
      parser(new QtFeedParser(this)),
      util(new QtDirUtil)
{
}

int64_t Application::parseRfc822Date(const std::string &datetime)
{
    QDateTime dateTime = QDateTime::fromString(QString::fromStdString(datetime), Qt::RFC2822Date);
    if (!dateTime.isValid())
    {
        return 0;
    }
    return dateTime.toSecsSinceEpoch();
}
void Application::start()
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Article>>> articles;
    DbManager manager("data.db");
    std::vector<Category> categories = manager.fetchCategoriesAndSources();
    for (auto category : categories)
    {
        for (auto source : category.getSources())
        {
            std::vector<Article> articleList = parser->getArticleList(fetcher->getFeedXmlData(source.getSourceUrl()), category.getCategoryName(), source.getSourceName());
            articles[category.getCategoryName()][source.getSourceName()] = articleList;
        }
    }
    manager.insertArticles(articles);
}