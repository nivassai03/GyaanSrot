#include "sourceinstalldialog.h"

SourceInstallDialog::SourceInstallDialog(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // tab widget
    tabWidget = new QTabWidget(this);

    // installed sources
    QWidget *installedPanel = new QWidget(this);
    QVBoxLayout *installedLayout = new QVBoxLayout(installedPanel);
    installedList = new QTreeWidget(installedPanel);
    installedList->setColumnCount(1);
    installedList->setHeaderHidden(true);
    installedLayout->addWidget(installedList);

    QHBoxLayout *installedButtonLayout = new QHBoxLayout();
    QPushButton *inUnselectAllButton = new QPushButton("Unselect All", installedPanel);
    QPushButton *inSelectAllButton = new QPushButton("Select All", installedPanel);
    uninstallButton = new QPushButton("Uninstall", installedPanel);
    installedButtonLayout->addWidget(inUnselectAllButton);
    installedButtonLayout->addWidget(inSelectAllButton);
    installedButtonLayout->addStretch();
    installedButtonLayout->addWidget(uninstallButton);

    installedLayout->addLayout(installedButtonLayout);
    tabWidget->addTab(installedPanel, "Installed");

    // available sources
    QWidget *availablePanel = new QWidget(this);
    QVBoxLayout *availableLayout = new QVBoxLayout(availablePanel);
    availableList = new QTreeWidget(availablePanel);
    availableList->setColumnCount(1);
    availableList->setHeaderHidden(true);
    availableLayout->addWidget(availableList);

    QHBoxLayout *availableButtonLayout = new QHBoxLayout();
    QPushButton *avUnselectAllButton = new QPushButton("Unselect All", availablePanel);
    QPushButton *avSelectAllButton = new QPushButton("Select All", availablePanel);
    installButton = new QPushButton("Install", availablePanel);
    availableButtonLayout->addWidget(avUnselectAllButton);
    availableButtonLayout->addWidget(avSelectAllButton);
    availableButtonLayout->addStretch();
    availableButtonLayout->addWidget(installButton);

    availableLayout->addLayout(availableButtonLayout);
    tabWidget->addTab(availablePanel, "Available");

    mainLayout->addWidget(tabWidget);

    closeButton = new QPushButton("Close", this);
    mainLayout->addWidget(closeButton, 0, Qt::AlignRight);

    setLayout(mainLayout);
    setMinimumSize(800, 600);

    // connections
    connect(closeButton, &QPushButton::clicked, this, &SourceInstallDialog::onClose);
    connect(inUnselectAllButton, &QPushButton::clicked, this, &SourceInstallDialog::onUnselectAll);
    connect(avUnselectAllButton, &QPushButton::clicked, this, &SourceInstallDialog::onUnselectAll);
    connect(inSelectAllButton, &QPushButton::clicked, this, &SourceInstallDialog::onSelectAll);
    connect(avSelectAllButton, &QPushButton::clicked, this, &SourceInstallDialog::onSelectAll);
    connect(availableList, &QTreeWidget::itemChanged, this, &SourceInstallDialog::onItemChanged);
    connect(installedList, &QTreeWidget::itemChanged, this, &SourceInstallDialog::onItemChanged);
    connect(installButton, &QPushButton::clicked, this, &SourceInstallDialog::onInstall);
    connect(uninstallButton, &QPushButton::clicked, this, &SourceInstallDialog::onUninstall);
}

