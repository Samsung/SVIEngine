package com.github.sviengine.performance;

import java.io.InputStream;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.MotionEvent;
import android.widget.TextView;
import android.widget.Toast;

import com.github.sviengine.R;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIBasicAnimation;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.animation.SVISpriteAnimation;
import com.github.sviengine.animation.SVITransitionAnimation;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlideManager.SVISlideImplicitAnimationListener;

public class AnimationListenerTestWindow extends PlatformWindow implements SVIAnimationListener, SVISlideImplicitAnimationListener{

	public final int BASIC_ANIMATION = 0;
	public final int KEYFRAME_ANIMATION = 1;
	public final int TRANSITION_ANIMATION = 2;
	public final int SPRITE_ANIMATION = 3;
	public final int ANIMATION_SET_ANIMATION =4;
	public final int IMPLICIT_ANIMATION	=5;
	
	private final int	SLIDE_WIDTH = 480;
	private final int	SLIDE_HEIGHT = 480;
	
	final protected float ICON_WIDTH		= 320.0f;
	final protected float ICON_HEIGHT		= 240.0f;
	
	private SVISlide		mTestSlide;
	private int			mTestCase;
	private int			mRepeatCount;
	private SVIImage		mSpriteImage;
	private TextView	mCallbackStatusView;
	private int			mCurrentRepeatCount;
	
	private int			mTransitionTouchCounter = 0;
	
	private int			DURATION = 2000;
	
	public AnimationListenerTestWindow(Context context) {
		super(context);
		
		// TODO Auto-generated constructor stub
		if( getMainSlide() == null )
			buildSubSlide();
		
		mTestCase = BASIC_ANIMATION;
		mRepeatCount = 0;
		mCurrentRepeatCount = 0;
		
		Activity activity = (Activity)context;
		InputStream is;
		
		is = activity.getResources().openRawResource(R.drawable.explosion_sprite3);
		Bitmap spriteBitmap = BitmapFactory.decodeStream(is);
		
		mSpriteImage = new SVIImage();
		mSpriteImage.setBitmap(spriteBitmap);
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
		
		
		if( event.getAction() == MotionEvent.ACTION_DOWN ){
			if(getSlideManager().isAnimating() )
				mTestSlide.stopAnimation();
			else
				runTestCase();
		}
		
		requestRender();
		return true;
	}
	
