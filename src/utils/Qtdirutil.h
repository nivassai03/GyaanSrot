#pragma once
#include "dirutil.h"
#include <QString>
#include <QObject>

class QtDirUtil : public QObject, public DirUtil
{
public:
    QtDirUtil(QObject *parent = nullptr);
    void initApplicationDirs() override;
    bool dirExists(std::string_view dir) override;
    void createDir(std::string_view dir) override;
    std::string getHomeDir() override;
    std::string getHomeDirFullPath() override;
    std::string getAppDataDir() override;
    std::string getThumbnailDir() override;
    std::string getThumbnailDirFullPath() override;
    std::string getAppDataDirFullPath() override;
    std::string getImgDirFullPath(std::string_view imgDir) override;

private:
    QString homeDirFullPath;
    QString appDataDirFullPath;
    QString thumbnailDir;
};