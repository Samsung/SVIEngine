package com.github.sviengine.unittest;

import java.util.Vector;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIVector2;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.SlideMorphingType;

import android.content.Context;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class TesselationWindow extends PlatformWindow implements SVIAnimationListener{
	
	public static final String TAG = "TesselationWindow";

	private final float MIN_DISTANCE = 10.0f;
	private final float POINT_SIZE = 4.0f;
	private SVISlide mMainSlide = null;
	private SVISlide mBackgroundSlide = null;
	private SVISlide mTesselationSlide = null;
	private Vector<SVIVector2> mPointList = new Vector<SVIVector2>();
	private Vector<SVISlide> mPointSlideList = new Vector<SVISlide>();
	private boolean mDrag = false;
	private boolean mCopy = false;
	
	private boolean mIsAnimating = false;
	
	
	//variables for customize demo
	private static final int		MORPHING_ANIMATION_DURATION = 700;
	private static final int		MORPHING_VERTEX_COUNT = 32;
	private static final String		COPY_ANIMATION_TAG = "CopyAnimation";
	private static final float 		MORPHING_COLOR[] = {0.2f, 0.3f, 0.8f, 0.7f};
	
	public TesselationWindow(Context context){
		super(context);
		
		
		buildSlideTree();
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}

	
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		if(mIsAnimating == true)
			return true;
		
		float x = event.getRawX();
		float y = event.getRawY();
		
		if(mCopy == false)
		{
			if(event.getAction() == MotionEvent.ACTION_DOWN)
			{
				mDrag = true;
				
				mPointList.clear();
				if(mPointSlideList.size() > 0)
				{
					for(int i = 0; i < mPointSlideList.size(); ++i)
					{
						mMainSlide.removeSlide(mPointSlideList.get(i));
					}
					mPointSlideList.clear();
				}
			}
			else if(mDrag == true && event.getAction() == MotionEvent.ACTION_MOVE)
			{
				if(mPointSlideList.size() > 0)
				{
					boolean added = false;
					SVISlide startSlide = mPointSlideList.get(0);
					SVISlide lastSlide = mPointSlideList.get(mPointSlideList.size() - 1);
					if( x > lastSlide.getPosition().mX + MIN_DISTANCE ||
						x < lastSlide.getPosition().mX - MIN_DISTANCE ||
						y > lastSlide.getPosition().mY + MIN_DISTANCE ||
						y < lastSlide.getPosition().mY - MIN_DISTANCE )
					{
						float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
						SVISlide newSlide = new SVISlide();
						newSlide.setRegion(x, y, POINT_SIZE, POINT_SIZE);
						newSlide.setBackgroundColor(color);
						mMainSlide.addSlide(newSlide);
						
						mPointSlideList.add(newSlide);
						
						SVIVector2 point = new SVIVector2(x, y);
						mPointList.add(point);
						
						added = true;
						
					}
					// check end drag
					if( mPointSlideList.size() > 3 &&
						x < startSlide.getPosition().mX + MIN_DISTANCE &&
						x > startSlide.getPosition().mX - MIN_DISTANCE && 
						y < startSlide.getPosition().mY + MIN_DISTANCE && 
						y > startSlide.getPosition().mY - MIN_DISTANCE )
					{
						if(added == false)
						{
							float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
							SVISlide newSlide = new SVISlide();
							newSlide.setRegion(x, y, POINT_SIZE, POINT_SIZE);
							newSlide.setBackgroundColor(color);
							mMainSlide.addSlide(newSlide);
							
							mPointSlideList.add(newSlide);
							
							SVIVector2 point = new SVIVector2(x, y);
							mPointList.add(point);
						}
						
						for(int i = 0; i < mPointSlideList.size(); ++i)
						{
							mMainSlide.removeSlide(mPointSlideList.get(i));
						}
						
						mPointSlideList.clear();
						
						SVISlide copySlide = new SVISlide();
						mMainSlide.addSlide(copySlide);
						mTesselationSlide = copySlide;
						copySlide.setPivotPoint(0.0f, 0.0f);
						copySlide.setRegion(0.0f, 0.0f, mMainSlide.getWidth(), mMainSlide.getHeight());
						
						copySlide.setBackgroundColor(new SVIColor(1.0f, 1.0f, 1.0f, 1.0f));
						
						//should be called prior to morphing animation.
						copySlide.setMorphingRadius(50.0f);
						//should be called prior to morphing animation.
						copySlide.setMorphingType(SlideMorphingType.MORPHING_TRICKLE);
						//set detail of morphing geometry.
						//copySlide.setMorphingVertexCount(MORPHING_VERTEX_COUNT);
						//set color of morphing geometry.
						copySlide.setMorphingColor(MORPHING_COLOR);
						
						mBackgroundSlide.setOpacity(0.7f);
						copySlide.setImage(mImages[mImages.length - 1]);
						copySlide.setTesselationPointList(mPointList);
						
						runAnimation(copySlide);
						
						mIsAnimating = true;
						mDrag = false;
						mCopy = false;
						
						//TODO : why not?
						/*for(int i = 0; i < mPointSlideList.size(); ++i)
						{
							mMainSlide.removeSlide(mPointSlideList.get(i));
						}
						
						mPointList.clear();
						mPointSlideList.clear();*/
					}
				}
				else
				{
					float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
					SVISlide newSlide = new SVISlide();
					newSlide.setRegion(x, y, POINT_SIZE, POINT_SIZE);
					newSlide.setBackgroundColor(color);
					mMainSlide.addSlide(newSlide);
					
					mPointSlideList.add(newSlide);
				}
				
			}
			else if(mDrag == true && event.getAction() == MotionEvent.ACTION_UP && mPointSlideList.size() > 3)
			{
				for(int i = 0; i < mPointSlideList.size(); ++i)
				{
					mMainSlide.removeSlide(mPointSlideList.get(i));
				}
				
				mPointSlideList.clear();
				
				SVISlide copySlide = new SVISlide();
				mMainSlide.addSlide(copySlide);
				mTesselationSlide = copySlide;
				copySlide.setPivotPoint(0.0f, 0.0f);
				copySlide.setRegion(0.0f, 0.0f, mMainSlide.getWidth(), mMainSlide.getHeight());
				
				copySlide.setBackgroundColor(new SVIColor(1.0f, 1.0f, 1.0f, 1.0f));
				
				//should be called prior to morphing animation.
				copySlide.setMorphingRadius(50.0f);
				//should be called prior to morphing animation.
				copySlide.setMorphingType(SlideMorphingType.MORPHING_TRICKLE);
				//set detail of morphing geometry.
				copySlide.setMorphingVertexCount(MORPHING_VERTEX_COUNT);
				//set color of morphing geometry.
				copySlide.setMorphingColor(MORPHING_COLOR);
				
				mBackgroundSlide.setOpacity(0.7f);
				copySlide.setImage(mImages[mImages.length - 1]);
				copySlide.setTesselationPointList(mPointList);
				
				runAnimation(copySlide);
				
				mIsAnimating = true;
				mDrag = false;
				mCopy = false;
			}
		}
		/*else
		{
			if(event.getAction() == MotionEvent.ACTION_DOWN)
			{
				mCopy = false;
				
				if(mTesselationSlide != null)
				{
					mTesselationSlide.setPosition(200.0f, 200.0f);
				}
			}

		}*/
		
		return true;
	}
		
	
	
	public void runAnimation(SVISlide slide){
		int duration = MORPHING_ANIMATION_DURATION;
		
		SVIAnimationSet aniSet = new SVIAnimationSet();
		aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
		aniSet.setListener(this);
		aniSet.setTag(COPY_ANIMATION_TAG);
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
		
		slide.startAnimation(aniSet);
	}
	
	
	protected void buildSlideTree(){
		super.buildSubSlide();
		
		mMainSlide = getMainSlide();
		mBackgroundSlide = new SVISlide();
		mMainSlide.addSlide(mBackgroundSlide);
		mBackgroundSlide.setRegion(0.0f, 0.0f, mMainSlide.getWidth(), mMainSlide.getHeight());
		mBackgroundSlide.setImage(mImages[mImages.length - 1]);
	}
	
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		/*if (item.getTitle() == "Change OnColor"){
			mChangeOnColor = !mChangeOnColor;
		} else if (item.getTitle() == "Change Light"){
			mLightReversed = !mLightReversed;
		} else if (item.getTitle() == "Change Cut"){
			if (mDepthCut == 0.6f) mDepthCut = 1.0f; else
				if (mDepthCut == 1.0f) mDepthCut = 0.6f; 
		} else if (item.getTitle() == "Change Form"){
			mChangeRotation = !mChangeRotation;
		}*/
		requestRender();
		return super.onOptionsItemSelected(item);
	}

	
	@Override
	public void onAnimationEnd(String tag) {
		if(tag == COPY_ANIMATION_TAG)
		{
			mBackgroundSlide.setOpacity(1.0f);
			mIsAnimating = false;
			if(mTesselationSlide != null)
			{
				mMainSlide.removeSlide(mTesselationSlide);
				mTesselationSlide = null;
			}
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
	
	
}
