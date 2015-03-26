package com.github.sviengine.glwallpaperservice;

import java.io.Writer;
import java.util.ArrayList;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.service.wallpaper.WallpaperService;
import android.util.Log;
import android.view.SurfaceHolder;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.glwallpaperservice.BaseConfigChooser.ComponentSizeChooser;
import com.github.sviengine.glwallpaperservice.BaseConfigChooser.SimpleEGLConfigChooser;
import com.github.sviengine.slide.SVISlideManager;


public class SVIGLWallpaperService extends WallpaperService {
	private static final String TAG = "GLWallpaperService";
	public SVIGLEngine mEngine;
    public int	mSurfaceNativeHandle = 0;//Multi-Instance-Support
    private SVISlideManager mSlideManager = null;
    
    SVIGLWallpaperService(SVISlideManager slideMngr) {
    	mSlideManager = slideMngr;
    }
    
	@Override
	public Engine onCreateEngine() {
		mEngine = new SVIGLEngine(getApplicationContext(), this); 
		return mEngine; 	
	}	

	public class SVIGLEngine extends Engine {
		public final static int RENDERMODE_WHEN_DIRTY = 0;
		public final static int RENDERMODE_CONTINUOUSLY = 1;
		private SVIGLThread mGLThread;
		private EGLConfigChooser mEGLConfigChooser;
		private EGLContextFactory mEGLContextFactory;
		private EGLWindowSurfaceFactory mEGLWindowSurfaceFactory;
		private GLWrapper mGLWrapper;
		private int mDebugFlags;
		private SVIGLWallpaperService mService= null; 

		public SVIGLEngine(Context context, SVIGLWallpaperService service) {
			super();
			mService = service;
			mSurfaceNativeHandle = SVIWallpaperNative.nativeGetHandle(service);//Multi_instance-Support
			//setRenderer();
		}

		@Override
		public void onVisibilityChanged(boolean visible) {
			if (visible) {
				onResume();
			} else {
				onPause();
			}
			super.onVisibilityChanged(visible);
		}

		@Override
		public void onCreate(SurfaceHolder surfaceHolder) {
			super.onCreate(surfaceHolder);
			// Log.d(TAG, "GLEngine.onCreate()");
		}

		@Override
		public void onDestroy() {
			super.onDestroy();
			// Log.d(TAG, "GLEngine.onDestroy()");
			mGLThread.requestExitAndWait();
		}

		@Override
		public void onSurfaceChanged(SurfaceHolder holder, int format, int width, int height) {
			// Log.d(TAG, "onSurfaceChanged()");
			mGLThread.onWindowResize(width, height);
			super.onSurfaceChanged(holder, format, width, height);
		}

		@Override
		public void onSurfaceCreated(SurfaceHolder holder) {
			Log.d(TAG, "onSurfaceCreated()");
			mGLThread.surfaceCreated(holder);
			super.onSurfaceCreated(holder);
		}

		@Override
		public void onSurfaceDestroyed(SurfaceHolder holder) {
			Log.d(TAG, "onSurfaceDestroyed()");
			mGLThread.surfaceDestroyed();
			super.onSurfaceDestroyed(holder);
		}

		/**
		 * An EGL helper class.
		 */
		public void setGLWrapper(GLWrapper glWrapper) {
			mGLWrapper = glWrapper;
		}

		public void setDebugFlags(int debugFlags) {
			mDebugFlags = debugFlags;
		}

		public int getDebugFlags() {
			return mDebugFlags;
		}

		public void setRenderer(SVIGLRenderer renderer) {
			checkRenderThreadState();
			if (mEGLConfigChooser == null) {
				mEGLConfigChooser = new SimpleEGLConfigChooser(true);
			}
			if (mEGLContextFactory == null) {
				mEGLContextFactory = new DefaultContextFactory();
			}
			if (mEGLWindowSurfaceFactory == null) {
				mEGLWindowSurfaceFactory = new DefaultWindowSurfaceFactory();
			}
			
			SVIAnimation.initAnimationOnMainThread();
			
			mGLThread = new SVIGLThread(mService, renderer, mEGLConfigChooser, mEGLContextFactory, mEGLWindowSurfaceFactory, mGLWrapper);
			mGLThread.start();
			
			SVIAnimation.initAnimationOnMainThread();
		}

