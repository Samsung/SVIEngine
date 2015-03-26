#include "SVIParticles.h"
#include "SVIParticleEffectManager.h"
#include "./Updater/SVIParticleUpdater.h"
#include "../Animation/SVIParticleKeyFrameAnimation.h"

namespace SVI {
	
SVIParticle::SVIParticle() {
	_active = SVITRUE;
	_life = 1.0f;

	_originSize.x = _originSize.y = _originSize.z = 1.0f;
	_size.x = _size.y = _size.z = 1.0f;
	_position.x = _position.y = _position.z = 0.0f;
	_velocity.x = _velocity.y = _velocity.z = 0.0f;

	_originParticleColor.mR = _originParticleColor.mG = _originParticleColor.mB = _originParticleColor.mA = 1.0f;
	_particleColor.mR = _particleColor.mG = _particleColor.mB = _particleColor.mA = 1.0f;
	_gravity.x = _gravity.y = _gravity.z = 0.0f; 
	_force.x = _force.y = _force.z = 0.0f;
	_mass = 0.0f;
}


SVIParticles::SVIParticles(SVIGLSurface* saGLSurface) :
mMaxDuration(0),
mRandomDuration(0),
mParticleCount(0),
mOffset(0),
mStartTime(0),
mUseTexture(SVIFALSE),
mBlendType(BLEND_MULTIPLY),
mStatus(IDLE),
mParticles(NULL),
m_pParticleUpdater(NULL),
mScaleKeyFrameAnimation(NULL),
mColorKeyFrameAnimation(NULL), 
mPositionKeyFrameAnimation(NULL),
mValidated(SVIFALSE),
mImage(NULL) {
	mParentPosition.x = mParentPosition.y = mParentPosition.z = 0.0f;
	mParentScale.x = mParentScale.y = mParentScale.z = 1.0f;
	mParentColor.mR = mParentColor.mG = mParentColor.mB = mParentColor.mA = 1.0f;

	mTextureFileName = "";
	
	// set member field value
	mDefaultPosition.x = mDefaultPosition.y = mDefaultPosition.z = 0.0f;
	mRandomPosition.x = mRandomPosition.y = mRandomPosition.z = 0.0f;
	mMaxColor.mR = mMaxColor.mG = mMaxColor.mB = mMaxColor.mA = 1.0f;
	mRandomColor.mR = mRandomColor.mG = mRandomColor.mB = mRandomColor.mA = 0.0f;
	mDefaultGravity.x = mDefaultGravity.y = mDefaultGravity.z = 0.0f;
	mRandomGravity.x = mRandomGravity.y = mRandomGravity.z = 0.0f;
	mDefaultForce.x = mDefaultForce.y = mDefaultForce.z = 0.0f;
	mRandomForce.x = mRandomForce.y = mRandomForce.z = 0.0f;
	mMaxMass = 0.0f;
	mRandomMass = 0.0f;
	mMaxParticleSize.x = mMaxParticleSize.y = mMaxParticleSize.z = 1.0f;
	mRandomParticleSize.x = mRandomParticleSize.y = mRandomParticleSize.z = 0.0f;

	mSVIGLSurface = saGLSurface;
}


SVIParticles::~SVIParticles() {
	SVI_SVIFE_DELETE_ARRAY(mParticles);
	
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
	//LOGI("SVIParticles: ~SVIParticles - end method !!!");
}


void SVIParticles::setParticleCount(SVIUInt particleCount) {
	mParticleCount = particleCount;
}


void SVIParticles::setTextureFileName(std::string fileName, SVIImage* image) {
	mTextureFileName = fileName;
	if(mTextureFileName == "") {
		mUseTexture = SVIFALSE;
		mImage = NULL;
	}
	else {
		mUseTexture = SVITRUE;
		mImage = image;

		mSVIGLSurface->getParticleEffectManager()->addTexture(mTextureFileName, image);
	}
}


void SVIParticles::setMaxDuration(SVIUInt duration) {
	mMaxDuration = duration;
}


void SVIParticles::setRandomDuration(SVIUInt duration) {
	mRandomDuration = duration;
}


void SVIParticles::setMaxSize(SVIVector3 size) {
	mMaxParticleSize = size;
}


void SVIParticles::setRandomSize(SVIVector3 size) {
	mRandomParticleSize = size;
}


void SVIParticles::setDefaultPosition(SVIVector3 pos) {
	mDefaultPosition = pos;
}


void SVIParticles::setRandomPosition(SVIVector3 pos) {
	mRandomPosition = pos;
}


void SVIParticles::setMaxColor(SVIColor color) {
	mMaxColor = color;
}


void SVIParticles::setRandomColor(SVIColor color) {
	mRandomColor = color;
}


void SVIParticles::setDefaultGravity(SVIVector3 gravity) {
	mDefaultGravity = gravity;
}


void SVIParticles::setRandomGravity(SVIVector3 gravity) {
	mRandomGravity = gravity;
}


void SVIParticles::setDefaultForce(SVIVector3 force) {
	mDefaultForce = force;
}


void SVIParticles::setRandomForce(SVIVector3 force) {
	mRandomForce = force;
}


void SVIParticles::setMaxMass(SVIFloat mass) {
	mMaxMass = mass;
}


void SVIParticles::setRandomMass(SVIFloat mass) {
	mRandomMass = mass;
}


void SVIParticles::setScaleKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset) {
	if(mScaleKeyFrameAnimation == NULL) {
		mScaleKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_SCALE, mSVIGLSurface);
		mScaleKeyFrameAnimation->setParticles(this);
	}
	mScaleKeyFrameAnimation->mDuration = duration;
	mScaleKeyFrameAnimation->setInterpolator(type);
	if(reset == 1) {
		mScaleKeyFrameAnimation->keyReset();
	}
}
		

