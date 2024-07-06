#pragma once
#include <string_view>

class FeedFetcher
{
public:
    virtual std::string getFeedXmlData(std::string_view url) = 0;
};