		public void setEGLContextFactory(EGLContextFactory factory) {
			checkRenderThreadState();
			mEGLContextFactory = factory;
		}

		public void setEGLWindowSurfaceFactory(EGLWindowSurfaceFactory factory) {
			checkRenderThreadState();
			mEGLWindowSurfaceFactory = factory;
		}

		public void setEGLConfigChooser(EGLConfigChooser configChooser) {
			checkRenderThreadState();
			mEGLConfigChooser = configChooser;
		}

		public void setEGLConfigChooser(boolean needDepth) {
			setEGLConfigChooser(new SimpleEGLConfigChooser(needDepth));
		}

		public void setEGLConfigChooser(int redSize, int greenSize, int blueSize, int alphaSize, int depthSize,
				int stencilSize) {
			setEGLConfigChooser(new ComponentSizeChooser(redSize, greenSize, blueSize, alphaSize, depthSize,
					stencilSize));
		}

		public void setRenderMode(int renderMode) {
			mGLThread.setRenderMode(renderMode);
		}

		public int getRenderMode() {
			return mGLThread.getRenderMode();
		}

		public void requestRender() {
			mGLThread.requestRender();
		}

		public void onPause() {
			mGLThread.onPause();
		}

		public void onResume() {
			mGLThread.onResume();
		}

		public void queueEvent(Runnable r) {
			mGLThread.queueEvent(r);
		}

		private void checkRenderThreadState() {
			if (mGLThread != null) {
				throw new IllegalStateException("setRenderer has already been called for this instance.");
			}
		}	
		
		public void swapAnimationSVIGLSurface(){
			if (mGLThread != null) mGLThread.swapAnimationSVIGLSurface();			
		}
		
		public void requestRenderView() {
			if (mGLThread != null) mGLThread.requestRender();			
		}
		
		public void setShowFPS(boolean isShow){
			if (mGLThread != null) mGLThread.enableShowFPS(isShow);			
		}
		
		public void setLogFPS(boolean isLog){
			if (mGLThread != null) mGLThread.enableLogFPS(isLog);			
		}
	}	
	
	public void setShowFPS(boolean isShow){
		if (mEngine != null) mEngine.setShowFPS(isShow);					
	}
	
	public void setLogFPS(boolean isLog){
		if (mEngine != null) mEngine.setLogFPS(isLog);					
	}
	
	public void swapAnimationSVIGLSurface(){
		if (mEngine != null)
			mEngine.swapAnimationSVIGLSurface();
	}
	
	public void requestRender(){
		if (mEngine != null)
			mEngine.requestRender();
	}
	
	public void requestRenderView(){//Multi-Instance-Support
		if (mEngine != null)
			mEngine.requestRenderView();
		mSlideManager.animaitonRenderNotify();
	}
	

	public class SVIGLRenderer implements GLSurfaceView.Renderer {	
	@Override
	public void onDrawFrame(GL10 gl) {
		// TODO Auto-generated method stub	
		//Log.e("Thread", "onDrawFrame :"+Thread.currentThread().getId());
		int updateMsg = mSlideManager.isPausedImplicitAnimation() == false ? 1 : 0;
		SVIWallpaperNative.nativeRenderSVIGLSurface(updateMsg, mSurfaceNativeHandle);
		mSlideManager.setRequestRendering(false);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		// TODO Auto-generated method stub
		int rotation = 0;
		SVIWallpaperNative.nativeResizeSVIGLSurface(width, height, rotation, mSurfaceNativeHandle);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		// TODO Auto-generated method stub		
		SVIWallpaperNative.nativeInitSVIGLSurface(0,mSurfaceNativeHandle);
	}	
}
}




class LogWriter extends Writer {
	private StringBuilder mBuilder = new StringBuilder();

	@Override
	public void close() {
		flushBuilder();
	}

	@Override
	public void flush() {
		flushBuilder();
	}

