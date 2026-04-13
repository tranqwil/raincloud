//
// Created by WillChapman on 10/04/2026.
//

#ifndef RAINCLOUDAPI_H
#define RAINCLOUDAPI_H

#ifdef _WIN32
    #ifdef RAINCLOUD_EXPORTS
        #define RAINCLOUD_API __declspec(dllexport)
    #else
        #define RAINCLOUD_API __declspec(dllimport)
    #endif
#else
    #define RAINCLOUD_API
#endif


#endif //RAINCLOUDAPI_H
