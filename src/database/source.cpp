#include "source.h"

Source::Source(std::string_view name, std::string_view url) : sourceName(name), sourceUrl(url) {}

Source::Source(const Source &rhs)
{
    this->sourceName = rhs.sourceName;
    this->sourceUrl = rhs.sourceUrl;
    this->isInstalled = rhs.isInstalled;
}

Source &Source::operator=(const Source &rhs)
{
    if (this != &rhs)
    {
        this->sourceName = rhs.sourceName;
        this->sourceUrl = rhs.sourceUrl;
        this->isInstalled = rhs.isInstalled;
    }
    return *this;
}
void Source::setSourceName(std::string_view sourceName)
{
    this->sourceName = sourceName;
}

void Source::setSourceUrl(std::string_view sourceUrl)
{
    this->sourceUrl = sourceUrl;
}
void Source::installSource()
{
    isInstalled = true;
}
void Source::uninstallSource()
{
    isInstalled = false;
}
std::string Source::getSourceName()
{
    return sourceName;
}
std::string Source::getSourceUrl()
{
    return sourceUrl;
}
bool Source::isSourceInstalled()
{
    return isInstalled;
}
