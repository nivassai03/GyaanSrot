#include "webview.h"

WebView::WebView(QWidget *parent) : QWebEngineView(parent)
{
    this->setUrl(QUrl("qrc:/html/default.html"));
}

void WebView::loadArticle(const QString &url)
{
    setUrl(QUrl(url));
}

void WebView::displayCustomHtml(const QString &html)
{
    setHtml(html);
}