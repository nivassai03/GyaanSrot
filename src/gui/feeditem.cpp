#include "feeditem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include "../utils/Qtdirutil.h"

FeedItem::FeedItem(QWidget *parent, const Article &art) : QWidget(parent), article(art)
{

    QtDirUtil util;
    title = QString::fromStdString(article.getTitle());
    description = QString::fromStdString(article.getDescription());
    articleUrl = QString::fromStdString(article.getArticleUrl());
    QString pixmapPath = QString::fromStdString(util.getThumbnailDirFullPath()).append("/default.jpg");
    qDebug() << pixmapPath << "\n";
    qDebug() << QFile::exists(pixmapPath) << "\n";
    thumbnail = QPixmap(pixmapPath);
    defaultThumbnail = QPixmap(pixmapPath);

    titleLabel = new QLabel(title, this);
    descriptionLabel = new ElidedLabel(description, this);
    descriptionLabel = new QLabel(description, this);
    thumbnailLabel = new QLabel(this);
    if (!thumbnail.isNull())
    {
        thumbnailLabel->setPixmap(thumbnail.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *contentLayout = new QHBoxLayout();

    contentLayout->addWidget(thumbnailLabel);
    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(descriptionLabel);
    contentLayout->addLayout(textLayout);
    mainLayout->addLayout(contentLayout);

    setLayout(mainLayout);
}