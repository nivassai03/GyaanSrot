#include "feedlist.h"

FeedList::FeedList(QWidget *parent) : QScrollArea(parent)
{
    containerWidget = new QWidget();
    layout = new QVBoxLayout(containerWidget);
    containerWidget->setLayout(layout);
    setWidget(containerWidget);
    setWidgetResizable(true);
}
void FeedList::addArticle(const Article &article)
{
    FeedItem *feeditem = new FeedItem(this, article);
    layout->addWidget(feeditem);
    containerWidget->adjustSize();
}
void FeedList::loadArticles(const std::vector<Article> &articles)
{
    for (const auto &article : articles)
    {
        addArticle(article);
    }
}