#include "SVIKeyFrameParticleEffect.h"
#include "SVIParticleEffectManager.h"
#include "../Animation/SVIParticleKeyFrameAnimation.h"

#include "./Updater/SVIParticleUpdater.h"
#include "./Updater/SVIPhysicParticleUpdater.h"
#include "./Updater/SVILinearParticleUpdater.h"

namespace SVI {

SVIKeyFrameParticleEffect::SVIKeyFrameParticleEffect(SVIGLSurface* saGLSurface) :
SVIParticleEffect(saGLSurface),
mGeneratorPositionKeyFrameAnimation(NULL) {
	mGeneratorPosition.x = mGeneratorPosition.y = mGeneratorPosition.z = 0.0f;
	
	mSVIGLSurface = saGLSurface;
}

SVIKeyFrameParticleEffect::SVIKeyFrameParticleEffect(const SVIKeyFrameParticleEffect& rhs)
:	SVIParticleEffect(rhs.mSVIGLSurface)
{
	copy(&rhs);
}


SVIKeyFrameParticleEffect::~SVIKeyFrameParticleEffect() {
	if(mGeneratorPositionKeyFrameAnimation != NULL) {
		mGeneratorPositionKeyFrameAnimation->endAnimation( mSVIGLSurface->getParticleEffectManager()->getDrawTime() );
		SVI_SVIFE_DELETE(mGeneratorPositionKeyFrameAnimation);
	}
}


void SVIKeyFrameParticleEffect::addParticles(SVIFloat keyTime, SVIParticles* particles) {
	SVIKeyFrameParticles* newParticles = new SVIKeyFrameParticles(mSVIGLSurface);
	( (SVIParticles*)newParticles )->copy(particles);
	newParticles->setKeyTime(keyTime);

	if( mDuration < newParticles->getDuration() ) {
		newParticles->checkMaxDuration(mDuration);
	}

	mParticlesList.push_back(newParticles);
}


void SVIKeyFrameParticleEffect::setGeneratorPosition(SVIVector3 pos) {
	if(mGeneratorPosition != pos) {
		mGeneratorPosition = pos;
		ParticlesList::iterator iter = mParticlesList.begin();
		for(iter; iter != mParticlesList.end(); ++iter) {
			( (SVIKeyFrameParticles*)(*iter) )->setGeneratorPosition(mGeneratorPosition);
		}
	}
}


void SVIKeyFrameParticleEffect::setGeneratorPositionKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset) {
	if(mGeneratorPositionKeyFrameAnimation == NULL) {
		mGeneratorPositionKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_EFFECT_GENERATOR, mSVIGLSurface);
		mGeneratorPositionKeyFrameAnimation->setParticleEffect(this);
	}
	mGeneratorPositionKeyFrameAnimation->mDuration = duration;
	mGeneratorPositionKeyFrameAnimation->setInterpolator(type);
	if(reset == 1) {
		mGeneratorPositionKeyFrameAnimation->keyReset();
	}
}


void SVIKeyFrameParticleEffect::addGeneratorPositionKeyFrame(SVIFloat keyTime, SVIVector3 pos) {
	if(mGeneratorPositionKeyFrameAnimation == NULL) {
		mGeneratorPositionKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_EFFECT_GENERATOR, mSVIGLSurface);
		mGeneratorPositionKeyFrameAnimation->setParticleEffect(this);
	}
	mGeneratorPositionKeyFrameAnimation->addKeyProperty( SVIParticleKeyFrameProperty(keyTime, pos) );
}


void SVIKeyFrameParticleEffect::update(SVIUInt drawTime) {
	
	if( mIsAnimationUpdate == SVITRUE ) {
		return;
	}

	if(mStatus == IDLE) {
		mStatus = START;
		mStartTime = drawTime;
	}
	else if(mStatus == START) {
		mStatus = ANIMATING;
	}
	else if(mStatus == END) {
		return;
	}
	
	if(mStatus == ANIMATING) {
		if(mGeneratorPositionKeyFrameAnimation != NULL) {
			mGeneratorPositionKeyFrameAnimation->update(drawTime);
		}
		if(mScaleKeyFrameAnimation != NULL) {
			mScaleKeyFrameAnimation->update(drawTime);
		}
		if(mColorKeyFrameAnimation != NULL) {
			mColorKeyFrameAnimation->update(drawTime);
		}
		if(mPositionKeyFrameAnimation != NULL) {
			mPositionKeyFrameAnimation->update(drawTime);
		}

		ParticlesList::iterator iter = mParticlesList.begin();
		for(iter; iter != mParticlesList.end(); ++iter) {
			(*iter)->setUpdater(mParticleUpdater);

			( (SVIKeyFrameParticles*)(*iter) )->update(drawTime, mStartTime, mDuration);
		}
	}

	if(mStartTime + mDuration < drawTime) {
		mStatus = END;
		// TODO: add kill code !!! 
	}
}


