#pragma once
#include "feedparser.h"
#include <QObject>
#include <QXmlStreamReader>

class QtFeedParser : public QObject, public FeedParser
{
public:
    explicit QtFeedParser(QObject *parent = nullptr);
    std::vector<Article> getArticleList(std::string_view feedData, std::string_view category, std::string_view source) override;

private:
    void parseItem(QXmlStreamReader &xml, Article &item);
    QString parseImageUrlFromMedia(QXmlStreamReader &xml);
    int64_t parseRfc822Date(const QString &datetime);
};