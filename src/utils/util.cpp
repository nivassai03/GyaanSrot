#include "util.h"
#include <sstream>
#include <iomanip>
#include <sstream>

std::string titleToSlug(const std::string &title, char del, int len)
{
    std::stringstream ss;
    int cnt{};
    for (auto it : title)
    {
        unsigned char ch = static_cast<unsigned char>(it);
        if (std::isalnum(ch))
        {
            ss.put(std::tolower(ch));
        }
        else if (std::isspace(ch))
        {
            ss.put(del);
        }
        cnt++;
        if (cnt == len)
        {
            break;
        }
    }
    return ss.str();
}

std::string generateImageDir(const std::string &m_category, const std::string &m_name)
{
    std::stringstream ss;
    ss << '.';
    for (char c : m_name)
    {
        if (c != ' ')
        {
            ss.put(std::tolower(static_cast<unsigned char>(c)));
        }
        else
        {
            ss << "_";
        }
    }
    ss << "_";
    for (char c : m_category)
    {
        if (c != ' ')
        {
            ss.put(std::tolower(static_cast<unsigned char>(c)));
        }
        else
        {
            ss << "_";
        }
    }
    return ss.str();
}
std::string generateImageDir(std::string_view category, std::string_view source)
{
    std::stringstream ss;
    ss << '.';
    for (char c : source)
    {
        if (c != ' ')
        {
            ss.put(std::tolower(static_cast<unsigned char>(c)));
        }
        else
        {
            ss << "_";
        }
    }
    ss << "_";
    for (char c : category)
    {
        if (c != ' ')
        {
            ss.put(std::tolower(static_cast<unsigned char>(c)));
        }
        else
        {
            ss << "_";
        }
    }
    return ss.str();
}
std::chrono::system_clock::time_point fromEpochSeconds(int64_t epochtime)
{
    return std::chrono::system_clock::time_point{std::chrono::seconds{epochtime}};
}
int64_t fromTimePoint(const std::chrono::system_clock::time_point &tp)
{
    return std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
}
