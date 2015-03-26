#include "SVIParticleAnimation.h"
#include "./Interpolator/SVIInterpolator.h"
#include "../Slide/SVISlide.h"
#include "../Slide/SVIBeamedSlide.h"
#include "../Slide/SVIProjectionSlide.h"
#include "../Particle/SVIParticleEffect.h"
#include "../Particle/SVIParticleEffectManager.h"

namespace SVI {

static const SVIBool DEBUG = SVIFALSE;

SVIParticleAnimation::SVIParticleAnimation(SVIGLSurface* saGLSurface) : SVIAnimation(saGLSurface),
mParticleEffect(NULL) {

}


SVIParticleAnimation::~SVIParticleAnimation() {
	if( mSlide != NULL && mSlide->getProjectionSlide() != NULL ) {
		if(mParticleEffect != NULL) {
			if( getStatus() != IDLE ) {
				SVIBool result = mSlide->getProjectionSlide()->removeParticleEffect(mParticleEffect);
				if(result == SVIFALSE) {
					SVI_SVIFE_DELETE(mParticleEffect);
				}
			}
			else {
				SVI_SVIFE_DELETE(mParticleEffect);
			}

			mParticleEffect = NULL;
		}
	}
	LOGI("SVIParticleAnimation: ~SVIParticleAnimation - end method");
}


void SVIParticleAnimation::update(SVIUInt& time) {
	if( getStatus() == IDLE ) {
	}

	updateInfo(time);

	if( getStatus() == ANIMATING ) {
		
		if(mParticleEffect != NULL) {
			if(mParticleEffect->getStatus() == SVIParticleEffect::IDLE) {
				mSlide->getProjectionSlide()->addParticleEffect(mParticleEffect);
			}

			SVIFloat interpolateTime = mInterpolator->getTimeInterpolation(time);
		if(mParticleEffect)
			mParticleEffect->updateFromAnimation(time, interpolateTime, mDuration);
		}



		if( isEnd(time) ) {
			setStatus(END);
			if( !mIsContainedInAniSet && mSlide != NULL ) {
				//mSlide->getProjectionSlide()->removeParticleEffect(mParticleEffect);
				mParticleEffect->stop();
				mParticleEffect = NULL;
				mSlide->removeAnimation();
			}
		}
	}
}

void SVIParticleAnimation::copy(const SVIParticleAnimation* rhs) {
	SVIAnimation::copy(rhs);
	mParticleEffect = rhs->mParticleEffect;
}


SVIAnimation* SVIParticleAnimation::clone() {

	SVIParticleAnimation* cloneAnimation = new SVIParticleAnimation(mSVIGLSurface);
	cloneAnimation->copy(this);
	cloneAnimation->mParticleEffect = mParticleEffect;

	return (SVIAnimation*)cloneAnimation;
}

	
void SVIParticleAnimation::print() {
	LOGI("AniClassType : ParticleAnimation");
}


void SVIParticleAnimation::cancel() {
	LOGI("SVIParticleAnimation: cancel");
	/*if( mSlide != NULL ) {
		if( mSlide->getProjectionSlide() != NULL ) {
			SVIBool result = mSlide->getProjectionSlide()->removeParticleEffect(mParticleEffect);
			if(result == SVIFALSE) {
				SVI_SVIFE_DELETE(mParticleEffect);
			}
		}
		if(!mIsContainedInAniSet) {
			mSlide->removeAnimation();
			mSlide = NULL;
		}
		setStatus(END);
	}*/
}


void SVIParticleAnimation::forceEnd() {
	LOGI("SVIParticleAnimation: forceEnd");
	if( mInterpolator != NULL )
		mInterpolator->forceEnd();
}


void SVIParticleAnimation::setParticleEffect(SVIParticleEffect* effect) {
	if(mParticleEffect != NULL) {
		SVI_SVIFE_DELETE(mParticleEffect);
	}
	
	mParticleEffect = effect;
}

} // end namespace SVI
