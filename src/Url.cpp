#define _CRT_SECURE_NO_WARNINGS
#include <Url.hpp>


/**
 *  Default constructor.
 */
Url::Url(void) :
    url(),
    protocol(),
    host(), 
    port(0),
    path()
{}

/**
 * Constructor providing parsing results of the given url - eg "http://192.168.178.19/status".
 * @param url_ the url string
 */
Url::Url(const std::string& url_) :
    url(url_),
    protocol(),
    host(),
    port(0),
    path()
{
    parseUrl(url, protocol, host, port, path);
}

/**
 * Get url string.
 * @return the url
 */
std::string Url::getUrl(void) {
    return url;
}

/**
 * Get protocol part of the url as string.
 * @return the protocol
 */
std::string Url::getProtocol(void) {
    return protocol;
}

/**
 * Get host part of the url as string.
 * @return the host name
 */
std::string Url::getHost(void) {
    return host;
}

/**
 * Get port number part of the url as integer.
 * @return the port number
 */
int Url::getPort(void) {
    return port;
}

/**
 * Get path part of the url as string.
 * @return the path
 */
std::string Url::getPath(void) {
    return path;
}

/**
 * Parse a given url string and extract protocol, host, port and path.
 * @param url input  - the url input string
 * @param protocol output - the protocol part of the url
 * @param host output - the host part of the url
 * @param port output - the port number part of the url
 * @param path output - the path part of the url
 * @return 0: if successful, -1: if unsuccesful
 */
int Url::parseUrl(const std::string& url, std::string& protocol, std::string& host, int& port, std::string& path) {
    std::string::size_type offs = 0;

    // parse protocol
    std::string::size_type i1 = url.find("http:", offs);
    if (i1 != std::string::npos) {
        protocol = "http";
        port = 80;
        offs = i1 + 4;
    }
    else {
        i1 = url.find("https:", offs);
        if (i1 != std::string::npos) {
            protocol = "https";
            port = 443;
            offs = i1 + 5;
        }
        else {
            return -1;
        }
    }

    // parse hostname
    std::string::size_type i2 = url.find("://", offs);
    std::string::size_type i3 = std::string::npos;
    if (i2 == offs) {
        offs = i2 + 3;
        i3 = url.find_first_of(":/", offs);
        if (i3 != std::string::npos) {
            host = url.substr(offs, i3 - offs);
            offs = i3;
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }

    // parse port
    std::string::size_type i4 = url.find(":", offs);
    if (i4 == offs) {
        offs = i4 + 1;
        int n = sscanf(url.c_str()+offs, "%d", &port);
        if (n != 1) {
            return -1;
        }
    }

    // parse path
    std::string::size_type i5 = url.find("/", offs);
    if (i5 != std::string::npos) {
        offs = i5;
        path = url.substr(i5);
    }
    else {
        path = "";
    }

    return 0;
}
