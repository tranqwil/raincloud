//
// Created by Will on 12/04/2025.
//

#ifndef STREAM_H
#define STREAM_H
#include <cstddef>
#include <memory>
#include <RaincloudTypes.h>

#include "RaincloudAPI.h"

namespace Raincloud
{
    class RAINCLOUD_API Stream {

    public:
        void Play();
        void Update();

        Stream(const char* filename);
        ~Stream();

    private:
        //No default constructor.
        Stream() = delete;

        struct Impl;
        std::unique_ptr<Impl> m_Impl;

    };

}




#endif //STREAM_H
