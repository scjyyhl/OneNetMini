//
//  http.h
//  OneNetMini
//
//  Created by HaiLong Yang on 2019/12/13.
//  Copyright © 2019 com.dragonlis. All rights reserved.
//

#ifndef http_h
#define http_h

#include <stdio.h>

#define HTTP_POST    "POST"
#define HTTP_GET     "GET"


#ifdef __cplusplus
extern "C" {
#endif
    
uint16_t createRequestData(const char *url, const char *method, const char *bodyParam, const char *headerParam, char **data);

    
#ifdef __cplusplus
}
#endif
        
#endif /* http_h */
