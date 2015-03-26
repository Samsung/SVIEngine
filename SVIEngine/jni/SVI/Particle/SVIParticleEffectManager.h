#ifndef __SVI_PARTICLE_EFFECT_MANAGER_H_
#define __SVI_PARTICLE_EFFECT_MANAGER_H_

#include "../SVICores.h"
#include "../SVIDefines.h"
#include "../BaseType/SVIExtendTypes.h"
#include "SVIParticleEffect.h"
//#include "SVIKeyFrameParticlesInfo.h"
//#include "SVIKeyFrameParticleEffectInfo.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "../GLSurface/SVIGLSurface.h"

namespace SVI {

	typedef std::vector<std::string> ParticleTextureNameList;

	// Creation Date	: 2012/06/04 rareboy0112@facebook.com
	// Description		: particle effect management class
	// Last Modified	: 2012/06/21 rareboy0112@facebook.com
	class SVIParticleEffectManager {

	public:
		SVIParticleEffectManager(SVIGLSurface* mSVIGLSurface);//Multi-Instance-Support
		virtual ~SVIParticleEffectManager() {}

		SVIBool	isExistTexture(std::string textureFileName);
		void	addTexture(std::string textureFileName, SVIImage *image);
		
		void	pause();
		void	resume();
		void	requestRender();

		void	update(SVIUInt drawTime);
		SVIUInt	getDrawTime();

		//SVIParticleEffectInfo*			parsingParticleEffectInfo(JNIEnv *env, jobject particleEffectInfo);
		//SVIKeyFrameParticleEffectInfo*	parsingKeyFrameParticleEffectInfo(JNIEnv *env, jobject particleEffectInfo);

		//SVIParticleEffect*				createParticleEffectFromInfo(SVIParticleEffectInfo* info);

		//2012-06-23 masterkeaton27@gmail.com
		void finalize();
	private:
		SVIGLSurface * mSVIGLSurface;//Multi-Instance-Support
		ParticleTextureNameList			mTextureFileNames;
		SVIUInt							mDrawTime;
	};

};

#endif //__SVI_PARTICLE_EFFECT_MANAGER_H_
