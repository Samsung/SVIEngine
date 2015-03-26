#ifndef __SVI_PARTICLES_H_
#define __SVI_PARTICLES_H_

#include "../SVICores.h"
#include "../Animation/SVIAnimation.h"
#include "../GLSurface/SVIGLSurface.h"

#include "./Updater/SVIParticleUpdater.h"
#include <string>

namespace SVI {

	class SVIParticleKeyFrameAnimation;

	// Creation Date	: 2012/06/04 rareboy0112@facebook.com
	// Description		: particles contain class
	// Last Modified	: 2012/06/21 rareboy0112@facebook.com
	
	//2012-06-13 masterkeaton27@gmail.comseperated from SVIParticles class
	struct SVIParticle {
	public:
		SVIParticle();
	
	public:
		SVIBool		_active;
		SVIUInt		_life;
        SVIVector3	_size;
        SVIVector3	_originSize;
        SVIVector3	_position;
        SVIVector3	_originPosition;
        SVIVector3	_velocity;
        SVIColor		_originParticleColor;
        SVIColor		_particleColor;
        SVIVector3	_gravity;
        SVIVector3	_force;
        SVIFloat		_mass;
    };

	class SVIParticles {
	public:
		
		enum ParticleBlendType {
            BLEND_MULTIPLY = 0, 
			BLEND_ADD = 1,
			BLEND_COUNT
		};


		enum ParticlesStatus {
			IDLE, 
			START, 
			ANIMATING, 
			END,
			STATUS_COUNT
		};

	public:
		SVIParticles(SVIGLSurface *surface);
		~SVIParticles();
		void					setTextureFileName(std::string fileName, SVIImage* image);
		void					setParticleCount(SVIUInt particleCount);
		
		void					setMaxDuration(SVIUInt duration);
		void					setRandomDuration(SVIUInt duration);

		void					setMaxSize(SVIVector3 size);
		void					setRandomSize(SVIVector3 size);
		
		void					setDefaultPosition(SVIVector3 pos);
		void					setRandomPosition(SVIVector3 pos);

		void					setMaxColor(SVIColor color);
		void					setRandomColor(SVIColor color);
		
		void					setDefaultGravity(SVIVector3 gravity);
		void					setRandomGravity(SVIVector3 gravity);
		
		void					setDefaultForce(SVIVector3 force);
		void					setRandomForce(SVIVector3 force);
		
		void					setMaxMass(SVIFloat mass);
		void					setRandomMass(SVIFloat mass);
		
		virtual void			validate();
		
		void					checkMaxDuration(SVIUInt maxDuration);

		virtual void			update(SVIUInt drawTime);
		virtual void			updateFromAnimation(SVIUInt drawTime, SVIFloat interpolateTime, SVIUInt animationDuration);
		
		void					setChangeScale(SVIVector3 scale);
		void					setChangeColor(SVIColor color);
		void					setChangePosition(SVIVector3 position);

		void					setChangeParentScale(SVIVector3 scale);
		void					setChangeParentColor(SVIColor color);
		void					setChangeParentPosition(SVIVector3 position);

		SVIParticle*				getParticles() { return mParticles; }
		SVIUInt			        getParticleCount() { return mParticleCount; }
		std::string		        getTextureFileName() { return mTextureFileName; }
		
		void					setStatus(ParticlesStatus status) { mStatus = status; }
		ParticlesStatus	        getStatus() { return mStatus; }
		SVIUInt					getDuration() { return mMaxDuration; }

		void					setScaleKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset);
		void					setColorKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset);
		void					setPositionKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset);

		void					addScaleKeyFrame(SVIFloat keyTime, SVIVector3 scale);
		void					addColorKeyFrame(SVIFloat keyTime, SVIColor color);
		void					addPositionKeyFrame(SVIFloat keyTime, SVIVector3 position);

		virtual SVIParticles*	clone();
		void					copy(const SVIParticles* rhs);

	private:
		SVIParticles(const SVIParticles& rhs){}
		void operator=(const SVIParticles& rhs){}
	public:
		//2012-06-13 masterkeaton27@gmail.com
		void					setUpdater(SVIParticleUpdater * pParticle);
		SVIParticleUpdater *		getUpdater();

	public:
		ParticleBlendType				mBlendType;
		SVIBool							mUseTexture;

	protected:
		//2012-06-13 masterkeaton27@gmail.com
		SVIParticleUpdater *				m_pParticleUpdater;

	protected:
		SVIBool							mValidated;

		SVIVector3						mParentPosition;
		SVIVector3						mParentScale;
		SVIColor							mParentColor;

		SVIParticle*						mParticles;
		SVIUInt							mOffset;
		SVIUInt							mStartTime;

		std::string						mTextureFileName;
		SVIImage*						mImage;
		
		SVIUInt							mParticleCount;
		SVIUInt							mMaxDuration;
		SVIUInt							mRandomDuration;
		
		SVIVector3						mDefaultPosition;
		SVIVector3						mRandomPosition;
		SVIColor							mMaxColor;
		SVIColor							mRandomColor;
		SVIVector3						mDefaultGravity;
		SVIVector3						mRandomGravity;
		SVIVector3						mDefaultForce;
		SVIVector3						mRandomForce;
		SVIFloat							mMaxMass;
		SVIFloat							mRandomMass;
		SVIVector3						mMaxParticleSize;
		SVIVector3						mRandomParticleSize;
		
		ParticlesStatus					mStatus;
		
		SVIParticleKeyFrameAnimation*	mScaleKeyFrameAnimation;
		SVIParticleKeyFrameAnimation*	mColorKeyFrameAnimation;
		SVIParticleKeyFrameAnimation*	mPositionKeyFrameAnimation;

		SVIGLSurface* mSVIGLSurface;
	};

} // end namespace SVI

#endif //__SVI_PARTICLES_H_
