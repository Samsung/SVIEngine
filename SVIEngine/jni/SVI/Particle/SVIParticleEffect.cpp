#include "SVIParticleEffect.h"
#include "SVIParticleEffectManager.h"
#include "../Animation/SVIParticleKeyFrameAnimation.h"

//2012-06-13 masterkeaton27@gmail.com
#include "./Updater/SVIParticleUpdater.h"
#include "./Updater/SVIPhysicParticleUpdater.h"
#include "./Updater/SVILinearParticleUpdater.h"

namespace SVI {

SVIParticleEffect::SVIParticleEffect(SVIGLSurface* saGLSurface) :
mDuration(0),
mStatus(IDLE),
mStartTime(0),
mParentTransform(NULL),
mIsAnimationUpdate(SVIFALSE),
mScaleKeyFrameAnimation(NULL),
mColorKeyFrameAnimation(NULL),
mPositionKeyFrameAnimation(NULL) {
	
	mPosition.x = mPosition.y = mPosition.z = 0.0f;
	mScale.x = mScale.y = mScale.z = 1.0f;
	mColor.mR = mColor.mG = mColor.mB = mColor.mA = 1.0f;

	//2012-06-13 masterkeaton27@gmail.com
	//temporary for test updater.
	mParticleUpdater = new SVIPhysicParticleUpdater();

	mSVIGLSurface = saGLSurface;
}


SVIParticleEffect::~SVIParticleEffect() {
	if(mScaleKeyFrameAnimation != NULL) {
		mScaleKeyFrameAnimation->endAnimation( mSVIGLSurface->getParticleEffectManager()->getDrawTime() );
		SVI_SVIFE_DELETE(mScaleKeyFrameAnimation);
	}
	if(mColorKeyFrameAnimation != NULL) {
		mColorKeyFrameAnimation->endAnimation( mSVIGLSurface->getParticleEffectManager()->getDrawTime() );
		SVI_SVIFE_DELETE(mColorKeyFrameAnimation);
	}
	if(mPositionKeyFrameAnimation != NULL) {
		mPositionKeyFrameAnimation->endAnimation( mSVIGLSurface->getParticleEffectManager()->getDrawTime() );
		SVI_SVIFE_DELETE(mPositionKeyFrameAnimation);
	}

	ParticlesList::iterator iter = mParticlesList.begin();
	while( !mParticlesList.empty() ) {
		if(*iter !=NULL){
		SVIParticles* effect = *iter;
		iter = mParticlesList.erase(iter);
		SVI_SVIFE_DELETE(effect);
		}
	}

	SVI_SVIFE_DELETE(mParticleUpdater);
	mParticlesList.clear();

	mSVIGLSurface = NULL;

	LOGI("SVIParticleEffect: ~SVIParticleEffect - end method !!!");
}


void SVIParticleEffect::setDuration(SVIUInt duration) {
	if(mDuration != duration) {
		mDuration = duration;
		ParticlesList::iterator iter = mParticlesList.begin();
		for(iter; iter != mParticlesList.end(); ++iter) {
			(*iter)->checkMaxDuration(mDuration);
		}
	}
}


void SVIParticleEffect::setPosition(SVIVector3 pos) {
	if(mPosition != pos) {
		mPosition = pos;
		ParticlesList::iterator iter = mParticlesList.begin();
		for(iter; iter != mParticlesList.end(); ++iter) {
			(*iter)->setChangeParentPosition(mPosition);
		}
	}
}


void SVIParticleEffect::setScale(SVIVector3 scale) {
	if(mScale != scale) {
		mScale = scale;
		ParticlesList::iterator iter = mParticlesList.begin();
		for(iter; iter != mParticlesList.end(); ++iter) {
			(*iter)->setChangeParentScale(scale);
		}
	}
}


void SVIParticleEffect::setColor(SVIColor color) {
	if(mColor != color) {
		mColor = color;
		ParticlesList::iterator iter = mParticlesList.begin();
		for(iter; iter != mParticlesList.end(); ++iter) {
			(*iter)->setChangeParentColor(color);
		}
	}
}


void SVIParticleEffect::addParticles(SVIParticles* particles) {
	LOGI("SVIParticleEffect: addParticles - start method !!!");

	SVIParticles* newParticles = particles->clone();

	if( mDuration < newParticles->getDuration() ) {
		newParticles->checkMaxDuration(mDuration);
	}

	mParticlesList.push_back(newParticles);
}


void SVIParticleEffect::setScaleKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset) {
	if(mScaleKeyFrameAnimation == NULL) {
		mScaleKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_EFFECT_SCALE, mSVIGLSurface);
		mScaleKeyFrameAnimation->setParticleEffect(this);
	}
	mScaleKeyFrameAnimation->mDuration = duration;
	mScaleKeyFrameAnimation->setInterpolator(type);
	if(reset == 1) {
		mScaleKeyFrameAnimation->keyReset();
	}
}
		

