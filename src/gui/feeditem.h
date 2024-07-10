#pragma once
#include <QWidget>
#include <QString>
#include <QLabel>
#include "elidedlabel.h"
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
    ElidedLabel *descriptionLabel;
    QLabel *thumbnailLabel;

signals:
    void articleClicked(const Article &article);

public:
    FeedItem(QWidget *parent, const Article &art);
};