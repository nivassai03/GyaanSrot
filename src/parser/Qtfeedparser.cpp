#include "Qtfeedparser.h"
#include <QDebug>
#include "../utils/util.h"
#include <QDateTime>

QtFeedParser::QtFeedParser(QObject *parent) : QObject(parent) {}

std::vector<Article> QtFeedParser::getArticleList(std::string_view feedData, std::string_view category, std::string_view source)
{
    std::vector<Article> articles;
    QXmlStreamReader xml(feedData);

    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name().toString() == "item")
            {
                Article article;
                parseItem(xml, article);
                article.setImageName(titleToSlug(article.getTitle()));
                article.setCategory(category);
                article.setSource(source);
                articles.push_back(article);
            }
        }
    }
    if (xml.hasError())
    {
        qWarning() << "XML error:" << xml.errorString();
    }
    return articles;
}

int64_t QtFeedParser::parseRfc822Date(const QString &datetime)
{
    QDateTime dateTime = QDateTime::fromString(datetime, Qt::RFC2822Date);
    if (!dateTime.isValid())
    {
        return 0;
    }
    return dateTime.toSecsSinceEpoch();
}

void QtFeedParser::parseItem(QXmlStreamReader &xml, Article &item)
{
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "item"))
    {
        if (xml.tokenType() == QXmlStreamReader::StartElement)
        {
            if (xml.name().toString() == "title")
            {
                item.setTitle(xml.readElementText().toStdString());
            }
            else if (xml.name().toString() == "link")
            {
                item.setUrl(xml.readElementText().toStdString());
            }
            else if (xml.name().toString() == "description")
            {
                item.setDescription(xml.readElementText().toStdString());
            }
            else if (xml.name().toString() == "pubDate")
            {
                item.setPubDate(fromEpochSeconds(parseRfc822Date(xml.readElementText())));
            }
            else if (xml.qualifiedName().toString() == "media:content")
            {
                QString imgUrl;
                QXmlStreamAttributes attributes = xml.attributes();
                if (attributes.hasAttribute("url"))
                {
                    imgUrl = attributes.value("url").toString();
                }
                else
                {
                    imgUrl = "empty";
                }
                item.setImgUrl(imgUrl.toStdString());
            }
            else if (xml.name().toString() == "guid")
            {
                item.setGuid(xml.readElementText().toStdString());
            }
        }
        xml.readNext();
    }
}