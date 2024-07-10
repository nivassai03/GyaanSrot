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
    void setCategoryList(const std::vector<Category> &categories);

signals:
    void sourceClicked(const std::string &category, const std::string &source);

private slots:
    void handleItemClicked(QTreeWidgetItem *item, int column);

private:
    void populateTree();
    std::vector<Category> categories;
};