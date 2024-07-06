#pragma once
#include <string>
#include <string_view>

class DirUtil
{
public:
    virtual void initApplicationDirs() = 0;
    virtual bool dirExists(std::string_view dir) = 0;
    virtual void createDir(std::string_view dir) = 0;
    virtual std::string getHomeDir() = 0;
    virtual std::string getHomeDirFullPath() = 0;
    virtual std::string getAppDataDir() = 0;
    virtual std::string getAppDataDirFullPath() = 0;
    virtual std::string getThumbnailDir() = 0;
    virtual std::string getThumbnailDirFullPath() = 0;
    virtual std::string getImgDirFullPath(std::string_view imgDir) = 0;
};