	@Override
	public void write(char[] buf, int offset, int count) {
		for (int i = 0; i < count; i++) {
			char c = buf[offset + i];
			if (c == '\n') {
				flushBuilder();
			} else {
				mBuilder.append(c);
			}
		}
	}

	private void flushBuilder() {
		if (mBuilder.length() > 0) {
			Log.v("GLSurfaceView", mBuilder.toString());
			mBuilder.delete(0, mBuilder.length());
		}
	}
}

// ----------------------------------------------------------------------

/**
 * An interface for customizing the eglCreateContext and eglDestroyContext calls.
 *

 * This interface must be implemented by clients wishing to call
 * {@link GLWallpaperService#setEGLContextFactory(EGLContextFactory)}
 */
interface EGLContextFactory {
	EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig);

	void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context);
}

class DefaultContextFactory implements EGLContextFactory {
	private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
	public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig config) {
		int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
		EGLContext context = egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT, attrib_list);
		
		return context;
	}

	public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
		egl.eglDestroyContext(display, context);
	}
}

/**
 * An interface for customizing the eglCreateWindowSurface and eglDestroySurface calls.
 *

 * This interface must be implemented by clients wishing to call
 * {@link GLWallpaperService#setEGLWindowSurfaceFactory(EGLWindowSurfaceFactory)}
 */
interface EGLWindowSurfaceFactory {
	EGLSurface createWindowSurface(EGL10 egl, EGLDisplay display, EGLConfig config, Object nativeWindow);

	void destroySurface(EGL10 egl, EGLDisplay display, EGLSurface surface);
}

class DefaultWindowSurfaceFactory implements EGLWindowSurfaceFactory {

	public EGLSurface createWindowSurface(EGL10 egl, EGLDisplay
			display, EGLConfig config, Object nativeWindow) {
		// this is a bit of a hack to work around Droid init problems - if you don't have this, it'll get hung up on orientation changes
		EGLSurface eglSurface = null;
		while (eglSurface == null) {
			try {
				eglSurface = egl.eglCreateWindowSurface(display,
						config, nativeWindow, null);
			} catch (Throwable t) {
			} finally {
				if (eglSurface == null) {
					try {
						Thread.sleep(10);
					} catch (InterruptedException t) {
					}
				}
			}
		}
		return eglSurface;
	}

	public void destroySurface(EGL10 egl, EGLDisplay display, EGLSurface surface) {
		egl.eglDestroySurface(display, surface);
	}
}

interface GLWrapper {
	/**
	 * Wraps a gl interface in another gl interface.
	 *
	 * @param gl
	 * a GL interface that is to be wrapped.
	 * @return either the input argument or another GL object that wraps the input argument.
	 */
	GL wrap(GL gl);
}

class EglHelper {

	private EGL10 mEgl;
	private EGLDisplay mEglDisplay;
	private EGLSurface mEglSurface;
	private EGLContext mEglContext;
	EGLConfig mEglConfig;

	private EGLConfigChooser mEGLConfigChooser;
	private EGLContextFactory mEGLContextFactory;
	private EGLWindowSurfaceFactory mEGLWindowSurfaceFactory;
	private GLWrapper mGLWrapper;

	public EglHelper(EGLConfigChooser chooser, EGLContextFactory contextFactory,
			EGLWindowSurfaceFactory surfaceFactory, GLWrapper wrapper) {
		this.mEGLConfigChooser = chooser;
		this.mEGLContextFactory = contextFactory;
		this.mEGLWindowSurfaceFactory = surfaceFactory;
		this.mGLWrapper = wrapper;
	}

