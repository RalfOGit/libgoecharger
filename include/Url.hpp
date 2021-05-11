#ifndef __URL_HPP__
#define __URL_HPP__

#include <string>

/**
 *  Class implementing url parsing.
 */
class Url {
public:

    Url(void);
    Url(const std::string& url);

    std::string getUrl(void);
    std::string getProtocol(void);
    std::string getHost(void);
    int         getPort(void);
    std::string getPath(void);

    static int parseUrl(const std::string& url, std::string& protocol, std::string& host, int& port, std::string& path);

private:
    std::string url;
    std::string protocol;
    std::string host;
    int         port;
    std::string path;
};

#endif
