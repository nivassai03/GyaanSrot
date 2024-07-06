#pragma once
#include "../database/article.h"
#include <vector>
#include <string>
#include <string_view>

class FeedParser
{
public:
    virtual std::vector<Article> getArticleList(std::string_view feedData, std::string_view category, std::string_view source) = 0;
};