package com.github.sviengine.particle;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.SVIEngineThreadProtection;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.glsurface.SVIGLSurface;

/**
 * @author rareboy0112@facebook.com
 * particlesEffect information
 */
public class SVIParticleEffect {
	
	/**
     * particle effect information type
     */
    public interface ParticleEffectType {
        public static final int PARTICLE_EFFECT_NONE = 0;
        public static final int PARTICLE_EFFECT_DEFAULT = 1;
        public static final int PARTICLE_EFFECT_KEYFRAME = 2;
    }
    
    public SVIParticleEffect() {
    	this(null);
    }
    
	public SVIParticleEffect(SVIGLSurface surface) {
		surface = SVIGLSurface.getSurface(surface);
		mSurfaceNativeHandle = surface.getNativeHandle();
		initialize();
		mNativeHandle = nativeCreateParticleEffect(mSurfaceNativeHandle);
	}
	
	
	protected void initialize() {
		SVIEngineThreadProtection.validateMainThread();
		mNativeHandle = -1;
		mPosition = new SVIVector3();
		mScale = new SVIVector3();
		mColor = new SVIColor();
		
		mPosition.mX = mPosition.mY = mPosition.mZ = 0.0f;
		mScale.mX = mScale.mY = mScale.mZ = 1.0f;
		mColor.mR = mColor.mG = mColor.mB = mColor.mA = 1.0f;
	}
	
	
	protected void finalize() throws Throwable {
		if (mNativeHandle != -1) {
			//nativeDestroy(mNativeHandle);
			mNativeHandle = -1;
        }
		super.finalize();
    }
	
	
	public int getType() {
		return ParticleEffectType.PARTICLE_EFFECT_DEFAULT;
	}
	
	
	public void setDuration(int duration) {
		SVIEngineThreadProtection.validateMainThread();
		mDuration = duration;
		
		nativeSetDuration(mNativeHandle, mDuration);
	}
	
	
	public void setPosition(SVIVector3 position) {
		mPosition.mX = position.mX;
		mPosition.mY = position.mY;
		mPosition.mZ = position.mZ;
		
		setPosition(mPosition.mX, mPosition.mY, mPosition.mZ);
	}
	
	
	public void setPosition(float x, float y, float z) {
		SVIEngineThreadProtection.validateMainThread();
		mPosition.mX = x;
		mPosition.mY = y;
		mPosition.mZ = z;
		
		nativeSetPosition(mNativeHandle, x, y, z);
	}
	
	
	public void setScale(SVIVector3 scale) {
		mScale.mX = scale.mX;
		mScale.mY = scale.mY;
		mScale.mZ = scale.mZ;
		
		setScale(mScale.mX, mScale.mY, mScale.mZ);
	}
	
	
	public void setScale(float x, float y, float z) {
		SVIEngineThreadProtection.validateMainThread();
		mScale.mX = x;
		mScale.mY = y;
		mScale.mZ = z;
		
		nativeSetScale(mNativeHandle, x, y, z);
	}
	
	
	
	public void setColor(SVIColor color) {
		mColor.mR = color.mR;
		mColor.mG = color.mG;
		mColor.mB = color.mB;
		mColor.mA = color.mA;
		
		setColor(mColor.mR, mColor.mG, mColor.mB, mColor.mA);
	}
	
	
	public void setColor(float r, float g, float b, float a) {
		SVIEngineThreadProtection.validateMainThread();
		mColor.mR = r;
		mColor.mG = g;
		mColor.mB = b;
		mColor.mA = a;
		
		nativeSetColor(mNativeHandle, r, g, b, a);
	}
	
	
	public void	setScaleKeyFrameAnimationProperty(int duration, int interpolatorType, boolean keyValueReset) {
		SVIEngineThreadProtection.validateMainThread();
		mScaleKeyFrameDuration = duration;
		mScaleKeyFrameInterpolaterType = interpolatorType;
		
		int reset = 0;
		if(keyValueReset == true)
			reset = 1;
		
		nativeSetScaleKeyFrameAnimationProperty(mNativeHandle, mScaleKeyFrameDuration, mScaleKeyFrameInterpolaterType, reset);
	}
	
