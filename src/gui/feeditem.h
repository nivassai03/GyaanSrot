#pragma once
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPixmap>
#include "../database/article.h"

class FeedItem : public QWidget
{
    Q_OBJECT

private:
    QString title;
    QString description;
    QString articleUrl;
    QPixmap thumbnail;
    QPixmap defaultThumbnail;
    Article article;

    QLabel *titleLabel;
    QLabel *descriptionLabel;
    QLabel *thumbnailLabel;

signals:
    void articleClicked(const Article &article);

public:
    FeedItem(QWidget *parent, const Article &art);
};