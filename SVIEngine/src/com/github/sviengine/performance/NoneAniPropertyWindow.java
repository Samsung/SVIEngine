package com.github.sviengine.performance;


import java.util.Random;

import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;

import android.content.Context;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.TextView;

public class NoneAniPropertyWindow extends PlatformWindow implements SVIAnimationListener{

	public NoneAniPropertyWindow(Context context) {
		super(context);

		if( getMainSlide() == null )
			buildSubSlide();




	}



	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();

		float color[] = { 0.651f, 0.792f, 0.941f, 1.0f };

		SVISlide mainSlide = getMainSlide();		

		mainSlide.setBackgroundColor(color);

//		parentSlide = new SVISlide(0,0,100.0f, 100.0f, 400.0f, 400.0f, defaultDarkColor);
		float parentSlideWidth = 400.0f;
		float parentSlideHeight = 400.0f;
		parentSlide = new SVISlide(0,0, (mWidth - parentSlideWidth) / 2, (mHeight - parentSlideHeight) / 2, parentSlideWidth, parentSlideHeight, defaultDarkColor);



		mIconSlide = new SVISlide[LANDSCAPE_COLUMN_COUNT*LANDSCAPE_ROW_COUNT];


		int imageCnt = mImages.length-1;
		int rowCount = PORTRAIT_ROW_COUNT;
		int columnCount = PORTRAIT_COLUMN_COUNT;
		int count = 0;

		if( mWidth > mHeight ) {
			columnCount = LANDSCAPE_COLUMN_COUNT;
			rowCount = LANDSCAPE_ROW_COUNT;
		}

		mCurrentRowCount = rowCount;
		mCurrentColumnCount = columnCount;

		mainSlide.addSlide(parentSlide);

		int  slideCount = rowCount * columnCount;

		StringBuffer log = new StringBuffer();

