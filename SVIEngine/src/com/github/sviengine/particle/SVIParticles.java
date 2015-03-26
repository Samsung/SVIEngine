package com.github.sviengine.particle;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.SVIEngineThreadProtection;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.particle.SVIParticles;

/**
 * particles information
 */
public class SVIParticles {
	public interface ParticleBlendType {
		public static final int BLEND_MULTIPLY = 0;
		public static final int BLEND_ADD = 1;
	}
	
	public SVIParticles() {
		this(null);
	}
	
	public SVIParticles(SVIGLSurface surface) {
		surface = SVIGLSurface.getSurface(surface);
		mNativeHandle = -1;
		
		initialize();
		
		mNativeHandle = nativeCreateParticles(surface.getNativeHandle());
	}
	
	
	protected void initialize() {
		SVIEngineThreadProtection.validateMainThread();
		mParticleCount = 0;
		mTextureFileName = null;
		mMaxDuration = 0;
		mRandomDuration = 0;
		
		mBlendType = ParticleBlendType.BLEND_MULTIPLY;
		
		mScaleKeyFrameDuration = 0;
		mScaleKeyFrameInterpolaterType = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
		mColorKeyFrameDuration = 0;
		mColorKeyFrameInterpolaterType = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
		mPositionKeyFrameDuration = 0;
		mPositionKeyFrameInterpolaterType = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
		
		/// create member field
		mDefaultPosition = new SVIVector3();
		mRandomPosition = new SVIVector3();
		mMaxColor = new SVIColor();
		mRandomColor = new SVIColor();
		mDefaultGravity = new SVIVector3();
		mRandomGravity = new SVIVector3();
		mDefaultForce = new SVIVector3();
		mRandomForce = new SVIVector3();
		mMaxParticleSize = new SVIVector3();
		mRandomParticleSize = new SVIVector3();
		
		/// set member field value
		mDefaultPosition.mX = mDefaultPosition.mY = mDefaultPosition.mZ = 0.0f;
		mRandomPosition.mX = mRandomPosition.mY = mRandomPosition.mZ = 0.0f;
		mMaxColor.mR = mMaxColor.mG = mMaxColor.mB = mMaxColor.mA = 1.0f;
		mRandomColor.mR = mRandomColor.mG = mRandomColor.mB = mRandomColor.mA = 0.0f;
		mDefaultGravity.mX = mDefaultGravity.mY = mDefaultGravity.mZ = 0.0f;
		mRandomGravity.mX = mRandomGravity.mY = mRandomGravity.mZ = 0.0f;
		mDefaultForce.mX = mDefaultForce.mY = mDefaultForce.mZ = 0.0f;
		mRandomForce.mX = mRandomForce.mY = mRandomForce.mZ = 0.0f;
		mMaxMass = 1.0f;
		mRandomMass = 0.0f;
		mMaxParticleSize.mX = mMaxParticleSize.mY = mMaxParticleSize.mZ = 1.0f;
		mRandomParticleSize.mX = mRandomParticleSize.mY = mRandomParticleSize.mZ = 0.0f;
		mImage = null;
		
		mScaleKeyFrameDuration = 0;
		mScaleKeyFrameInterpolaterType = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
		mColorKeyFrameDuration = 0;
		mColorKeyFrameInterpolaterType = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
		mPositionKeyFrameDuration = 0;
		mPositionKeyFrameInterpolaterType = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
	}
	
	
	protected void finalize() throws Throwable {
        if (mNativeHandle != -1) {
            mImage = null;
            //nativeDestroy(mNativeHandle);
            mNativeHandle = -1;
        }
		super.finalize();
    }
	
	
	
