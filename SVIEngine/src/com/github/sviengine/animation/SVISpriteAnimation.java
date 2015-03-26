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

package com.github.sviengine.animation;

import java.util.ArrayList;

import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype._PackageAccessBasetype;
import com.github.sviengine.glsurface.SVIGLSurface;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.util.Log;


/** 
 * SVISpriteAnimation Class
 * Sprite Animation use One image resource and make animation via divided same rectangle from resource.
 */
public class SVISpriteAnimation extends SVIAnimation {
	
	public SVISpriteAnimation(PlayType playType, SVIImage image, int frameWidth, int frameHeight) {
		this(null, playType, image, frameWidth, frameHeight);
	}
	public SVISpriteAnimation(PlayType playType, ArrayList<Bitmap> listBitmap) {
		this(null, playType, listBitmap);
	}
    
    /** 
     * PlayType
     * PLAY_ALL or PLAY_PARTIAL
     */
    public enum PlayType {PLAY_ALL, PLAY_PARTIAL };
    
    /** 
     * constructor use property
     * 
     * @see SVIImage
     * @see PlayType
     * @param playType > playtype
     * @param image > image resource (SVIImage)
     * @param frameWidth > one frame width
     * @param frameHeight > one frame height
     */
    public SVISpriteAnimation(SVIGLSurface saglSurface, PlayType playType, SVIImage image, int frameWidth, int frameHeight) {
    	super(saglSurface);
    	
        mPlayType = playType;

        int type = 0;
        if( playType == PlayType.PLAY_PARTIAL )
            type = 1;

        mClassType = SVIAnimationClass.SPRITE;
        mNativeAnimation = nativeCreateSVISpriteAnimation(type, image.getNativeHandle(), frameWidth, frameHeight, mSVIGLSurface.getNativeHandle());
    }

    /** 
     * constructor use PlayType and bitmap array
     * 
     * @see PlayType
     * @param playType > play type
     * @param listBitmap > bitmap array
     */
    public SVISpriteAnimation(SVIGLSurface saglSurface, PlayType playType, ArrayList<Bitmap> listBitmap) {
    	super(saglSurface);
    	
        mPlayType = playType;

        // int type = 0;
        // if( playType == PlayType.PLAY_PARTIAL )
        // type = 1;

        // Make single image from multiple images.
        final int bitmapWidth = listBitmap.get(0).getWidth();
        final int bitmapHeight = listBitmap.get(0).getHeight();
        final int listLength = listBitmap.size();
        int left = 0;
        int top = 0;
        Bitmap srcImg = null;
        Bitmap spriteBitmap = null;

        // temp
        final int maxWidth = 4000;
        // final int maxHeight = 4000;
        int maxColumn = maxWidth / bitmapWidth;

        int rowCount = listLength / maxColumn + 1;
        int colCount = 0;
        if (rowCount > 1)
        colCount = maxColumn;
        else
        colCount = listLength;

        spriteBitmap = Bitmap.createBitmap(bitmapWidth * colCount, bitmapHeight * rowCount, Bitmap.Config.ARGB_8888);
        Canvas bitmapCanvas = new Canvas(spriteBitmap);

        for (int i = 0; i < listLength; i++) {
            // Left position !!
            if (i % colCount == 0) {
                left = 0;
                if (i == 0) {
                    top = 0;
                } else {
                    top += bitmapHeight;
                }
            } else {
                left += bitmapWidth;
            }

            // Draw bitmap to bitmap-canvas.
            srcImg = listBitmap.get(i);
            bitmapCanvas.drawBitmap(srcImg, left, top, null);
        }

        mSpriteImage = _PackageAccessBasetype.createSVIImage(saglSurface);
        mSpriteImage.setBitmap(spriteBitmap);

        mClassType = SVIAnimationClass.SPRITE;
        mNativeAnimation = nativeCreateSVISpriteAnimation(1, mSpriteImage.getNativeHandle(), bitmapWidth, bitmapHeight, mSVIGLSurface.getNativeHandle());
        nativeSetIntervalSVISpriteAnimation(mNativeAnimation, 0, listLength - 1);
    }

    /** 
     * set interval
     * 
     * @param fromIndex > start frame index 
     * @param toIndex > end frame index
     */
    public void setInterval(int fromIndex, int toIndex) {
        if (mPlayType == PlayType.PLAY_PARTIAL)
            nativeSetIntervalSVISpriteAnimation(mNativeAnimation, fromIndex, toIndex);
    }
    
    /** 
     * finalize
     * 
     * @see com.github.sviengine.animation.SVIAnimation#finalize()
     */
    protected void finalize() throws Throwable {
	super.finalize();
	Log.e("SVIEngine", "SVISpriteAnimation's finalize");
    	deleteNativeAnimationHandle();
    }

    public PlayType mPlayType;
    private SVIImage mSpriteImage;

    private static native int nativeCreateSVISpriteAnimation(int playType, int imageHandle, int frameWidth, int frameHeight, int nativeGLSurface);
    private static native void nativeSetIntervalSVISpriteAnimation(int nativeHandle, int fromIndex, int toIndex);

}
