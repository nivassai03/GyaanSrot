#pragma once
#include <QTreeWidget>
#include <QWidget>
#include <vector>
#include "../database/category.h"

class SourceTree : public QTreeWidget
{
    Q_OBJECT
public:
    SourceTree(QWidget *parent = nullptr);
    QString getCurrentCategory();
    QString getCurrentSource();
    void setCategoryList(const std::vector<Category> &categories);

signals:
    void sourceClicked(const QString &category, const QString &source);

private slots:
    void handleItemClicked(QTreeWidgetItem *item, int column);

private:
    void populateTree();
    QString category = "";
    QString source = "";
    std::vector<Category> categories;
};