//
//  url.h
//  OneNetMini
//
//  Created by HaiLong Yang on 2019/12/16.
//  Copyright © 2019 com.dragonlis. All rights reserved.
//

#ifndef url_h
#define url_h

#include <stdio.h>

/**
 * url结构定义
 */
typedef struct _dls_url_type_define {
    char *protocol;
    char *host;
    char *path;
    uint16_t port;
} DLSUrlTypeDefine;

typedef enum {
    OK = 0,
    ERROR = 1
} DLSUrlStatus;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 从url字符串中解析URL信息，使用后，调用free_url释放；
 */
DLSUrlStatus get_url(const char *url, DLSUrlTypeDefine *urlStruct);

/**
 * 释放url空间
 */
void free_url(DLSUrlTypeDefine url);

    
#ifdef __cplusplus
}
#endif
        
#endif /* url_h */
