//
//  http.c
//  OneNetMini
//
//  Created by HaiLong Yang on 2019/12/13.
//  Copyright © 2019 com.dragonlis. All rights reserved.
//

#include "http.h"
#include "url.h"
#include <stdlib.h>
#include <string.h>

uint16_t createRequestData(const char *url, const char *method, const char *bodyParam, const char *headerParam, char **data) {
    char *pos;
    uint16_t len, headerLen, bodyLen, pathLen, hostLen, contentLen;
    DLSUrlTypeDefine urlInfo;
    DLSUrlStatus status;
    char contentLenStr[32] = {0};
    
    status = get_url(url, &urlInfo);
    if (status == ERROR) {
        free_url(urlInfo);
        return 1;
    }
    len = 0;
    bodyLen = 0;
    headerLen = 0;
    contentLen = 0;
    
    if (strcasecmp(method, "post") == 0) {
        // line 1: "POST xx_path_xx HTTP/1.1\r\n";
        pathLen = strlen(urlInfo.path);
        len = 5 + pathLen + 11;
        // line 2: "Host: xx_host_xx\r\n";
        hostLen = strlen(urlInfo.host);
        len += 6 + hostLen + 2;
        // line 3: "Content-Type: application/json\r\n";
        len += 32;
        // line 4: "User-Agent: dlsHttp/1.0.0\r\n";
        len += 27;
        // line 5: "Accept: */*\r\n";
        len += 13;
        // line 6: "Content-Length: xx_bodylen_xx\r\n";
        if (bodyParam != NULL) {
            bodyLen = strlen(bodyParam);
        }
        sprintf(contentLenStr, "Content-Length: %d\r\n", bodyLen);
        contentLen = strlen(contentLenStr);
        len += contentLen;
        // add header param.
        if (headerParam != NULL) {
            headerLen = strlen(headerParam);
            len += headerLen + 2;
        }
        // line 7: "\r\n";
        len += 2;
        // line 8+: body;
        len += bodyLen;
        
        // compent string
        *data = malloc(len + 0);
        pos = *data;
        pos[len] = 0;
        // add line 1.
        memcpy(pos, "POST ", 5);
        pos += 5;
        memcpy(pos, urlInfo.path, pathLen);
        pos += pathLen;
        memcpy(pos, " HTTP/1.1\r\n", 11);
        pos += 11;
        // add line 2.
        memcpy(pos, "Host: ", 6);
        pos += 6;
        memcpy(pos, urlInfo.host, hostLen);
        pos += hostLen;
        memcpy(pos, "\r\n", 2);
        pos += 2;
        // add line 3.
        memcpy(pos, "Content-Type: application/json\r\n", 32);
        pos += 32;
        // add line 4.
        memcpy(pos, "User-Agent: dlsHttp/1.0.0\r\n", 27);
        pos += 27;
        // add line 5.
        memcpy(pos, "Accept: */*\r\n", 13);
        pos += 13;
        // add line 6.
        if (contentLenStr != NULL) {
            memcpy(pos, contentLenStr, contentLen);
            pos += contentLen;
        }
        // add header param.
        if (headerParam != NULL) {
            memcpy(pos, headerParam, headerLen);
            pos += headerLen;
            memcpy(pos, "\r\n", 2);
            pos += 2;
        }
        // add line 7.
        memcpy(pos, "\r\n", 2);
        pos += 2;
        // add line 8+
        if (bodyParam != NULL) {
            memcpy(pos, bodyParam, bodyLen);
        }
    }
    free_url(urlInfo);
    return len;
}