void SVIParticles::setColorKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset) {
	if(mColorKeyFrameAnimation == NULL) {
		mColorKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_COLOR, mSVIGLSurface);
		mColorKeyFrameAnimation->setParticles(this);
	}
	mColorKeyFrameAnimation->mDuration = duration;
	mColorKeyFrameAnimation->setInterpolator(type);
	if(reset == 1) {
		mColorKeyFrameAnimation->keyReset();
	}
}


void SVIParticles::setPositionKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset) {
	if(mPositionKeyFrameAnimation == NULL) {
		mPositionKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_POSITION, mSVIGLSurface);
		mPositionKeyFrameAnimation->setParticles(this);
	}
	mPositionKeyFrameAnimation->mDuration = duration;
	mPositionKeyFrameAnimation->setInterpolator(type);
	if(reset == 1) {
		mPositionKeyFrameAnimation->keyReset();
	}
}


void SVIParticles::addScaleKeyFrame(SVIFloat keyTime, SVIVector3 scale) {
	if(mScaleKeyFrameAnimation == NULL) {
		mScaleKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_SCALE, mSVIGLSurface);
		mScaleKeyFrameAnimation->setParticles(this);
	}
	mScaleKeyFrameAnimation->addKeyProperty( SVIParticleKeyFrameProperty(keyTime, scale) );
}


void SVIParticles::addColorKeyFrame(SVIFloat keyTime, SVIColor color) {
	if(mColorKeyFrameAnimation == NULL) {
		mColorKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_COLOR, mSVIGLSurface);
		mColorKeyFrameAnimation->setParticles(this);
	}
	mColorKeyFrameAnimation->addKeyProperty( SVIParticleKeyFrameProperty(keyTime, color) );
}


void SVIParticles::addPositionKeyFrame(SVIFloat keyTime, SVIVector3 position) {
	if(mPositionKeyFrameAnimation == NULL) {
		mPositionKeyFrameAnimation = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_POSITION, mSVIGLSurface);
		mPositionKeyFrameAnimation->setParticles(this);
	}
	mPositionKeyFrameAnimation->addKeyProperty( SVIParticleKeyFrameProperty(keyTime, position) );
}


SVIParticles* SVIParticles::clone() {
	SVIParticles* cloneParticles = new SVIParticles(mSVIGLSurface);
	cloneParticles->copy(this);
	
	return cloneParticles;
}


