#include "Qtfeedimagedownloader.h"
#include <QEventLoop>
#include <QNetworkReply>
#include "../utils/Qtdirutil.h"
#include "../utils/util.h"
#include <QNetworkRequest>
#include <QDir>
#include <QImage>

QtFeedImageDownloader::QtFeedImageDownloader(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

void QtFeedImageDownloader::downloadImage(const std::string &url, const std ::string &dir, const std::string &fileName)
{

    if (url.empty())
    {
        return;
    }
    QtDirUtil util;
    QString imgDir = QString::fromStdString(util.getImgDirFullPath(dir));
    QString fileFullPath = imgDir + QString("/") + QString(fileName.data()) + QString(".jpg");
    if (QFile().exists(fileFullPath))
    {
        return;
    }

    QNetworkRequest request(QUrl(QString::fromStdString(url)));
    QNetworkReply *reply = manager->get(request);

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    QByteArray imgData;

    if (reply->error() == QNetworkReply::NoError)
    {
        imgData = reply->readAll();
        saveToDisk(fileFullPath, imgData);
    }
    reply->deleteLater();
}

void QtFeedImageDownloader::saveToDisk(const QString &fileFullPath, const QByteArray &imgData)
{
    if (imgData.isEmpty())
    {
        return;
    }
    QFileInfo fileInfo(fileFullPath);

    if (!fileInfo.exists())
    {
        QDir().mkdir(fileInfo.dir().path());
    }
    if (!QFile().exists(fileFullPath))
    {
        QImage image;
        image.loadFromData(imgData);
        QImage thumbnail = image.scaled(150, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        thumbnail.save(fileFullPath);
    }
}

void QtFeedImageDownloader::downloadAllImages(const std::vector<Article> &articles, const std::string &category, const std::string &source)
{
    const std::string &dir = generateImageDir(category, source);
    for (const auto &article : articles)
    {
        downloadImage(article.getImageUrl(), dir, article.getImageName());
    }
}