#include "SVIParticleKeyFrameProperty.h"

namespace SVI {

#define SET_VALUE(type)										\
								mKeyTime = keyTime;		\
								mProperty = property		\

	SVIParticleKeyFrameProperty::SVIParticleKeyFrameProperty() {
		mKeyTime = 0.0f;
	}

	SVIParticleKeyFrameProperty::SVIParticleKeyFrameProperty(const SVIParticleKeyFrameProperty& rhs) {
		mProperty = rhs.mProperty;
		mKeyTime = rhs.mKeyTime;
	}

	SVIParticleKeyFrameProperty::SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIFloat& property) {
		SET_VALUE(SVIUnionProperty::FLOAT);
	}

	SVIParticleKeyFrameProperty::SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIPoint& property) {
		SET_VALUE(SVIUnionProperty::POINT);
	}

	SVIParticleKeyFrameProperty::SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVISize& property) {
		SET_VALUE(SVIUnionProperty::SIZE);
	}

	SVIParticleKeyFrameProperty::SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIColor& property) {
		SET_VALUE(SVIUnionProperty::COLOR);
	}

	SVIParticleKeyFrameProperty::SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIRect& property) {
		SET_VALUE(SVIUnionProperty::RECT);
	}

	SVIParticleKeyFrameProperty::SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIVector3& property) {
		SET_VALUE(SVIUnionProperty::VECTOR3);
	}

	SVIParticleKeyFrameProperty::SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIVector4& property) {
		SET_VALUE(SVIUnionProperty::VECTOR4);
	}

}