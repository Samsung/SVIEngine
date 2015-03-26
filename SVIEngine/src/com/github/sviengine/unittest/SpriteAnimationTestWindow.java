package com.github.sviengine.unittest;

import java.io.InputStream;
import java.util.Random;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import com.github.sviengine.R;
import com.github.sviengine.animation.SVISpriteAnimation;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.slide.SVISlide;

public class SpriteAnimationTestWindow extends PlatformWindow{

	public SpriteAnimationTestWindow(Context context) {
		super(context, false);
		Log.i("SVI", "SM_SET_IMAGE SpriteAnimationTestWindow(Context context)");
		// TODO Auto-generated constructor stub
		Activity activity = (Activity)context;
		InputStream is;
		
		is = activity.getResources().openRawResource(R.drawable.explosion_sprite3);
		mSpriteBitmap = BitmapFactory.decodeStream(is);
		
		mSpriteImage = new SVIImage();
		mSpriteImage.setBitmap(mSpriteBitmap);
		//mSpriteImage.setReportFinalize(true);
		
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
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if( event.getAction() == MotionEvent.ACTION_DOWN ) {
			runSpriteAnimation();
		}
		return true;
	}
	
	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				
		SVISlide mainSlide = getMainSlide();		
		
		mIconSlide = new SVISlide[LANDSCAPE_COLUMN_COUNT*LANDSCAPE_ROW_COUNT];
		
		int rowCount = PORTRAIT_ROW_COUNT;
		int columnCount = PORTRAIT_COLUMN_COUNT;
		int count = 0;

		if( mWidth > mHeight ) {
			columnCount = LANDSCAPE_COLUMN_COUNT;
			rowCount = LANDSCAPE_ROW_COUNT;
		}

		mCurrentRowCount = rowCount;
		mCurrentColumnCount = columnCount;

		float x, y;
		float width = mWidth / columnCount;
		float height = mHeight / rowCount;

		mLeftMargin = (width - ICON_WIDTH) / 2.0f;
		mRightMargin = mLeftMargin;
		mTopMargin = (height - ICON_HEIGHT) / 2.0f;
		mBottomMargin = mTopMargin;
		int  slideCount = rowCount * columnCount;
		
		for(int i=0; i<slideCount; i++) {
			x = (count%columnCount) * width;
			y = (count/columnCount) * height;
		
			mIconSlide[i] = new SVISlide(0, 0, 
					(x+mLeftMargin), (y+mTopMargin), ICON_WIDTH, ICON_HEIGHT, color);
			
			mainSlide.addSlide(mIconSlide[i]);
			mIconSlide[i].setImage(mSpriteImage);
			//mIconSlide[i].setTextureRegion(0.0f, 0.0f, 100.0f, 100.0f, 0);
			count++;
		}
	}
	
	private void runSpriteAnimation() {
		int frameWidth = mSpriteBitmap.getWidth() / 4;
		int frameHeight = mSpriteBitmap.getHeight() / 5;
		
		SVISpriteAnimation ani = new SVISpriteAnimation(this, SVISpriteAnimation.PlayType.PLAY_PARTIAL, 
				mSpriteImage, frameWidth, frameHeight);
		ani.setDuration(400);
		ani.setInterval(5, 9);
		
		for(int i=0; i<mIconSlide.length; i++) {
			mIconSlide[i].startAnimation(ani);
		}
	}
	
	private SVIImage mSpriteImage;
	private Bitmap	mSpriteBitmap;
	
	//private SVISlide mTestSlide;
	
	final protected int PORTRAIT_COLUMN_COUNT = 1;
	final protected int PORTRAIT_ROW_COUNT = 1;

	final protected float ICON_WIDTH		= 320.0f;
	final protected float ICON_HEIGHT		= 240.0f;
	
	final protected int LANDSCAPE_COLUMN_COUNT	= PORTRAIT_ROW_COUNT;
	final protected int LANDSCAPE_ROW_COUNT		= PORTRAIT_COLUMN_COUNT;
	
	protected SVISlide[]		mIconSlide;
	
	Random mRandom = new Random();
	int mCurrentRowCount = 0;
	int mCurrentColumnCount = 0;
	float mLeftMargin = 0.0f;
	float mRightMargin = 0.0f;
	float mTopMargin = 0.0f;
	float mBottomMargin = 0.0f;
}
