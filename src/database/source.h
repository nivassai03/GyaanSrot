#pragma once
#include <string>
#include <string_view>

class Source
{
public:
    Source() = default;
    Source(std::string_view sourceName, std::string_view sourceUrl);
    Source(const Source &rhs);
    Source &operator=(const Source &rhs);
    void setSourceName(std::string_view sourceName);
    void setSourceUrl(std::string_view sourceUrl);
    void installSource();
    void uninstallSource();
    std::string getSourceName();
    std::string getSourceUrl();
    bool isSourceInstalled();

private:
    std::string sourceName;
    std::string sourceUrl;
    bool isInstalled = false;
};