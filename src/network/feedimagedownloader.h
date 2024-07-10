#pragma once

#include <string>
#include <vector>
#include "../database/article.h"

class FeedImageDownloader
{
public:
    virtual void downloadImage(const std::string &url, const std::string &dir, const std::string &fileName) = 0;
    virtual void downloadAllImages(const std::vector<Article> &articles, const std::string &category, const std::string &source) = 0;
};