	/**
	 * Initialize EGL for a given configuration spec.
	 *
	 * @param configSpec
	 */
	public void start() {
		// Log.d("EglHelper" + instanceId, "start()");
		if (mEgl == null) {
			// Log.d("EglHelper" + instanceId, "getting new EGL");
			/*
			 * Get an EGL instance
			 */
			mEgl = (EGL10) EGLContext.getEGL();
		} else {
			// Log.d("EglHelper" + instanceId, "reusing EGL");
		}

		if (mEglDisplay == null) {
			// Log.d("EglHelper" + instanceId, "getting new display");
			/*
			 * Get to the default display.
			 */
			mEglDisplay = mEgl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
		} else {
			// Log.d("EglHelper" + instanceId, "reusing display");
		}

		if (mEglConfig == null) {
			// Log.d("EglHelper" + instanceId, "getting new config");
			/*
			 * We can now initialize EGL for that display
			 */
			int[] version = new int[2];
			mEgl.eglInitialize(mEglDisplay, version);
			mEglConfig = mEGLConfigChooser.chooseConfig(mEgl, mEglDisplay);
		} else {
			// Log.d("EglHelper" + instanceId, "reusing config");
		}

		if (mEglContext == null) {
			// Log.d("EglHelper" + instanceId, "creating new context");
			/*
			 * Create an OpenGL ES context. This must be done only once, an OpenGL context is a somewhat heavy object.
			 */
			mEglContext = mEGLContextFactory.createContext(mEgl, mEglDisplay, mEglConfig);
			if (mEglContext == null || mEglContext == EGL10.EGL_NO_CONTEXT) {
				throw new RuntimeException("createContext failed");
			}
		} else {
			// Log.d("EglHelper" + instanceId, "reusing context");
		}

		mEglSurface = null;
	}

	/*
	 * React to the creation of a new surface by creating and returning an OpenGL interface that renders to that
	 * surface.
	 */
	public GL createSurface(SurfaceHolder holder) {
		/*
		 * The window size has changed, so we need to create a new surface.
		 */
		if (mEglSurface != null && mEglSurface != EGL10.EGL_NO_SURFACE) {

			/*
			 * Unbind and destroy the old EGL surface, if there is one.
			 */
			mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
			mEGLWindowSurfaceFactory.destroySurface(mEgl, mEglDisplay, mEglSurface);
		}

		/*
		 * Create an EGL surface we can render into.
		 */
		mEglSurface = mEGLWindowSurfaceFactory.createWindowSurface(mEgl, mEglDisplay, mEglConfig, holder);

		if (mEglSurface == null || mEglSurface == EGL10.EGL_NO_SURFACE) {
			throw new RuntimeException("createWindowSurface failed");
		}

		/*
		 * Before we can issue GL commands, we need to make sure the context is current and bound to a surface.
		 */
		if (!mEgl.eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
			throw new RuntimeException("eglMakeCurrent failed.");
		}

		GL gl = mEglContext.getGL();
		if (mGLWrapper != null) {
			gl = mGLWrapper.wrap(gl);
		}

		/*
		 * if ((mDebugFlags & (DEBUG_CHECK_GL_ERROR | DEBUG_LOG_GL_CALLS))!= 0) { int configFlags = 0; Writer log =
		 * null; if ((mDebugFlags & DEBUG_CHECK_GL_ERROR) != 0) { configFlags |= GLDebugHelper.CONFIG_CHECK_GL_ERROR; }
		 * if ((mDebugFlags & DEBUG_LOG_GL_CALLS) != 0) { log = new LogWriter(); } gl = GLDebugHelper.wrap(gl,
		 * configFlags, log); }
		 */
		return gl;
	}

	/**
	 * Display the current render surface.
	 *
	 * @return false if the context has been lost.
	 */
	public boolean swap() {
		mEgl.eglSwapBuffers(mEglDisplay, mEglSurface);

		/*
		 * Always check for EGL_CONTEXT_LOST, which means the context and all associated data were lost (For instance
		 * because the device went to sleep). We need to sleep until we get a new surface.
		 */
		return mEgl.eglGetError() != EGL11.EGL_CONTEXT_LOST;
	}

	public void destroySurface() {
		if (mEglSurface != null && mEglSurface != EGL10.EGL_NO_SURFACE) {
			mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
			mEGLWindowSurfaceFactory.destroySurface(mEgl, mEglDisplay, mEglSurface);
			mEglSurface = null;
		}
	}

	public void finish() {
		if (mEglContext != null) {
			mEGLContextFactory.destroyContext(mEgl, mEglDisplay, mEglContext);
			mEglContext = null;
		}
		if (mEglDisplay != null) {
			mEgl.eglTerminate(mEglDisplay);
			mEglDisplay = null;
		}
	}
}

