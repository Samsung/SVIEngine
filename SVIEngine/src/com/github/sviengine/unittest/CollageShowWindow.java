package com.github.sviengine.unittest;

import java.util.Random;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Message;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class CollageShowWindow extends PlatformWindow implements SVIAnimationListener{
	
	public static final String TAG = "PageTurnEffectWindow";
	
	public CollageShowWindow(Context context){
		super(context);
			
		buildSlideTree();
		
		mHandler = new Handler() {
		    @Override
		    public void handleMessage(Message message) {
		        switch (message.what) {
		            case MSG_READY_TO_SHOW:
		                startCollageSlideShow(mIndex);
		                break;
		            case MSG_RESET_SHOW:
		            	resetCollageAnimation();
		            	break;
		            default: throw new AssertionError();
		        }
		    }
		};
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		
		if(event.getAction() == MotionEvent.ACTION_UP)
			startCollageBackgroundAnimation();
		
		return true;
	}
	
	
	private void buildCollageSlideShow(){
		if(mCollageBGSlide == null){
			mSize = mBaseSlide.getRegion().mSize;
			mCollageBGSlide = new SVISlide(0,0,
					0, 0,
					mSize.mWidth, mSize.mHeight,
					mDefaultColor);
			
			mBaseSlide.addSlide(mCollageBGSlide);
			mBaseSlide.setOrthogonal(false);
		}
		
		if(mCollageSubSlide == null){
			mCollageSubSlide = new SVISlide[6];
			for(int i = 0; i< mCollageSubSlide.length; i++){
				
				
				SVIRect subSmallRect = getScaleRect(mArtImages[i].mBitmap);
				
				mCollageSubSlide[i] = new SVISlide();
				
				mCollageSubSlide[i].setRegion(subSmallRect.mOrigin.mX, subSmallRect.mOrigin.mY, subSmallRect.mSize.mWidth / 4, subSmallRect.mSize.mHeight / 4);
				
				mCollageSubSlide[i].setImage(mArtImages[i]);
				mCollageBGSlide.addSlide(mCollageSubSlide[i]);
			}
		}
		
		setSubSlidePoint();
	}
		
	
	private SVIRect getScaleRect(Bitmap bitmap) {
		SVIRect scaleRect = null;
		
		int imageWidth = bitmap.getWidth();
		int imageHeight = bitmap.getHeight();
		
		int reSizeWidth;
		int reSizeHeight;
		
		float scaleValue = 1.0f ;
		
		scaleValue = mSize.mWidth / imageWidth;
		
		if (scaleValue > (mSize.mHeight / imageHeight)){
			scaleValue = mSize.mHeight / imageHeight;
		}
		
		reSizeWidth = (int)(imageWidth * scaleValue);
		reSizeHeight =  (int)(imageHeight * scaleValue);
		
		if(reSizeWidth%2 != 0){
			reSizeWidth = reSizeWidth-1;
			if(reSizeWidth%4 != 0){
				reSizeWidth = reSizeWidth -2;
			}	
		}
		
		if(reSizeHeight%2 != 0){
			reSizeHeight = reSizeHeight-1;
			if(reSizeHeight%4 != 0){
				reSizeHeight = reSizeHeight -2;
			}	
		}
		
		scaleRect = new SVIRect(0.0f, 0.0f, reSizeWidth, reSizeHeight);
		return scaleRect;
	}

	
	
	
	private void setSubSlidePoint() {
		
		mOriginPoint = new SVIPoint[6];
		mOriginPoint[0] = new SVIPoint(-100.0f, -100.0f);
		mOriginPoint[1] = new SVIPoint(mBaseSlide.getPosition().mX, -100.0f);
		mOriginPoint[2] = new SVIPoint(mBaseSlide.getPosition().mX*2 + 100.0f, -100.0f);
		mOriginPoint[3] = new SVIPoint(-100.0f, mBaseSlide.getPosition().mY*2+100.0f);
		mOriginPoint[4] = new SVIPoint(mBaseSlide.getPosition().mX, mBaseSlide.getPosition().mY*2+100.0f);
		mOriginPoint[5] = new SVIPoint(mBaseSlide.getPosition().mX*2 + 100.0f, mBaseSlide.getPosition().mY*2+100.0f);
		
		for(int i = 0; i< mCollageSubSlide.length; i++){
			mCollageSubSlide[i].setPosition(mOriginPoint[i].mX, mOriginPoint[i].mY);
		}
		
	}
	
	private void resetCollageAnimation(){
		
		int duration = mDuration / 3;
		
		for(int i = 0; i< mCollageSubSlide.length; i++){
			
			mCollageSubSlide[i].setPosition(mOriginPoint[i].mX, mOriginPoint[i].mY, duration);
			mCollageSubSlide[i].setRotation(0.0f, 0.0f, 0.0f, 0.0f, duration);
			
		}
	}

	private void startCollageBackgroundAnimation() {
		
		SVIPoint pt = mBaseSlide.getPosition();
		
		int duration = mDuration / 3;
		
		mCollageSubSlide[0].setPosition(pt.mX * 0.7f,  pt.mY * 0.7f, duration);
		mCollageSubSlide[1].setPosition(pt.mX * 1.0f,  pt.mY * 0.5f, duration);
		mCollageSubSlide[2].setPosition(pt.mX * 1.3f,  pt.mY * 0.7f, duration);
		mCollageSubSlide[3].setPosition(pt.mX * 0.7f,  pt.mY * 1.3f, duration);
		mCollageSubSlide[4].setPosition(pt.mX * 1.0f,  pt.mY * 1.5f, duration);
		mCollageSubSlide[5].setPosition(pt.mX * 1.3f,  pt.mY * 1.3f, duration);
		
		mRotationArray = new float[mCollageSubSlide.length];
		for(int i =0; i< mCollageSubSlide.length; i++){
			Random rotateRandom = new Random();
			mRotationArray[i] = rotateRandom.nextInt(12) * 30.0f;
			mCollageSubSlide[i].setRotation(0.0f, 0.0f, mRotationArray[i], 0.0f, duration);
		}
		
		mHandler.sendEmptyMessageDelayed(MSG_READY_TO_SHOW, duration * 2);
	}
	
	
	private void startCollageSlideShow(int index){
		
		int duration = mDuration;
		SVIPoint originPoint = mCollageSubSlide[index].getPosition();
		SVIPoint destPoint = mCollageBGSlide.getPosition();
		
		float originZposition = mCollageSubSlide[index].getZPosition();
		float destZposition = -0.1f;
		
		SVIVector3 originRotateValue = mCollageSubSlide[index].getRotation();
		SVIVector3 destRotateValue = new SVIVector3(0.0f, 0.0f, 0.0f);
		
		
		SVIRect  originRect = mCollageSubSlide[index].getRegion();
		SVIRect destRect = getScaleRect(mCollageSubSlide[index].getImage().mBitmap);
		
		
		SVIAnimationSet slideAniSet = new SVIAnimationSet();
		slideAniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.SINEEASEINOUT);
		slideAniSet.setDuration(duration);
		slideAniSet.setRepeatCount(0);
		slideAniSet.setListener(this);
		slideAniSet.setAutoReverse(false);
		
		SVIKeyFrameAnimation regionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.REGION);
		regionAni.setDuration(duration);
		regionAni.setRepeatCount(0);
		regionAni.setAutoReverse(false);
		regionAni.setOffset(0);
		regionAni.addKeyProperty(0.0f, originRect);
		regionAni.addKeyProperty(0.3f, destRect);
		regionAni.addKeyProperty(0.7f, destRect);
		regionAni.addKeyProperty(1.0f, originRect);
		
		slideAniSet.addAnimation(regionAni);
		
		
		SVIKeyFrameAnimation positionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.POSITION);
		positionAni.setDuration(duration);
		positionAni.setRepeatCount(0);
		positionAni.setAutoReverse(false);
		positionAni.setOffset(0);
		positionAni.addKeyProperty(0.0f, originPoint);
		positionAni.addKeyProperty(0.3f, destPoint);
		positionAni.addKeyProperty(0.7f, destPoint);
		positionAni.addKeyProperty(1.0f, originPoint);
		
		slideAniSet.addAnimation(positionAni);
		
		SVIKeyFrameAnimation rotateAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ROTATION);
		rotateAni.setDuration(duration);
		rotateAni.setRepeatCount(0);
		rotateAni.setAutoReverse(false);
		rotateAni.setOffset(0);
		rotateAni.addKeyProperty(0.0f, originRotateValue);
		rotateAni.addKeyProperty(0.3f, destRotateValue);
		rotateAni.addKeyProperty(0.7f, destRotateValue);
		rotateAni.addKeyProperty(1.0f, originRotateValue);
		
		slideAniSet.addAnimation(rotateAni);
		
		SVIKeyFrameAnimation zPositionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ZPOSITION);
		zPositionAni.setDuration(duration);
		zPositionAni.setRepeatCount(0);
		zPositionAni.setAutoReverse(false);
		zPositionAni.setOffset(0);
		zPositionAni.addKeyProperty(0.0f, originZposition);
		zPositionAni.addKeyProperty(0.3f, destZposition);
		zPositionAni.addKeyProperty(0.7f, destZposition);
		zPositionAni.addKeyProperty(1.0f, originZposition);
		
		slideAniSet.addAnimation(zPositionAni);
		
		
		mCollageSubSlide[index].startAnimation(slideAniSet);
		
		
	}
	
	
	
	
	
	

	protected void buildSlideTree(){
		super.buildSubSlide();
		
		float color[] = { 0.0f, 0.0f, 0.3f, 1.0f };
		
		mBaseSlide = getMainSlide();		
		mBaseSlide.setBackgroundColor(color);
		mBaseSlide.setRegion(0.0f, 0.0f, mWidth, mHeight);
		
		mSize= mBaseSlide.getRegion().mSize;						
		
		mBaseSlide.setOrthogonal(false);
		
		buildCollageSlideShow();
	}
	
	
	
	
	private SVISlide mCollageBGSlide = null;
	private SVISlide[] mCollageSubSlide = null;
	
	private SVIPoint[] mOriginPoint = null;
	
	private int mIndex = 0;

	private SVISlide mBaseSlide = null;
	private SVISize mSize = null;
	private float[] mRotationArray = null;
	
	private float mDefaultColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	
	private Handler mHandler = null;
	
	private int mDuration = 3000;
	
	private static final int MSG_READY_TO_SHOW = 1;
	private static final int MSG_RESET_SHOW = 2;
	
		
	@Override
	public void onAnimationEnd(String tag) {
		
		mIndex++;
		if(mIndex==mCollageSubSlide.length){
			mIndex = 0;
			mHandler.sendEmptyMessage(MSG_RESET_SHOW);
		}else{
			mHandler.sendEmptyMessage(MSG_READY_TO_SHOW);
		}
		
	}
	
	@Override
	public void onAnimationRepeat(String tag) {
		// TODO Auto-generated method stub		
	}

	@Override
	public void onAnimationStart(String tag) {
		// TODO Auto-generated method stub
						
		
	}
	
	
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}
	
	private static native void nativeTouchEvent(int type, float x, float y);
}
