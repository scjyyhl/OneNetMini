//
//  OneNet.h
//  OneNetMini
//
//  Created by HaiLong Yang on 2019/12/17.
//  Copyright © 2019 com.dragonlis. All rights reserved.
//

#ifndef OneNet_h
#define OneNet_h

#include <stdio.h>

#define ONENET_DEVICE_ID    "576849548"
#define ONENET_API_KEY      "COpIHELw2dllJBXOAiiOAMZ2s6g="

#define ONENET_HTTP_HEADER_PARAM  "api-key: "ONENET_API_KEY

#define ONENET_API_IP       "183.230.40.33"
#define ONENET_API_PORT     80
#define ONENET_API_HOST     "http://api.heclouds.com"

#define ONENET_API_URL_DATAPOINTS  ONENET_API_HOST"/devices/"ONENET_DEVICE_ID"/datapoints"

int8_t checkReportDataPointResponse(void);
void reportSHTDataPoint(float temperature, float humidity);
#endif /* OneNet_h */
