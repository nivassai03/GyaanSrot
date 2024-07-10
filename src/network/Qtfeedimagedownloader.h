#pragma once
#include "feedimagedownloader.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QObject>
#include "../database/article.h"

class QtFeedImageDownloader : public QObject, public FeedImageDownloader
{

    Q_OBJECT
private:
    QNetworkAccessManager *manager;
    void saveToDisk(const QString &fileFullPath, const QByteArray &imageData);

public:
    explicit QtFeedImageDownloader(QObject *parent = nullptr);
    void downloadAllImages(const std::vector<Article> &articles, const std::string &category, const std::string &source) override;
    void downloadImage(const std::string &url, const std::string &dir, const std::string &fileName) override;
};