#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <Winsock2.h>
#include <Ws2tcpip.h>
#define poll(a, b, c)  WSAPoll((a), (b), (c))
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#endif

#include <HttpClient.hpp>
#include <Url.hpp>


/**
 *  Convert an ipv4 string to an ipv4 binary address
 */
static struct in_addr toInAddress(const std::string& ipv4_address) {
    struct in_addr addr;
    memset(&addr, 0, sizeof(addr));
    if (inet_pton(AF_INET, ipv4_address.c_str(), &(addr)) != 1) {
        perror("inet_pton failure");
    }
    return addr;
}

/**
 *  Close the given socket in a platform portable way.
 */
static void close_socket(const int socket_fd) {
#ifdef _WIN32
    int result = closesocket(socket_fd);
#else
    int result = close(socket_fd);
#endif
}


/**
 *  Constructor.
 */
HttpClient::HttpClient(void) {
#ifdef _WIN32
    // initialize Windows Socket API with given VERSION.
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        perror("WSAStartup failure");
    }
#endif
}


/**
 * Send http get request and receive http response and content payload
 */
int HttpClient::sendHttpGetRequest(const std::string& url, std::string& response, std::string& content) {

    // parse the given url
    std::string protocol;
    std::string host;
    int         port;
    std::string path;
    if (Url::parseUrl(url, protocol, host, port, path) < 0) {
        perror("url failure");
        return -1;
    }

    // open socket
    int socket_fd = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd < 0) {
        perror("socket open failure");
        return -1;
    }

    // open connection to http server
    struct sockaddr_in http_server;
    http_server.sin_family = AF_INET;
    http_server.sin_addr = toInAddress(host);
    http_server.sin_port = htons(port);

    if (connect(socket_fd, (sockaddr*)&http_server, sizeof(struct sockaddr_in)) < 0) {
        perror("connecting stream socket failure");
        close_socket(socket_fd);
        return -1;
    }

    // assemble http get request
    std::string request;
    request.append("GET ").append(path).append(" HTTP/1.1\r\n");
    request.append("Host: ").append(host).append("\r\n");
    request.append("User-Agent: RalfOGit/1.0\r\n");
    request.append("Accept: application/json\r\n");
    request.append("Accept-Language: de,en-US;q=0.7,en;q=0.3\r\n");
    request.append("Connection: keep-alive\r\n");
    request.append("\r\n");

    // send http get request string
    if (send(socket_fd, request.c_str(), (int)request.length(), 0) != (int)request.length()) {
        perror("send stream socket failure");
        close_socket(socket_fd);
        return -1;
    }

    // receive http get response data
    char recv_buffer[4096];
    int nbytes_total = recv_http_response(socket_fd, recv_buffer, sizeof(recv_buffer));
    std::string answer = std::string(recv_buffer, nbytes_total);
    close_socket(socket_fd);

    // parse http response data
    int http_return_code = parse_http_response(answer, response, content);

    return http_return_code;
}


/**
 * Receive http response and payload
 */
int HttpClient::recv_http_response(int socket_fd, char* recv_buffer, int recv_buffer_size) {
    struct pollfd fds;
    int nbytes_total = 0;

    while (1) {
        fds.fd = socket_fd;
        fds.events = POLLIN;
        fds.revents = 0;

        // wait for a packet on the configured socket
        if (poll(&fds, 1, 5000) < 0) {
            perror("poll failure");
            return -1;
        }

        // check poll result
        if ((fds.revents & POLLIN) != 0) {
            // receive data
            int nbytes = recv(socket_fd, recv_buffer + nbytes_total, recv_buffer_size - nbytes_total - 1, 0);
            if (nbytes < 0) {
                perror("recv stream socket failure");
                exit(1);
            }
            nbytes_total += nbytes;
            recv_buffer[nbytes_total] = '\0';
        }
        else {
            break;
        }
    }
    return nbytes_total;
}


/**
 * Parse http response and payload
 */
int HttpClient::parse_http_response(const std::string& answer, std::string& http_response, std::string& http_content) {

    // extract http return code
    std::string::size_type i1 = answer.find("HTTP/1.1 ");
    std::string::size_type offs = 0;
    int http_return_code = 404;
    if (i1 != std::string::npos) {
        offs = i1 + strlen("HTTP/1.1 ");
        if (sscanf(answer.c_str() + offs, " %d", &http_return_code) != 1) {
            http_response = answer;
            return -1;
        }
    }

    // extract content length
    std::string::size_type i2 = answer.find("\r\nContent-Length: ", offs);
    int content_length = -1;
    if (i2 != std::string::npos) {
        offs = i2 + strlen("\r\nContent-Length: ");
        if (sscanf(answer.c_str() + offs, " %d", &content_length) != 1) {
            http_response = answer;
            return -1;
        }
    }

    // determine content offset and prepare content string
    std::string::size_type i3 = answer.find("\r\n\r\n", offs);
    std::string::size_type content_offset = std::string::npos;
    if (i3 != std::string::npos) {
        content_offset = i3 + strlen("\r\n\r\n");
        if (content_offset + content_length > answer.length()) {
            http_response = answer;
            return -1;
        }
        http_content  = answer.substr(content_offset);
        http_response = answer.substr(0, content_offset);
    }
    else {
        http_response = answer;
    }

    return http_return_code;
}
