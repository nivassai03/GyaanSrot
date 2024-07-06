#pragma once
#include <string>
#include <chrono>

class Article
{
public:
    Article() = default;
    Article(const std::string &title, const std::string &description, const std::string &url, const std::string &imgUrl);
    Article(const Article &rhs);
    Article &operator=(const Article &rhs);
    void setTitle(const std::string &title);
    void setDescription(const std::string &description);
    void setUrl(const std::string &url);
    void setImgUrl(const std::string &imgUrl);
    void setGuid(const std::string &guid);
    void setPubDate(const std::chrono::system_clock::time_point &timepoint);
    void setImageName(const std::string &imageName);
    void setCategory(const std::string &category);
    void setCategory(std::string_view category);
    void setSource(const std::string &source);
    void setSource(std::string_view source);
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getImageUrl() const;
    std::string getArticleUrl() const;
    std::string getImageName() const;
    std::string getGuid() const;
    std::chrono::system_clock::time_point getPubDate() const;
    int64_t getPubDateEpochSecs() const;
    std::string getCategory() const;
    std::string getSource() const;

private:
    std::string title;
    std::string description;
    std::string url;
    std::string imgUrl;
    std::string guid;
    std::chrono::system_clock::time_point pubDate;
    std::string imgName;
    std::string category;
    std::string source;
};
