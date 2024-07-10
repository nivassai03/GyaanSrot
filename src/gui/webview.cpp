#include "webview.h"

WebView::WebView(QWidget *parent) : QWebEngineView(parent)
{
    const QString &html = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Welcome to RSS Reader</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f9f9f9;
            margin: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        .container {
            padding: 20px;
            background-color: #fff;
            border-radius: 10px;
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
            text-align: center;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Welcome to RSS Reader</h1>
        <p>This is a simple RSS reader application.</p>
    </div>
</body>
</html>
)";
    displayCustomHtml(html);
}

void WebView::loadArticle(const QString &url)
{
    setUrl(QUrl(url));
}

void WebView::displayCustomHtml(const QString &html)
{
    setHtml(html);
}