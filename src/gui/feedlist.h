#pragma once
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include "feeditem.h"
#include "../database/article.h"

class FilterSelector : public QWidget
{

    Q_OBJECT
public:
    explicit FilterSelector(QWidget *parent);
    QString getFilter();
    QString getSortOrder();
    QString getSearchText();

signals:
    void filterChanged(const QString &filter);
    void sortChanged(const QString &sortorder);
    void searchTextChanged(const QString &searchText);
    void searchTextCleared();

private slots:
    void onFilterChanged();
    void onSortChanged();
    void onSearchTextChanged();
    void onSearchTextCleared();

private:
    QString filter = "";
    QString sortOrder = "";
    QString searchText = "";
    QComboBox *filterComboBox;
    QComboBox *sortComboBox;
    QLineEdit *searchLineEdit;
    QLabel *filterLabel;
    QLabel *sortLabel;
};

class FeedList : public QWidget
{
    Q_OBJECT

private:
    QListWidget *listWidget;
    void clearArticles();

signals:
    void articleClicked(const QString &url);

public:
    FeedList(QWidget *parent = nullptr);
    FilterSelector *filterSelector;
    QString getFilter();
    QString getSortOrder();
    QString getSearchText();
    void loadArticles(const std::vector<Article> &articles);
};