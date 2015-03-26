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
package com.github.sviengine.basetype;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.util.Log;

import java.nio.ByteBuffer;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.glsurface.SVIGLSurface;



/**
 * SVIImage Class
 * make SVIImage from image resource
 */
public class SVIImage {
	
	/**
     * Compress Texture type
     */
    public interface CompressTextureType {
        public static final int COMPRESS_ATC = 0;
        public static final int COMPRESS_PVR = 1;
    }
	
	/**
	 * AlpahType { NORMAL, PREMULTIPLIED }
	 */
	public enum AlphaType { NORMAL, PREMULTIPLIED };
	
	private SVIGLSurface mSVIGLSurface;
	
	public SVIImage() {
		this(null);
	}
	
	/**
	 * constructor
	 */
	SVIImage(SVIGLSurface surface) {
		mSVIGLSurface = SVIGLSurface.getSurface(surface);
		mSVIGLSurface.incrementUsageCount(this);
		
		mSurfaceNativeHandle = mSVIGLSurface.getNativeHandle();
		mNativeHandle = nativeCreateSVIImage();
		mAlphaType = AlphaType.NORMAL;
	}

	/**
	 * lock
	 */
	public void lock() {
		nativeLock(mNativeHandle);
	}

	/**
	 * unlock
	 */
	public void unlock() {
		nativeUnlock(mNativeHandle);
	}

	/**
	 * set bitmap to SVIImage
	 *
	 * @param bitmap > bitmap resource
	 */
	public void setBitmap(Bitmap bitmap) {
		if( mNativeHandle == 0 )
			return;
		
		if (bitmap != null && (bitmap.getWidth() <= 0 || bitmap.getHeight() <= 0)){
        	return;
        }
        

		if( mBitmap != null )
			mBitmap = null;
		
		mBitmap = bitmap;

		lock();
		nativeSetBitmap(mSurfaceNativeHandle, mNativeHandle, mBitmap);
		unlock();
	}
	
	/**
	 * copy bitmap to SVIImage
	 *
	 * @param bitmap > bitmap resource
	 */
	public void copyBitmap(Bitmap bitmap) {
		if( mNativeHandle == 0 )
			return;
		
		if (bitmap != null && (bitmap.getWidth() <= 0 || bitmap.getHeight() <= 0)){
        	return;
        }
        
		if( mBitmap != null )
			mBitmap = null;
		
		mBitmap = bitmap;

		lock();
		nativeSetCopyBitmap(mNativeHandle, mBitmap);
		unlock();
	}
	
	/**
	 * set bitmap and AlphaType to SVIImage
	 * @see AlphaType
	 *
	 * @param bitmap > bitmap resource
	 * @param alphaType > AlphaType
	 */
	public void setBitmap(Bitmap bitmap, AlphaType alphaType) {
		if( mNativeHandle == 0 )
			return;

		if (bitmap != null && (bitmap.getWidth() <= 0 || bitmap.getHeight() <= 0)){
        	return;
        }
        		
		if( mBitmap != null )
			mBitmap = null;
		
		mBitmap = bitmap;
		mAlphaType = alphaType;

		lock();
		nativeSetBitmap(mSurfaceNativeHandle, mNativeHandle, mBitmap);
		if( mAlphaType == AlphaType.NORMAL )
			nativeSetAlphaType(mNativeHandle, 0);
		else
			nativeSetAlphaType(mNativeHandle, 1);
		unlock();
	}
	
	/**
	 * copy bitmap and AlphaType to SVIImage
	 * @see AlphaType
	 *
	 * @param bitmap > bitmap resource
	 * @param alphaType > AlphaType
	 */
	public void copyBitmap(Bitmap bitmap, AlphaType alphaType) {
		if( mNativeHandle == 0 )
			return;
		
		if (bitmap != null && (bitmap.getWidth() <= 0 || bitmap.getHeight() <= 0)){
        	return;
        }
        
		if( mBitmap != null )
			mBitmap = null;
		
		mBitmap = bitmap;
		mAlphaType = alphaType;

		lock();
		nativeSetCopyBitmap(mNativeHandle, mBitmap);
		if( mAlphaType == AlphaType.NORMAL )
			nativeSetAlphaType(mNativeHandle, 0);
		else
			nativeSetAlphaType(mNativeHandle, 1);
		unlock();
	}
	
	/**
	 * set Raw image Buffer to SVIImage
	 * @param width > width of image
	 * @param height > height of image
	 * @param stride > stride of image
	 * @param format > format of image (int)
	 * @param buffer > Raw Buffer
	 */
	public void setBuffer(int width, int height, int stride, int format, byte[] buffer){
		if( mNativeHandle == 0 )
			return;
		
		mBitmap = null;
		mAlphaType = AlphaType.NORMAL;

		lock();
		nativeSetRawBufferDirect(mNativeHandle, width, height, stride, format,  buffer);
		if( mAlphaType == AlphaType.NORMAL )
			nativeSetAlphaType(mNativeHandle, 0);
		else
			nativeSetAlphaType(mNativeHandle, 1);
		unlock();
	}
	
