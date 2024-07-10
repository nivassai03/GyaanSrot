#pragma once
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include "feeditem.h"
#include "../database/article.h"

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
    void loadArticles(const std::vector<Article> &articles);
};