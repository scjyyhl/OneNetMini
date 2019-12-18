//
//  OneNet.c
//  OneNetMini
//
//  Created by HaiLong Yang on 2019/12/17.
//  Copyright © 2019 com.dragonlis. All rights reserved.
//

#include "OneNet.h"
#include "http.h"
#include "ESP8266.h"
#include "uuart.h"
#include <string.h>
#include <stdlib.h>

char *responseBuffer = NULL;

int8_t checkReportDataPointResponse(void) {
    uprint((const char *)responseBuffer);
    if (strstr(responseBuffer, "succ") != NULL) {
        return 0;
    }
    return 1;
}

void reportDataPoint(const char *bodyData) {
    char *requestData;
    uint16_t size = createRequestData(ONENET_API_URL_DATAPOINTS, "POST", bodyData, ONENET_HTTP_HEADER_PARAM, &requestData);
    if (responseBuffer != NULL) {
        free(responseBuffer);
    }
    responseBuffer = malloc(64);
    memset(responseBuffer, 0, 64);
    ESP8266_doHttpRequest(ONENET_API_IP, ONENET_API_PORT, requestData, size, responseBuffer, 63);
    free(requestData);
}

void reportSHTDataPoint(float temperature, float humidity) {
    char httpBody[128] = {0};
    sprintf(httpBody, "{\"datastreams\":[{\"id\":\"temperature\",\"datapoints\":[{\"value\":%.2f}]},{\"id\":\"humidity\",\"datapoints\":[{\"value\":%.2f}]}]}", temperature, humidity);
    reportDataPoint(httpBody);
}
