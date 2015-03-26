package com.github.sviengine.particle;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.glsurface.SVIGLSurface;

public class SVIKeyFrameParticles extends SVIParticles{
	public SVIKeyFrameParticles() {
		this(null);
	}
	
	public SVIKeyFrameParticles(SVIGLSurface surface) {
		super(surface);
		mNativeHandle = -1;
		
		initialize();
		mKeyTime = 0.0f;
		
		mNativeHandle = nativeCreateKeyFrameParticles();
	}
	
	
	protected void finalize() throws Throwable {
		if (mNativeHandle != -1) {
			//nativeDestroy(mNativeHandle);
			mNativeHandle = -1;
        }
		super.finalize();
    }
	
	
	protected float mKeyTime;
	
	private int 	mNativeHandle;

	
	/*****************************************************************************************************************/
	/**
	  * SVIKeyFrameParticleEffect native interface.
	  *
	  */
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}
	
	private static native int nativeCreateKeyFrameParticles();
}
