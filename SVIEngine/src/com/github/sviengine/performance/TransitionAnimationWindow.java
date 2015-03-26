package com.github.sviengine.performance;

import com.github.sviengine.animation.SVITransitionAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.animation.SVITransitionAnimation.DirectionType;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlideManager;

import android.content.Context;
import android.view.MotionEvent;
import android.widget.TextView;
import android.widget.Toast;

public class TransitionAnimationWindow extends PlatformWindow implements SVIAnimationListener{
	
	public TransitionAnimationWindow(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		if( getMainSlide() == null )
			buildSubSlide();
		
	}
	
	void setTestCase(int testCase) {
		mTestCase = testCase;
		buildSubSlide();
	}
	
	void setCallbackStatusView(TextView view) {
		mCallbackStatusView = view;
	}
	
	void increaseRepeatCount() {
		mRepeatCount++;
		Toast.makeText(getContext(), "RepeatCount:"+mRepeatCount, Toast.LENGTH_SHORT).show();
	}
	
	void decreaseRepeatCount() {
		mRepeatCount--;
		if( mRepeatCount < 0 )
			mRepeatCount = 0;
		Toast.makeText(getContext(), "RepeatCount:"+mRepeatCount, Toast.LENGTH_SHORT).show();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		if(event.getAction() == MotionEvent.ACTION_DOWN) {
			mDownMousePoint.mX = event.getX();
			mDownMousePoint.mY = event.getY();
		}
		
		if(event.getAction() == MotionEvent.ACTION_UP) {
			if( mSlideManager.isAnimating() )
				return true;
			mUpMousePoint.mX = event.getX();
			mUpMousePoint.mY = event.getY();
			
			startAni();
			
		}
		return true;
	}
	
	public void startAni(){
		
		SVITransitionAnimation transAni = SVITransitionAnimation.createAnimation(mTestCase);
		
		if(!mFlag) {
			mNewSlide.setImage(mImages[mNextImage]);
			mFlag = true;
		}
		else {
			mNewSlide.setImage(mImages[mPreImage]);
			mFlag = false;
		}
		
		if( transAni != null ) {
			transAni.setRepeatCount(mRepeatCount);
			transAni.setListener(this);
			transAni.setDirectionType( getDirectionType() );
			transAni.setDuration(DURATION);	
			mNewSlide.startAnimation(transAni);
		}
	}
	
	
	public int getDirectionType() {
		int directionType;
		float gapX, gapY;
		
		gapX = mUpMousePoint.mX - mDownMousePoint.mX;
		gapY = mUpMousePoint.mY - mDownMousePoint.mY;
		if( Math.abs(gapX) > Math.abs(gapY) ) {
			if(gapX < 0) {
				directionType = DirectionType.LEFT;
			}
			else {
				directionType = DirectionType.RIGHT;
			}
				
		}
		else {
			if(gapY < 0) {
				directionType = DirectionType.UP;
			}
			else {
				directionType = DirectionType.DOWN;
			}
		}
		
		return directionType;
	}
	
	
	
	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();
	
		super.buildSubSlide();
		float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		
		mFlag = false;
		SVISlide mainSlide = getMainSlide();
		mainSlide.setBackgroundColor(color);
		
		mNewSlide = new SVISlide(0,0, 0, 0, mWidth, mHeight,	color);
		mNewSlide.setImage(mImages[mPreImage]);
		mainSlide.addSlide(mNewSlide);
		
		mainSlide.setOrthogonal(false);
		
		mDownMousePoint = new SVIPoint();
		mUpMousePoint = new SVIPoint();
		
//		startAni();
		
	}
	
	@Override
	public void onAnimationEnd(String tag) {
		// TODO Auto-generated method stub
		mCallbackStatusView.setText("AnimationEnd(Tag:" + tag +")");
	}

	@Override
	public void onAnimationRepeat(String tag) {
		// TODO Auto-generated method stub
		mCurrentRepeatCount++;
		mCallbackStatusView.setText("AnimationRepeat(Tag:" + tag +")"+" Repeat Count:"+mCurrentRepeatCount);
	}

	@Override
	public void onAnimationStart(String tag) {
		// TODO Auto-generated method stub
		mCallbackStatusView.setText("AnimationStart(Tag:" + tag +")");
	}

	
	protected void removeall(){
		SVISlide mainSlide = getMainSlide();	
		mainSlide.removeSlide(mNewSlide);
		
	}
	
	
	
	final protected float ICON_WIDTH		= 320.0f;
	final protected float ICON_HEIGHT		= 240.0f;
	
	
	private int			mTestCase;
	private int			mRepeatCount;
	private TextView	mCallbackStatusView;
	private int			mCurrentRepeatCount;
	
	private int			DURATION = 2000;
	
	boolean mFlag = false;
	
	static int mPreImage = 0;
	static int mNextImage = 1;
	protected SVISlide mNewSlide;
	protected SVIPoint mUpMousePoint;
	protected SVIPoint mDownMousePoint;

}