	/**
	 * set Raw image Buffer to SVIImage
	 * @param width > width of image
	 * @param height > height of image
	 * @param stride > stride of image
	 * @param config > format of image (android bitmap configuration format)
	 * @param buffer > Raw Buffer
	 */
	public void setBuffer(int width, int height, int stride, Config config, byte[] buffer){
		if( mNativeHandle == 0 )
			return;
		
		mBitmap = null;
		mAlphaType = AlphaType.NORMAL;

		int format = 1;
		if (config == Config.ALPHA_8){
			format = 8;
		}else if (config == Config.ARGB_4444){
			format = 7;
		}else if (config == Config.RGB_565){
			format = 4;
		}
		
		lock();
		nativeSetRawBufferDirect(mNativeHandle, width, height, stride, format,  buffer);
		if( mAlphaType == AlphaType.NORMAL )
			nativeSetAlphaType(mNativeHandle, 0);
		else
			nativeSetAlphaType(mNativeHandle, 1);
		unlock();
	}
	
	
		
	/**
	 * copy bitmap and AlphaType to SVIImage
	 * @see AlphaType
	 *
	 * @param bitmap > bitmap resource
	 * @param alphaType > AlphaType
	 */
	public void setEmpty(){
		if( mNativeHandle == 0 )
			return;
		
		mBitmap = null;
		lock();
		nativeSetEmpty(mNativeHandle);
		unlock();
	}

	/** finalize
	 * @see java.lang.Object#finalize()
	 */
	public void finalize() throws Throwable  {
		setBitmap(null);
		mBitmap = null;
		if( mReportFinalize )
			Log.i("SVI", "SVIImage is finalized nativeHandle:"+mNativeHandle);
		if( mNativeHandle != 0 )
			nativeDeleteSVIImage(mSurfaceNativeHandle, mNativeHandle);
		
		mSVIGLSurface.decrementUsageCount(this);
		super.finalize();
	}

	/**
	 * set AlphaType
	 *
	 * @param alphaType > AlphaType 
	 */
	public void setAlphaType(AlphaType alphaType)
	{
		if( mNativeHandle == 0 )
			return;
		
		mAlphaType = alphaType;
		
		if( mAlphaType == AlphaType.NORMAL )
			nativeSetAlphaType(mNativeHandle, 0);
		else
			nativeSetAlphaType(mNativeHandle, 1);
	}
	
	/**
	 * get AlphaType
	 *
	 * @see AlphaType
	 * @return AlphaType mAlphaType 
	 */
	public AlphaType getAlphaType() { return mAlphaType; }

	/**
	 * set native handle
	 *
	 * @return int mNativeHandle
	 */
	public int getNativeHandle() { return mNativeHandle; }

	/**
	 * set report finalize
	 *
	 * @param report > true = report finalize  false = do not report finalize
	 */
	public void setReportFinalize(boolean report) { mReportFinalize = report; }

	public Bitmap		mBitmap = null;
	private int			mNativeHandle = 0;
	private AlphaType	mAlphaType;
	private int mSurfaceNativeHandle = 0;
	private boolean		mReportFinalize = false;

	
	//2013/01/25 rareboy0112@facebook.com
	//start support compressed texture
	public byte[]		mBytes = null;
	
	public void setEmpty_compress(){
		if( mNativeHandle == 0 )
			return;
		
		mBytes = null;
		lock();
		nativeSetEmpty(mNativeHandle);
		unlock();
	}
	
	public void setBitmap_compress(int type, byte[] bytes) {
		if( mNativeHandle == 0 )
			return;

		if( mBytes != null )
			mBytes = null;
		
		mBytes = bytes;

		lock();
		nativeSetBitmapCompress(mNativeHandle, type, mBytes);
		unlock();
	}
	
	/*****************************************************************************************************************/
	/**
	  * SVIImage native interface.
	  *
	  */
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}

	private static native int  nativeCreateSVIImage();
	private static native void nativeDeleteSVIImage(int surfaceNativeHandle, int handle);
	private static native void nativeLock(int handle);
	private static native void nativeUnlock(int handle);
	private static native void nativeSetEmpty(int handle);
	private static native void nativeSetCopyBitmap(int handle, Bitmap bitmap);
	private static native void nativeSetRawBufferDirect(int handle, int width, int height, int stride, int format, byte bytes[]);
	private static native void nativeSetBitmap(int surfaceNativeHandle, int handle, Bitmap bitmap);
	private static native void nativeSetBitmapCompress(int handle, int type, byte bytes[]);
	private static native void nativeSetAlphaType(int handle, int alphaType);
}
