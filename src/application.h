#include "network/feedfetcher.h"
#include "network/Qtfeedfetcher.h"
#include "parser/Qtfeedparser.h"
#include "utils/Qtdirutil.h"
#include <string_view>

class Application : public QObject
{
public:
    Application(QObject *parent = nullptr);
    void start();
    int64_t parseRfc822Date(const std::string &datetime);

private:
    FeedFetcher *fetcher;
    FeedParser *parser;
    DirUtil *util;
};