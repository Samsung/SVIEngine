package com.github.sviengine.particle;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.glsurface.SVIGLSurface;


public class SVIKeyFrameParticleEffect extends SVIParticleEffect {
	
	public SVIKeyFrameParticleEffect() {
		this(null);
	}
	
	public SVIKeyFrameParticleEffect(SVIGLSurface surface) {
		super(surface);
		initialize();
		mNativeHandle = nativeCreateKeyFrameParticleEffect();
	}
	
	
	protected void finalize() throws Throwable {
		if (mNativeHandle != -1) {
			//nativeDestroy(mNativeHandle);
			mNativeHandle = -1;
        }
		super.finalize();
    }
	
	@Override
	public int getType() {
		return ParticleEffectType.PARTICLE_EFFECT_KEYFRAME;
	}
	
	
	@Override
	public void addParticles(SVIParticles particles) {
		addParticles(0.0f, particles);
	}
	
	
	public void addParticles(float keyTime, SVIParticles particles) {
		nativeAddParticles( mNativeHandle, keyTime, particles.getNativeHandle() );
	}
	
	
	public void setGeneratorPositionKeyFrameAnimationProperty(int duration, int interpolatorType, boolean keyValueReset) {
		mGeneratorPositionKeyFrameDuration = duration;
		mGeneratorPositionKeyFrameInterpolaterType = interpolatorType;
		
		int reset = 0;
		if(keyValueReset == true)
			reset = 1;
		
		nativeSetGeneratorPositionKeyFrameAnimationProperty(mNativeHandle, mGeneratorPositionKeyFrameDuration, mGeneratorPositionKeyFrameInterpolaterType, reset);
	}
	
	
	public void addGeneratorPositionKeyFrame(float keyTime, SVIVector3 pos) {
		nativeAddGeneratorPositionKeyFrame(mNativeHandle, keyTime, pos.mX, pos.mY, pos.mZ);
	}
	
	
	protected int	mGeneratorPositionKeyFrameDuration;
	protected int	mGeneratorPositionKeyFrameInterpolaterType;
	
		
	/*****************************************************************************************************************/
	/**
	  * SVIKeyFrameParticleEffect native interface.
	  *
	  */
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}
	
	private static native int nativeCreateKeyFrameParticleEffect();
	
	protected static native void nativeAddParticles(int effectHandle, float keyTime, int particlesHandle);
	
	protected static native void nativeSetGeneratorPositionKeyFrameAnimationProperty(int effectHandle, int duration, int interpolatorType, int reset);
	
	protected static native void nativeAddGeneratorPositionKeyFrame(int effectHandle, float keyTime, float x, float y, float z);
}

