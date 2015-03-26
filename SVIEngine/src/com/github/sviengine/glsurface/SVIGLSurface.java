/*
 * Copyright (C) 2011 SVI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.github.sviengine.glsurface;

import java.lang.Thread.State;
import java.lang.ref.WeakReference;
import java.util.Calendar;
import java.util.HashMap;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.SVIEngineThreadProtection;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.posteffect.SVIPostEffectManager;
import com.github.sviengine.slide.SVISlideManager;
import com.github.sviengine.slide.SVISlideManager.SVISlideRequestRender;

/**
 * A reference to the native resources of SVIGLSurface.
 * Objects of this class are used for reference counting in order to release native resources at right time.
 */
class NativeReference{
	boolean glSurfaceGCed;
	int mNativeHandle;
	public NativeReference(int nativeHandle){
		mNativeHandle = nativeHandle;
	}
	
	public void cleanup(){
		if(glSurfaceGCed){
			SVIGLSurface.deInit(mNativeHandle);
		}
	}
}


/**
 * SVIGLSurface Class extend GLSurfaceView and use openGL ES 2.0 for drawing screen
 * 
 */
public class SVIGLSurface extends GLSurfaceView implements SVISlideRequestRender{
	private String TAG = "sviengine" + this;
	private static final boolean DEBUG = false;	
	protected boolean	mUseNativeTestBed = false;	
	protected boolean	mUseThread = false;
	private boolean mUseUpdateListener = true;

	private boolean isUseRenderNotifier = false;
	private Handler mTimeHandler = null;	
	
	private final String BUILD_VERSION = "(Apr 16 2014)";
	private final String BUILD_TIME = "(Apr 16 2014:19:30)";
	
	protected SVISlideManager mSlideManager;
	protected SVIPostEffectManager mPostEffectManager;
	
	private RenderNotifier 	mRenderNotifier = null;
	private int			mNativeHandle = -1;//Multi-Instance-Support
	//2013-03-14 rareboy0112@facebook.com
	//Preserve gl context value
	protected boolean mPreserveEGLContext = false;
	
	private NativeReference nativeReference;	
	static HashMap<NativeReference, Integer> surfacetoCountMap = new HashMap<NativeReference, Integer>();

	private static WeakReference<SVIGLSurface> lastCreatedSurface;
	
	public static SVIGLSurface getSurface(SVIGLSurface saGLSurface){
		SVIEngineThreadProtection.validateMainThread();
		if(saGLSurface != null){
			return saGLSurface;
		}else if(lastCreatedSurface != null){
			return lastCreatedSurface.get();
		}
		
		throw new RuntimeException("No SVIGLSurface available !");
	}
	
	public void incrementUsageCount(Object obj){
		synchronized(surfacetoCountMap){
		Integer count = surfacetoCountMap.get(this.nativeReference);
		if(count == null){
			surfacetoCountMap.put(nativeReference, 1);
		}else{
			surfacetoCountMap.put(nativeReference, count+1);
		}
		}
	}
	
	public void decrementUsageCount(Object obj){
		synchronized(surfacetoCountMap){
		Integer count = surfacetoCountMap.get(this.nativeReference);
		if(count==1){
			surfacetoCountMap.remove(nativeReference);
			nativeReference.cleanup();
		}else{
			surfacetoCountMap.put(nativeReference, count-1);
		}
		}
	}

	/**
	 * constructor
	 *
	 * @param context > context 
	 */
	public SVIGLSurface(Context context) {
		super(context);
		
		
		WeakReference<SVIGLSurface> weakReference = new WeakReference<SVIGLSurface>(this);		
		mNativeHandle = nativeGetHandle(weakReference);
		
		nativeReference = new NativeReference(mNativeHandle);
		
		mSlideManager = new SVISlideManager(this);
		mPostEffectManager = new SVIPostEffectManager(this);
		
		lastCreatedSurface = weakReference;
			
		mUseNativeTestBed = false;

		/// Required set !!!
		AssetManager assetMgr = context.getResources().getAssets();
		nativeSetAssetManager(assetMgr,mNativeHandle);
		mSlideManager.setSVISlideRequestRender(this);
	}
	