void SVIParticles::copy(const SVIParticles* rhs) {
	mBlendType = rhs->mBlendType;
	mUseTexture = rhs->mUseTexture;

	m_pParticleUpdater = rhs->m_pParticleUpdater;

	mValidated = SVIFALSE;

	mParentPosition = rhs->mParentPosition;
	mParentScale = rhs->mParentScale;
	mParentColor = rhs->mParentColor;

	mParticles = NULL;
	mOffset = rhs->mOffset;
	
	mTextureFileName = rhs->mTextureFileName;
	mImage = rhs->mImage;
		
	mParticleCount = rhs->mParticleCount;
	mMaxDuration = rhs->mMaxDuration;
	mRandomDuration = rhs->mRandomDuration;
		
	mDefaultPosition = rhs->mDefaultPosition;
	mRandomPosition = rhs->mRandomPosition;
	mMaxColor = rhs->mMaxColor;
	mRandomColor = rhs->mRandomColor;
	mDefaultGravity = rhs->mDefaultGravity;
	mRandomGravity = rhs->mRandomGravity;
	mDefaultForce = rhs->mDefaultForce;
	mRandomForce = rhs->mRandomForce;
	mMaxMass = rhs->mMaxMass;
	mRandomMass = rhs->mRandomMass;
	mMaxParticleSize = rhs->mMaxParticleSize;
	mRandomParticleSize = rhs->mRandomParticleSize;
		
	mStatus = IDLE;
	
	SVI_SVIFE_DELETE(mScaleKeyFrameAnimation);
	if(rhs->mScaleKeyFrameAnimation != NULL) {
		mScaleKeyFrameAnimation = (SVIParticleKeyFrameAnimation*)rhs->mScaleKeyFrameAnimation->clone();
		mScaleKeyFrameAnimation->setParticles(this);
	}
	
	SVI_SVIFE_DELETE(mColorKeyFrameAnimation);
	if(rhs->mColorKeyFrameAnimation != NULL) {
		mColorKeyFrameAnimation = (SVIParticleKeyFrameAnimation*)rhs->mColorKeyFrameAnimation->clone();
		mColorKeyFrameAnimation->setParticles(this);
	}
	
	SVI_SVIFE_DELETE(mPositionKeyFrameAnimation);
	if(rhs->mPositionKeyFrameAnimation != NULL) {
		mPositionKeyFrameAnimation = (SVIParticleKeyFrameAnimation*)rhs->mPositionKeyFrameAnimation->clone();
		mPositionKeyFrameAnimation->setParticles(this);
	}
}


