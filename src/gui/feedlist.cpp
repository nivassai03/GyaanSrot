#include "feedlist.h"

FeedList::FeedList(QWidget *parent)
    : QWidget(parent), listWidget(new QListWidget(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(listWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    setLayout(layout);
}

void FeedList::clearArticles()
{
    listWidget->clear();
}

void FeedList::loadArticles(const std::vector<Article> &articles)
{
    clearArticles();
    for (const auto &article : articles)
    {
        QListWidgetItem *item = new QListWidgetItem(listWidget);
        FeedItem *feedItem = new FeedItem(listWidget, article);
        item->setSizeHint(feedItem->sizeHint());
        listWidget->addItem(item);
        listWidget->setItemWidget(item, feedItem);

        connect(feedItem, &FeedItem::articleClicked, this, &FeedList::articleClicked);
    }
}