	/**
	 * constructor with AttriButeSet
	 *
	 * @param context > context
	 * @param attrs > read only interface
	 */
	public SVIGLSurface(Context context, AttributeSet attrs) {
		super(context, attrs);
		
		WeakReference<SVIGLSurface> weakReference = new WeakReference<SVIGLSurface>(this);		
		mNativeHandle = nativeGetHandle(weakReference);
		
		nativeReference = new NativeReference(mNativeHandle);
		
		mSlideManager = new SVISlideManager(this);
		mPostEffectManager = new SVIPostEffectManager(this);
		
		lastCreatedSurface = weakReference;

		mUseNativeTestBed = false;
		
		/// Required set !!!
		AssetManager assetMgr = context.getResources().getAssets();
		nativeSetAssetManager(assetMgr,mNativeHandle);
		mSlideManager.setSVISlideRequestRender(this);
	}

	public String getRevision(){
		return BUILD_VERSION;
	}

	/**
	 * initialize surface
	 *
	 * @param translucent > true = set format use pixel format  false = nothing 
	 * @param depth > depth buffer
	 * @param stencil > stencil buffer
	 */
	public void initialize(boolean translucent, int depth, int stencil) {
		
		mTimeHandler = new Handler() {
			@Override
			public void handleMessage(Message message) {
				requestRender();
			}
		};
		
		//2013-03-19 masterkeaton27@gmail.com
		if (mUseThread) createTimeThread();
		
		mRenderer = new GLRenderer();

		if (translucent) {
			this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
		}

		/* Setup the context factory for 2.0 rendering.
		* See ContextFactory class definition below
		*/
		setEGLContextFactory(new ContextFactory());

		/* We need to choose an EGLConfig that matches the format of
		* our surface exactly. This is going to be done in our
		* custom config chooser. See ConfigChooser class definition
		* below.
		*/
		//ToDo : Fix ConfigChooser function because, if enable below function, application will be die. 
		setEGLConfigChooser(translucent ?
					new ConfigChooser(8, 8, 8, 8, depth, stencil) :
					new ConfigChooser(8, 8, 8, 0, depth, stencil) );

		setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR | GLSurfaceView.DEBUG_LOG_GL_CALLS);
		setRenderer(mRenderer);
		//setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
		mGLView = this;
		
		
		//2012-01-13 masterkeaton27@gmail.com
		//set clear color to zero when translucent is true.
		enableTranslucent(translucent);
		
		SVIAnimation.initAnimationOnMainThread();
		