class SVIGLThread extends Thread {
	private final static boolean LOG_THREADS = true;
	public final static int DEBUG_CHECK_GL_ERROR = 1;
	public final static int DEBUG_LOG_GL_CALLS = 2;

	private final GLThreadManager sGLThreadManager = new GLThreadManager();
	private SVIGLThread mEglOwner;

	private EGLConfigChooser mEGLConfigChooser;
	private EGLContextFactory mEGLContextFactory;
	private EGLWindowSurfaceFactory mEGLWindowSurfaceFactory;
	private GLWrapper mGLWrapper;

	public SurfaceHolder mHolder;
	private boolean mSizeChanged = true;

	// Once the thread is started, all accesses to the following member
	// variables are protected by the sGLThreadManager monitor
	public boolean mDone;
	private boolean mPaused;
	private boolean mHasSurface;
	private boolean mWaitingForSurface;
	private boolean mHaveEgl;
	private int mWidth;
	private int mHeight;
	private int mRenderMode;
	private boolean mRequestRender;
	private boolean mEventsWaiting;
	public int mSurfaceNativeHandle;
	// End of member variables protected by the sGLThreadManager monitor.

	private SVIGLWallpaperService.SVIGLRenderer mRenderer;
	private ArrayList<Runnable> mEventQueue = new ArrayList<Runnable>();
	private EglHelper mEglHelper;

	SVIGLThread(SVIGLWallpaperService service, SVIGLWallpaperService.SVIGLRenderer renderer, EGLConfigChooser chooser, EGLContextFactory contextFactory,
			EGLWindowSurfaceFactory surfaceFactory, GLWrapper wrapper) {
		super();
		mDone = false;
		mWidth = 0;
		mHeight = 0;
		mRequestRender = true;
		mRenderMode = SVIGLWallpaperService.SVIGLEngine.RENDERMODE_CONTINUOUSLY;
		mRenderer = renderer;
		this.mEGLConfigChooser = chooser;
		this.mEGLContextFactory = contextFactory;
		this.mEGLWindowSurfaceFactory = surfaceFactory;
		this.mGLWrapper = wrapper;
		mSurfaceNativeHandle = service.mSurfaceNativeHandle;
		
	}

	@Override
	public void run() {
		setName("GLThread " + getId());
		if (LOG_THREADS) {
			Log.i("GLThread", "starting tid=" + getId());
		}

		try {
			guardedRun();
		} catch (InterruptedException e) {
			// fall thru and exit normally
		} finally {
			sGLThreadManager.threadExiting(this);
		}
	}

	/*
	 * This private method should only be called inside a synchronized(sGLThreadManager) block.
	 */
	private void stopEglLocked() {
		if (mHaveEgl) {
			mHaveEgl = false;
			mEglHelper.destroySurface();
			sGLThreadManager.releaseEglSurface(this);
		}
	}

