#ifndef __SVI_KEYFRAME_ANIMATION_H_
#define __SVI_KEYFRAME_ANIMATION_H_

#include "SVIPropertyAnimation.h"
#include "SVIKeyFrameProperty.h"
#include <vector>

namespace SVI {

	typedef	std::vector<SVIKeyFrameProperty>		KeyFramePropertyList;

	class SVIKeyFrameAnimation : public SVIPropertyAnimation {
	public:
		virtual ~SVIKeyFrameAnimation();

		static SVIKeyFrameAnimation* create(PropertyAnimationType type, SVIGLSurface* saGLSurface);

		void update(SVIUInt& time);
		void addKeyProperty(const SVIKeyFrameProperty& keyFrameProperty);
		void print();
		virtual SVIAnimation* clone();

	protected :
		void calculateAndApply();
		void copy(SVIKeyFrameAnimation* rhs);

	private:
		SVIKeyFrameAnimation(SVIGLSurface* saGLSurface);
		SVIKeyFrameAnimation(PropertyAnimationType type, SVIGLSurface* saGLSurface);
		SVIKeyFrameAnimation(const SVIKeyFrameAnimation* rhs);
		SVIBool findKeyFrame();
		void checkType();


		KeyFramePropertyList		mKeyFrameList;
		SVIKeyFrameProperty*			mFromKey;
		SVIKeyFrameProperty*			mToKey;
		SVIBool						mTypeChecked;
		SVIBool						mTypeCheckSuccess;
	};

};

#endif //__SVI_KEYFRAME_ANIMATION_H_