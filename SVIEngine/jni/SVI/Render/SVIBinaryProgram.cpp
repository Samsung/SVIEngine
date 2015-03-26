#include "../SVICores.h"
#include "SVIBinaryProgram.h"

#include "SVIBinaryAdreno320.h"
#include "SVIBinarySGX544.h"

#include "SVIProgramManager.h"

namespace SVI{


namespace Getter{

    SVIBool PR_COLOR(SVIBinaryInfo * info, int typeGPU){
        switch (typeGPU){
        case SVIBinaryProgram::SGX_544: 
            info->mBinaryFormat = SGX544::mFormat_PR_COLOR;
            info->mBinaryLength = SGX544::mLength_PR_COLOR;
            info->mBinaryData = SGX544::PR_COLOR;
            break;

        case SVIBinaryProgram::ADRENO_320:       
            info->mBinaryFormat = Adreno320::mFormat_PR_COLOR;
            info->mBinaryLength = Adreno320::mLength_PR_COLOR;
            info->mBinaryData = Adreno320::PR_COLOR;
            break;

        default: return SVIFALSE;
        }
        return SVITRUE;
    }


    SVIBool PR_COLOR_LT_SPOT(SVIBinaryInfo * info, int typeGPU){
        switch (typeGPU){
        case SVIBinaryProgram::SGX_544: 
            info->mBinaryFormat = SGX544::mFormat_PR_COLOR_LT_SPOT;
            info->mBinaryLength = SGX544::mLength_PR_COLOR_LT_SPOT;
            info->mBinaryData = SGX544::PR_COLOR_LT_SPOT;
            break;

        case SVIBinaryProgram::ADRENO_320:       
            info->mBinaryFormat = Adreno320::mFormat_PR_COLOR_LT_SPOT;
            info->mBinaryLength = Adreno320::mLength_PR_COLOR_LT_SPOT;
            info->mBinaryData = Adreno320::PR_COLOR_LT_SPOT;
            break;
        default: return SVIFALSE;
        }
        return SVITRUE;
    }

SVIBool PR_COLOR_LT_LINEAR(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_COLOR_LT_LINEAR;
        info->mBinaryLength = SGX544::mLength_PR_COLOR_LT_LINEAR;
        info->mBinaryData = SGX544::PR_COLOR_LT_LINEAR;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_COLOR_LT_LINEAR;
        info->mBinaryLength = Adreno320::mLength_PR_COLOR_LT_LINEAR;
        info->mBinaryData = Adreno320::PR_COLOR_LT_LINEAR;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_COLOR_SHADOW(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_COLOR_SHADOW;
        info->mBinaryLength = SGX544::mLength_PR_COLOR_SHADOW;
        info->mBinaryData = SGX544::PR_COLOR_SHADOW;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_COLOR_SHADOW;
        info->mBinaryLength = Adreno320::mLength_PR_COLOR_SHADOW;
        info->mBinaryData = Adreno320::PR_COLOR_SHADOW;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_ADD(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_ADD;
        info->mBinaryLength = SGX544::mLength_PR_TEX_ADD;
        info->mBinaryData = SGX544::PR_TEX_ADD;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_ADD;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_ADD;
        info->mBinaryData = Adreno320::PR_TEX_ADD;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_ADD_LT_SPOT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_ADD_LT_SPOT;
        info->mBinaryLength = SGX544::mLength_PR_TEX_ADD_LT_SPOT;
        info->mBinaryData = SGX544::PR_TEX_ADD_LT_SPOT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_ADD_LT_SPOT;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_ADD_LT_SPOT;
        info->mBinaryData = Adreno320::PR_TEX_ADD_LT_SPOT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_ADD_LT_LINEAR(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_ADD_LT_LINEAR;
        info->mBinaryLength = SGX544::mLength_PR_TEX_ADD_LT_LINEAR;
        info->mBinaryData = SGX544::PR_TEX_ADD_LT_LINEAR;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_ADD_LT_LINEAR;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_ADD_LT_LINEAR;
        info->mBinaryData = Adreno320::PR_TEX_ADD_LT_LINEAR;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_ADD_SHADOW(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_ADD_SHADOW;
        info->mBinaryLength = SGX544::mLength_PR_TEX_ADD_SHADOW;
        info->mBinaryData = SGX544::PR_TEX_ADD_SHADOW;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_ADD_SHADOW;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_ADD_SHADOW;
        info->mBinaryData = Adreno320::PR_TEX_ADD_SHADOW;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_MLT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_MLT;
        info->mBinaryLength = SGX544::mLength_PR_TEX_MLT;
        info->mBinaryData = SGX544::PR_TEX_MLT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_MLT;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_MLT;
        info->mBinaryData = Adreno320::PR_TEX_MLT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_MLT_LT_SPOT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_MLT_LT_SPOT;
        info->mBinaryLength = SGX544::mLength_PR_TEX_MLT_LT_SPOT;
        info->mBinaryData = SGX544::PR_TEX_MLT_LT_SPOT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_MLT_LT_SPOT;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_MLT_LT_SPOT;
        info->mBinaryData = Adreno320::PR_TEX_MLT_LT_SPOT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_MLT_LT_LINEAR(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_MLT_LT_LINEAR;
        info->mBinaryLength = SGX544::mLength_PR_TEX_MLT_LT_LINEAR;
        info->mBinaryData = SGX544::PR_TEX_MLT_LT_LINEAR;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_MLT_LT_LINEAR;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_MLT_LT_LINEAR;
        info->mBinaryData = Adreno320::PR_TEX_MLT_LT_LINEAR;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_MLT_SHADOW(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_MLT_SHADOW;
        info->mBinaryLength = SGX544::mLength_PR_TEX_MLT_SHADOW;
        info->mBinaryData = SGX544::PR_TEX_MLT_SHADOW;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_MLT_SHADOW;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_MLT_SHADOW;
        info->mBinaryData = Adreno320::PR_TEX_MLT_SHADOW;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_ADD_FIT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_ADD_FIT;
        info->mBinaryLength = SGX544::mLength_PR_TEX_ADD_FIT;
        info->mBinaryData = SGX544::PR_TEX_ADD_FIT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_ADD_FIT;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_ADD_FIT;
        info->mBinaryData = Adreno320::PR_TEX_ADD_FIT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_ADD_LT_SPOT_FIT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_ADD_LT_SPOT_FIT;
        info->mBinaryLength = SGX544::mLength_PR_TEX_ADD_LT_SPOT_FIT;
        info->mBinaryData = SGX544::PR_TEX_ADD_LT_SPOT_FIT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_ADD_LT_SPOT_FIT;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_ADD_LT_SPOT_FIT;
        info->mBinaryData = Adreno320::PR_TEX_ADD_LT_SPOT_FIT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_ADD_LT_LINEAR_FIT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_ADD_LT_LINEAR_FIT;
        info->mBinaryLength = SGX544::mLength_PR_TEX_ADD_LT_LINEAR_FIT;
        info->mBinaryData = SGX544::PR_TEX_ADD_LT_LINEAR_FIT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_ADD_LT_LINEAR_FIT;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_ADD_LT_LINEAR_FIT;
        info->mBinaryData = Adreno320::PR_TEX_ADD_LT_LINEAR_FIT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_ADD_SHADOW_FIT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_ADD_SHADOW_FIT;
        info->mBinaryLength = SGX544::mLength_PR_TEX_ADD_SHADOW_FIT;
        info->mBinaryData = SGX544::PR_TEX_ADD_SHADOW_FIT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_ADD_SHADOW_FIT;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_ADD_SHADOW_FIT;
        info->mBinaryData = Adreno320::PR_TEX_ADD_SHADOW_FIT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_MLT_FIT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_MLT_FIT;
        info->mBinaryLength = SGX544::mLength_PR_TEX_MLT_FIT;
        info->mBinaryData = SGX544::PR_TEX_MLT_FIT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_MLT_FIT;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_MLT_FIT;
        info->mBinaryData = Adreno320::PR_TEX_MLT_FIT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_MLT_LT_SPOT_FIT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_MLT_LT_SPOT_FIT;
        info->mBinaryLength = SGX544::mLength_PR_TEX_MLT_LT_SPOT_FIT;
        info->mBinaryData = SGX544::PR_TEX_MLT_LT_SPOT_FIT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_MLT_LT_SPOT_FIT;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_MLT_LT_SPOT_FIT;
        info->mBinaryData = Adreno320::PR_TEX_MLT_LT_SPOT_FIT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_MLT_LT_LINEAR_FIT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_MLT_LT_LINEAR_FIT;
        info->mBinaryLength = SGX544::mLength_PR_TEX_MLT_LT_LINEAR_FIT;
        info->mBinaryData = SGX544::PR_TEX_MLT_LT_LINEAR_FIT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_MLT_LT_LINEAR_FIT;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_MLT_LT_LINEAR_FIT;
        info->mBinaryData = Adreno320::PR_TEX_MLT_LT_LINEAR_FIT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_TEX_MLT_SHADOW_FIT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_TEX_MLT_SHADOW_FIT;
        info->mBinaryLength = SGX544::mLength_PR_TEX_MLT_SHADOW_FIT;
        info->mBinaryData = SGX544::PR_TEX_MLT_SHADOW_FIT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_TEX_MLT_SHADOW_FIT;
        info->mBinaryLength = Adreno320::mLength_PR_TEX_MLT_SHADOW_FIT;
        info->mBinaryData = Adreno320::PR_TEX_MLT_SHADOW_FIT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}


SVIBool PR_3DSHADOW(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_3DSHADOW;
        info->mBinaryLength = SGX544::mLength_PR_3DSHADOW;
        info->mBinaryData = SGX544::PR_3DSHADOW;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_3DSHADOW;
        info->mBinaryLength = Adreno320::mLength_PR_3DSHADOW;
        info->mBinaryData = Adreno320::PR_3DSHADOW;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}



#if SVI_USE_POST_EFFECT
SVIBool PR_RIPPLE_RT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_RIPPLE_RT;
        info->mBinaryLength = SGX544::mLength_PR_RIPPLE_RT;
        info->mBinaryData = SGX544::PR_RIPPLE_RT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_RIPPLE_RT;
        info->mBinaryLength = Adreno320::mLength_PR_RIPPLE_RT;
        info->mBinaryData = Adreno320::PR_RIPPLE_RT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_PULSE_RT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_PULSE_RT;
        info->mBinaryLength = SGX544::mLength_PR_PULSE_RT;
        info->mBinaryData = SGX544::PR_PULSE_RT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_PULSE_RT;
        info->mBinaryLength = Adreno320::mLength_PR_PULSE_RT;
        info->mBinaryData = Adreno320::PR_PULSE_RT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_DREAMVISION_RT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_DREAMVISION_RT;
        info->mBinaryLength = SGX544::mLength_PR_DREAMVISION_RT;
        info->mBinaryData = SGX544::PR_DREAMVISION_RT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_DREAMVISION_RT;
        info->mBinaryLength = Adreno320::mLength_PR_DREAMVISION_RT;
        info->mBinaryData = Adreno320::PR_DREAMVISION_RT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_WIZZLE_RT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_WIZZLE_RT;
        info->mBinaryLength = SGX544::mLength_PR_WIZZLE_RT;
        info->mBinaryData = SGX544::PR_WIZZLE_RT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_WIZZLE_RT;
        info->mBinaryLength = Adreno320::mLength_PR_WIZZLE_RT;
        info->mBinaryData = Adreno320::PR_WIZZLE_RT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_PAPERFOLD_RT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_PAPERFOLD_RT;
        info->mBinaryLength = SGX544::mLength_PR_PAPERFOLD_RT;
        info->mBinaryData = SGX544::PR_PAPERFOLD_RT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_PAPERFOLD_RT;
        info->mBinaryLength = Adreno320::mLength_PR_PAPERFOLD_RT;
        info->mBinaryData = Adreno320::PR_PAPERFOLD_RT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_MAGNIFY_RT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_MAGNIFY_RT;
        info->mBinaryLength = SGX544::mLength_PR_MAGNIFY_RT;
        info->mBinaryData = SGX544::PR_MAGNIFY_RT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_MAGNIFY_RT;
        info->mBinaryLength = Adreno320::mLength_PR_MAGNIFY_RT;
        info->mBinaryData = Adreno320::PR_MAGNIFY_RT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_GLASSTILE_RT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_GLASSTILE_RT;
        info->mBinaryLength = SGX544::mLength_PR_GLASSTILE_RT;
        info->mBinaryData = SGX544::PR_GLASSTILE_RT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_GLASSTILE_RT;
        info->mBinaryLength = Adreno320::mLength_PR_GLASSTILE_RT;
        info->mBinaryData = Adreno320::PR_GLASSTILE_RT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_ZOOMBLUR_RT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_ZOOMBLUR_RT;
        info->mBinaryLength = SGX544::mLength_PR_ZOOMBLUR_RT;
        info->mBinaryData = SGX544::PR_ZOOMBLUR_RT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_ZOOMBLUR_RT;
        info->mBinaryLength = Adreno320::mLength_PR_ZOOMBLUR_RT;
        info->mBinaryData = Adreno320::PR_ZOOMBLUR_RT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_DIRECTIONBLUR_RT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_DIRECTIONBLUR_RT;
        info->mBinaryLength = SGX544::mLength_PR_DIRECTIONBLUR_RT;
        info->mBinaryData = SGX544::PR_DIRECTIONBLUR_RT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_DIRECTIONBLUR_RT;
        info->mBinaryLength = Adreno320::mLength_PR_DIRECTIONBLUR_RT;
        info->mBinaryData = Adreno320::PR_DIRECTIONBLUR_RT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

#endif

SVIBool PR_3DMODEL(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_3DMODEL;
        info->mBinaryLength = SGX544::mLength_PR_3DMODEL;
        info->mBinaryData = SGX544::PR_3DMODEL;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_3DMODEL;
        info->mBinaryLength = Adreno320::mLength_PR_3DMODEL;
        info->mBinaryData = Adreno320::PR_3DMODEL;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_3DMODEL_OFFSET(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_3DMODEL_OFFSET;
        info->mBinaryLength = SGX544::mLength_PR_3DMODEL_OFFSET;
        info->mBinaryData = SGX544::PR_3DMODEL_OFFSET;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_3DMODEL_OFFSET;
        info->mBinaryLength = Adreno320::mLength_PR_3DMODEL_OFFSET;
        info->mBinaryData = Adreno320::PR_3DMODEL_OFFSET;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_PHYSIC3DMODEL(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_PHYSIC3DMODEL;
        info->mBinaryLength = SGX544::mLength_PR_PHYSIC3DMODEL;
        info->mBinaryData = SGX544::PR_PHYSIC3DMODEL;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_PHYSIC3DMODEL;
        info->mBinaryLength = Adreno320::mLength_PR_PHYSIC3DMODEL;
        info->mBinaryData = Adreno320::PR_PHYSIC3DMODEL;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_PHYSIC3DMODELAA(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_PHYSIC3DMODELAA;
        info->mBinaryLength = SGX544::mLength_PR_PHYSIC3DMODELAA;
        info->mBinaryData = SGX544::PR_PHYSIC3DMODELAA;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_PHYSIC3DMODELAA;
        info->mBinaryLength = Adreno320::mLength_PR_PHYSIC3DMODELAA;
        info->mBinaryData = Adreno320::PR_PHYSIC3DMODELAA;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
/*
void PR_LINEAR_GRADATION(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_LINEAR_GRADATION;
        info->mBinaryLength = SGX544::mLength_PR_LINEAR_GRADATION;
        info->mBinaryData = SGX544::PR_LINEAR_GRADATION;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_LINEAR_GRADATION;
        info->mBinaryLength = Adreno320::mLength_PR_LINEAR_GRADATION;
        info->mBinaryData = Adreno320::PR_LINEAR_GRADATION;
        break;
    }
}
void PR_RADIAL_GRADATION(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_RADIAL_GRADATION;
        info->mBinaryLength = SGX544::mLength_PR_RADIAL_GRADATION;
        info->mBinaryData = SGX544::PR_RADIAL_GRADATION;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_RADIAL_GRADATION;
        info->mBinaryLength = Adreno320::mLength_PR_RADIAL_GRADATION;
        info->mBinaryData = Adreno320::PR_RADIAL_GRADATION;
        break;
    }
}*/

SVIBool PR_LINEAR_SWIPE(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_LINEAR_SWIPE;
        info->mBinaryLength = SGX544::mLength_PR_LINEAR_SWIPE;
        info->mBinaryData = SGX544::PR_LINEAR_SWIPE;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_LINEAR_SWIPE;
        info->mBinaryLength = Adreno320::mLength_PR_LINEAR_SWIPE;
        info->mBinaryData = Adreno320::PR_LINEAR_SWIPE;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_RADIAL_SWIPE(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_RADIAL_SWIPE;
        info->mBinaryLength = SGX544::mLength_PR_RADIAL_SWIPE;
        info->mBinaryData = SGX544::PR_RADIAL_SWIPE;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_RADIAL_SWIPE;
        info->mBinaryLength = Adreno320::mLength_PR_RADIAL_SWIPE;
        info->mBinaryData = Adreno320::PR_RADIAL_SWIPE;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_NORMALMAP(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_NORMALMAP;
        info->mBinaryLength = SGX544::mLength_PR_NORMALMAP;
        info->mBinaryData = SGX544::PR_NORMALMAP;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_NORMALMAP;
        info->mBinaryLength = Adreno320::mLength_PR_NORMALMAP;
        info->mBinaryData = Adreno320::PR_NORMALMAP;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_SPECULAR_NORMALMAP(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_SPECULAR_NORMALMAP;
        info->mBinaryLength = SGX544::mLength_PR_SPECULAR_NORMALMAP;
        info->mBinaryData = SGX544::PR_SPECULAR_NORMALMAP;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_SPECULAR_NORMALMAP;
        info->mBinaryLength = Adreno320::mLength_PR_SPECULAR_NORMALMAP;
        info->mBinaryData = Adreno320::PR_SPECULAR_NORMALMAP;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_POST_HIGHLIGHT(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_POST_HIGHLIGHT;
        info->mBinaryLength = SGX544::mLength_PR_POST_HIGHLIGHT;
        info->mBinaryData = SGX544::PR_POST_HIGHLIGHT;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_POST_HIGHLIGHT;
        info->mBinaryLength = Adreno320::mLength_PR_POST_HIGHLIGHT;
        info->mBinaryData = Adreno320::PR_POST_HIGHLIGHT;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_POST_MERGE(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_POST_MERGE;
        info->mBinaryLength = SGX544::mLength_PR_POST_MERGE;
        info->mBinaryData = SGX544::PR_POST_MERGE;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_POST_MERGE;
        info->mBinaryLength = Adreno320::mLength_PR_POST_MERGE;
        info->mBinaryData = Adreno320::PR_POST_MERGE;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_POST_FINALE(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_POST_FINALE;
        info->mBinaryLength = SGX544::mLength_PR_POST_FINALE;
        info->mBinaryData = SGX544::PR_POST_FINALE;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_POST_FINALE;
        info->mBinaryLength = Adreno320::mLength_PR_POST_FINALE;
        info->mBinaryData = Adreno320::PR_POST_FINALE;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_POST_BLUR(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_POST_BLUR;
        info->mBinaryLength = SGX544::mLength_PR_POST_BLUR;
        info->mBinaryData = SGX544::PR_POST_BLUR;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_POST_BLUR;
        info->mBinaryLength = Adreno320::mLength_PR_POST_BLUR;
        info->mBinaryData = Adreno320::PR_POST_BLUR;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_POST_DEBUG(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_POST_DEBUG;
        info->mBinaryLength = SGX544::mLength_PR_POST_DEBUG;
        info->mBinaryData = SGX544::PR_POST_DEBUG;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_POST_DEBUG;
        info->mBinaryLength = Adreno320::mLength_PR_POST_DEBUG;
        info->mBinaryData = Adreno320::PR_POST_DEBUG;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_POST_BLOOM(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_POST_BLOOM;
        info->mBinaryLength = SGX544::mLength_PR_POST_BLOOM;
        info->mBinaryData = SGX544::PR_POST_BLOOM;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_POST_BLOOM;
        info->mBinaryLength = Adreno320::mLength_PR_POST_BLOOM;
        info->mBinaryData = Adreno320::PR_POST_BLOOM;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
SVIBool PR_POST_DOWN8X8(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_POST_DOWN8X8;
        info->mBinaryLength = SGX544::mLength_PR_POST_DOWN8X8;
        info->mBinaryData = SGX544::PR_POST_DOWN8X8;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_POST_DOWN8X8;
        info->mBinaryLength = Adreno320::mLength_PR_POST_DOWN8X8;
        info->mBinaryData = Adreno320::PR_POST_DOWN8X8;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}

SVIBool PR_POST_RIPPLE(SVIBinaryInfo * info, int typeGPU){
    switch (typeGPU){
    case SVIBinaryProgram::SGX_544: 
        info->mBinaryFormat = SGX544::mFormat_PR_POST_RIPPLE;
        info->mBinaryLength = SGX544::mLength_PR_POST_RIPPLE;
        info->mBinaryData = SGX544::PR_POST_RIPPLE;
        break;

    case SVIBinaryProgram::ADRENO_320:       
        info->mBinaryFormat = Adreno320::mFormat_PR_POST_RIPPLE;
        info->mBinaryLength = Adreno320::mLength_PR_POST_RIPPLE;
        info->mBinaryData = Adreno320::PR_POST_RIPPLE;
        break;
    default: return SVIFALSE;
    }
    return SVITRUE;
}
};


SVIBool SVIBinaryProgram::getProgram(int typeProgram, int typeGPU, SVIBinaryInfo * pResult){
    switch (typeProgram){
        case PR_COLOR: 
            return Getter::PR_COLOR(pResult, typeGPU);
            
            break;
        case PR_COLOR_LT_SPOT: 
            return Getter::PR_COLOR_LT_SPOT(pResult, typeGPU);
            break;
        case PR_COLOR_LT_LINEAR:
            return Getter::PR_COLOR_LT_LINEAR(pResult, typeGPU);
            break;
        case PR_COLOR_SHADOW:
            return Getter::PR_COLOR_SHADOW(pResult, typeGPU);
            break;

        case PR_TEX_ADD: 
            return Getter::PR_TEX_ADD(pResult, typeGPU);
            break;
        
        case PR_TEX_ADD_LT_SPOT: 
            return Getter::PR_TEX_ADD_LT_SPOT(pResult, typeGPU);
            break;
        case PR_TEX_ADD_LT_LINEAR: 
            return Getter::PR_TEX_ADD_LT_LINEAR(pResult, typeGPU);
            break;
        case PR_TEX_ADD_SHADOW: 
            return Getter::PR_TEX_ADD_SHADOW(pResult, typeGPU);
            break;
        case PR_TEX_MLT: 
            return Getter::PR_TEX_MLT(pResult, typeGPU);
            break;
        case PR_TEX_MLT_LT_SPOT: 
            return Getter::PR_TEX_MLT_LT_SPOT(pResult, typeGPU);
            break;
        case PR_TEX_MLT_LT_LINEAR: 
            return Getter::PR_TEX_MLT_LT_LINEAR(pResult, typeGPU);
            break;	
        case PR_TEX_MLT_SHADOW: 
            return Getter::PR_TEX_MLT_SHADOW(pResult, typeGPU);
            
            break;

        case PR_TEX_ADD_FIT: 
            return Getter::PR_TEX_ADD_FIT(pResult, typeGPU);
            
            break;
        case PR_TEX_ADD_LT_SPOT_FIT: 
            return Getter::PR_TEX_ADD_LT_SPOT_FIT(pResult, typeGPU);
            
            break;
        case PR_TEX_ADD_LT_LINEAR_FIT: 
            return Getter::PR_TEX_ADD_LT_LINEAR_FIT(pResult, typeGPU);
            break;
        case PR_TEX_ADD_SHADOW_FIT: 
            return Getter::PR_TEX_ADD_SHADOW_FIT(pResult, typeGPU);
            break;
        case PR_TEX_MLT_FIT: 
            return Getter::PR_TEX_MLT_FIT(pResult, typeGPU);
            break;	
        case PR_TEX_MLT_LT_SPOT_FIT: 
            return Getter::PR_TEX_MLT_LT_SPOT_FIT(pResult, typeGPU);
            break;
        case PR_TEX_MLT_LT_LINEAR_FIT: 
            return Getter::PR_TEX_MLT_LT_LINEAR_FIT(pResult, typeGPU);
            break;	
        case PR_TEX_MLT_SHADOW_FIT: 
            return Getter::PR_TEX_MLT_SHADOW_FIT(pResult, typeGPU);
            break;
               
        case PR_3DSHADOW: 
            return Getter::PR_3DSHADOW(pResult, typeGPU);
            break;

    #if SVI_USE_POST_EFFECT
        case PR_RIPPLE_RT: 
            return Getter::PR_RIPPLE_RT(pResult, typeGPU);
            break;
        case PR_PULSE_RT: 
            return Getter::PR_PULSE_RT(pResult, typeGPU);
            break;
        case PR_DREAMVISION_RT: 
            return Getter::PR_DREAMVISION_RT(pResult, typeGPU);
            break;
        case PR_WIZZLE_RT: 
            return Getter::PR_WIZZLE_RT(pResult, typeGPU);
            break;
        case PR_PAPERFOLD_RT: 
            return Getter::PR_PAPERFOLD_RT(pResult, typeGPU);
            break;
        case PR_MAGNIFY_RT: 
            return Getter::PR_MAGNIFY_RT(pResult, typeGPU);
            break;
        case PR_GLASSTILE_RT: 
            return Getter::PR_GLASSTILE_RT(pResult, typeGPU);
            break;
        case PR_ZOOMBLUR_RT: 
            return Getter::PR_ZOOMBLUR_RT(pResult, typeGPU);
            break;
        case PR_DIRECTIONBLUR_RT: 
            return Getter::PR_DIRECTIONBLUR_RT(pResult, typeGPU);
            break;
    #endif
        case PR_3DMODEL: 
            return Getter::PR_3DMODEL(pResult, typeGPU);
            break;
        case PR_3DMODEL_OFFSET: 
            return Getter::PR_3DMODEL_OFFSET(pResult, typeGPU);
            break;
        case PR_PHYSIC3DMODEL: 
            return Getter::PR_PHYSIC3DMODEL(pResult, typeGPU);
            break; 
        case PR_PHYSIC3DMODELAA: 
            return Getter::PR_PHYSIC3DMODELAA(pResult, typeGPU);
            break; 
        /*
        case PR_LINEAR_GRADATION: 
            Getter::PR_LINEAR_GRADATION(&result, typeGPU);
            break;
        case PR_RADIAL_GRADATION: 
            Getter::PR_RADIAL_GRADATION(&result, typeGPU);
            break;*/        
        case PR_LINEAR_SWIPE: 
            return Getter::PR_LINEAR_SWIPE(pResult, typeGPU);
            break;
        case PR_RADIAL_SWIPE: 
            return Getter::PR_RADIAL_SWIPE(pResult, typeGPU);
            break;
        case PR_NORMALMAP: 
            return Getter::PR_NORMALMAP(pResult, typeGPU);
            break;       
        case PR_SPECULAR_NORMALMAP: 
            return Getter::PR_SPECULAR_NORMALMAP(pResult, typeGPU);
            break;
        case PR_POST_HIGHLIGHT: 
            return Getter::PR_POST_HIGHLIGHT(pResult, typeGPU);
            break;
        case PR_POST_MERGE: 
            return Getter::PR_POST_MERGE(pResult, typeGPU);
            break;
        case PR_POST_FINALE: 
            return Getter::PR_POST_FINALE(pResult, typeGPU);
            break;
        case PR_POST_BLUR: 
            return Getter::PR_POST_BLUR(pResult, typeGPU);
            break;
        case PR_POST_DEBUG: 
            return Getter::PR_POST_DEBUG(pResult, typeGPU);
            break;
        case PR_POST_BLOOM: 
            return Getter::PR_POST_BLOOM(pResult, typeGPU);
            break;
        case PR_POST_DOWN8X8: 
            return Getter::PR_POST_DOWN8X8(pResult, typeGPU);
            break;

        //2013-02-04 masterkeaton27@gmail.com
        case PR_POST_RIPPLE:
            return Getter::PR_POST_RIPPLE(pResult, typeGPU);
            break;
    }

    return SVIFALSE;
}


}