void SVIKeyFrameParticleEffect::updateFromAnimation(SVIUInt drawTime, SVIFloat interpolateTime, SVIUInt animationDuration) {
	mIsAnimationUpdate = SVITRUE;
	
	if(mStatus == IDLE) {
		mStatus = ANIMATING;
		mStartTime = drawTime;
		if(mDuration > animationDuration) {
			setDuration(animationDuration);
		}
	}
	else if(mStatus == END) {
		return;
	}
	
	if(mStatus == ANIMATING) {
		if(mGeneratorPositionKeyFrameAnimation != NULL) {
			mGeneratorPositionKeyFrameAnimation->update(drawTime);
		}
		if(mScaleKeyFrameAnimation != NULL) {
			mScaleKeyFrameAnimation->update(drawTime);
		}
		if(mColorKeyFrameAnimation != NULL) {
			mColorKeyFrameAnimation->update(drawTime);
		}
		if(mPositionKeyFrameAnimation != NULL) {
			mPositionKeyFrameAnimation->update(drawTime);
		}

		ParticlesList::iterator iter = mParticlesList.begin();
		for(iter; iter != mParticlesList.end(); ++iter) {
			(*iter)->setUpdater(mParticleUpdater);

			(*iter)->updateFromAnimation(drawTime, interpolateTime, animationDuration);
		}
	}

	//if(mStartTime + mDuration < drawTime) {
	//	LOGI("SVIParticleEffect: update - mStatus = END");
	//	mStatus = END;
	//	// TODO: add kill code !!!
	//}
}


SVIParticleEffect* SVIKeyFrameParticleEffect::clone() {
	SVIKeyFrameParticleEffect* cloneParticleEffect = new SVIKeyFrameParticleEffect(mSVIGLSurface);
	cloneParticleEffect->copy(this);

	return cloneParticleEffect;
}


void SVIKeyFrameParticleEffect::copy(const SVIKeyFrameParticleEffect* rhs) {
	mStatus = IDLE;
	mDuration = rhs->mDuration;

	mPosition = rhs->mPosition;
	mScale = rhs->mScale;
	mColor = rhs->mColor;

	mParentTransform = rhs->mParentTransform;
	mParticleUpdater = rhs->mParticleUpdater;
	
	SVIKeyFrameParticleEffect* effect = (SVIKeyFrameParticleEffect*)rhs;

	ParticlesList::iterator iter = effect->mParticlesList.begin();
	for(iter; iter != effect->mParticlesList.end(); ++iter) {
		SVIKeyFrameParticles* particles = (SVIKeyFrameParticles*)( (SVIKeyFrameParticles*)(*iter) )->clone();
		mParticlesList.push_back(particles);
	}

	SVI_SVIFE_DELETE(mScaleKeyFrameAnimation);
	if(rhs->mScaleKeyFrameAnimation != NULL) {
		mScaleKeyFrameAnimation = (SVIParticleKeyFrameAnimation*)rhs->mScaleKeyFrameAnimation->clone();
		mScaleKeyFrameAnimation->setParticleEffect(this);
	}

	SVI_SVIFE_DELETE(mColorKeyFrameAnimation);
	if(rhs->mColorKeyFrameAnimation != NULL) {
		mColorKeyFrameAnimation = (SVIParticleKeyFrameAnimation*)rhs->mColorKeyFrameAnimation->clone();
		mColorKeyFrameAnimation->setParticleEffect(this);
	}

	SVI_SVIFE_DELETE(mPositionKeyFrameAnimation);
	if(rhs->mPositionKeyFrameAnimation != NULL) {
		mPositionKeyFrameAnimation = (SVIParticleKeyFrameAnimation*)rhs->mPositionKeyFrameAnimation->clone();
		mPositionKeyFrameAnimation->setParticleEffect(this);
	}

	SVI_SVIFE_DELETE(mGeneratorPositionKeyFrameAnimation);
	if(rhs->mGeneratorPositionKeyFrameAnimation != NULL) {
		mGeneratorPositionKeyFrameAnimation = (SVIParticleKeyFrameAnimation*)rhs->mGeneratorPositionKeyFrameAnimation->clone();
		mGeneratorPositionKeyFrameAnimation->setParticleEffect(this);
	}
}

} // end namespace SVI
