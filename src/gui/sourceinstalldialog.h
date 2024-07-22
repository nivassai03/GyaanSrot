#pragma once

#include <QDialog>
#include <QTreeWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QObject>
#include "../database/category.h"

class SourceInstallDialog : public QDialog
{
    Q_OBJECT

public:
    SourceInstallDialog(QWidget *parent = nullptr);
    void makeAvailableList(const std::vector<Category> &categories);
    void makeInstalledList(const std::vector<Category> &categories);

private slots:
    void onClose();
    void onSelectAll();
    void onUnselectAll();
    void onItemChanged(QTreeWidgetItem *item, int column);
    void onInstall();
    void onUninstall();

signals:
    void installSources(const QList<QPair<QString, QString>> &sourcesToInstall);
    void uninstallSources(const QList<QPair<QString, QString>> &sourcesToUninstall);

private:
    QTabWidget *tabWidget;
    QTreeWidget *availableList;
    QTreeWidget *installedList;
    QPushButton *closeButton;
    QPushButton *installButton;
    QPushButton *uninstallButton;
    QPushButton *selectAllButton;
    QPushButton *unselectAllButton;
};
