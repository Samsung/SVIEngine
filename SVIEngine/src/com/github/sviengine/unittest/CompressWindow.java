package com.github.sviengine.unittest;


import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import com.github.sviengine.R;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.slide.SVISlide;

public class CompressWindow extends PlatformWindow implements SVIAnimationListener, SVIGLSurface.RenderNotifier {
	
	public interface ImageTestType {
		public static final int SMALL_TEST = 0;
		public static final int LARGE_TEST = 1;
		public static final int ALL_TEST = 2;
	}
	
	private final String TAG = "SVIEngine";
	
	private final boolean USE_COMPRESS_TEXTURE = false;

	private int mTestType = ImageTestType.ALL_TEST;
	
	private SVISlide mMainSlide = null;
	
	private boolean mCalledRender = false;
	private long mStartTime;
	
	private final float mSlideWidth = 400.0f;
	private final float mSlideHeight = 400.0f;
	
	private int mImages[];
	
	private int mSmallImages[] = {
			R.drawable.btn_0,
			R.drawable.btn_1,
			R.drawable.btn_3,
			R.drawable.btn_4,
			R.drawable.btn_5,
			R.drawable.btn_6,
			R.drawable.btn_7,
			R.drawable.btn_8,
			R.drawable.btn_9
	};
	
	private int mLargeImages[] = {
			R.drawable.art_0,
			R.drawable.art_1,
			R.drawable.art_2,
			R.drawable.art_3,
			R.drawable.art_4,
			R.drawable.art_5,
			R.drawable.art_6
	};
	
	private int mCompressImages[];
	
	private int mSmallCompressImages[] = {
			R.drawable.btn_0_compress_atc,
			R.drawable.btn_1_compress_atc,
			R.drawable.btn_3_compress_atc,
			R.drawable.btn_4_compress_atc,
			R.drawable.btn_5_compress_atc,
			R.drawable.btn_6_compress_atc,
			R.drawable.btn_7_compress_atc,
			R.drawable.btn_8_compress_atc,
			R.drawable.btn_9_compress_atc
	};
	
	private int mLargeCompressImages[] = {
			R.drawable.art_0_compress_atc,
			R.drawable.art_1_compress_atc,
			R.drawable.art_2_compress_atc,
			R.drawable.art_3_compress_atc,
			R.drawable.art_4_compress_atc,
			R.drawable.art_5_compress_atc,
			R.drawable.art_6_compress_atc
	};
	
	private Bitmap mBitmaps[];
	
	private byte mBytes[][];
	
	public CompressWindow(Context context){
		super(context);
		
		System.gc();
		
		//this.enableShowFPS(false);
		
		setRenderNotifier(this);
		enableUpdateListener(true);
		
		if(mTestType == ImageTestType.SMALL_TEST) {
			mImages = mSmallImages;
			mCompressImages = mSmallCompressImages;
		}
		else if(mTestType == ImageTestType.LARGE_TEST) {
			mImages = mLargeImages;
			mCompressImages = mLargeCompressImages;
		}
		else {
			int images[] = new int[mSmallImages.length + mLargeImages.length];
			int imagesCompress[] = new int[mSmallCompressImages.length + mLargeCompressImages.length];
			
			for(int i = 0; i < mSmallImages.length; ++i) {
				images[i] = mSmallImages[i];
				imagesCompress[i] = mSmallCompressImages[i];
			}
			for(int i = 0; i < mLargeImages.length; ++i) {
				images[mSmallImages.length + i] = mLargeImages[i];
				imagesCompress[mSmallCompressImages.length + i] = mLargeCompressImages[i];
			}
			
			mImages = images;
			mCompressImages = imagesCompress;
		}
		
		long curTime;
		
		mStartTime = System.currentTimeMillis();
		
		if(USE_COMPRESS_TEXTURE)
			buildSlideTree_compress();
		else
			buildSlideTree();
		
		curTime = System.currentTimeMillis();
		
		Log.i(TAG, "Image loading time(millis) = " + (curTime - mStartTime));
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}

	
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		return true;
	}
	
	
	protected void buildSlideTree(){
		super.buildSubSlide();
		
		mMainSlide = getMainSlide();
		
		InputStream is;		
		
		mBitmaps = new Bitmap[mImages.length];
		
		float color[] = {1.0f, 1.0f, 1.0f, 1.0f};
		
		for (int n = 0; n < mImages.length; ++n){
			is = getResources().openRawResource(mImages[n]);
			mBitmaps[n] = BitmapFactory.decodeStream(is);
			
			SVIImage newImage = new SVIImage();
			newImage.setBitmap(mBitmaps[n]);
			
			SVISlide newSlide = new SVISlide(0, 0, 0.0f, 0.0f, mSlideWidth, mSlideHeight, color);
			//this code only i'm very hate warning
			mMainSlide.addSlide(newSlide);
			newSlide.setImage(newImage);
		}
	}
	
	protected void buildSlideTree_compress(){
		super.buildSubSlide();
		
		mMainSlide = getMainSlide();
		
		InputStream is;
		
		mBytes = new byte[mCompressImages.length][];
		
		float color[] = {1.0f, 1.0f, 1.0f, 1.0f};
		
		int fileSize;
		for (int n = 0; n < mCompressImages.length; ++n){
			is = getResources().openRawResource(mCompressImages[n]);
			try {
				fileSize = is.available();
				mBytes[n] = new byte[fileSize];
				is.read(mBytes[n]);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			SVIImage newImage = new SVIImage();
			newImage.setBitmap_compress(SVIImage.CompressTextureType.COMPRESS_ATC, mBytes[n]);
			
			SVISlide newSlide = new SVISlide(0, 0, 0.0f, 0.0f, mSlideWidth, mSlideHeight, color);
			//this code only i'm very hate warning
			mMainSlide.addSlide(newSlide);
			newSlide.setImage(newImage);
		}
	}
	
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		requestRender();
		return super.onOptionsItemSelected(item);
	}

	
	@Override
	public void onAnimationEnd(String tag) {
		
	}
	
	@Override
	public void onAnimationRepeat(String tag) {
		// TODO Auto-generated method stub
	}

	@Override
	public void onAnimationStart(String tag) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onUpdateFinished() {
		if(mCalledRender == false) {
			long curTime = System.currentTimeMillis();
			
			Log.i(TAG, "First Render time(millis) = " + (curTime - mStartTime));
			
			mCalledRender = true;
		}
	}
	
}
