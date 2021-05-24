#ifndef __LIBGOECHARGER_HTTPCLIENT_HPP__
#define __LIBGOECHARGER_HTTPCLIENT_HPP__

#include <string>

namespace libgoecharger {

    /**
     *  Class implementing a very basic http client.
     */
    class HttpClient {
    public:

        HttpClient(void);

        int sendHttpGetRequest(const std::string& url, std::string& response, std::string& content);
        int sendHttpPutRequest(const std::string& url, std::string& response, std::string& content);

    protected:

        int connect_to_server(const std::string& url, std::string& host, std::string& path);
        int communicate_with_server(const int socket_fd, const std::string& request, std::string& response, std::string& content);
        size_t recv_http_response(int socket_fd, char* buffer, int buffer_size);
        int    parse_http_response(const std::string& answer, std::string& http_response, std::string& http_content);
        int    get_http_return_code(char* buffer, size_t buffer_size);
        size_t get_content_length(char* buffer, size_t buffer_size);
        size_t get_content_offset(char* buffer, size_t buffer_size);
    };

}   // namespace libgoecharger

#endif
