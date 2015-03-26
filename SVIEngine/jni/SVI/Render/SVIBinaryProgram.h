#pragma once
#include "../SVICores.h"


namespace SVI{


    struct SVIBinaryInfo{
        unsigned char * mBinaryData;
        unsigned int    mBinaryLength;
        unsigned int    mBinaryFormat;

        SVIBinaryInfo(){
            mBinaryData = NULL;
            mBinaryLength = 0;
            mBinaryFormat = 0;
        }
    };

    class SVIBinaryProgram{
    public:
        enum GpuType {
            GPU_UNKNOWN = 0,
            ADRENO_320  = 1,
            SGX_544     = 2,
        };
    public:
        static SVIBool getProgram(int typeProgram, int typeGPU, SVIBinaryInfo * pResult);


    };
};