	private void guardedRun() throws InterruptedException {
		mEglHelper = new EglHelper(mEGLConfigChooser, mEGLContextFactory, mEGLWindowSurfaceFactory, mGLWrapper);
		try {
			GL10 gl = null;
			boolean tellRendererSurfaceCreated = true;
			boolean tellRendererSurfaceChanged = true;

			/*
			 * This is our main activity thread's loop, we go until asked to quit.
			 */
			while (!isDone()) {
				/*
				 * Update the asynchronous state (window size)
				 */
				int w = 0;
				int h = 0;
				boolean changed = false;
				boolean needStart = false;
				boolean eventsWaiting = false;

				synchronized (sGLThreadManager) {
					while (true) {
						// Manage acquiring and releasing the SurfaceView
						// surface and the EGL surface.
						if (mPaused) {
							stopEglLocked();
						}
						if (!mHasSurface) {
							if (!mWaitingForSurface) {
								stopEglLocked();
								mWaitingForSurface = true;
								sGLThreadManager.notifyAll();
							}
						} else {
							if (!mHaveEgl) {
								if (sGLThreadManager.tryAcquireEglSurface(this)) {
									mHaveEgl = true;
									mEglHelper.start();
									mRequestRender = true;
									needStart = true;
								}
							}
						}

						// Check if we need to wait. If not, update any state
						// that needs to be updated, copy any state that
						// needs to be copied, and use "break" to exit the
						// wait loop.

						if (mDone) {
							return;
						}

						if (mEventsWaiting) {
							eventsWaiting = true;
							mEventsWaiting = false;
							break;
						}

						if ((!mPaused) && mHasSurface && mHaveEgl && (mWidth > 0) && (mHeight > 0)
								&& (mRequestRender || (mRenderMode == SVIGLWallpaperService.SVIGLEngine.RENDERMODE_CONTINUOUSLY))) {
							changed = mSizeChanged;
							w = mWidth;
							h = mHeight;
							mSizeChanged = false;
							mRequestRender = false;
							if (mHasSurface && mWaitingForSurface) {
								changed = true;
								mWaitingForSurface = false;
								sGLThreadManager.notifyAll();
							}
							break;
						}

						// By design, this is the only place where we wait().

						if (LOG_THREADS) {
							Log.i("GLThread", "waiting tid=" + getId());
						}
						sGLThreadManager.wait();
					}
				} // end of synchronized(sGLThreadManager)

				/*
				 * Handle queued events
				 */
				if (eventsWaiting) {
					Runnable r;
					while ((r = getEvent()) != null) {
						r.run();
						if (isDone()) {
							return;
						}
					}
					// Go back and see if we need to wait to render.
					continue;
				}

				if (needStart) {
					tellRendererSurfaceCreated = true;
					changed = true;
				}
				if (changed) {
					gl = (GL10) mEglHelper.createSurface(mHolder);
					tellRendererSurfaceChanged = true;
				}
				if (tellRendererSurfaceCreated) {
					mRenderer.onSurfaceCreated(gl, mEglHelper.mEglConfig);
					tellRendererSurfaceCreated = false;
				}
				if (tellRendererSurfaceChanged) {
					mRenderer.onSurfaceChanged(gl, w, h);
					tellRendererSurfaceChanged = false;
				}
				if ((w > 0) && (h > 0)) {
					/* draw a frame here */
					mRenderer.onDrawFrame(gl);

					/*
					 * Once we're done with GL, we need to call swapBuffers() to instruct the system to display the
					 * rendered frame
					 */
					mEglHelper.swap();
					Thread.sleep(10);
				}
			}
		} finally {
			/*
			 * clean-up everything...
			 */
			synchronized (sGLThreadManager) {
				stopEglLocked();
				mEglHelper.finish();
			}
		}
	}

	private boolean isDone() {
		synchronized (sGLThreadManager) {
			return mDone;
		}
	}

	public void setRenderMode(int renderMode) {
		if (!((SVIGLWallpaperService.SVIGLEngine.RENDERMODE_WHEN_DIRTY <= renderMode) && (renderMode <= SVIGLWallpaperService.SVIGLEngine.RENDERMODE_CONTINUOUSLY))) {
			throw new IllegalArgumentException("renderMode");
		}
		synchronized (sGLThreadManager) {
			mRenderMode = renderMode;
			if (renderMode == SVIGLWallpaperService.SVIGLEngine.RENDERMODE_CONTINUOUSLY) {
				sGLThreadManager.notifyAll();
			}
		}
	}

	public int getRenderMode() {
		synchronized (sGLThreadManager) {
			return mRenderMode;
		}
	}

	public void requestRender() {
		synchronized (sGLThreadManager) {
			mRequestRender = true;
			sGLThreadManager.notifyAll();
		}
	}

	public void surfaceCreated(SurfaceHolder holder) {
		mHolder = holder;
		synchronized (sGLThreadManager) {
			if (LOG_THREADS) {
				Log.i("GLThread", "surfaceCreated tid=" + getId());
			}
			mHasSurface = true;
			sGLThreadManager.notifyAll();			
		}
	}

