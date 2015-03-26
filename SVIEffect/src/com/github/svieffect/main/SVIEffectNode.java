/**
 * SGIEffectNode.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.main;

import android.graphics.Bitmap;
import android.view.MotionEvent;

import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.slide.SVISlide;


public class SVIEffectNode implements SVIEffectNodeEvent{
	
	public SVIEffectNode(){
		mColor = new SVIColor(1.0f,1.0f,1.0f,1.0f);
		mRegion = new SVIRect();
		mBaseRegion = new SVIRect();
		mContentRegion = new SVIRect();
		mOverlayContentRegion = new SVIRect();
	}
	
	public SVIEffectNode(SVISlide slide){
		mColor = new SVIColor(1.0f,1.0f,1.0f,1.0f);
		mRegion = new SVIRect();
		mBaseRegion = new SVIRect();
		mContentRegion = new SVIRect();
		mOverlayContentRegion = new SVIRect();
		mSlide = slide;
	}
	
	public SVIEffectNode(SVISlide slide, SVISlide contentSlide){
		mColor = new SVIColor(1.0f,1.0f,1.0f,1.0f);
		mRegion = new SVIRect();
		mBaseRegion = new SVIRect();
		mContentRegion = new SVIRect();
		mOverlayContentRegion = new SVIRect();
		mSlide = slide;
		mContentSlide = contentSlide;
	}
	
	public void setNormalHeight(float height){
		mNormalHeight = height;
	}
	
	public void setSlide(SVISlide slide){
		mSlide = slide;
	}
	
	public void setBaseSlide(SVISlide slide){
		mBaseSlide = slide;
	}
	
	public void setContentSlide(SVISlide slide){
		mContentSlide = slide;
	}
	
	public void setOverlayContentSlide(SVISlide slide){
		mOverlayContentSlide = slide;
	}
	
	
	public void setIndex(int index){
		mIndex = index;
	}	
	
	public void setRegion(SVIRect region){
		mRegion.mOrigin.mX = region.mOrigin.mX;
		mRegion.mOrigin.mY = region.mOrigin.mY;		
		mRegion.mSize.mWidth = region.mSize.mWidth;
		mRegion.mSize.mHeight = region.mSize.mHeight;
	}
	
	public void setRegion(float fLeft, float fTop, float fWidth, float fHeight){
		mRegion.mOrigin.mX = fLeft;
		mRegion.mOrigin.mY = fTop;		
		mRegion.mSize.mWidth = fWidth;
		mRegion.mSize.mHeight = fHeight;
	}
	
	
	public void setContentRegion(SVIRect region){
		mContentRegion.mOrigin.mX = region.mOrigin.mX;
		mContentRegion.mOrigin.mY = region.mOrigin.mY;		
		mContentRegion.mSize.mWidth = region.mSize.mWidth;
		mContentRegion.mSize.mHeight = region.mSize.mHeight;
		
		mContentRegionSetted = true;
	}
		
	public void setContentRegion(float left, float top, float width, float height){
		mContentRegion.mOrigin.mX = left;
		mContentRegion.mOrigin.mY = top;		
		mContentRegion.mSize.mWidth = width;
		mContentRegion.mSize.mHeight = height;				
		mContentRegionSetted = true;
	}
	
	public void setBaseRegion(float left, float top, float width, float height){
		mBaseRegion.mOrigin.mX = left;
		mBaseRegion.mOrigin.mY = top;		
		mBaseRegion.mSize.mWidth = width;
		mBaseRegion.mSize.mHeight = height;				
		mBaseRegionSetted = true;
	}
	
	public void setBaseRegion(SVIRect region){
		mBaseRegion.mOrigin.mX = region.mOrigin.mX;
		mBaseRegion.mOrigin.mY = region.mOrigin.mY;		
		mBaseRegion.mSize.mWidth = region.mSize.mWidth;
		mBaseRegion.mSize.mHeight = region.mSize.mHeight;
		mBaseRegionSetted = true;
	}
		
	public void setOverlayContentRegion(SVIRect region){
		mOverlayContentRegion.mOrigin.mX = region.mOrigin.mX;
		mOverlayContentRegion.mOrigin.mY = region.mOrigin.mY;		
		mOverlayContentRegion.mSize.mWidth = region.mSize.mWidth;
		mOverlayContentRegion.mSize.mHeight = region.mSize.mHeight;
		mOverlayContentRegionSetted = true;
	}
		
	public void setOverlayContentRegion(float left, float top, float width, float height){
		mOverlayContentRegion.mOrigin.mX = left;
		mOverlayContentRegion.mOrigin.mY = top;		
		mOverlayContentRegion.mSize.mWidth = width;
		mOverlayContentRegion.mSize.mHeight = height;		
		mOverlayContentRegionSetted = true;
	}
	
		
	public void setColor(SVIColor color){
		mColor.mR = color.mR;
		mColor.mG = color.mG;
		mColor.mB = color.mB;
		mColor.mA = color.mA;
	}
	
	public void setColor(float fR, float fG, float fB, float fAlpha){
		mColor.mR = fR;
		mColor.mG = fG;
		mColor.mB = fB;
		mColor.mA = fAlpha;
	}
	
	public void setContentGUI(Bitmap colorBmp){
		mContentBitmap = colorBmp;
		
		if (mContentImage == null){
			mContentImage = new SVIImage();
			mContentImage.setBitmap(mContentBitmap);
		}else{			
			mContentImage = new SVIImage();
			mContentImage.setBitmap(mContentBitmap);						
			if (mContentSlide != null){
				mContentSlide.setImage((SVIImage)null);
				mContentSlide.setImage(mContentImage);
			}
		}
		mHasContent = true;
	}
	
	public void setOverlayContentGUI(Bitmap colorBmp){
		mOverlayContentBitmap = colorBmp;
		if (mOverlayContentImage == null){
			mOverlayContentImage = new SVIImage();
			mOverlayContentImage.setBitmap(mOverlayContentBitmap);
		}else{			
			mOverlayContentImage = new SVIImage();
			mOverlayContentImage.setBitmap(mOverlayContentBitmap);						
			if (mOverlayContentSlide != null){
				mOverlayContentSlide.setImage((SVIImage)null);
				mOverlayContentSlide.setImage(mOverlayContentImage);
			}
		}
		mHasOverlayContent = true;
	}
	
	public void setColorGUI(Bitmap colorBmp){
		mColorBitmap = colorBmp;
		if (mColorImage == null){
			mColorImage = new SVIImage();
			//mColorImage.setAlphaType(AlphaType.PREMULTIPLIED);
			mColorImage.setBitmap(mColorBitmap);
		}else{
			mColorImage = new SVIImage();
			mColorImage.setBitmap(mColorBitmap);						
			if (mSlide != null){
				mSlide.setImage((SVIImage)null);
				mSlide.setImage(mColorBitmap);
			}
		}
	}
	
	public void setBaseGUI(Bitmap baseBmp){
		mBaseBitmap = baseBmp;
		if (mBaseImage == null){
			mBaseImage = new SVIImage();
			//mBaseImage.setAlphaType(AlphaType.PREMULTIPLIED);
			mBaseImage.setBitmap(mBaseBitmap);
		}else{
			mBaseImage = new SVIImage();
			mBaseImage.setBitmap(mBaseBitmap);						
			if (mBaseSlide != null){
				mBaseSlide.setImage((SVIImage)null);
				mBaseSlide.setImage(mBaseBitmap);
			}
		}
		mHasBase = true;
	}
	
	public void setNormalMapGUI(Bitmap normalmapBmp){
		mNormalBitmap = normalmapBmp;
		if (mNormalImage == null){
			mNormalImage = new SVIImage();
			mNormalImage.setBitmap(mNormalBitmap);
		}else{
			mNormalImage = new SVIImage();
			mNormalImage.setBitmap(mNormalBitmap);						
			if (mSlide != null){
				mSlide.setNormalImage((SVIImage)null);
				mSlide.setNormalImage(mNormalBitmap);
			}
		}
	}
	
	public void setSpecularMapGUI(Bitmap specularmapBmp){
		mSpecularBitmap = specularmapBmp;
		if (mSpecularImage == null){
			mSpecularImage = new SVIImage();
			mSpecularImage.setBitmap(mSpecularBitmap);
		}else{
			mSpecularImage = new SVIImage();
			mSpecularImage.setBitmap(mSpecularBitmap);						
			if (mSlide != null){
				mSlide.setSpecularImage((SVIImage)null);
				mSlide.setSpecularImage(mSpecularBitmap);
			}
		}
	}
	
	public void cancelUpload(){
		/*
		if (mOverlayContentImage != null){
			mOverlayContentImage.setEmpty();
		}
		
		if (mContentImage != null){
			mContentImage.setEmpty();
		}
		
		if (mColorImage != null){
			mColorImage.setEmpty();
		}
		
		if (mNormalImage != null){
			mNormalImage.setEmpty();
		}
		
		if (mSpecularImage != null){
			mSpecularImage.setEmpty();
		}
		
		if (mBaseImage != null){
			mBaseImage.setEmpty();
		}*/
	}
	
	public SVISlide getSlide(){
		return mSlide;
	}
	
	public SVISlide getContentSlide(){
		return mContentSlide;
	}
	
	public SVISlide getBaseSlide(){
		return mBaseSlide;
	}
	
	public SVISlide getOverlayContentSlide(){
		return mOverlayContentSlide;
	}
	
	public int getIndex(){return mIndex;}
	
	public SVIRect getRegion(){
		return mRegion;
	}
	
	public SVIRect getBaseRegion(){
		return mBaseRegion;
	}
	
	public SVIRect getContentRegion(){
		return mContentRegion;
	}
	
	public SVIRect getOverlayContentRegion(){
		return mOverlayContentRegion;
	}
	
	public float getNormalHeight(){
		return mNormalHeight;
	}
	
	public SVIColor getColor(){
		return mColor;
	}
	
	public boolean isOverlayContentRegionSetted(){
		return mOverlayContentRegionSetted;
	}
	
	public boolean isContentRegionSetted(){
		return mContentRegionSetted;
	}
	
	public boolean isBaseRegionSetted(){
		return mBaseRegionSetted;
	}
	
	public boolean hasContent(){
		return mHasContent;
	}
	
	public boolean hasBase(){
		return mHasBase;
	}
	
	public boolean hasOverlayContent(){
		return mHasOverlayContent;
	}
	
	public SVIImage getColorGUIImage(){
		return mColorImage;
	}
	
	public SVIImage getContentGUIImage(){
		return mContentImage;
	}
	
	public SVIImage getBaseGUIImage(){
		return mBaseImage;
	}
	
	public SVIImage getOverlayContentGUIImage(){
		return mOverlayContentImage;
	}
	
	public SVIImage getNormalMapGUIImage(){
		return mNormalImage;
	}
	
	public SVIImage getSpecularMapGUIImage(){
		return mSpecularImage;
	}
	
	
	private boolean mBaseRegionSetted = false;
	private boolean mContentRegionSetted = false;
	private boolean mOverlayContentRegionSetted = false;
	
	private boolean mHasContent = false;
	private boolean mHasBase = false;
	private boolean mHasOverlayContent = false;
	
	private int		mIndex = 0;
	private float	mNormalHeight = -1.0f;
	private SVIColor mColor = null;
	
	private SVIRect 	mRegion = null;
	private SVIRect 	mBaseRegion = null;	
	private SVIRect 	mContentRegion = null;
	private SVIRect 	mOverlayContentRegion = null;
	
	
	//2012-12-07 content slide added. due to UX request.
	private SVISlide mContentSlide = null;
	private SVISlide mOverlayContentSlide = null;
	private SVISlide mSlide = null;
	private SVISlide mBaseSlide = null;
	
	private Bitmap 	mOverlayContentBitmap = null;
	private SVIImage mOverlayContentImage = null;
	
	private Bitmap 	mContentBitmap = null;
	private SVIImage mContentImage = null;
	
	private Bitmap 	mColorBitmap = null;
	private SVIImage mColorImage = null;
	
	private Bitmap 	mBaseBitmap = null;
	private SVIImage mBaseImage = null;
	
	private SVIImage mNormalImage = null;
	private Bitmap 	mNormalBitmap = null;
	
	private SVIImage mSpecularImage = null;
	private Bitmap 	mSpecularBitmap = null;
	
	
	
	@Override
	public boolean onClick(int idx, int type, MotionEvent event) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onLongPress(int idx, int type, MotionEvent event) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onHovering(int idx, int type, MotionEvent event) {
		// TODO Auto-generated method stub
		return false;
	}
	
}
