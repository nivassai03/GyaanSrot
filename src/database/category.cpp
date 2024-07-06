#include "category.h"

Category::Category(std::string_view category) : categoryName(category) {}
void Category::addSource(const Source &source)
{
    sources.push_back(source);
}
std::string Category::getCategoryName() const
{
    return categoryName;
}
std::vector<Source> Category::getSources() const
{
    return sources;
}