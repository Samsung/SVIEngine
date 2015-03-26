#ifndef __SVI_KEYFRAME_PROPERTY_H_
#define __SVI_KEYFRAME_PROPERTY_H_

#include "SVIPropertyAnimation.h"

namespace SVI {

	class SVIKeyFrameProperty {
	public:

        SVIKeyFrameProperty();
        SVIKeyFrameProperty(const SVIKeyFrameProperty& rhs);
        SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIFloat& rhs);
        SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIPoint& rhs);
        SVIKeyFrameProperty(const SVIFloat& keyTime, const SVISize& rhs);
        SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIColor& rhs);
        SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIRect& rhs);
        SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIVector3& rhs);
        SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIVector4& rhs);

    private:

        SVIFloat				mKeyTime;
        SVIUnionProperty		mProperty;

        friend class SVIKeyFrameAnimation;
    };

};

#endif //__SVI_KEYFRAME_PROPERTY_H_