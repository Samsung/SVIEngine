package com.github.sviengine.unittest;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.slide.SVISlide;

import android.content.Context;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class JacketViewTestWindow extends PlatformWindow{

	public JacketViewTestWindow(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		
		if( getMainSlide() == null )
			buildSubSlide();
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}
	
	@Override
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub
		super.onResize(width, height);
		
		mViewWidth = width;
		mViewHeight = height;
		
		initPosition(false, mJacketSlides, false);
		initPosition(false, mShadowSlides, true);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if( event.getAction() == MotionEvent.ACTION_DOWN ) {
			mDownXPos = event.getX();
			mLastMovePosX = mDownXPos;
			mStartJacketIndex = mCurrentJacket;
		} else if( event.getAction() == MotionEvent.ACTION_UP ) {
			initPosition(true, mJacketSlides, false);
			initPosition(true, mShadowSlides, true);
		} else if( event.getAction() == MotionEvent.ACTION_MOVE ) {
			float deltaX = event.getX() - mDownXPos;
			float lastDeltaX = event.getX() - mLastMovePosX;
			runAnimation(deltaX, lastDeltaX);
			mLastMovePosX = event.getX();
		}
		return true;
	}
	
	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();
		
		mJacketSlides = new SVISlide[JACKET_COUNT];
		mShadowSlides = new SVISlide[JACKET_COUNT];
		mCurrentJacket = (int)(JACKET_COUNT / 2.0f);
		
		SVISlide slide = null;
		SVIColor colors[] = new SVIColor[7];
		colors[0] = new SVIColor(1.0f, 0.0f, 0.0f, 1.0f);
		colors[1] = new SVIColor(0.0f, 1.0f, 0.0f, 1.0f);
		colors[2] = new SVIColor(0.0f, 0.0f, 1.0f, 1.0f);
		colors[3] = new SVIColor(1.0f, 1.0f, 0.0f, 1.0f);
		colors[4] = new SVIColor(1.0f, 0.0f, 1.0f, 1.0f);
		colors[5] = new SVIColor(0.5f, 0.5f, 0.5f, 1.0f);
		colors[6] = new SVIColor(1.0f, 1.0f, 1.0f, 1.0f);
		int imageIndex = 0;
		int imageCount = mImages.length;
		//float opacity = 0.3f;
		for(int i=0; i<JACKET_COUNT; i++) {
			slide = new SVISlide();
			imageIndex = i%imageCount;
			slide.setImage(mImages[imageIndex]);
			//slide.setBackgroundColor(colors[colorIndex]);
			getMainSlide().addSlide(slide);
			mJacketSlides[i] = slide;
		}
		
		for(int i=0; i<JACKET_COUNT; i++) {
			slide = new SVISlide();
			imageIndex = i%imageCount;
			slide.setImage(mImages[imageIndex]);
			slide.setBackgroundColor(colors[5]);
			slide.setBlendType(SVISlide.BlendType.BLEND_MULTIPLY);
			getMainSlide().addSlide(slide);
			mShadowSlides[i] = slide;
			//mShadowSlides[i].setOpacity(opacity);
		}
	}
	
	protected void initPosition(boolean ani, SVISlide[] jackSlide, boolean shadow) {
		//int duration = 0;
		
		int centerX = (int)(mViewWidth / 2.0f);
		int centerY = (int)(mViewHeight / 2.0f);
		
		int jacketWidth = JACKET_WIDTH;
		int jacketHeight = JACKET_HEIGHT;
		
		int xMargine = ITEM_MARGINE;
		int halfWidth = (int)(jacketWidth / 2.0f);
		int halfHeight = (int)(jacketHeight / 2.0f);
		SVISlide slide = null;
		
		int height = 0;
		float angleX = 0.0f;
		float angle = ANGLE;
	
		if(shadow){
			height = JACKET_HEIGHT;
			angleX = 180.0f;
		}
			
		if( ani ) {
			mSlideManager.checkoutAnimation();
			mSlideManager.setDuration(DURATION);
			mSlideManager.setInterpolatorType(mInterpolator);
		}
		for(int i=0; i<JACKET_COUNT; i++) {			
			slide = jackSlide[i];
			int x = centerX + ((i - mCurrentJacket)*xMargine) - halfWidth;
			int y = centerY - halfHeight;
			
			slide.setRegion(x, y+height, jacketWidth, jacketHeight);
			slide.setOrthogonal(false);
			if( i < mCurrentJacket ) {
				slide.setRotation(angleX, -angle, 0.0f, 0.0f);
				slide.setZPosition((mCurrentJacket-i)*10.0f);
			} else if( i > mCurrentJacket ) {
				slide.setRotation(angleX, angle, 0.0f, 0.0f);
				slide.setZPosition((i-mCurrentJacket)*10.0f);
			} else if(mCurrentJacket == i){
				slide.setRotation(angleX, 0.0f, 0.0f, 0.0f);
				slide.setZPosition(CENTER_ZPOSITION);
			}
		}
		if( ani )
			mSlideManager.checkinAnimation();
	}
	
	protected boolean checkUpdatePosition(float delta){
		if(((mCurrentJacket == 0) && delta > 0) 
				|| ((mCurrentJacket == JACKET_COUNT-1) && (delta < 0))){

				return true;
		}else{
			if((delta / 4.0f > 0.75)
					|| (delta / 4.0f < -0.75)){

				return false;
			}
		}
		return true;
	}
	
	protected boolean checkInitPosition(int newIndex){
		if(newIndex <=  -1 || newIndex >= JACKET_COUNT){
			return false;
		}
		return true;
	}

	protected void updatePosition(float deltaX, SVISlide[] jacketSlide, boolean shadow) {
		SVISlide slide = null;
		SVIPoint point = null;
		float newXPos = 0.0f;
		float angle = ANGLE;
		float angleX = 0.0f;
		
		if(!checkUpdatePosition(deltaX)){
			return;
		}
		
		if(shadow){
			angleX = 180.0f;
		}
			
		for(int i=0; i<JACKET_COUNT; i++) {			
			slide = jacketSlide[i];
			point = slide.getPosition();
			newXPos = point.mX + (deltaX/4.0f);
			
			slide.setPosition(newXPos, point.mY);
			if(i<mCurrentJacket){
				slide.setRotation(angleX, -angle, 0.0f, 0.0f);
				slide.setZPosition((mCurrentJacket-i)*10.0f);
			}else if(i>mCurrentJacket){
				slide.setRotation(angleX, angle, 0.0f, 0.0f);
				slide.setZPosition((i-mCurrentJacket)*10.0f);
			} else if(mCurrentJacket == i){
				slide.setRotation(angleX, 0.0f, 0.0f, 0.0f);
				slide.setZPosition(CENTER_ZPOSITION);
			}
		}
	}
	
	protected void runAnimation(float deltaX, float lastDeltaX) {
		int newIndex = calculateCurrentJacket(deltaX);
		if( newIndex != mCurrentJacket ) {
			if(!checkInitPosition(newIndex)){
				return;
			}
			mCurrentJacket = newIndex;
			initPosition(true ,mShadowSlides, true);
			initPosition(true ,mJacketSlides, false);
			
		} else {
			if( !mSlideManager.isAnimating() ) {
				updatePosition(lastDeltaX, mJacketSlides, false);
				updatePosition(lastDeltaX, mShadowSlides, true);
			}
		}
	}
	
	protected int calculateCurrentJacket(float deltaX) {
		int index = (int)(deltaX / ((float)ITEM_MARGINE/1.5f)) * -1;
		int newIndex = mStartJacketIndex + index;
		return newIndex;
	}
	 
	final float CENTER_ZPOSITION = -50.0f;
	final float	ANGLE = 70.0f;
	final int	DURATION = 400;
	final int	JACKET_COUNT = 50;
	final int	ITEM_MARGINE = 200;
	final int	JACKET_WIDTH = 256;
	final int	JACKET_HEIGHT = 256;
	int			mInterpolator = SVIAnimation.InterpolatorType.DECELERATE;
	SVISlide		mJacketSlides[];
	SVISlide		mShadowSlides[];
	int			mCurrentJacket = 0;
	int			mViewWidth;
	int			mViewHeight;
	float		mDownXPos;
	float		mLastMovePosX;
	int			mStartJacketIndex;
}