	public void copy(SVIParticles info) {
		info.mParticleCount = mParticleCount;
		info.mTextureFileName = mTextureFileName;
		info.mMaxDuration = mMaxDuration;
		info.mRandomDuration = mRandomDuration;
		
		info.mScaleKeyFrameDuration = mScaleKeyFrameDuration;
		info.mScaleKeyFrameInterpolaterType = mScaleKeyFrameInterpolaterType;
		info.mColorKeyFrameDuration = mColorKeyFrameDuration;
		info.mColorKeyFrameInterpolaterType = mColorKeyFrameInterpolaterType;
		info.mPositionKeyFrameDuration = mPositionKeyFrameDuration;
		info.mPositionKeyFrameInterpolaterType = mPositionKeyFrameInterpolaterType;
		
		info.mDefaultPosition.mX = mDefaultPosition.mX;
		info.mDefaultPosition.mY = mDefaultPosition.mY;
		info.mDefaultPosition.mZ = mDefaultPosition.mZ;
		info.mRandomPosition.mX = mRandomPosition.mX;
		info.mRandomPosition.mY = mRandomPosition.mY;
		info.mRandomPosition.mZ = mRandomPosition.mZ;
		info.mMaxColor.mR = mMaxColor.mR;
		info.mMaxColor.mG = mMaxColor.mG;
		info.mMaxColor.mB = mMaxColor.mB;
		info.mMaxColor.mA = mMaxColor.mA;
		info.mRandomColor.mR = mRandomColor.mR;
		info.mRandomColor.mG = mRandomColor.mG;
		info.mRandomColor.mB = mRandomColor.mB;
		info.mRandomColor.mA = mRandomColor.mA;
		info.mDefaultGravity.mX = mDefaultGravity.mX;
		info.mDefaultGravity.mY = mDefaultGravity.mY;
		info.mDefaultGravity.mZ = mDefaultGravity.mZ;
		info.mRandomGravity.mX = mRandomGravity.mX;
		info.mRandomGravity.mY = mRandomGravity.mY;
		info.mRandomGravity.mZ = mRandomGravity.mZ;
		info.mDefaultForce.mX = mDefaultForce.mX;
		info.mDefaultForce.mY = mDefaultForce.mY;
		info.mDefaultForce.mZ = mDefaultForce.mZ;
		info.mRandomForce.mX = mRandomForce.mX;
		info.mRandomForce.mY = mRandomForce.mY;
		info.mRandomForce.mZ = mRandomForce.mZ;
		info.mMaxMass = mMaxMass;
		info.mRandomMass = mRandomMass;
		info.mMaxParticleSize.mX = mMaxParticleSize.mX;
		info.mMaxParticleSize.mY = mMaxParticleSize.mY;
		info.mMaxParticleSize.mZ = mMaxParticleSize.mZ;
		info.mRandomParticleSize.mX = mRandomParticleSize.mX;
		info.mRandomParticleSize.mY = mRandomParticleSize.mY;
		info.mRandomParticleSize.mZ = mRandomParticleSize.mZ;
		info.mImage = mImage;
		
		mScaleKeyFrameDuration = 0;
		mScaleKeyFrameInterpolaterType = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
		mColorKeyFrameDuration = 0;
		mColorKeyFrameInterpolaterType = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
		mPositionKeyFrameDuration = 0;
		mPositionKeyFrameInterpolaterType = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
	}
	
	
	public void	setTextureFile(String textureFileName, SVIImage image) {
		SVIEngineThreadProtection.validateMainThread();
		mTextureFileName = textureFileName;
		mImage = image;
		
		nativeSetTextureFile(mNativeHandle, mTextureFileName, mImage.getNativeHandle());
	}
	
	
	public void	setParticleCount(int particleCount) {
		SVIEngineThreadProtection.validateMainThread();
		mParticleCount = particleCount;
		
		nativeSetParticleCount(mNativeHandle, mParticleCount);
	}
	
	
	public void setMaxDuration(int duration) {
		SVIEngineThreadProtection.validateMainThread();
		mMaxDuration = duration;
		
		nativeSetMaxDuration(mNativeHandle, duration);
	}
	
	
	public void setRandomDuration(int duration) {
		SVIEngineThreadProtection.validateMainThread();
		mRandomDuration = duration;
		
		nativeSetRandomDuration(mNativeHandle, duration);
	}
	
	
	public void setMaxSize(float x, float y, float z) {
		SVIEngineThreadProtection.validateMainThread();
		mMaxParticleSize.mX = x;
		mMaxParticleSize.mY = y;
		mMaxParticleSize.mZ = z;
		
		nativeSetMaxSize(mNativeHandle, x, y, z);
	}
	
	
	public void setRandomSize(float x, float y, float z) {
		SVIEngineThreadProtection.validateMainThread();
		mRandomParticleSize.mX = x;
		mRandomParticleSize.mY = y;
		mRandomParticleSize.mZ = z;
		
		nativeSetRandomSize(mNativeHandle, x, y, z);
	}
	
	
	public void setDefaultPosition(float x, float y, float z) {
		SVIEngineThreadProtection.validateMainThread();
		mDefaultPosition.mX = x;
		mDefaultPosition.mY = y;
		mDefaultPosition.mZ = z;
		
		nativeSetDefaultPosition(mNativeHandle, x, y, z);
	}
	
	
	public void setRandomPosition(float x, float y, float z) {
		SVIEngineThreadProtection.validateMainThread();
		mRandomPosition.mX = x;
		mRandomPosition.mY = y;
		mRandomPosition.mZ = z;
		
		nativeSetRandomPosition(mNativeHandle, x, y, z);
	}
	
	
	public void setMaxColor(float r, float g, float b, float a) {
		SVIEngineThreadProtection.validateMainThread();
		mMaxColor.mR = r;
		mMaxColor.mG = g;
		mMaxColor.mB = b;
		mMaxColor.mA = a;
		
		nativeSetMaxColor(mNativeHandle, r, g, b, a);
	}
	
	
	public void setRandomColor(float r, float g, float b, float a) {
		SVIEngineThreadProtection.validateMainThread();
		mRandomColor.mR = r;
		mRandomColor.mG = g;
		mRandomColor.mB = b;
		mRandomColor.mA = a;
		
		nativeSetRandomColor(mNativeHandle, r, g, b, a);
	}
	
	
	public void setDefaultGravity(float x, float y, float z) {
		SVIEngineThreadProtection.validateMainThread();
		mDefaultGravity.mX = x;
		mDefaultGravity.mY = y;
		mDefaultGravity.mZ = z;
		
		nativeSetDefaultGravity(mNativeHandle, x, y, z);
	}
	
	
	public void setRandomGravity(float x, float y, float z) {
		SVIEngineThreadProtection.validateMainThread();
		mRandomGravity.mX = x;
		mRandomGravity.mY = y;
		mRandomGravity.mZ = z;
		
		nativeSetRandomGravity(mNativeHandle, x, y, z);
	}
	
	
	public void setDefaultForce(float x, float y, float z) {
		SVIEngineThreadProtection.validateMainThread();
		mDefaultForce.mX = x;
		mDefaultForce.mY = y;
		mDefaultForce.mZ = z;
		
		nativeSetDefaultForce(mNativeHandle, x, y, z);
	}
	
	
	public void setRandomForce(float x, float y, float z) {
		SVIEngineThreadProtection.validateMainThread();
		mRandomForce.mX = x;
		mRandomForce.mY = y;
		mRandomForce.mZ = z;
		
		nativeSetRandomForce(mNativeHandle, x, y, z);
	}
	
	
	public void setMaxMass(float mass) {
		SVIEngineThreadProtection.validateMainThread();
		mMaxMass = mass;
		
		nativeSetMaxMass(mNativeHandle, mass);
	}
	
	
	public void setRandomMass(float mass) {
		SVIEngineThreadProtection.validateMainThread();
		mRandomMass = mass;
		
		nativeSetRandomMass(mNativeHandle, mass);
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
		SVIEngineThreadProtection.validateMainThread();
		nativeAddPositionKeyFrame(mNativeHandle, keyTime, position.mX, position.mY, position.mZ);
	}
	
	
	public int getNativeHandle() {
		return mNativeHandle;
	}
	
