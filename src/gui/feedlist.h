#pragma once
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "feeditem.h"
#include "../database/article.h"

class FeedList : public QScrollArea
{
    Q_OBJECT

private:
    QWidget *containerWidget;
    QVBoxLayout *layout;
    void addArticle(const Article &article);

public:
    FeedList(QWidget *parent = nullptr);
    void loadArticles(const std::vector<Article> &articles);
};