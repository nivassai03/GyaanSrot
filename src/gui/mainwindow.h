#pragma once

#include <QObject>
#include <QMainWindow>
#include <QWidget>

class MainWindow : public QMainWindow
{

    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
};