		Log.i(TAG,"SVI ENGINE-JAR-RELEASE DATE"+BUILD_VERSION+"");
		Log.i(TAG,"SVI ENGINE-JAR-BUILD TIME  "+BUILD_TIME+"");
	}
	
	public SVISlideManager getSlideManager() {
		SVIEngineThreadProtection.validateMainThread();
		return mSlideManager;
	}
	
	public SVIPostEffectManager getPostEffectManager(){
		return mPostEffectManager;
	}
	
	protected void finalize() throws Throwable {
		synchronized(surfacetoCountMap){
		Integer count = surfacetoCountMap.get(nativeReference);
		if(count == null){//No clients clean immediately
			deInit(mNativeHandle);
		}
		else{//Clean native resources only when all clients of this SVIGLSurface have been GCed.
			nativeReference.glSurfaceGCed = true;
		}
		}
		super.finalize();
	}

	/**
	 * pause
	 * 
	 * @see android.opengl.GLSurfaceView#onPause()
	 */
	@Override
	public void onPause() {
		// TODO Auto-generated method stub
		Log.e("Thread", "onPause :"+Thread.currentThread().getId());
		
		//2013-03-15 rareboy0112@facebook.com
		if(mPreserveEGLContext == false) {
			nativePauseSVIGLSurface(mNativeHandle);
		}
		else {
			nativeForceEndAllAnimationSVIGLSurface(mNativeHandle);
		}
		
		if (mUseThread) terminateTimeThread();
		
		super.onPause();
	}

	public void setSVIPreserveEGLContext(boolean preserveEGLContext) {
		mPreserveEGLContext = preserveEGLContext;
		setPreserveEGLContextOnPause(mPreserveEGLContext);
	}
	
	/**
	 * resume
	 * 
	 * @see android.opengl.GLSurfaceView#onResume()
	 */
	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		Log.e("Thread", "onResume :"+Thread.currentThread().getId());
		super.onResume();
		
		//2013-03-15 rareboy0112@facebook.com
		if(mPreserveEGLContext == false) {
			nativeResumeSVIGLSurface(mNativeHandle);
		}
		
		if (mUseThread) createTimeThread();	
	}
	
	private void createTimeThread(){
		if (mTimeThread == null){
			mTimeThread = new TimeThread();
			mTimeThread.setDaemon(true);
		}
	}
	
	
	
	private void terminateTimeThread(){
		if (mTimeThread != null){
			//2013-03-19 masterkeaton27@gmail.com
			mTimeThread.terminate();
			//mTimeThread.interrupt();
			mTimeThread = null;
		}
	}

	/**
	 * resize
	 * when call changed surface size (screen rotation) 
	 *
	 * @param width > current surface width
	 * @param height > current surface height
	 */
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub
	}
	
	/**
	 * touch event
	 * when touch the surface this function will be loaded.
	 * 
	 * @see android.view.View#onTouchEvent(android.view.MotionEvent)
	 * @return boolean true
	 */
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		return true;
	}



	class GLRenderer implements GLSurfaceView.Renderer {
		/**
		* Reference to the last gl context.
		*/
		GL10 lastGlContext;
		
		@Override
		public void onDrawFrame(GL10 gl) {			
			// TODO Auto-generated method stub	
			int updateMsg = mSlideManager.isPausedImplicitAnimation() == false ? 1 : 0;
			nativeRenderSVIGLSurface(updateMsg, (lastGlContext != gl),mNativeHandle);
			lastGlContext = gl;
			mSlideManager.setRequestRendering(false);
		}

		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			// TODO Auto-generated method stub
			int rotation = 0;
			if( mUseNativeTestBed ) {
				rotation = ((Activity)getContext()).getWindowManager().getDefaultDisplay().getRotation();
			}
			onResize(width, height);	
			nativeResizeSVIGLSurface(width, height, rotation,mNativeHandle);
		}

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			// TODO Auto-generated method stub
			int enableNativeTestBed = 0;	
			if(mUseNativeTestBed)
				enableNativeTestBed = 1;
			nativeInitSVIGLSurface(enableNativeTestBed, mNativeHandle);
		}
	}

	private static class ContextFactory implements GLSurfaceView.EGLContextFactory {
		String TAG = "ContextFactory" + this;
		private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
		public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
			Log.w(TAG, "creating OpenGL ES 2.0 context");
			checkEglError("Before eglCreateContext", egl);
			int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
			EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
			checkEglError("After eglCreateContext", egl);
			return context;
		}

		public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
			egl.eglDestroyContext(display, context);
		}
	}

	private static void checkEglError(String prompt, EGL10 egl) {
		int error;
		while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
			Log.e("saglsurface", String.format("%s: EGL error: 0x%x", prompt, error));
		}
	}

	private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser {
		String TAG = "ConfigChooser" + this;
		
		public ConfigChooser(int r, int g, int b, int a, int depth, int stencil) {
			mRedSize = r;
			mGreenSize = g;
			mBlueSize = b;
			mAlphaSize = a;
			mDepthSize = depth;
			mStencilSize = stencil;
		}

		/* This EGL config specification is used to specify 2.0 rendering.
		* We use a minimum size of 4 bits for red/green/blue, but will
		* perform actual matching in chooseConfig() below.
		*/

		private static int EGL_OPENGL_ES2_BIT = 4;
		private static int[] s_configAttribs2 =
		{
			EGL10.EGL_RED_SIZE, 5,
			EGL10.EGL_GREEN_SIZE, 6,
			EGL10.EGL_BLUE_SIZE, 5,
			EGL10.EGL_DEPTH_SIZE, 16, 
			EGL10.EGL_STENCIL_SIZE, 0,
			//EGL10.EGL_SVIMPLES, 4,
			// EGL10.EGL_SVIMPLE_BUFFERS, 1,
			EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL10.EGL_NONE
		};

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

			/* Get the number of minimally matching EGL configurations
			*/
			int[] num_config = new int[1];
			egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

			int numConfigs = num_config[0];

			if (numConfigs <= 0) {
				throw new IllegalArgumentException("No configs match configSpec");
			}

			/* Allocate then read the array of minimally matching EGL configs
			*/
			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

			if (DEBUG) {
				printConfigs(egl, display, configs);
			}
			/* Now return the "best" one
			*/
			return chooseConfig(egl, display, configs);
		}

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs) {
			for(EGLConfig config : configs) {
				int d = findConfigAttrib(egl, display, config,
				EGL10.EGL_DEPTH_SIZE, 0);
				int s = findConfigAttrib(egl, display, config,
				EGL10.EGL_STENCIL_SIZE, 0);

				// We need at least mDepthSize and mStencilSize bits
				if (d < mDepthSize || s < mStencilSize)
					continue;

				// We want an *exact* match for red/green/blue/alpha
				int r = findConfigAttrib(egl, display, config,
				EGL10.EGL_RED_SIZE, 0);
				int g = findConfigAttrib(egl, display, config,
				EGL10.EGL_GREEN_SIZE, 0);
				int b = findConfigAttrib(egl, display, config,
				EGL10.EGL_BLUE_SIZE, 0);
				int a = findConfigAttrib(egl, display, config,
				EGL10.EGL_ALPHA_SIZE, 0);

				if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize && 
						s == mStencilSize ) {
					Log.e(TAG, "[Final Choose Config]R:"+r+", G:"+g+", B:"+b+", A:"+a );
					printConfig(egl, display, config);
					return config;
				}
			}
			return null;
		}

		private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue) {
			if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
				return mValue[0];
			}
			return defaultValue;
		}

		private void printConfigs(EGL10 egl, EGLDisplay display, EGLConfig[] configs) {
			int numConfigs = configs.length;
			Log.w(TAG, String.format("%d configurations", numConfigs));
			for (int i = 0; i < numConfigs; i++) {
				Log.w(TAG, String.format("Configuration %d:\n", i));
				printConfig(egl, display, configs[i]);
			}
		}

		private void printConfig(EGL10 egl, EGLDisplay display, EGLConfig config) {
			int[] attributes = {
				EGL10.EGL_BUFFER_SIZE,
				EGL10.EGL_ALPHA_SIZE,
				EGL10.EGL_BLUE_SIZE,
				EGL10.EGL_GREEN_SIZE,
				EGL10.EGL_RED_SIZE,
				EGL10.EGL_DEPTH_SIZE,
				EGL10.EGL_STENCIL_SIZE,
				EGL10.EGL_CONFIG_CAVEAT,
				EGL10.EGL_CONFIG_ID,
				EGL10.EGL_LEVEL,
				EGL10.EGL_MAX_PBUFFER_HEIGHT,
				EGL10.EGL_MAX_PBUFFER_PIXELS,
				EGL10.EGL_MAX_PBUFFER_WIDTH,
				EGL10.EGL_NATIVE_RENDERABLE,
				EGL10.EGL_NATIVE_VISUAL_ID,
				EGL10.EGL_NATIVE_VISUAL_TYPE,
				0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
				EGL10.EGL_SAMPLES,
				EGL10.EGL_SAMPLE_BUFFERS,
				EGL10.EGL_SURFACE_TYPE,
				EGL10.EGL_TRANSPARENT_TYPE,
				EGL10.EGL_TRANSPARENT_RED_VALUE,
				EGL10.EGL_TRANSPARENT_GREEN_VALUE,
				EGL10.EGL_TRANSPARENT_BLUE_VALUE,
				0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
				0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
				0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
				0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
				EGL10.EGL_LUMINANCE_SIZE,
				EGL10.EGL_ALPHA_MASK_SIZE,
				EGL10.EGL_COLOR_BUFFER_TYPE,
				EGL10.EGL_RENDERABLE_TYPE,
				0x3042 // EGL10.EGL_CONFORMANT
			};

			String[] names = {
				"EGL_BUFFER_SIZE",
				"EGL_ALPHA_SIZE",
				"EGL_BLUE_SIZE",
				"EGL_GREEN_SIZE",
				"EGL_RED_SIZE",
				"EGL_DEPTH_SIZE",
				"EGL_STENCIL_SIZE",
				"EGL_CONFIG_CAVEAT",
				"EGL_CONFIG_ID",
				"EGL_LEVEL",
				"EGL_MAX_PBUFFER_HEIGHT",
				"EGL_MAX_PBUFFER_PIXELS",
				"EGL_MAX_PBUFFER_WIDTH",
				"EGL_NATIVE_RENDERABLE",
				"EGL_NATIVE_VISUAL_ID",
				"EGL_NATIVE_VISUAL_TYPE",
				"EGL_PRESERVED_RESOURCES",
				"EGL_SVIMPLES",
				"EGL_SVIMPLE_BUFFERS",
				"EGL_SURFACE_TYPE",
				"EGL_TRANSPARENT_TYPE",
				"EGL_TRANSPARENT_RED_VALUE",
				"EGL_TRANSPARENT_GREEN_VALUE",
				"EGL_TRANSPARENT_BLUE_VALUE",
				"EGL_BIND_TO_TEXTURE_RGB",
				"EGL_BIND_TO_TEXTURE_RGBA",
				"EGL_MIN_SWAP_INTERVAL",
				"EGL_MAX_SWAP_INTERVAL",
				"EGL_LUMINANCE_SIZE",
				"EGL_ALPHA_MASK_SIZE",
				"EGL_COLOR_BUFFER_TYPE",
				"EGL_RENDERABLE_TYPE",
				"EGL_CONFORMANT"
			};
			
			int[] value = new int[1];
			for (int i = 0; i < attributes.length; i++) {
				int attribute = attributes[i];
				String name = names[i];
				if ( egl.eglGetConfigAttrib(display, config, attribute, value)) {
					Log.w(TAG, String.format("  %s: %d\n", name, value[0]));
				} 
				else {
					Log.w(TAG, String.format("  %s: failed\n", name));
					//while (egl.eglGetError() != EGL10.EGL_SUCCESS);
				}
			}
		}

		// Subclasses can adjust these values:
		protected int mRedSize;
		protected int mGreenSize;
		protected int mBlueSize;
		protected int mAlphaSize;
		protected int mDepthSize;
		protected int mStencilSize;
		private int[] mValue = new int[1];
	}

	/**
	 * swap animation SVIGLSurface
	 * this method call the native method. native method get instance and push the message in message que.
	 * then renderer read that message and rendering the surface.
	 */
	public void swapAnimationSVIGLSurface() {
		nativeSwapAnimationSVIGLSurface(mNativeHandle);
	}

	private Calendar mPrevTime = null;
	private long mPrevTimeMilli = 0;
	private long mKeepTime = 0;
	private int DRAW_TIME = 1000 / 60;
	
	protected int mFPS = 60;
	
	public void setRenderFPS(int aFPS) {
		mFPS = aFPS;
		DRAW_TIME = 1000 / mFPS;
	}
	
	public int getRenderFPS(){

		return mFPS;
		
	}
	
	
	public class TimeThread extends Thread {
				
		private long mGap = 0;
		private boolean mIsTerminated = false;
		
		public synchronized void terminate(){
			mIsTerminated = true;
			if (getState() == State.WAITING){
				notifyAll();
			}
		}
		
		public synchronized void start(long gab){
			mGap = gab;
			super.start();			
		}		
		
		public synchronized void notify(long gab){
			mGap = gab;
			super.notifyAll();
		}
		
		@Override
		public void run(){
			super.run();			
			
			while (!mIsTerminated){								
				synchronized (this) {
					try {
						long gap = mGap;
						wait(gap);
					} catch (InterruptedException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
				}
				
				requestRender();
				
				if (mIsTerminated){ 
					Log.i(TAG, "SurfaceView RequestThread<" + this.getId() + "> Safely Returned.");
					return;
				}
									
				synchronized (this) {
					try {
						wait();
					} catch (InterruptedException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
				}
			}
			
			Log.i(TAG, "SurfaceView RequestThread<" + this.getId() + "> Safely Terminated.");
		}
	}
	
	private TimeThread mTimeThread = null;
	
	public void requestRenderView() {//Multi-Instance-Support
		if(mPrevTime == null ) {
			mPrevTime = Calendar.getInstance();
		}
				
		
		Calendar curTime = Calendar.getInstance();
		long deltaTime = curTime.getTimeInMillis() - mPrevTime.getTimeInMillis();
		
		//2013-03-07 masterkeaton27@gmail.com
		//due to date problem, exception case added.
		if (curTime.getTimeInMillis() < mPrevTime.getTimeInMillis()){
			mPrevTime = curTime;
			mKeepTime = 0;
			deltaTime = 0;
		}		
		
		mKeepTime += deltaTime;
		
		if(mKeepTime >= DRAW_TIME) {
			if( mGLView != null )
				mGLView.requestRender();
			
			mKeepTime = 0;
		} else {
			long gap = DRAW_TIME - mKeepTime;				
			//2013-03-07 masterkeaton27@gmail.com
			//changing delay method with thread using.
					
			if (mUseThread){
				if (mTimeThread != null){
					if (mTimeThread.getState() == State.NEW){ 
						mTimeThread.start(gap);
					}else if (mTimeThread.getState() == State.WAITING){				
						mTimeThread.notify(gap);					
					}
				}
			}else mTimeHandler.sendEmptyMessageDelayed(0, gap);
		}
		
		
		mSlideManager.animaitonRenderNotify();
		
		mPrevTime = curTime;
	}

	private GLRenderer					mRenderer;
	private GLSurfaceView		mGLView = null;


	/*****************************************************************************************************************/
	/**
	  * SVIGLSurface native interface.
	  *
	  */
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}

	/**
	 * enable blur
	 *
	 * @param enable > true = enable false = disable
	 * @param level > blue level (int)
	 */
	/* this function is deprecated
	public void enableBlur(boolean enable, int level) {
		if( enable )
			nativeSetBlurCapture(level,mNativeHandle);
		else
			nativeClearBlurCapture(mNativeHandle);
	}  
	*/
	

	//2012-01-02 masterkeaton27@gmail.com
	/**
	 * enable show fps
	 * if you want show fps in your screen, use this method
	 *
	 * @param enable > true = show fps  false = hide fps
	 */
	/*These days devices has very.. slow if it use..
	
	public void enableShowFPS(boolean enable){
		//nativeEnableShowFPS(enable,mNativeHandle);
	}
	*/
	/**
	 * enable log about fps
	 * if you want remain log about fps, use this method
	 * can check from log reader (etc. eclipse)
	 *
	 * @param enable > true = remain log  false = hide log
	 */
	public void enableLogFPS(boolean enable){
		nativeEnableLogFPS(enable,mNativeHandle);
	}
	
	public void clearRemainTextures(){
		nativeClearRemainTexture(mNativeHandle);
	}
	
	//2012-01-13 masterkeaton27@gmail.com
	public void enableTranslucent(boolean enable){
		nativeEnableTranslucent(enable,mNativeHandle);
	}
	
	public void setRenderNotifier(RenderNotifier notifier) {
		if(mRenderNotifier != null)
			mRenderNotifier = null;
		
		mRenderNotifier = notifier;
		isUseRenderNotifier = true;
	
	}
	
	public RenderNotifier getRenderNotifier(){
		return mRenderNotifier;
	}
	
	public boolean isUseRenderNotifier(){
		return isUseRenderNotifier;
		}

	public void useRenderNotifier(boolean useRenderNotifier){
		 isUseRenderNotifier = useRenderNotifier;
	}

	public int getNativeHandle() {
		SVIEngineThreadProtection.validateMainThread();
		return mNativeHandle;
	}
	
	//2012/04/03 rareboy0112@facebook.com
	//enable call-back update method.
	public void enableUpdateListener(boolean enable) {
		mUseUpdateListener = enable;
		nativeEnableUpdateListener(enable,mNativeHandle);
	}
	
	//2012/04/03 rareboy0112@facebook.com
	public void nativeUpdate() {//Multi-Instance-Support
		if(mRenderNotifier != null) {
			mRenderNotifier.onUpdateFinished();
		}
	}
		
	public interface RenderNotifier {
		public void onUpdateFinished();		
	}
	
	static void  deInit(int mNativeHandle){
		nativeDeInitSVIGLSurface(mNativeHandle);
	}
	
	
	private static native void nativeInitSVIGLSurface(int enableNativeTestBed,int mNativeHandle);
	private static native void nativeDeInitSVIGLSurface(int mNativeHandle);
	
	private static native int nativeGetHandle(WeakReference<SVIGLSurface> surface);
	private static native void nativeResizeSVIGLSurface(int w, int h, int rotation,int mNativeHandle);
	private static native void nativeRenderSVIGLSurface(int updateMsg,boolean glContextChanged,int mNativeHandle);
	private static native void nativeResumeSVIGLSurface(int mNativeHandle);
	private static native void nativePauseSVIGLSurface(int mNativeHandle);
	private static native void nativeForceEndAllAnimationSVIGLSurface(int mNativeHandle);
	private static native void nativeClearBlurCapture(int mNativeHandle);	
	private static native void nativeSetBlurCapture(int level,int mNativeHandle);
	private static native void nativeSwapAnimationSVIGLSurface(int mNativeHandle);
	private static native void nativeSetAssetManager(AssetManager assetManager,int mNativeHandle);
	private static native void nativeEnableShowFPS(boolean enable,int mNativeHandle);
	private static native void nativeEnableLogFPS(boolean enable,int mNativeHandle);	
	private static native void nativeEnableTranslucent(boolean enable,int mNativeHandle);
	private static native void nativeEnableUpdateListener(boolean enable,int mNativeHandle);
	private static native boolean nativeGetFirstLoadFinished(int mNativeHandle);
	//2012-06-22 masterkeaton27@gmail.com
	private native void nativeClearRemainTexture(int mNativeHandle);
	@Override
	public boolean requestSlideRender() {
		swapAnimationSVIGLSurface();
		requestRender();
		return false;
	}

	@Override
	public void animationSlideNotify() {
		// TODO Auto-generated method stub
		
	}

}




