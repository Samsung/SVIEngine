/*
 * Copyright (C) 2008 The Android Open Source Project
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
package com.github.svieffect.copyandpaste;

import java.util.Vector;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.Log;

public class FXGCopyAndPasteView extends GLSurfaceView
{
	FXGCopyAndPasteRenderer mRenderer = null;
	
    public FXGCopyAndPasteView(Context context)
    {
        super(context);
        init(context);
    }
    
    /**
     * A dummy.
     */
    public boolean createSlide()
    {
    	return true;
    }

	/**
	 * A dummy.
	 */
    public void createEffectSlide()
    {
    	
    }

	/**
	 * A dummy.
	 */
    public void updateRoot()
    {
    	
    }
    
    /**
     * A dummy.
     * @return true
     */
    public boolean reLayout()
    {
    	return true;
    }

	/**
	 * A dummy.
	 */
    public void addPoint()
    {
    	
    }
    
    public void setCallbackListener(SVICopyAndPasteCallback callback)
    {
    	mRenderer.setCallbackListener(callback);
    }
    
    public void setIntPoints(Vector<Point> points)
    {
    	float[] vertices = new float[points.size() * 2];
    	for(int i = 0; i < points.size(); i++)
    	{
    		vertices[i*2] = points.get(i).x;
    		vertices[i*2+1] = points.get(i).y;
    	}
    	
    	mRenderer.setPoints(vertices);
    }

    /*
     * Needs setFloatPoints(Vector<SAVector2>):void to be added here.
     */

    /**
     * A dummy.
     * @param a nothing
     * @param b nothing
     * @param c nothing
     * @param d nothing
     */
    public void setWindowRegion(float a, float b, float c, float d)
    {
    	
    }
    
    public void setWindowImage(Bitmap bitmap)
    {
    	mRenderer.setWindowImage(bitmap);
    }
    
    /**
     * A dummy
     * @param a nothing
     */
    public void setMorphingRadius(float a)
    {
    	
    }
    
    /**
     * A dummy
     * @param a nothing
     */
    public void setMorphingGridCount(int a)
    {
    	
    }
    
    /**
     * A dummy
     * @param a nothing
     */
    public void setMorphingVertexCount(int a)
    {
    	
    }
    
    /**
     * A dummy
     * @param a nothing
     */
    public void setMorphingColor(float[] a)
    {
    	
    }
    
    public void play(int duration)
    {
    	mRenderer.setAnimationDurationTime(duration / 1000.0f);
    	mRenderer.play();
    }

	/**
	 * A dummy.
	 */
    public void onUpdateFinished()
    {
    	
    }

	/**
	 * A dummy.
	 */
    public void animationSlideNotify()
    {
    	
    }

    /**
     * A dummy.
     * @return true
     */
    public boolean requestSlideRender()
    {
    	return true;
    }
    
	/**
	 * A dummy.
	 */
    public void destroy()
    {
    }
    
    /**
     * A dummy.
     * @return true
     */
    public boolean destroySlide()
    {
    	return true;
    }
    
    /**
     * A dummy.
     */
    public void show()
    {
    }
    
    /**
     * A dummy.
     */
    public void hide()
    {
    }
    
    private void init(Context context)
    {
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
        setEGLContextFactory(new ContextFactory());
        setEGLConfigChooser(new ConfigChooser(8, 8, 8, 8, 0, 0));
        
        mRenderer = new FXGCopyAndPasteRenderer(context);
        setRenderer(mRenderer);
    }
    
    public static class ContextFactory implements GLSurfaceView.EGLContextFactory 
    {
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig)
        {
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
                EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
                
                checkEglError("createContext", egl);
                
                return context;
            }

            public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context)
            {
                egl.eglDestroyContext(display, context);
            }
        }

        private static void checkEglError(String prompt, EGL10 egl)
        {
            int error;
            while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS)
            {
                Log.e("", String.format("%s: EGL error: 0x%x", prompt, error));
            }
        }

        public static class ConfigChooser implements GLSurfaceView.EGLConfigChooser
        {

            public ConfigChooser(int r, int g, int b, int a, int depth, int stencil)
            {
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
                EGL10.EGL_RED_SIZE, 4,
                EGL10.EGL_GREEN_SIZE, 4,
                EGL10.EGL_BLUE_SIZE, 4,
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

//                if (DEBUG) {
//                     printConfigs(egl, display, configs);
//                }
                /* Now return the "best" one
                 */
                return chooseConfig(egl, display, configs);
            }

            public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
                    EGLConfig[] configs) {
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

                    if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                        return config;
                }
                return null;
            }

            private int findConfigAttrib(EGL10 egl, EGLDisplay display,
                    EGLConfig config, int attribute, int defaultValue) {

                if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
                    return mValue[0];
                }
                return defaultValue;
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

    public class FXGCopyAndPasteRenderer implements GLSurfaceView.Renderer
    {
    	private Context mContext = null;
    	private SVICopyAndPasteCallback mCallback;

    	private float[] mVertices = null;
    	private Bitmap mWindowImage = null;

    	private boolean mWillAnimate = false;
    	private boolean mIsAnimating = false;
    	private boolean mWasAnimating = false;

    	private long previousTimeMillis;
    	
    	public FXGCopyAndPasteRenderer(Context context)
    	{
    		mContext = context;
    	}

        public void onSurfaceChanged(GL10 gl, int width, int height)
        {
        	nativeInit(width, height);
        	previousTimeMillis = System.currentTimeMillis();
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            // Do nothing.
        }
    	
        public void onDrawFrame(GL10 gl)
        {
        	if(mWillAnimate)
        	{
            	nativeSetup(mVertices, mWindowImage);
        		mWillAnimate = false;
            	mIsAnimating = true;
    			previousTimeMillis = System.currentTimeMillis();
        	}
        	
        	if(mIsAnimating)
        	{
            	float frameTime = (System.currentTimeMillis() - previousTimeMillis) / 1000.0f;
    			previousTimeMillis = System.currentTimeMillis();
        		mIsAnimating = nativeUpdate(frameTime);
        		nativeRender();
        		
        		if(mCallback != null)
        		{
        			if(!mWasAnimating)
        			{
        				((Activity)mContext).runOnUiThread(new Runnable()
        				{
        					@Override
        					public void run()
        					{
                				mCallback.onFirstRenderFinished();
        					}
        				});
        			}
        			if(!mIsAnimating)
        			{
//        				mWindowImage.recycle();
        				
        				((Activity)mContext).runOnUiThread(new Runnable()
        				{
        					@Override
        					public void run()
        					{
                				mCallback.onAnimationEnd();
        					}
        				});
        			}
        		}
        	}
        	else
        	{
				GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
        	}
        	
        	mWasAnimating = mIsAnimating;
        }
        
        public void setCallbackListener(SVICopyAndPasteCallback callback)
        {
        	mCallback = callback;
        }
        
        public void setWindowImage(Bitmap bitmap)
        {
//        	mWindowImage = bitmap;

            Log.v("", "bitmap.getWidth() : " + bitmap.getWidth());
            Log.v("", "bitmap.getHeight() : " + bitmap.getHeight());
            mWindowImage = Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), Bitmap.Config.ARGB_8888);
            Canvas canvas = new Canvas();
            canvas.setBitmap(mWindowImage);
            Paint paint = new Paint();
            paint.setFilterBitmap(true);
            canvas.drawBitmap(bitmap, 0, 0, paint);
            Log.v("", "bitmap.getConfig() : " + bitmap.getConfig());
        }
        
        public void setPoints(float[] points)
        {
        	mVertices = points;
        }
        
        public void play()
        {
        	mWillAnimate = true;
        }
        
        public void setAnimationDurationTime(float duration)
        {
        	nativeSetAnimationDuration(duration);
        }
    }

    public static native void nativeInit(int width, int height);
    public static native void nativeSetup(float[] verts, Bitmap bitmap);
    public static native boolean nativeUpdate(float frameTime);
    public static native boolean nativeRender();
    public static native void nativeSetAnimationDuration(float duration);
   
    static {
        System.loadLibrary("FXGCopyAndPaste");
    }
}