	protected int							mParticleCount;

	protected int							mMaxDuration;
	protected int							mRandomDuration;

	protected int							mBlendType;
	
	protected int							mScaleKeyFrameDuration;
	protected int							mScaleKeyFrameInterpolaterType;
	
	protected int							mColorKeyFrameDuration;
	protected int							mColorKeyFrameInterpolaterType;
	
	protected int							mPositionKeyFrameDuration;
	protected int							mPositionKeyFrameInterpolaterType;
	
	protected SVIVector3						mDefaultPosition;
	protected SVIVector3						mRandomPosition;

	protected SVIColor						mMaxColor;
	protected SVIColor						mRandomColor;

	protected SVIVector3						mDefaultGravity;
	protected SVIVector3						mRandomGravity;

	protected SVIVector3						mDefaultForce;
	protected SVIVector3						mRandomForce;
	
	protected float							mMaxMass;
	protected float							mRandomMass;
	
	protected SVIVector3						mMaxParticleSize;
	protected SVIVector3						mRandomParticleSize;
	
	protected String						mTextureFileName;
	protected SVIImage						mImage;
	
	private int 							mNativeHandle;
	
	
	/*****************************************************************************************************************/
	/**
	  * SVIParticles native interface.
	  *
	  */
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}
	
	private static native int nativeCreateParticles(int surfaceNativeHandle);
	
	protected static native void nativeSetTextureFile(int particlesHandle, String textureFileName, int imageHandle);
	
	protected static native void nativeSetParticleCount(int particlesHandle, int particleCount);
	
	protected static native void nativeSetMaxDuration(int particlesHandle, int duration);
	protected static native void nativeSetRandomDuration(int particlesHandle, int duration);
	
	protected static native void nativeSetMaxSize(int particlesHandle, float x, float y, float z);
	protected static native void nativeSetRandomSize(int particlesHandle, float x, float y, float z);
	
	protected static native void nativeSetDefaultPosition(int particlesHandle, float x, float y, float z);
	protected static native void nativeSetRandomPosition(int particlesHandle, float x, float y, float z);
	
	protected static native void nativeSetMaxColor(int particlesHandle, float r, float g, float b, float a);
	protected static native void nativeSetRandomColor(int particlesHandle, float r, float g, float b, float a);
	
	protected static native void nativeSetDefaultGravity(int particlesHandle, float x, float y, float z);
	protected static native void nativeSetRandomGravity(int particlesHandle, float x, float y, float z);
	
	protected static native void nativeSetDefaultForce(int particlesHandle, float x, float y, float z);
	protected static native void nativeSetRandomForce(int particlesHandle, float x, float y, float z);
	
	protected static native void nativeSetMaxMass(int particlesHandle, float mass);
	protected static native void nativeSetRandomMass(int particlesHandle, float mass);
	
	protected static native void nativeSetScaleKeyFrameAnimationProperty(int particlesHandle, int duration, int interpolatorType, int reset);
	
	protected static native void nativeSetColorKeyFrameAnimationProperty(int particlesHandle, int duration, int interpolatorType, int reset);
	
	protected static native void nativeSetPositionKeyFrameAnimationProperty(int particlesHandle, int duration, int interpolatorType, int reset);
	
	protected static native void nativeAddScaleKeyFrame(int particlesHandle, float keyTime, float x, float y, float z);
	
	protected static native void nativeAddColorKeyFrame(int particlesHandle, float keyTime, float r, float g, float b, float a);
	
	protected static native void nativeAddPositionKeyFrame(int particlesHandle, float keyTime, float x, float y, float z);
	
} // end definition SVIParticlesInfo
