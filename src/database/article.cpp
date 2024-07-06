#include "article.h"
#include "../utils/util.h"

Article::Article(const std::string &title, const std::string &description, const std::string &url, const std::string &imgUrl) : title(title), description(description), url(url), imgUrl(imgUrl)
{
}

Article::Article(const Article &rhs)
{
    title = rhs.title;
    description = rhs.description;
    url = rhs.url;
    imgUrl = rhs.imgUrl;
    guid = rhs.guid;
    imgName = rhs.imgName;
    pubDate = rhs.pubDate;
    category = rhs.category;
    source = rhs.source;
}

Article &Article::operator=(const Article &rhs)
{

    this->title = rhs.title;
    this->description = rhs.description;
    this->url = rhs.url;
    this->imgUrl = rhs.imgUrl;
    this->guid = rhs.guid;
    this->imgName = rhs.imgName;
    this->pubDate = rhs.pubDate;
    this->category = rhs.category;
    this->source = rhs.source;
    return *this;
}

void Article::setTitle(const std::string &title)
{
    this->title = title;
}
void Article::setDescription(const std::string &description)
{
    this->description = description;
}
void Article::setUrl(const std::string &url)
{
    this->url = url;
}
void Article::setImgUrl(const std::string &imgUrl)
{
    this->imgUrl = imgUrl;
}
void Article::setGuid(const std::string &guid)
{
    this->guid = guid;
}

void Article::setPubDate(const std::chrono::system_clock::time_point &timepoint)
{
    this->pubDate = timepoint;
}

void Article::setImageName(const std::string &imageName)
{
    this->imgName = imageName;
}

void Article::setCategory(const std::string &category)
{
    this->category = category;
}

void Article::setCategory(std::string_view category)
{
    this->category = category;
}

void Article::setSource(std::string_view source)
{
    this->source = source;
}

void Article::setSource(const std::string &source)
{
    this->source = source;
}

std::string Article::getTitle() const
{
    return title;
}

std::string Article::getImageUrl() const
{
    return imgUrl;
}

std::string Article::getDescription() const
{
    return description;
}

std::string Article::getArticleUrl() const
{
    return url;
}

std::string Article::getImageName() const
{
    return imgName;
}

std::string Article::getGuid() const
{
    return guid;
}
std::chrono::system_clock::time_point Article::getPubDate() const
{
    return pubDate;
}
int64_t Article::getPubDateEpochSecs() const
{
    return fromTimePoint(pubDate);
}
std::string Article::getCategory() const
{
    return category;
}
std::string Article::getSource() const
{
    return source;
}