		for(int i=0; i<slideCount; i++) {

			int imgIdx = count % (imageCnt);
			mIconSlide[i] = new SVISlide(0, 0, 50.0f + i*50.0f, 50.0f + i*50.0f, ICON_WIDTH, ICON_HEIGHT, color);
			mIconSlide[i].setRegion(50.0f + i*50.0f, 50.0f + i*50.0f, ICON_WIDTH, ICON_HEIGHT);


			//			if( i == 0 )
			parentSlide.addSlide(mIconSlide[i]);
			//			else 
			//				mIconSlide[i-1].addSlide(mIconSlide[i]);


			mIconSlide[i].setImage(mImages[imgIdx]);


			//			if(isBLEND_TYPE){
			//				mIconSlide[i].setBlendType(BlendType.BLEND_MULTIPLY);

			//			}

			mIconSlide[i].setShadowRadius(20.0f);
			mIconSlide[i].setShadowOffset(0.0f, 10.0f);
			mIconSlide[i].setShadowOpacity(0.5f);
			mIconSlide[i].setShadowRadius(40.0f);

			if(!isORTHOGONAL){
				mIconSlide[i].setOrthogonal(false);
				mIconSlide[i].setZPosition(100.0f*(i+1));
			}

			if(mTextViewLog != null)
				mTextViewLog.setText("");


			if(mCheckedList[CLIP_SUB_SLIDE-1]) {
				if(i == mIconSlide.length -1) {						
					mClipSubSlides = !mClipSubSlides;
					parentSlide.setClipSubSlides(mClipSubSlides);

					log.append("mClipSubSlides is " + String.valueOf(mClipSubSlides) + "\r\n");
				}
			}

			if(mCheckedList[ZORDER_TYPE-1]) {
				if(i == 1) {
					mIconSlide[i].setZOrderType((mIconSlide[i].getZOrderType() + ++mZOrderType) % (SVISlide.SlideZOrderType.TOPMOST + 1));

					String argument = null;
					switch(mIconSlide[i].getZOrderType()) {
					case SVISlide.SlideZOrderType.BOTTOM:
						argument = "BOTTOM";
						break;
					case SVISlide.SlideZOrderType.NORMAL:
						argument = "NORMAL";
						break;
					case SVISlide.SlideZOrderType.TOPMOST:
						argument = "TOPMOST";
						break;
					}
					log.append("mZOrderType is " + argument + "\r\n");
				}
			}

			if(mCheckedList[ORTHOGONAL-1]) {
				if(i == 0) {
					mOrthogonal = !mOrthogonal;
					log.append("mOrthogonal is " + String.valueOf(mOrthogonal) + "\r\n");
				}
				mIconSlide[i].setOrthogonal(mOrthogonal);
			}

			if(mCheckedList[HOLD_OPACITY-1]) {
				if(i == 0) {
					mHoldOpacity = !mHoldOpacity;
					log.append("mHoldOpacity is " + String.valueOf(mHoldOpacity) + "\r\n");
				}

				mIconSlide[i].setHoldOpacity(mHoldOpacity);
				//
				//					if(i == mIconSlide.length -1) {
				//						parentSlide.setOpacity(0.5f);
				//					}
			}

			if(mCheckedList[HOLD_SCALE-1]) {
				if(i == 0) {
					mHoldScale = !mHoldScale;
					log.append("mHoldScale is " + String.valueOf(mHoldScale) + "\r\n");
				}

				mIconSlide[i].setHoldScale(mHoldScale);
				//
				//					if(i == mIconSlide.length -1) {					
				//						SVIVector3 originalScale = parentSlide.getScale();
				//						parentSlide.setScale(
				//								(originalScale.mX * 2) % mWidth,
				//								(originalScale.mY * 2) % mHeight,
				//								originalScale.mZ, 0.0f);
				//					}
			}

			if(mCheckedList[BLEND_TYPE-1]) {
				if(i == 2) {
					mIconSlide[i].setZOrderType(SVISlide.SlideZOrderType.TOPMOST);
					
					SVIVector3 originalScale = mIconSlide[i].getScale();
					mIconSlide[i].setScale(
							(originalScale.mX * 2.0f) % mWidth,
							(originalScale.mY * 2.0f) % mHeight,
							originalScale.mZ);
					
					mBlendType = (mBlendType + 1) % (SVISlide.BlendType.BLEND_ADD + 1);
					String argument = null;
					switch(mBlendType) {
					case SVISlide.BlendType.BLEND_ADD:
						argument = "BLEND_ADD";
						break;
					case SVISlide.BlendType.BLEND_MULTIPLY:
						argument = "BLEND_MULTIPLY";
						break;
					}
					log.append("mBlendType is " + argument + "\r\n");
				}
				mIconSlide[i].setBlendType(mBlendType);
			}

			if(mCheckedList[LIGHT_TYPE-1]) {
				if(i == 0) {
					mLightType = (mLightType + 1) % (SVISlide.LightType.LINEAR_LIGHT + 1);

					String argument = null;
					switch(mLightType) {
					case SVISlide.LightType.LINEAR_LIGHT:
						argument = "LINEAR_LIGHT";
						break;
					case SVISlide.LightType.NO_LIGHT:
						argument = "NO_LIGHT";
						break;
					case SVISlide.LightType.SPOT_LIGHT:
						argument = "SPOT_LIGHT";
						break;
					}

					log.append("mLightType is " + argument + "\r\n");
				}
				mIconSlide[i].setLightType(mLightType);
			}

			if(mCheckedList[IMAGESCALETYPE-1]) {				
				if(i == 0) {
					mImageScaleType = (mImageScaleType + 1) % (SVISlide.ImageScaleType.CENTER_INSIDE + 1);

					String argument = null;
					switch(mImageScaleType) {
					case SVISlide.ImageScaleType.CENTER:
						argument = "CENTER";
						break;
					case SVISlide.ImageScaleType.CENTER_CROP:
						argument = "CENTER_CROP";
						break;
					case SVISlide.ImageScaleType.CENTER_INSIDE:
						argument = "CENTER_INSIDE";
						break;
					case SVISlide.ImageScaleType.FIT_CENTER:
						argument = "FIT_CENTER";
						break;
					case SVISlide.ImageScaleType.FIT_END:
						argument = "FIT_END";
						break;
					case SVISlide.ImageScaleType.FIT_START:
						argument = "FIT_START";
						break;
					case SVISlide.ImageScaleType.FIT_XY:
						argument = "FIT_XY";
						break;
					case SVISlide.ImageScaleType.MATRIX:
						argument = "MATRIX";
						break;
					}
					log.append("ImageScaleType is " + argument + "\r\n");
				}
				float backgroundColor[]= {0.0f, 1.0f, 0.0f, 1.0f};
				mIconSlide[i].setBackgroundColor(backgroundColor);

				//mIconSlide[i].setScaleToFitRegion(rect.mOrigin.mX, rect.mOrigin.mY, rect.mSize.mWidth, rect.mSize.mHeight);
				mIconSlide[i].setImageScaleType(mImageScaleType);						
				mIconSlide[i].setImage(mImages[imgIdx]);
			}

			count++;
		}
		if(mTextViewLog != null)
			mTextViewLog.setText(log);
	}

	protected void removeall(){
		SVISlide mainSlide = getMainSlide();		
		mainSlide.removeSlide(parentSlide);
		if (mIconSlide != null){
			int  slideCount = mCurrentRowCount * mCurrentColumnCount;			
			for(int i=0; i<slideCount; i++) {

				parentSlide.removeSlide(mIconSlide[i]);
				mIconSlide[i] = null;
			}
		}
		mIconSlide = null;
	}


	protected void touchDraggingAnimation(float x, float y) {


		parentSlide.setPosition(x, y);
		requestRender();


	}


	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub	

		//		if( event.getAction() == MotionEvent.ACTION_DOWN || 
		//			event.getAction() == MotionEvent.ACTION_MOVE )
		//			touchDraggingAnimation(event.getX(), event.getY());
		//		
		//		return true;
		if( event.getAction() == MotionEvent.ACTION_DOWN ){
			mCheckedList[mProperty] = true;			
			//			mRebuildSubSlide = true;

			setNonAnimatableProperties();
			//			removeall();
			//			buildSubSlide();

			Log.i("test", mProperty+"");
		}

		return true;
	}



	public void setNonAnimatableProperties() {
		// TODO Auto-generated method stub
		int duration = 1000;
		
		removeall();
		buildSubSlide();
		
		if(mCheckedList[CLIP_SUB_SLIDE-1]) {
			SVIPoint parentSlidePosition = parentSlide.getPosition();
			mIconSlide[2].setPosition(parentSlidePosition.mX, parentSlidePosition.mY, duration);
		}
			
		if(mCheckedList[HOLD_OPACITY-1])
			parentSlide.setOpacity(0.5f, duration);

		if(mCheckedList[HOLD_SCALE-1]) {
			SVIVector3 originalScale = parentSlide.getScale();
			parentSlide.setScale(
					(originalScale.mX * 1.5f) % mWidth,
					(originalScale.mY * 1.5f) % mHeight,
					originalScale.mZ, duration);
		}
		
		if(mCheckedList[BLEND_TYPE-1])
			parentSlide.setBackgroundColor(new float[]{0.0f, 1.0f, 0.0f, 1.0f}, duration);
		
		if(mCheckedList[LIGHT_TYPE-1])
			for(SVISlide slide : mIconSlide)
				slide.setLightColor(new float[]{0.0f, 1.0f, 0.0f, 1.0f}, duration);

		requestRender();

		mCheckedList = new boolean[IMAGESCALETYPE - CLIP_SUB_SLIDE + 1];	
	}



	@Override
	public void onAnimationEnd(String tag) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onAnimationRepeat(String tag) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onAnimationStart(String tag) {
		// TODO Auto-generated method stub

	}

	public boolean[] getCheckedList() {
		return mCheckedList;
	}

	public void setCheckedList(boolean[] checkedList) {
		mCheckedList = checkedList;
	}

	public TextView getTextViewLog() {
		return mTextViewLog;
	}



	public void setTextViewLog(TextView mTextViewLog) {
		this.mTextViewLog = mTextViewLog;
	}

	public static final int 				CLIP_SUB_SLIDE=1;									
	public static final int 				ZORDER_TYPE=2; 
	public static final int 				ORTHOGONAL=3; 			
	public static final int 				HOLD_OPACITY=4; 				
	public static final int 				HOLD_SCALE=5;			
	public static final int 				BLEND_TYPE=6; 				
	public static final int 				LIGHT_TYPE=7; 				
	public static final int 				IMAGESCALETYPE=8;


	protected boolean 				isCLIP_SUB_SLIDE = false;									
	protected boolean  				isZORDER_TYPE = false;
	protected boolean  				isORTHOGONAL = false; 			
	protected boolean  				isHOLD_OPACITY = false; 				
	protected boolean  				isHOLD_SCALE = false;			
	protected boolean  				isBLEND_TYPE = false; 				
	protected boolean  				isLIGHT_TYPE = false; 				
	protected boolean  				isIMAGESCALETYPE = false;

	protected boolean  				mRebuildSubSlide = false;

	protected boolean  				mClipSubSlides = false;
	protected int  					mZOrderType = 0;
	protected boolean  				mOrthogonal	= false;
	protected boolean  				mHoldOpacity = false;
	protected boolean  				mHoldScale = false;
	protected int  					mBlendType = 0;
	protected int  					mLightType = 0;
	protected int  					mImageScaleType = 0;

	protected SVISlide[]				mIconSlide = null;
	protected SVISlide				parentSlide = null;

	protected int					PORTRAIT_COLUMN_COUNT = 1;
	protected int 					PORTRAIT_ROW_COUNT = 3;

	protected float 				ICON_WIDTH		= 180.0f;
	protected float 				ICON_HEIGHT		= 180.0f;

	protected float 				defalutLocation = 100.0f;

	protected int 					LANDSCAPE_COLUMN_COUNT	= PORTRAIT_ROW_COUNT;
	protected int 					LANDSCAPE_ROW_COUNT		= PORTRAIT_COLUMN_COUNT;

	protected float[]				defaultColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	protected float[]				defaultDarkColor = { 0.5f, 0.5f, 0.5f, 1.0f };

	protected int 					exDuration = 5000;
	protected int					imDuration = 1000;

	final protected int  			offset = 0;
	final protected int 			repeatCount = 0;

	protected boolean 				isImplicit = true;

	protected int 					mProperty=0;





	Random 							mRandom = new Random();
	int								mCurrentRowCount = 0;
	int								mCurrentColumnCount = 0;


	protected boolean[]				mCheckedList = new boolean[IMAGESCALETYPE - CLIP_SUB_SLIDE + 1];
	protected TextView				mTextViewLog;				





}