void SVIParticleEffect::setColorKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset) {
	if(mColorKeyFrameAnimation == NULL) {
		mColorKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_EFFECT_COLOR, mSVIGLSurface);
		mColorKeyFrameAnimation->setParticleEffect(this);
	}
	mColorKeyFrameAnimation->mDuration = duration;
	mColorKeyFrameAnimation->setInterpolator(type);
	if(reset == 1) {
		mColorKeyFrameAnimation->keyReset();
	}
}


void SVIParticleEffect::setPositionKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset) {
	if(mPositionKeyFrameAnimation == NULL) {
		mPositionKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_EFFECT_POSITION, mSVIGLSurface);
		mPositionKeyFrameAnimation->setParticleEffect(this);
	}
	mPositionKeyFrameAnimation->mDuration = duration;
	mPositionKeyFrameAnimation->setInterpolator(type);
	if(reset == 1) {
		mPositionKeyFrameAnimation->keyReset();
	}
}


void SVIParticleEffect::addScaleKeyFrame(SVIFloat keyTime, SVIVector3 scale) {
	if(mScaleKeyFrameAnimation == NULL) {
		mScaleKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_EFFECT_SCALE, mSVIGLSurface);
		mScaleKeyFrameAnimation->setParticleEffect(this);
	}
	mScaleKeyFrameAnimation->addKeyProperty( SVIParticleKeyFrameProperty(keyTime, scale) );
}


void SVIParticleEffect::addColorKeyFrame(SVIFloat keyTime, SVIColor color) {
	if(mColorKeyFrameAnimation == NULL) {
		mColorKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_EFFECT_COLOR, mSVIGLSurface);
		mColorKeyFrameAnimation->setParticleEffect(this);
	}
	mColorKeyFrameAnimation->addKeyProperty( SVIParticleKeyFrameProperty(keyTime, color) );
}


void SVIParticleEffect::addPositionKeyFrame(SVIFloat keyTime, SVIVector3 position) {
	if(mPositionKeyFrameAnimation == NULL) {
		mPositionKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_EFFECT_POSITION, mSVIGLSurface);
		mPositionKeyFrameAnimation->setParticleEffect(this);
	}
	mPositionKeyFrameAnimation->addKeyProperty( SVIParticleKeyFrameProperty(keyTime, position) );
}


void SVIParticleEffect::checkMaxDuration(SVIUInt maxDuration) {
	if(mDuration > maxDuration) {
		mDuration = maxDuration;
		ParticlesList::iterator iter = mParticlesList.begin();
		for(iter; iter != mParticlesList.end(); ++iter) {
			(*iter)->checkMaxDuration(mDuration);
		}
	}
}


void SVIParticleEffect::update(SVIUInt drawTime) {
	
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
			//2012-06-13 masterkeaton27@gmail.comupdater binding
			(*iter)->setUpdater(mParticleUpdater);

			(*iter)->update(drawTime);
		}
	}

	if(mStartTime + mDuration < drawTime) {
		mStatus = END;
		// TODO: add kill code !!! 
	}
}


void SVIParticleEffect::updateFromAnimation(SVIUInt drawTime, SVIFloat interpolateTime, SVIUInt animationDuration) {
	LOGI("SVIParticleEffect:updateFromAnimation - start method !!!");
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


SVIParticleEffect* SVIParticleEffect::clone() {
	LOGI("SVIParticleEffect: clone - start method !!!");
	SVIParticleEffect* cloneParticleEffect = new SVIParticleEffect(mSVIGLSurface);
	cloneParticleEffect->copy(this);

	return cloneParticleEffect;
}


void SVIParticleEffect::copy(const SVIParticleEffect* rhs) {
	LOGI("SVIParticleEffect: copy - start method !!!");
	
	mStatus = IDLE;
	mDuration = rhs->mDuration;

	mSVIGLSurface = rhs->mSVIGLSurface;
	mPosition = rhs->mPosition;
	mScale = rhs->mScale;
	mColor = rhs->mColor;

	mParentTransform = rhs->mParentTransform;
	mParticleUpdater = rhs->mParticleUpdater;
	
	SVIParticleEffect* effect = (SVIParticleEffect*)rhs;

	ParticlesList::iterator iter = effect->mParticlesList.begin();
	for(iter; iter != effect->mParticlesList.end(); ++iter) {
		SVIParticles* particles = (*iter)->clone();
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
}


void SVIParticleEffect::stop() {
	mStatus = END;
}

} // end namespace SVI
