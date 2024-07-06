#include "Qtfeedfetcher.h"
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QEventLoop>
#include <QUrl>
#include <QDebug>

QtFeedFetcher::QtFeedFetcher(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

std::string QtFeedFetcher::getFeedXmlData(std::string_view url)
{
    const QUrl feedUrl(url.data());
    QNetworkRequest request(feedUrl);

    QNetworkReply *reply = manager->get(request);

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    std::string data;

    if (reply->error() == QNetworkReply::NoError)
    {
        data = reply->readAll().toStdString();
    }
    else
    {
        qDebug() << "Network Error\n";
    }
    reply->deleteLater();
    return data;
}