	public void surfaceDestroyed() {
		synchronized (sGLThreadManager) {
			if (LOG_THREADS) {
				Log.i("GLThread", "surfaceDestroyed tid=" + getId());
			}
			mHasSurface = false;
			sGLThreadManager.notifyAll();
			while (!mWaitingForSurface && isAlive() && !mDone) {
				try {
					sGLThreadManager.wait();
				} catch (InterruptedException e) {
					Thread.currentThread().interrupt();
				}
			}
		}
	}

	public void onPause() {
		synchronized (sGLThreadManager) {
			mPaused = true;
			sGLThreadManager.notifyAll();			
			SVIWallpaperNative.nativePauseSVIGLSurface(mSurfaceNativeHandle);
		}
	}

	public void onResume() {
		synchronized (sGLThreadManager) {
			mPaused = false;
			mRequestRender = true;
			sGLThreadManager.notifyAll();			
			SVIWallpaperNative.nativeResumeSVIGLSurface(mSurfaceNativeHandle);
		}
	}

	public void onWindowResize(int w, int h) {
		synchronized (sGLThreadManager) {
			mWidth = w;
			mHeight = h;
			mSizeChanged = true;
			sGLThreadManager.notifyAll();
		}
	}

	public void requestExitAndWait() {
		// don't call this from GLThread thread or it is a guaranteed
		// deadlock!
		synchronized (sGLThreadManager) {
			mDone = true;
			sGLThreadManager.notifyAll();
		}
		try {
			join();
		} catch (InterruptedException ex) {
			Thread.currentThread().interrupt();
		}
	}

	/**
	 * Queue an "event" to be run on the GL rendering thread.
	 *
	 * @param r
	 * the runnable to be run on the GL rendering thread.
	 */
	public void queueEvent(Runnable r) {
		synchronized (this) {
			mEventQueue.add(r);
			synchronized (sGLThreadManager) {
				mEventsWaiting = true;
				sGLThreadManager.notifyAll();
			}
		}
	}

	private Runnable getEvent() {
		synchronized (this) {
			if (mEventQueue.size() > 0) {
				return mEventQueue.remove(0);
			}

		}
		return null;
	}

	private class GLThreadManager {

		public synchronized void threadExiting(SVIGLThread thread) {
			if (LOG_THREADS) {
				Log.i("GLThread", "exiting tid=" + thread.getId());
			}
			thread.mDone = true;
			if (mEglOwner == thread) {
				mEglOwner = null;
			}
			notifyAll();
		}

		/*
		 * Tries once to acquire the right to use an EGL surface. Does not block.
		 *
		 * @return true if the right to use an EGL surface was acquired.
		 */
		public synchronized boolean tryAcquireEglSurface(SVIGLThread thread) {
			if (mEglOwner == thread || mEglOwner == null) {
				mEglOwner = thread;
				notifyAll();
				return true;
			}
			return false;
		}

		public synchronized void releaseEglSurface(SVIGLThread thread) {
			if (mEglOwner == thread) {
				mEglOwner = null;
			}
			notifyAll();
		}
	}
	
	
	public void enableShowFPS(boolean enable){
		SVIWallpaperNative.nativeEnableShowFPS(enable,mSurfaceNativeHandle);
	}
	
	public void enableLogFPS(boolean enable){
		SVIWallpaperNative.nativeEnableLogFPS(enable,mSurfaceNativeHandle);
	}
	
	//2012-01-13 masterkeaton27@gmail.com
	public void enableTranslucent(boolean enable){
		SVIWallpaperNative.nativeEnableTranslucent(enable,mSurfaceNativeHandle);
	}	
	
	public void swapAnimationSVIGLSurface() {
		SVIWallpaperNative.nativeSwapAnimationSVIGLSurface(mSurfaceNativeHandle);
	}	
}

interface EGLConfigChooser {
	EGLConfig chooseConfig(EGL10 egl, EGLDisplay display);
}

abstract class BaseConfigChooser implements EGLConfigChooser {
	public BaseConfigChooser(int[] configSpec) {
		mConfigSpec = configSpec;
	}

	public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
		int[] num_config = new int[1];
		egl.eglChooseConfig(display, mConfigSpec, null, 0, num_config);

		int numConfigs = num_config[0];

		if (numConfigs <= 0) {
			throw new IllegalArgumentException("No configs match configSpec");
		}

