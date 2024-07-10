#pragma once
#include <QWidget>
#include <QString>
#include <QEnterEvent>
#include <QLabel>
#include "elidedlabel.h"
#include <QFrame>
#include <QMouseEvent>
#include <QPixmap>
#include "../database/article.h"

class FeedItem : public QFrame
{
    Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

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
    void articleClicked(const QString &article);

public:
    FeedItem(QWidget *parent, const Article &art);
};