	private void runTestCase() {
		SVIAnimation animation = null;
		mCurrentRepeatCount = 0;
		switch(mTestCase) {
		case BASIC_ANIMATION : {
			int type = SVIPropertyAnimation.PropertyAnimationType.ROTATION;
			SVIVector3 from = new SVIVector3(0.0f, 0.0f, 0.0f);
			SVIVector3 to = new SVIVector3(0.0f, 0.0f, 180.0f);
			animation = new SVIBasicAnimation(type, from, to);
			mTestSlide.setImage(mImages[0]);
		}
			break;
		case KEYFRAME_ANIMATION : {
			int type = SVIPropertyAnimation.PropertyAnimationType.ROTATION;
			SVIKeyFrameAnimation ani = new SVIKeyFrameAnimation(type);
			ani.addKeyProperty(0.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
			ani.addKeyProperty(0.3f, new SVIVector3(0.0f, 0.0f, 90.0f));
			ani.addKeyProperty(1.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
			animation = ani;
			mTestSlide.setImage(mImages[0]);
		}
			break;
		case TRANSITION_ANIMATION : {
			mTransitionTouchCounter++;
			
			if( (mTransitionTouchCounter % 2) == 0 )
				mTestSlide.setImage(mImages[0]);
			else 
				mTestSlide.setImage(mImages[1]);
			
			//int type = SVITransitionAnimation.TransitionType.ROTATION_CUBE;
			int type = SVITransitionAnimation.TransitionType.BREAK;
			SVITransitionAnimation ani = SVITransitionAnimation.createAnimation(type);
			ani.setDirectionType(3);
			animation = ani;
		}
			break;
		case SPRITE_ANIMATION : {
			SVISpriteAnimation.PlayType type = SVISpriteAnimation.PlayType.PLAY_ALL;
			SVISpriteAnimation ani = new SVISpriteAnimation(this, type, mSpriteImage, (int)ICON_WIDTH, (int)ICON_HEIGHT);
			animation = ani;
		}
			break;
		case ANIMATION_SET_ANIMATION : {
			SVIAnimationSet aniSet = new SVIAnimationSet();
			
			mTestSlide.setImage(mImages[0]);
			int type = SVIPropertyAnimation.PropertyAnimationType.ROTATION;
			SVIVector3 from1 = new SVIVector3(0.0f, 0.0f, 0.0f);
			SVIVector3 to1 = new SVIVector3(0.0f, 0.0f, 90.0f);
			SVIBasicAnimation ani1 = new SVIBasicAnimation(type, from1, to1);
			ani1.setDuration(300);
			ani1.setRepeatCount(mRepeatCount);
			
			type = SVIPropertyAnimation.PropertyAnimationType.BG_COLOR;
			SVIColor from2 = new SVIColor(1.0f, 1.0f, 1.0f, 1.0f);
			SVIColor to2 = new SVIColor(1.0f, 0.0f, 0.0f, 0.5f);
			SVIBasicAnimation ani2 = new SVIBasicAnimation(type, from2, to2);
			ani2.setDuration(300);
			ani2.setRepeatCount(mRepeatCount);
			
			
			aniSet.addAnimation(ani1);
			aniSet.addAnimation(ani2);
			
			animation = aniSet;
		}
			break;
		case IMPLICIT_ANIMATION : {
			getSlideManager().checkoutAnimation();
			getSlideManager().setImplicitListener(mTestSlide, this);
			getSlideManager().setInterpolatorType(SVIAnimation.InterpolatorType.BOUNCE);
			getSlideManager().setDuration(1000);
			mTestSlide.initRotation(0.0f, 0.0f, 0.0f, 0.0f);
			mTestSlide.setRotation(0.0f, 0.0f, 90.0f, 0.0f, DURATION);
			mTestSlide.initScale(1.0f, 1.0f, 1.0f, 0.0f);
			mTestSlide.setScale(2.0f, 2.0f, 1.0f, DURATION);
			getSlideManager().checkinAnimation();
		}
			break;
		}
		
		if( animation != null ) {
			animation.setRepeatCount(mRepeatCount);
			animation.setListener(this);
			animation.setDuration(DURATION);	
			mTestSlide.startAnimation(animation);
		}
	}
	
	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();
	
		if( mTestSlide != null ) {
			getMainSlide().removeSlide(mTestSlide);
			mTestSlide = null;
		}
		float x = (mWidth / 2.0f) - (SLIDE_WIDTH / 2.0f);
		float y = (mHeight / 2.0f) - (SLIDE_HEIGHT / 2.0f);
		float bgColor[] = { 0.651f, 0.792f, 0.941f, 1.0f };
		mTestSlide = new SVISlide(0, 0, x, y, SLIDE_WIDTH, SLIDE_HEIGHT, bgColor);
		getMainSlide().addSlide(mTestSlide);
		mTestSlide.setAntiAliasing(true);
		mTestSlide.setBorderWidth(20.0f);
		mTestSlide.setCornerRadius(200.0f);
		//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float borderColor[] = { 0.90f, 0.90f, 0.90f, 1.0f };
		mTestSlide.setBorderColor(borderColor);
		float color[] = { 0.651f, 0.792f, 0.941f, 1.0f };
		getMainSlide().setBackgroundColor(color);
		
		mTestSlide.setBorderWidth(20.0f);
		//mTestSlide.setCornerRadius(200.0f);
		//float borderColor[] = { 0.90f, 0.90f, 0.90f, 1.0f };
		mTestSlide.setBorderColor(borderColor);
		
		mTestSlide.setImage(mImages[0]);
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

	@Override
	public void onImplicitAnimationEnd(String tag) {
		// TODO Auto-generated method stub
		mCallbackStatusView.setText("Implicit AnimationEnd(Tag:" + tag +")");
	}

	@Override
	public void onImplicitAnimationRepeat(String tag) {
		// TODO Auto-generated method stub
		mCurrentRepeatCount++;
		mCallbackStatusView.setText("Implicit AnimationRepeat(Tag:" + tag +")"+" Repeat Count:"+mCurrentRepeatCount);
	}

	@Override
	public void onImplicitAnimationStart(String tag) {
		// TODO Auto-generated method stub
		mCallbackStatusView.setText("Implicit AnimationStart(Tag:" + tag +")");
	}

}
