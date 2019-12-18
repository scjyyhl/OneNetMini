//
//  url.c
//  OneNetMini
//
//  Created by HaiLong Yang on 2019/12/16.
//  Copyright © 2019 com.dragonlis. All rights reserved.
//

#include "url.h"
#include <stdlib.h>
#include <string.h>

void setPathRoot(char *path) {
    path = malloc(2);
    path[0] = '/';
    path[1] = 0;
}

DLSUrlStatus get_url(const char *url, DLSUrlTypeDefine *urlStruct) {
    char *pos, *end, *port;
    uint8_t len;
    DLSUrlStatus rtn = ERROR;
    if (url == NULL || strlen(url) < 3) {
        return rtn;
    }
    pos = strstr(url, "://");
    if (pos != NULL) {
        // get protocol
        len = pos - url;
        if (len > 0) {
            urlStruct->protocol = malloc(len + 1);
            memcpy(urlStruct->protocol, url, len);
            urlStruct->protocol[len] = 0;
        }
        pos += 3;
    }
    else {
        pos = (char*)url;
    }
    end = strchr(pos, '/');
    if (end != NULL) {
        // get path
        len = strlen(url) - (end - url);
        if (len > 0) {
            urlStruct->path = malloc(len + 1);
            memcpy(urlStruct->path, end, len);
            urlStruct->path[len] = 0;
        }
        else {
            setPathRoot(urlStruct->path);
        }
    }
    else {
        setPathRoot(urlStruct->path);
    }
    // get host
    len = end - pos;
    if (len > 0) {
        urlStruct->host = malloc(len + 1);
        memcpy(urlStruct->host, pos, len);
        urlStruct->host[len] = 0;
        rtn = OK;
    }
    // get port
    pos = strchr(urlStruct->host, ':');
    if (pos != NULL) {
        len = end - pos;
        port = malloc(len);
        memcpy(port, pos + 1, len);
        port[len - 1] = 0;
        urlStruct->port = atoi(port);
        free(port);
    }
    else if (urlStruct->protocol != NULL){
        if (strcasecmp(urlStruct->protocol, "http") == 0) {
            urlStruct->port = 80;
        }
        else if (strcasecmp(urlStruct->protocol, "https") == 0) {
            urlStruct->port = 443;
        }
    }
    return rtn;
}

void free_url(DLSUrlTypeDefine url) {
    if (url.protocol != NULL) {
        free(url.protocol);
    }
    if (url.host != NULL) {
        free(url.host);
    }
    if (url.path != NULL) {
        free(url.path);
    }
}