	public void	setColorKeyFrameAnimationProperty(int duration, int interpolatorType, boolean keyValueReset) {
		SVIEngineThreadProtection.validateMainThread();
		mColorKeyFrameDuration = duration;
		mColorKeyFrameInterpolaterType = interpolatorType;
		
		int reset = 0;
		if(keyValueReset == true)
			reset = 1;
		
		nativeSetColorKeyFrameAnimationProperty(mNativeHandle, mColorKeyFrameDuration, mColorKeyFrameInterpolaterType, reset);
	}
	
	
	public void setPositionKeyFrameAnimationProperty(int duration, int interpolatorType, boolean keyValueReset) {
		SVIEngineThreadProtection.validateMainThread();
		mPositionKeyFrameDuration = duration;
		mPositionKeyFrameInterpolaterType = interpolatorType;
		
		int reset = 0;
		if(keyValueReset == true)
			reset = 1;
		
		nativeSetPositionKeyFrameAnimationProperty(mNativeHandle, mPositionKeyFrameDuration, mPositionKeyFrameInterpolaterType, reset);
	}
	
	
	public void addScaleKeyFrame(float keyTime, SVIVector3 scale) {
		SVIEngineThreadProtection.validateMainThread();
		nativeAddScaleKeyFrame(mNativeHandle, keyTime, scale.mX, scale.mY, scale.mZ);
	}
	
	
	public void addColorKeyFrame(float keyTime, SVIColor color) {
		SVIEngineThreadProtection.validateMainThread();
		nativeAddColorKeyFrame(mNativeHandle, keyTime, color.mR, color.mG, color.mB, color.mA);
	}
	
	
	public void addPositionKeyFrame(float keyTime, SVIVector3 position) {
		nativeAddPositionKeyFrame(mNativeHandle, keyTime, position.mX, position.mY, position.mZ);
	}
	
	
	
	public void addParticles(SVIParticles particles) {
		SVIEngineThreadProtection.validateMainThread();
		nativeAddParticles(mNativeHandle, particles.getNativeHandle());
	}
	
	
	public int getNativeHandle() {
		return mNativeHandle;
	}
	
	
	protected int					mDuration;
	
	protected int					mScaleKeyFrameDuration;
	protected int					mScaleKeyFrameInterpolaterType;
	
	protected int					mColorKeyFrameDuration;
	protected int					mColorKeyFrameInterpolaterType;
	
	protected int					mPositionKeyFrameDuration;
	protected int					mPositionKeyFrameInterpolaterType;
	
	protected SVIVector3				mPosition;
	protected SVIVector3				mScale;
	protected SVIColor				mColor;
	
	protected int					mNativeHandle;
	protected int 					mSurfaceNativeHandle;
	
	
	
	/*****************************************************************************************************************/
	/**
	  * SVIParticleEffect native interface.
	  *
	  */
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}
	
	private native int nativeCreateParticleEffect(int surfaceNativeHandle);
	
	protected native void nativeAddParticles(int effectHandle, int particlesHandle);
	
	protected native void nativeSetDuration(int effectHandle, int duration);
	
	protected native void nativeSetPosition(int effectHandle, float x, float y, float z);
	
	protected native void nativeSetScale(int effectHandle, float x, float y, float z);
	
	protected native void nativeSetColor(int effectHandle, float r, float g, float b, float a);
	
	protected native void nativeSetScaleKeyFrameAnimationProperty(int effectHandle, int duration, int interpolatorType, int reset);
	
	protected native void nativeSetColorKeyFrameAnimationProperty(int effectHandle, int duration, int interpolatorType, int reset);
	
	protected native void nativeSetPositionKeyFrameAnimationProperty(int effectHandle, int duration, int interpolatorType, int reset);
	
	protected native void nativeAddScaleKeyFrame(int effectHandle, float keyTime, float x, float y, float z);
	
	protected native void nativeAddColorKeyFrame(int effectHandle, float keyTime, float r, float g, float b, float a);
	
	protected native void nativeAddPositionKeyFrame(int effectHandle, float keyTime, float x, float y, float z);

} // end definition SVIParticlesEffectInfo