		EGLConfig[] configs = new EGLConfig[numConfigs];
		egl.eglChooseConfig(display, mConfigSpec, configs, numConfigs, num_config);
		EGLConfig config = chooseConfig(egl, display, configs);
		if (config == null) {
			throw new IllegalArgumentException("No config chosen");
		}
		return config;
	}

	abstract EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs);

	protected int[] mConfigSpec;
	public static class ComponentSizeChooser extends BaseConfigChooser {
		public ComponentSizeChooser(int redSize, int greenSize, int blueSize, int alphaSize, int depthSize,
				int stencilSize) {
			super(new int[] { EGL10.EGL_RED_SIZE, redSize, EGL10.EGL_GREEN_SIZE, greenSize, EGL10.EGL_BLUE_SIZE,
					blueSize, EGL10.EGL_ALPHA_SIZE, alphaSize, EGL10.EGL_DEPTH_SIZE, depthSize, EGL10.EGL_STENCIL_SIZE,
					stencilSize, EGL10.EGL_NONE });
			mValue = new int[1];
			mRedSize = redSize;
			mGreenSize = greenSize;
			mBlueSize = blueSize;
			mAlphaSize = alphaSize;
			mDepthSize = depthSize;
			mStencilSize = stencilSize;
		}

		@Override
		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs) {
			EGLConfig closestConfig = null;
			int closestDistance = 1000;
			for (EGLConfig config : configs) {
				int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
				int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);
				if (d >= mDepthSize && s >= mStencilSize) {
					int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE, 0);
					int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
					int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);
					int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);
					int distance = Math.abs(r - mRedSize) + Math.abs(g - mGreenSize) + Math.abs(b - mBlueSize)
					+ Math.abs(a - mAlphaSize);
					if (distance < closestDistance) {
						closestDistance = distance;
						closestConfig = config;
					}
				}
			}
			return closestConfig;
		}

		private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue) {

			if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
				return mValue[0];
			}
			return defaultValue;
		}

		private int[] mValue;
		// Subclasses can adjust these values:
		protected int mRedSize;
		protected int mGreenSize;
		protected int mBlueSize;
		protected int mAlphaSize;
		protected int mDepthSize;
		protected int mStencilSize;
	}

	/**
	 * This class will choose a supported surface as close to RGB565 as possible, with or without a depth buffer.
	 *
	 */
	public static class SimpleEGLConfigChooser extends ComponentSizeChooser {
		public SimpleEGLConfigChooser(boolean withDepthBuffer) {
			super(8, 8, 8, 8, withDepthBuffer ? 16 : 0, 0);
			// Adjust target values. This way we'll accept a 4444 or
			// 555 buffer if there's no 565 buffer available.
			mRedSize = 8;
			mGreenSize = 8;
			mBlueSize = 8;
		}
	}
}


class SVIWallpaperNative{
	
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}

	public static native int nativeGetHandle(SVIGLWallpaperService obj);//Multi-Instance-Support
	public static native void nativeInitSVIGLSurface(int enableNativeTestBed,int surfaceNativeHandle);
	public static native void nativeResizeSVIGLSurface(int w, int h, int rotation,int surfaceNativeHandle);
	public static native void nativeRenderSVIGLSurface(int updateMsg,int surfaceNativeHandle);
	public static native void nativeResumeSVIGLSurface(int surfaceNativeHandle);
	public static native void nativePauseSVIGLSurface(int surfaceNativeHandle);
	public static native void nativeClearBlurCapture(int surfaceNativeHandle);	
	public static native void nativeSetBlurCapture(int level,int surfaceNativeHandle);
	public static native void nativeSwapAnimationSVIGLSurface(int surfaceNativeHandle);
	public static native void nativeSetAssetManager(AssetManager assetManager,int surfaceNativeHandle);
	public static native void nativeEnableShowFPS(boolean enable,int surfaceNativeHandle);
	public static native void nativeEnableLogFPS(boolean enable,int surfaceNativeHandle);
	public static native void nativeEnableTranslucent(boolean enable,int surfaceNativeHandle);
}