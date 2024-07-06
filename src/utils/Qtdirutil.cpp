#include "Qtdirutil.h"
#include <QStandardPaths>
#include <string>
#include <QFileInfo>
#include <string_view>
#include <QDir>

QtDirUtil::QtDirUtil() : homeDirFullPath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)),
                         appDataDirFullPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)),
                         thumbnailDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).append("/.thumbnails")) {}

void QtDirUtil::initApplicationDirs()
{
    if (!QDir().exists(appDataDirFullPath))
    {
        QDir().mkdir(appDataDirFullPath);
    }
    if (!QDir().exists(thumbnailDir))
    {
        QDir().mkdir(thumbnailDir);
    }
}
void QtDirUtil::createDir(std::string_view dir)
{
    if (!QDir().exists(QString(dir.data())))
    {
        QDir().mkdir(QString(dir.data()));
    }
}
bool QtDirUtil::dirExists(std::string_view dir)
{
    return QDir().exists(QString(dir.data()));
}
std::string QtDirUtil::getHomeDir()
{
    return QFileInfo(homeDirFullPath).fileName().toStdString();
}
std::string QtDirUtil::getHomeDirFullPath()
{
    return homeDirFullPath.toStdString();
}
std::string QtDirUtil::getAppDataDir()
{
    return QFileInfo(appDataDirFullPath).fileName().toStdString();
}
std::string QtDirUtil::getThumbnailDir()
{
    return QFileInfo(thumbnailDir).fileName().toStdString();
}
std::string QtDirUtil::getThumbnailDirFullPath()
{
    return thumbnailDir.toStdString();
}
std::string QtDirUtil::getAppDataDirFullPath()
{
    return appDataDirFullPath.toStdString();
}
std::string QtDirUtil::getImgDirFullPath(std::string_view imgDir)
{
}
