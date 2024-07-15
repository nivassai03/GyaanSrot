#include "feedlist.h"
#include <QHBoxLayout>

FilterSelector::FilterSelector(QWidget *parent) : QWidget(parent)
{
    filterLabel = new QLabel("Filter", this);
    filterComboBox = new QComboBox(this);
    filterComboBox->addItem("");
    filterComboBox->addItem("Today");
    filterComboBox->addItem("Yesterday");
    filterComboBox->addItem("This Week");
    filterComboBox->addItem("This Month");
    filterComboBox->addItem("All Articles");

    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Search....");
    searchLineEdit->setClearButtonEnabled(true);

    sortLabel = new QLabel("Sort", this);
    sortComboBox = new QComboBox(this);
    sortComboBox->addItem("");
    sortComboBox->addItem("Date Ascending");
    sortComboBox->addItem("Date Descending");
    sortComboBox->addItem("Title");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(15, 5, 15, 0);
    layout->addWidget(filterLabel);
    layout->addWidget(filterComboBox);
    layout->addSpacing(15);
    layout->addWidget(searchLineEdit);
    layout->addStretch();
    layout->addWidget(sortLabel);
    layout->addWidget(sortComboBox);

    connect(filterComboBox, &QComboBox::currentTextChanged, this, &FilterSelector::onFilterChanged);
    connect(sortComboBox, &QComboBox::currentTextChanged, this, &FilterSelector::onSortChanged);
    connect(searchLineEdit, &QLineEdit::returnPressed, this, &FilterSelector::onSearchTextChanged);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &FilterSelector::onSearchTextCleared);
}

void FilterSelector::onFilterChanged()
{
    filter = filterComboBox->currentText();
    emit filterChanged(filter);
}
void FilterSelector::onSortChanged()
{
    sortOrder = sortComboBox->currentText();
    emit sortChanged(sortComboBox->currentText());
}
void FilterSelector::onSearchTextChanged()
{
    searchText = searchLineEdit->text();
    emit searchTextChanged(searchText);
}
void FilterSelector::onSearchTextCleared()
{
    searchText = searchLineEdit->text();
    if (searchText.isEmpty())
    {
        emit searchTextCleared();
    }
}

QString FilterSelector::getFilter()
{
    return filter;
}
QString FilterSelector::getSortOrder()
{
    return sortOrder;
}
QString FilterSelector::getSearchText()
{
    return searchText;
}

FeedList::FeedList(QWidget *parent)
    : QWidget(parent), listWidget(new QListWidget(this)), filterSelector(new FilterSelector(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(filterSelector);
    layout->addWidget(listWidget);
    layout->setContentsMargins(0, 0, 0, 0);
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

QString FeedList::getFilter()
{
    return filterSelector->getFilter();
}
QString FeedList::getSortOrder()
{
    return filterSelector->getSortOrder();
}
QString FeedList::getSearchText()
{
    return filterSelector->getSearchText();
}