#include "feeditem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QColor>
#include "../utils/Qtdirutil.h"
#include "../utils/util.h"

FeedItem::FeedItem(QWidget *parent, const Article &art) : QFrame(parent), article(art)
{

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(10);

    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Plain);

    QtDirUtil util;
    title = QString::fromStdString(article.getTitle());
    description = QString::fromStdString(article.getDescription());
    articleUrl = QString::fromStdString(article.getArticleUrl());

    // default thumbnail
    defaultThumbnail = QPixmap(":/images/default.jpg");

    // thumbnail
    if (!article.getImageName().empty())
    {
        QString pixmapFilePath = QString::fromStdString(util.getImgDirFullPath(generateImageDir(article.getCategory(), article.getSource()))).append("/").append(article.getImageName()).append(".jpg");
        thumbnail = QPixmap(pixmapFilePath);
        if (thumbnail.isNull())
        {
            thumbnail = defaultThumbnail;
        }
    }
    else
    {
        thumbnail = defaultThumbnail;
    }

    titleLabel = new QLabel(title, this);
    titleLabel->setWordWrap(true);
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    titleLabel->setStyleSheet("font-weight:bold; font-size:16px;");

    // description label
    descriptionLabel = new ElidedLabel(description, this);
    descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    thumbnailLabel = new QLabel(this);
    if (!thumbnail.isNull())
    {
        thumbnailLabel->setPixmap(thumbnail);
    }

    contentLayout->addWidget(thumbnailLabel);
    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(descriptionLabel);
    contentLayout->addLayout(textLayout);
    mainLayout->addLayout(contentLayout);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setLayout(mainLayout);
}

void FeedItem::enterEvent(QEnterEvent *event)
{
    QPalette palette = this->palette();
    QColor highlightColor = palette.color(QPalette::Highlight);
    setStyleSheet(QString("background-color: %1;").arg(highlightColor.name()));
    QWidget::enterEvent(event);
}
void FeedItem::leaveEvent(QEvent *event)
{
    setStyleSheet("");
    QWidget::leaveEvent(event);
}

void FeedItem::mousePressEvent(QMouseEvent *event)
{
    emit articleClicked(articleUrl);
    QFrame::mousePressEvent(event);
}