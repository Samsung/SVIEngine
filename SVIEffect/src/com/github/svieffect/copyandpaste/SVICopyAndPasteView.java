/**
 * SGIEffectView.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.copyandpaste;

import java.util.Vector;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Point;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector2;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.SlideMorphingType;
import com.github.sviengine.slide.SVISlideManager;
import com.github.sviengine.slide.SVISlideManager.SVISlideRequestRender;

public class SVICopyAndPasteView extends SVIGLSurface implements SVISlideRequestRender, SVIGLSurface.RenderNotifier{
	
	private final String BUILD_VERSION = "RELEASE v1.0a Date:2013-12-14 14:40";
	
	private static final int MSG_SURFACE_RESIZE 	= 0x9001;
	private static final int MSG_INVISIBLE 			= 0x9002;
	private static final int MSG_VISIBLE 			= 0x9003;
    private static final int MSG_UPDATE_FINISHED	= 0x9004;
	
	
	protected	SVISize					mSize = new SVISize(720.0f,1280.0f);
	protected   SVISize					mPrevSize = new SVISize(720.0f,1280.0f);
	protected 	SVISlide 				mRootSlide = null;
	
	protected   SVIImage					mEffectImage = null;
	protected   SVISlide					mEffectSlide = null;
	protected   SVIRect					mEffectRegion = new SVIRect();
	protected	SVIColor					mEffectColor = new SVIColor(0.2f, 0.3f, 0.8f, 0.7f);
	protected   int						mEffectVertexCount = 32;
	protected	float					mEffectRadius = 50.0f;
	
	protected   Vector<SVIVector2>		mPoints = new Vector<SVIVector2>();
	protected   SVICopyAndPasteCallback	mCallback = null;
	
	protected	SVIAnimationListener		mAnimationListener = new SVIAnimationListener(){

		public void onAnimationEnd(String arg0) {
			// TODO Auto-generated method stub
			if (mCallback != null) mCallback.onAnimationEnd();
			
			mPoints.clear();
		}

		public void onAnimationRepeat(String arg0) {
			// TODO Auto-generated method stub
			
		}

		public void onAnimationStart(String arg0) {
			// TODO Auto-generated method stub
			
		}
		
	};
	
	
	public SVICopyAndPasteView(Context context) {
		super(context);
		
		initialize(true, 8, 8);
		SVISlideManager.getInstance().setSVISlideRequestRender(this);
		
		createSlide();
		setRenderNotifier(this);
		enableUpdateListener(true);
		setZOrderOnTop(true);
		
		Log.i("SGICopyAndPaste","BUILD VERSION:" + BUILD_VERSION);
	}
	
	protected boolean createSlide() {
		if( mRootSlide == null ){
			float color[] = {0.0f, 0.0f, 0.0f, 0.0f};
			mRootSlide = new SVISlide(0, 0, 0.0f, 0.0f, mSize.mWidth, mSize.mHeight, color);
			SVISlide.nativeSetRootSlideSVISlide(mRootSlide.getNativeSlideHandle());
		}		
		return true;
	}
	
	protected void createEffectSlide(){
		if(mEffectSlide != null)
		{
			mRootSlide.removeSlide(mEffectSlide);
			mEffectSlide = null;
		}
		
		float color[] = {1.0f, 1.0f, 1.0f, 1.0f};
		//mEffectSlide = new SVISlide(mRootSlide.getNativeSlideHandle(), 0, 0.0f, 0.0f, mSize.mWidth, mSize.mHeight, color);
		mEffectSlide = new SVISlide();
		mRootSlide.addSlide(mEffectSlide);
		mRootSlide.setRegion(0.0f, 0.0f, mSize.mWidth, mSize.mHeight);
		mRootSlide.setBackgroundColor(color);
		mEffectSlide.setPivotPoint(0.0f, 0.0f);
		mEffectSlide.setMorphingType(SlideMorphingType.MORPHING_TRICKLE);
		
		mEffectSlide.setOpacity(0.0f);
	}
	
	protected void updateRoot(){
		if (mRootSlide != null){
			mRootSlide.setRegion(0.0f, 0.0f, mSize.mWidth, mSize.mHeight);
		}
	}
			
	protected boolean reLayout(){
		return false;
	}
		
	public void addPoint(float x, float y){
		mPoints.add(new SVIVector2(x,y));
	}
	
	public void addPoint(SVIVector2 vector2) {
		mPoints.add(vector2);
	}
	
	public void setCallbackListener(SVICopyAndPasteCallback callback){
		mCallback = callback;
	}
	
	public void setIntPoints(Vector<Point> points){
		mPoints.clear();
		if (points != null) {
			for (Point p : points) {
				addPoint((float)p.x,(float)p.y);
			}
		}
	}
	
	public void setFloatPoints(Vector<SVIVector2> points){
		mPoints.clear();
		if (points != null) {
			for (SVIVector2 vec : points) {
				addPoint(vec);
			}
		}
	}
	
	public void setWindowRegion(float offsetX, float offsetY, float width, float height){		
		mEffectRegion.mOrigin.mX = offsetX;
		mEffectRegion.mOrigin.mY = offsetY;
		
		mEffectRegion.mSize.mWidth = width;
		mEffectRegion.mSize.mHeight = height;
	}
	
	public void setWindowImage(Bitmap bitmap){
		if (mEffectImage == null){
			mEffectImage = new SVIImage();
		}
		mEffectImage.setBitmap(bitmap);
	}
	
	public void setMorphingRadius(float radius){
		mEffectRadius = radius;
	}
	
	public void setMorphingGridCount(int gridCount) {
		mEffectVertexCount = gridCount * 2;
	}
	
	public void setMorphingVertexCount(int vertexCount) {
		mEffectVertexCount = vertexCount;
	}
	
	public void setMorphingColor(float[] morphingColor){
		mEffectColor.mR = morphingColor[0];
		mEffectColor.mG = morphingColor[1];
		mEffectColor.mB = morphingColor[2];
		mEffectColor.mA = morphingColor[3];
	}
	
	public void play(int duration){
		if(mPoints.size() < 3)
			return;
		
		createEffectSlide();
		
		mEffectSlide.setRegion(mEffectRegion.mOrigin.mX, mEffectRegion.mOrigin.mY, mEffectRegion.mSize.mWidth, mEffectRegion.mSize.mHeight);
		float[] color = {mEffectColor.mR, mEffectColor.mG, mEffectColor.mB, mEffectColor.mA}; 
		mEffectSlide.setMorphingColor(color);
		mEffectSlide.setMorphingVertexCount(mEffectVertexCount);
		mEffectSlide.setImage((SVIImage)null);
		mEffectSlide.setImage(mEffectImage);
		mEffectSlide.setMorphingRadius(mEffectRadius);
		
		
		mEffectSlide.setTesselationPointList(mPoints);
		
		mEffectSlide.setMorphingType(SlideMorphingType.MORPHING_TRICKLE);
		
		SVIAnimationSet aniSet = new SVIAnimationSet();
		aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
		aniSet.setListener(mAnimationListener);
		aniSet.setDuration(duration);
		aniSet.setRepeatCount(0);
		aniSet.setAutoReverse(false);
		aniSet.setOffset(0);
				
		SVIKeyFrameAnimation keyMorphingTimeAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.MORPHING_TIME);
		keyMorphingTimeAni.setDuration(duration);
		keyMorphingTimeAni.setRepeatCount(0);
		keyMorphingTimeAni.setAutoReverse(false);
		keyMorphingTimeAni.setOffset(0);
		keyMorphingTimeAni.addKeyProperty(0.0f, 0.0f);
		keyMorphingTimeAni.addKeyProperty(1.0f, 1.0f);
		aniSet.addAnimation(keyMorphingTimeAni);
		
		mEffectSlide.startAnimation(aniSet);
		
		enableUpdateListener(true);
	}
	
	
	@Override
	public void onUpdateFinished() {
		// TODO Auto-generated method stub
		if (mCallback != null) {
			mCallback.onFirstRenderFinished();
		}

		mViewHandler.sendEmptyMessage(MSG_UPDATE_FINISHED);
	}
	
	public void animationSlideNotify() {
		// TODO Auto-generated method stub
	}

	
	public boolean requestSlideRender() {
		// TODO Auto-generated method stub
		swapAnimationSVIGLSurface();
		requestRender();
		return false;
	}
			
	public void destroy(){
		destroySlide();
	}	
	
	protected boolean destroySlide() {
		if( mRootSlide != null ) {
			for(int i=mRootSlide.getSubSlideCount()-1; i>=0; i--) {
				SVISlide removeSlide = mRootSlide.getSubSlide(i);
				if( removeSlide != null )
					mRootSlide.removeSlide(removeSlide);
			}
			SVISlide.nativeSetRootSlideSVISlide(0);
		}
		return true;
	}
	
	@Override
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub
		super.onResize(width, height);
		Message msg = null; 
		msg = mViewHandler.obtainMessage();
		if ( msg != null) {
			msg.what = MSG_SURFACE_RESIZE;
			msg.arg1 = width;
			msg.arg2 = height;
			mViewHandler.sendMessage(msg);
		}
	}
		
	public void show() {
		mViewHandler.sendEmptyMessage(MSG_VISIBLE);
	}
	
	public void hide(){
		mViewHandler.sendEmptyMessage(MSG_INVISIBLE);
	}
	
	private Handler mViewHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch(msg.what) {
			case MSG_SURFACE_RESIZE:
				mPrevSize.mWidth = mSize.mWidth;
				mPrevSize.mHeight = mSize.mHeight;
				mSize.mWidth = msg.arg1;
				mSize.mHeight = msg.arg2;
				updateRoot();
				break;
				
			case MSG_INVISIBLE:
				setVisibility(View.INVISIBLE);
				break;
				
			case MSG_VISIBLE:
				setVisibility(View.VISIBLE);
				break;
				
			case MSG_UPDATE_FINISHED:
				if (mEffectSlide != null) {
					mEffectSlide.setOpacity(1.0f);
					enableUpdateListener(false);
				} else {
					enableUpdateListener(true);
				}
				break;
		}
		}
	};	
	
}
