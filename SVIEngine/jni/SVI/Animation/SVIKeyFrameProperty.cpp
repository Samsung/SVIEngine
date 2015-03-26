#include "SVIKeyFrameProperty.h"

namespace SVI {

#define SET_VALUE(type)										\
								mKeyTime = keyTime;		\
								mProperty = property		\

	SVIKeyFrameProperty::SVIKeyFrameProperty() {
		mKeyTime = 0.0f;
	}

	SVIKeyFrameProperty::SVIKeyFrameProperty(const SVIKeyFrameProperty& rhs) {
		mProperty = rhs.mProperty;
		mKeyTime = rhs.mKeyTime;
	}

	SVIKeyFrameProperty::SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIFloat& property) {
		SET_VALUE(SVIUnionProperty::FLOAT);
	}

	SVIKeyFrameProperty::SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIPoint& property) {
		SET_VALUE(SVIUnionProperty::POINT);
	}

	SVIKeyFrameProperty::SVIKeyFrameProperty(const SVIFloat& keyTime, const SVISize& property) {
		SET_VALUE(SVIUnionProperty::SIZE);
	}

	SVIKeyFrameProperty::SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIColor& property) {
		SET_VALUE(SVIUnionProperty::COLOR);
	}

	SVIKeyFrameProperty::SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIRect& property) {
		SET_VALUE(SVIUnionProperty::RECT);
	}

	SVIKeyFrameProperty::SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIVector3& property) {
		SET_VALUE(SVIUnionProperty::VECTOR3);
	}

	SVIKeyFrameProperty::SVIKeyFrameProperty(const SVIFloat& keyTime, const SVIVector4& property) {
		SET_VALUE(SVIUnionProperty::VECTOR4);
	}

}