void SourceInstallDialog::makeAvailableList(const std::vector<Category> &categories)
{
    availableList->clear();
    for (const auto &category : categories)
    {
        QTreeWidgetItem *categoryItem = new QTreeWidgetItem(availableList);
        categoryItem->setText(0, QString::fromStdString(category.getCategoryName()));
        categoryItem->setFlags(categoryItem->flags() | Qt::ItemIsUserCheckable);
        categoryItem->setCheckState(0, Qt::Unchecked);
        categoryItem->setExpanded(true);

        for (const auto &source : category.getSources())
        {
            QTreeWidgetItem *sourceItem = new QTreeWidgetItem(categoryItem);
            sourceItem->setText(0, QString::fromStdString(source.getSourceName()));
            sourceItem->setCheckState(0, Qt::Unchecked);
            sourceItem->setData(0, Qt::UserRole, QString::fromStdString(source.getSourceName()));
        }
    }
}
void SourceInstallDialog::makeInstalledList(const std::vector<Category> &categories)
{
    installedList->clear();
    for (const auto &category : categories)
    {
        QTreeWidgetItem *categoryItem = new QTreeWidgetItem(installedList);
        categoryItem->setText(0, QString::fromStdString(category.getCategoryName()));
        categoryItem->setFlags(categoryItem->flags() | Qt::ItemIsUserCheckable);
        categoryItem->setCheckState(0, Qt::Unchecked);
        categoryItem->setExpanded(true);

        for (const auto &source : category.getSources())
        {
            QTreeWidgetItem *sourceItem = new QTreeWidgetItem(categoryItem);
            sourceItem->setText(0, QString::fromStdString(source.getSourceName()));
            sourceItem->setCheckState(0, Qt::Unchecked);
            sourceItem->setData(0, Qt::UserRole, QString::fromStdString(source.getSourceName()));
        }
    }
}

void SourceInstallDialog::onClose()
{
    close();
}
void SourceInstallDialog::onSelectAll()
{
    QTreeWidget *list = tabWidget->currentWidget() == tabWidget->widget(0) ? installedList : availableList;
    for (int i = 0; i < list->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = list->topLevelItem(i);
        item->setCheckState(0, Qt::Checked);
        for (int j = 0; j < item->childCount(); ++j)
        {
            QTreeWidgetItem *childItem = item->child(j);
            childItem->setCheckState(0, Qt::Checked);
        }
    }
}
void SourceInstallDialog::onUnselectAll()
{

    QTreeWidget *list = tabWidget->currentWidget() == tabWidget->widget(0) ? installedList : availableList;
    for (int i = 0; i < list->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = list->topLevelItem(i);
        item->setCheckState(0, Qt::Unchecked);
        for (int j = 0; j < item->childCount(); ++j)
        {
            QTreeWidgetItem *childItem = item->child(j);
            childItem->setCheckState(0, Qt::Unchecked);
        }
    }
}

void SourceInstallDialog::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (item->childCount() > 0)
    {
        Qt::CheckState state = item->checkState(column);
        for (int i = 0; i < item->childCount(); ++i)
        {
            QTreeWidgetItem *childItem = item->child(i);
            childItem->setCheckState(column, state);
        }
    }
}
void SourceInstallDialog::onInstall()
{
    QList<QPair<QString, QString>> sourcesToInstall;
    QTreeWidget *list = tabWidget->currentWidget() == tabWidget->widget(0) ? installedList : availableList;
    for (int i = 0; i < list->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = list->topLevelItem(i);
        for (int j = 0; j < item->childCount(); ++j)
        {
            QTreeWidgetItem *childItem = item->child(j);
            if (childItem->checkState(0) == Qt::Checked)
            {
                sourcesToInstall.push_back({item->text(0), childItem->text(0)});
            }
        }
    }
    emit installSources(sourcesToInstall);
}
void SourceInstallDialog::onUninstall()
{
    QList<QPair<QString, QString>> sourcesToUninstall;
    QTreeWidget *list = tabWidget->currentWidget() == tabWidget->widget(0) ? installedList : availableList;
    for (int i = 0; i < list->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = list->topLevelItem(i);
        for (int j = 0; j < item->childCount(); ++j)
        {
            QTreeWidgetItem *childItem = item->child(j);
            if (childItem->checkState(0) == Qt::Checked)
            {
                sourcesToUninstall.push_back({item->text(0), childItem->text(0)});
            }
        }
    }
    emit uninstallSources(sourcesToUninstall);
}
