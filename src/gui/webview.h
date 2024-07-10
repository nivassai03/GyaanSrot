#pragma once

#include <QWebEngineView>

class WebView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit WebView(QWidget *parent = nullptr);
    void displayCustomHtml(const QString &html);

public slots:
    void loadArticle(const QString &url);
};
