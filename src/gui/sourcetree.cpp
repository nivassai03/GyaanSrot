#include "sourcetree.h"
#include <QTreeWidgetItem>

SourceTree::SourceTree(QWidget *parent) : QTreeWidget(parent)
{
    setHeaderHidden(true);
    connect(this, &QTreeWidget::itemClicked, this, &SourceTree::handleItemClicked);
}

void SourceTree::setCategoryList(const std::vector<Category> &categories)
{
    this->categories = categories;
    populateTree();
}

void SourceTree::populateTree()
{
    clear();
    for (const auto &category : categories)
    {
        QTreeWidgetItem *categoryItem = new QTreeWidgetItem(this, QStringList() << QString::fromStdString(category.getCategoryName()));

        for (const auto &source : category.getSources())
        {
            QTreeWidgetItem *sourceItem = new QTreeWidgetItem(categoryItem, QStringList() << QString::fromStdString(source.getSourceName()));
            sourceItem->setData(0, Qt::UserRole, QString::fromStdString(category.getCategoryName()));
            sourceItem->setData(1, Qt::UserRole, QString::fromStdString(source.getSourceName()));
        }
    }
}

void SourceTree::handleItemClicked(QTreeWidgetItem *item, int column)
{
    if (item->parent())
    {
        category = item->data(0, Qt::UserRole).toString();
        source = item->data(1, Qt::UserRole).toString();
        emit sourceClicked(category, source);
    }
}
QString SourceTree::getCurrentCategory()
{
    return category;
}
QString SourceTree::getCurrentSource()
{
    return source;
}