void SVIParticles::validate() {
	mParticles = new SVIParticle[mParticleCount];

	SVIInt randomValue = 0;
	SVIInt changeValue = 0;

	for(SVIUInt i = 0; i < mParticleCount; ++i) {
		mParticles[i]._active = SVITRUE;

		// set duration
		if(mRandomDuration > 0) {
			randomValue = rand() % mRandomDuration;
			mParticles[i]._life = (mMaxDuration - mRandomDuration) + randomValue;
		}
		else {
			mParticles[i]._life = mMaxDuration;
		}

		if( (SVIInt)mMaxDuration - (SVIInt)mRandomDuration + randomValue < 0 ) {
			mParticles[i]._life = 0;
		}

		// set vertex position
		SVIFloat x, y, z;
		if(mRandomPosition.x > 0.0f) {
			changeValue = SVIInt(mRandomPosition.x * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			x = mDefaultPosition.x + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			x = mDefaultPosition.x;
		}
		if(mRandomPosition.y > 0.0f) {
			changeValue = SVIInt(mRandomPosition.y * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			y = mDefaultPosition.y + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			y = mDefaultPosition.y;
		}
		if(mRandomPosition.z > 0.0f) {
			changeValue = SVIInt(mRandomPosition.z * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			z = mDefaultPosition.z + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			z = mDefaultPosition.z;
		}
		
		mParticles[i]._originPosition.x = x;
		mParticles[i]._originPosition.y = y;
		mParticles[i]._originPosition.z = z;
		
		mParticles[i]._position = mParticles[i]._originPosition + mParentPosition;

		// set color
		if(mRandomColor.mR > 0.0f) {
			changeValue = SVIInt(mRandomColor.mR * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originParticleColor.mR = (mMaxColor.mR - mRandomColor.mR) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originParticleColor.mR = mMaxColor.mR;
		}
		if(mParticles[i]._originParticleColor.mR > 1.0f) {
			mParticles[i]._originParticleColor.mR = 1.0f;
		}
		else if(mParticles[i]._originParticleColor.mR < 0.0f) {
			mParticles[i]._originParticleColor.mR = 0.0f;
		}

		if(mRandomColor.mG > 0.0f) {
			changeValue = SVIInt(mRandomColor.mG * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originParticleColor.mG = (mMaxColor.mG - mRandomColor.mG) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originParticleColor.mG = mMaxColor.mG;
		}
		if(mParticles[i]._originParticleColor.mG > 1.0f) {
			mParticles[i]._originParticleColor.mG = 1.0f;
		}
		else if(mParticles[i]._originParticleColor.mG < 0.0f) {
			mParticles[i]._originParticleColor.mG = 0.0f;
		}

		if(mRandomColor.mB > 0.0f) {
			changeValue = SVIInt(mRandomColor.mB * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originParticleColor.mB = (mMaxColor.mB - mRandomColor.mB) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originParticleColor.mB = mMaxColor.mB;
		}
		if(mParticles[i]._originParticleColor.mB > 1.0f) {
			mParticles[i]._originParticleColor.mB = 1.0f;
		}
		else if(mParticles[i]._originParticleColor.mB < 0.0f) {
			mParticles[i]._originParticleColor.mB = 0.0f;
		}

		if(mRandomColor.mA > 0.0f) {
			changeValue = SVIInt(mRandomColor.mA * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originParticleColor.mA = (mMaxColor.mA - mRandomColor.mA) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originParticleColor.mA = mMaxColor.mA;
		}
		if(mParticles[i]._originParticleColor.mA > 1.0f) {
			mParticles[i]._originParticleColor.mA = 1.0f;
		}
		else if(mParticles[i]._originParticleColor.mA < 0.0f) {
			mParticles[i]._originParticleColor.mA = 0.0f;
		}

		mParticles[i]._particleColor.mR = mParticles[i]._originParticleColor.mR * mParentColor.mR;
		mParticles[i]._particleColor.mG = mParticles[i]._originParticleColor.mG * mParentColor.mG;
		mParticles[i]._particleColor.mB = mParticles[i]._originParticleColor.mB * mParentColor.mB;
		mParticles[i]._particleColor.mA = mParticles[i]._originParticleColor.mA * mParentColor.mA;

		// set gravity
		if(mRandomGravity.x > 0.0f) {
			changeValue = SVIInt(mRandomGravity.x * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._gravity.x = mDefaultGravity.x + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._gravity.x = mDefaultGravity.x;
		}
		
		if(mRandomGravity.y > 0.0f) {
			changeValue = SVIInt(mRandomGravity.y * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._gravity.y = mDefaultGravity.y + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._gravity.y = mDefaultGravity.y;
		}

		if(mRandomGravity.z > 0.0f) {
			changeValue = SVIInt(mRandomGravity.z * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._gravity.z = mDefaultGravity.z + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._gravity.z = mDefaultGravity.z;
		}

		// set force
		if(mRandomForce.x > 0.0f) {
			changeValue = SVIInt(mRandomForce.x * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._force.x = mDefaultForce.x + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._force.x = mDefaultForce.x;
		}

		if(mRandomForce.y > 0.0f) {
			changeValue = SVIInt(mRandomForce.y * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._force.y = mDefaultForce.y + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._force.y = mDefaultForce.y;
		}

		if(mRandomForce.z > 0.0f) {
			changeValue = SVIInt(mRandomForce.z * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._force.z = mDefaultForce.z + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._force.z = mDefaultForce.z;
		}

		// set mass
		if(mRandomMass > 0.0f) {
			changeValue = SVIInt(mRandomMass * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._mass = (mMaxMass - mRandomMass) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._mass = mMaxMass;
		}

		if(mParticles[i]._mass < 0.0f) {
			mParticles[i]._mass = 0.0f;
		}

		// set size
		if(mRandomParticleSize.x > 0.0f) {
			changeValue = SVIInt(mRandomParticleSize.x * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originSize.x = (mMaxParticleSize.x - mRandomParticleSize.x) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originSize.x = mMaxParticleSize.x;
		}

		if(mParticles[i]._originSize.x < 0.0f) {
			mParticles[i]._originSize.x = 0.0f;
		}
		if(mRandomParticleSize.y > 0.0f) {
			changeValue = SVIInt(mRandomParticleSize.y * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originSize.y = (mMaxParticleSize.y - mRandomParticleSize.y) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originSize.y = mMaxParticleSize.y;
		}

		if(mParticles[i]._originSize.y < 0.0f) {
			mParticles[i]._originSize.y = 0.0f;
		}
		if(mRandomParticleSize.z > 0.0f) {
			changeValue = SVIInt(mRandomParticleSize.z * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originSize.z = (mMaxParticleSize.z - mRandomParticleSize.z) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originSize.z = mMaxParticleSize.z;
		}

		if(mParticles[i]._originSize.z < 0.0f) {
			mParticles[i]._originSize.z = 0.0f;
		}

		mParticles[i]._size = mParticles[i]._originSize * mParentScale;
	}

	if(mScaleKeyFrameAnimation != NULL && mMaxDuration < mScaleKeyFrameAnimation->mDuration) {
		mScaleKeyFrameAnimation->mDuration = mMaxDuration;
	}
	if(mColorKeyFrameAnimation != NULL && mMaxDuration < mColorKeyFrameAnimation->mDuration) {
		mColorKeyFrameAnimation->mDuration = mMaxDuration;
	}
	if(mPositionKeyFrameAnimation != NULL && mMaxDuration < mPositionKeyFrameAnimation->mDuration) {
		mPositionKeyFrameAnimation->mDuration = mMaxDuration;
	}

	mValidated = SVITRUE;
}


void SVIParticles::checkMaxDuration(SVIUInt maxDuration) {
	if(mMaxDuration > maxDuration) {
		mMaxDuration = maxDuration;
	}
}


void SVIParticles::update(SVIUInt drawTime) {
	
	if(mStatus == IDLE) {
		validate();
		mStatus = ANIMATING;
		mStartTime = drawTime;
	}
	else if(mStatus == END) {
		return;
	}
	
	if(mUseTexture == SVITRUE) {
		// TODO: bind texture
		// textureFileName = mTextureFileName
	}

	if(mStatus == ANIMATING) {
		SVIUInt activeTime = drawTime - mStartTime;
		SVIFloat fDeltaTime = activeTime / 1000.0f;

		if(mScaleKeyFrameAnimation != NULL) {
			mScaleKeyFrameAnimation->update(drawTime);
		}
		if(mColorKeyFrameAnimation != NULL) {
			mColorKeyFrameAnimation->update(drawTime);
		}
		if(mPositionKeyFrameAnimation != NULL) {
			mPositionKeyFrameAnimation->update(drawTime);
		}

		if (m_pParticleUpdater != NULL){
			for(SVIUInt i = 0; i < mParticleCount; ++i) {
				m_pParticleUpdater->update(&mParticles[i], fDeltaTime);
			}
		}
	}

	if(mStartTime + mMaxDuration < drawTime) {
		mStatus = END;
		// TODO: add kill code !!!
	}
}


void SVIParticles::updateFromAnimation(SVIUInt drawTime, SVIFloat interpolateTime, SVIUInt animationDuration) {
	update(drawTime);
}


void SVIParticles::setUpdater(SVIParticleUpdater * pParticle) {
	m_pParticleUpdater = pParticle;
}		


SVIParticleUpdater * SVIParticles::getUpdater() {
	return m_pParticleUpdater;
}


void SVIParticles::setChangeScale(SVIVector3 scale) {
	if(mValidated == SVITRUE) {
		for(SVIUInt i = 0; i < mParticleCount; ++i) {
			mParticles[i]._size = mParticles[i]._originSize * mParentScale * scale;
		}
	}
}


void SVIParticles::setChangeColor(SVIColor color) {
	if(mValidated == SVITRUE) {
		for(SVIUInt i = 0; i < mParticleCount; ++i) {
			mParticles[i]._particleColor.mR = mParticles[i]._originParticleColor.mR * mParentColor.mR * color.mR;
			mParticles[i]._particleColor.mG = mParticles[i]._originParticleColor.mG * mParentColor.mG * color.mG;
			mParticles[i]._particleColor.mB = mParticles[i]._originParticleColor.mB * mParentColor.mB * color.mB;
			mParticles[i]._particleColor.mA = mParticles[i]._originParticleColor.mA * mParentColor.mA * color.mA;
		}
	}
}


void SVIParticles::setChangePosition(SVIVector3 position) {
	if(mValidated == SVITRUE) {
		for(SVIUInt i = 0; i < mParticleCount; ++i) {
			mParticles[i]._position = mParticles[i]._originPosition + mParentPosition + position;
		}
	}
}


void SVIParticles::setChangeParentScale(SVIVector3 scale) {
	mParentScale = scale;
	if(mValidated == SVITRUE) {
		for(SVIUInt i = 0; i < mParticleCount; ++i) {
			mParticles[i]._size = mParticles[i]._originSize * mParentScale;
		}
	}
}


void SVIParticles::setChangeParentColor(SVIColor color) {
	mParentColor = color;
	if(mValidated == SVITRUE) {
		for(SVIUInt i = 0; i < mParticleCount; ++i) {
			mParticles[i]._particleColor.mR = mParticles[i]._originParticleColor.mR * mParentColor.mR;
			mParticles[i]._particleColor.mG = mParticles[i]._originParticleColor.mG * mParentColor.mG;
			mParticles[i]._particleColor.mB = mParticles[i]._originParticleColor.mB * mParentColor.mB;
			mParticles[i]._particleColor.mA = mParticles[i]._originParticleColor.mA * mParentColor.mA;
		}
	}
}


void SVIParticles::setChangeParentPosition(SVIVector3 position) {
	mParentPosition = position;
	if(mValidated == SVITRUE) {
		for(SVIUInt i = 0; i < mParticleCount; ++i) {
			mParticles[i]._position = mParticles[i]._originPosition + mParentPosition;
		}
	}
}


} // end namespace SVI