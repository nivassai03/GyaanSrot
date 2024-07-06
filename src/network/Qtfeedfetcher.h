#pragma once
#include "feedfetcher.h"
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>

class QtFeedFetcher : public QObject, public FeedFetcher
{
public:
    explicit QtFeedFetcher(QObject *parent = nullptr);
    std::string getFeedXmlData(std::string_view url) override;

private:
    QNetworkAccessManager *manager;
};
