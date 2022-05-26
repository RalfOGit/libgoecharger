#define _CRT_SECURE_NO_WARNINGS
#include <Url.hpp>
using namespace libgoecharger;


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
    path(),
    query(),
    fragment()
{
    parseUrl(url, protocol, host, port, path, query, fragment);
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
 * Get query part of the url as string.
 * @return the path
 */
std::string Url::getQuery(void) {
    return query;
}

/**
 * Get fragment part of the url as string.
 * @return the path
 */
std::string Url::getFragment(void) {
    return fragment;
}

/**
 * Parse a given url string and extract protocol, host, port and path.
 * @param url input  - the url input string
 * @param protocol output - the protocol part of the url
 * @param host output     - the host part of the url
 * @param port output     - the port number part of the url
 * @param path output     - the path part of the url
 * @param query output    - the query part of the url
 * @param fragment output - the fragment part of the url
 * @return 0: if successful, -1: if unsuccesful
 */
int Url::parseUrl(const std::string& url, std::string& protocol, std::string& host, int& port, std::string& path, std::string& query, std::string& fragment) {
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
        int n = sscanf(url.c_str() + offs, "%d", &port);
        if (n != 1) {
            return -1;
        }
    }

    // parse path
    std::string::size_type i5 = url.find("/", offs);
    if (i5 != std::string::npos) {
        offs = i5;
        std::string::size_type i6 = url.find_first_of("?#", offs);
        if (i6 != std::string::npos) {
            path = url.substr(i5, i6 - offs);
            offs = i6;
        }
        else {
            path = url.substr(i5);
        }
    }
    else {
        path = "";
    }

    // parse query
    std::string::size_type i7 = url.find("?", offs);
    if (i7 != std::string::npos) {
        offs = i7 + 1;
        std::string::size_type i8 = url.find("#", offs);
        if (i8 != std::string::npos) {
            query = url.substr(i7, i8 - offs);
            offs = i8;
        }
        else {
            query = url.substr(i7);
        }
    }
    else {
        query = "";
    }

    // parse fragment
    std::string::size_type i9 = url.find("#", offs);
    if (i9 != std::string::npos) {
        offs = i9 + 1;
        fragment = url.substr(offs);
    }
    else {
        fragment = "";
    }

    // replace special characters in the path, query and fragment with their %hex url encoding; e.g. ' ' is replace by %20
    path     = replaceSpecialCharacters(path, '/');
    query    = replaceSpecialCharacters(query, '?');
    fragment = replaceSpecialCharacters(fragment, '#');

    return 0;
}

/**
 * Replace special characters in the query with their %hex url encoding; e.g. ' ' is replace by %20
 * @param url_component input - the input string
 * @param url_component_identifier input - the first character in the url component, i.e. '/', '?' or '#'
 * @return the input string, where all special characters are replaced
 */
std::string Url::replaceSpecialCharacters(const std::string& url_component, const std::string::value_type url_component_identifier) {
    std::string result;
    size_t offs = 0;

    // keep the first '/', '?' or '#' character
    if (url_component.length() > 1) {
        if (url_component[0] == url_component_identifier) {
            result.append(1, url_component_identifier);
            offs = 1;
        }
    }

    // loop over all characters and replace special chaacters with their %hex url encoding
    for (size_t i = offs; i < url_component.length(); ++i) {
        std::string::value_type c = url_component[i];

        // RFC 3986 defines the following characters to be allowed in url components:
        //   path component:      pchar    = unreserved / pct-encoded / sub-delims / ":" / "@"
        //   query component:     query    = *(pchar / "/" / "?")
        //   fragment component:  fragment = *(pchar / "/" / "?")
        bool unreserved = (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ||
                           c >= '0' && c <= '9' || c == '-' || c == '.' || c == '_' || c == '~');
        bool subdelims  = (c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' ||
                           c == '*' || c == '+' || c == ',' || c == ';' || c == '=');
        bool extra      = (c == ':' || c == '@');
        bool extra_qf   = (c == '/' || c == '?');

        // copy an allowed character to the result string
        if (unreserved || subdelims || extra || (url_component_identifier == '?' || url_component_identifier == '#') && extra_qf) {
            result.append(1, c);
        }
        // copy any pct-encoded character (i.e. "%" HEXDIG HEXDIG) to the result string
        else if (c == '%' && (i + 2) < url_component.length()) {
            std::string::value_type h1 = url_component[i + 1];
            std::string::value_type h2 = url_component[i + 2];
            if ((h1 >= 'a' && h1 <= 'f' || h1 >= 'A' && h1 <= 'F' || h1 >= '0' && h1 <= '9') &&
                (h2 >= 'a' && h2 <= 'f' || h2 >= 'A' && h2 <= 'F' || h2 >= '0' && h2 <= '9')) {
                result.append(1, c);
                result.append(1, h1);
                result.append(1, h2);
                i = i + 2;
            }
        }
        // pct-encode any other character and append it to the result string
        else {
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "%02x", c);
            result.append(1, '%');
            result.append(buffer);
        }
    }
    return result;
}
