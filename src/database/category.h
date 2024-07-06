#pragma once
#include "source.h"
#include <vector>
#include <string_view>

class Category
{

public:
    Category() = default;
    Category(std::string_view cateogry);
    void addSource(const Source &source);
    std::string getCategoryName() const;
    std::vector<Source> getSources() const;

private:
    std::string categoryName;
    std::vector<Source> sources;
};