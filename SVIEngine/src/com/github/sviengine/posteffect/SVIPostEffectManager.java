package com.github.sviengine.posteffect;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.slide.SVISlideManager;

public class SVIPostEffectManager {
	
	public static final int POST_EFFECT_BLOOM = 0;
	public static final int POST_EFFECT_RIPPLE = 1;
		
	private int mEffectType = POST_EFFECT_RIPPLE; 
	private boolean mEnable = false;

	private int mNativeSVIGLSurfaceHandle = 0;
	
	private  SVIRippleListener mRippleListener = null;
	
	public SVIPostEffectManager() {
		this(null);
	}

	public SVIPostEffectManager(SVIGLSurface surface) {
		surface = SVIGLSurface.getSurface(surface);
		mNativeSVIGLSurfaceHandle  = surface.getNativeHandle();
	}
	
	public static SVIPostEffectManager getInstance() {
		return SVIGLSurface.getSurface(null).getPostEffectManager();
	}

	 public void setType(int type){
		mEffectType = type;
		nativeSetType(mNativeSVIGLSurfaceHandle, mEffectType);
	}
	
	public int getType(){
		return mEffectType;
	}
	
	 public void enable(boolean isEnable){
		mEnable = isEnable;
		nativeEnable(mNativeSVIGLSurfaceHandle, mEnable ? 1 : 0);
	}
	
	public boolean getEnable(){
		return mEnable;
	}
	
	public void resetRipple(){
		nativeResetRipple(mNativeSVIGLSurfaceHandle);
	}
	
	public void setRipple(float x, float y, float intensity){
		nativeSetRipple(mNativeSVIGLSurfaceHandle, x,y,intensity);
	}
	
	public void setRippleInfo(float reductionRatio, float cutThreshold){
		nativeSetRippleInfo(mNativeSVIGLSurfaceHandle, reductionRatio, cutThreshold);
	}
	
	public void setRippleEnvironmentImage(SVIImage image){
		nativeSetRippleEnvImage(mNativeSVIGLSurfaceHandle, image.getNativeHandle());
	}
		
	public void onRippleFinished(){
		if (mRippleListener != null) mRippleListener.onRippleFinished();
	}
	
	public void setRippleListener(SVIRippleListener listener){
		mRippleListener = listener;
	}
	
	public interface SVIRippleListener {
		public void onRippleFinished();
	}
	
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}		

	
	static private native void nativeResetRipple(int natSurfaceHandle);
	static private native void nativeSetRipple(int natSurfaceHandle, float x, float y, float intensity);
	static private native void nativeSetRippleInfo(int natSurfaceHandle, float reductionRatio, float cutThreshold);
	static private native void nativeSetRippleEnvImage(int natSurfaceHandle, int imageHandle);
	static private native void nativeEnable(int natSurfaceHandle, int enable);
	static private native void nativeSetType(int natSurfaceHandle, int type);
}

	
