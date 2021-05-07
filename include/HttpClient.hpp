#ifndef __HTTPCLIENT_HPP__
#define __HTTPCLIENT_HPP__

#include <string>

/**
 *  Class implementing a very basic http client
 */
class HttpClient {
public:

    HttpClient(void);

    int sendHttpGetRequest(const std::string& url, std::string& response, std::string& content);

protected:
    int recv_http_response(int socket_fd, char* buffer, int buffer_size);
    int parse_http_response(const std::string &answer, std::string &http_response, std::string &http_content);

};

#endif
