package com.github.sviengine.unittest;

import java.io.InputStream;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.MenuItem;

import com.github.sviengine.R;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlideManager.SVISlideRequestRender;

public class PlatformWindow extends SVIGLSurface implements SVISlideRequestRender{
	
	//private static String TAG = "SVI";
    //private static final boolean DEBUG = false;
    
    protected static final int	ROTATION_0	= 0;
    protected static final int ROTATION_90 = 1;
    protected static final int ROTATION_180 = 2;
    protected static final int ROTATION_270 = 3;

	public PlatformWindow(Context context) {
		super(context);
		init(context, true);
		
		enableLogFPS(false);
	}
	
	public PlatformWindow(Context context, boolean loadResource) {
		super(context);
		init(context, loadResource);
	}
	
	public void onConfigurationChanged(int rotation) {
		mPrevRotation = mRotation;
		mRotation = rotation;
	}
	
	protected void buildSubSlide() {
		if( mRootSlide == null ) {
			float color[] = {0.0f, 0.0f, 0.0f, 1.0f};
			mRootSlide = new SVISlide(0, 0, 0.0f, 0.0f, 1, 1, color);
			SVISlide.nativeSetRootSlideSVISlide(mRootSlide.getNativeSlideHandle());
			//mMainSlide = new SVISlide(0, mRootSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);
			mMainSlide = new SVISlide(0, mRootSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);
		}
	}
	
	protected void buildSubSlide(float [] color) {
		if( mRootSlide == null && color != null ) {
			mRootSlide = new SVISlide(0, 0, 0.0f, 0.0f, 1, 1, color);
			SVISlide.nativeSetRootSlideSVISlide(mRootSlide.getNativeSlideHandle());
			mMainSlide = new SVISlide(0, mRootSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);
		}
	}
	
	public boolean onOptionsItemSelected(MenuItem item) { 	
		return false;
	}
	
	private void init(Context context, boolean loadResource) {
		Activity activity = (Activity)context;
		InputStream is;
		if( loadResource ) {
			int testImage[] = { 
								R.drawable.mainmenu_icon_backupmanager,
								R.drawable.mainmenu_icon_browser,
								R.drawable.mainmenu_icon_calculator,
								R.drawable.mainmenu_icon_email,
								R.drawable.mainmenu_icon_facebook,
								R.drawable.mainmenu_icon_twitter,
								R.drawable.mainmenu_icon_voicecommand,
								R.drawable.mainmenu_icon_youtube,
								R.drawable.mainmenu_icon_image_editor,
								R.drawable.mainmenu_icon_game,
								R.drawable.mainmenu_icon_memo,
								R.drawable.mainmenu_icon_radio,
								R.drawable.mainmenu_icon_google,
								R.drawable.mainmenu_icon_googlemap,
								R.drawable.mainmenu_icon_find_music, 
								R.drawable.mainmenu_icon_calendar,
								R.drawable.mainmenu_icon_clock,
								R.drawable.mainmenu_icon_contacts,
								R.drawable.mainmenu_icon_dictionary,
								R.drawable.mainmenu_icon_rss_reader,
								R.drawable.mainmenu_icon_search,
								R.drawable.mainmenu_icon_file_viewer,
								R.drawable.mainmenu_icon_photocontact,
																	
								R.drawable.dialer_back,
								R.drawable.dialer_01,
								R.drawable.dialer_02,
								R.drawable.dialer_03,
								R.drawable.dialer_04,
								R.drawable.dialer_05,
								R.drawable.dialer_06,
								R.drawable.dialer_07,
								R.drawable.dialer_08,
								R.drawable.dialer_09,
								R.drawable.dialer_10,
								R.drawable.dialer_11,
								R.drawable.dialer_12
								};
			
			int artImage[] = {R.drawable.art_0,
			};
			
			int coverImage[] = {
					R.drawable.s4lock_home,
					R.drawable.s4lock_base,
					R.drawable.s4lock_paper,
					R.drawable.rome,
			};
			
			
			mArtBitmap = new Bitmap[artImage.length];
			mArtImages = new SVIImage[artImage.length];
			
			for(int i=0; i<artImage.length; i++) {
				is = activity.getResources().openRawResource(artImage[i]);
				mArtBitmap[i] = BitmapFactory.decodeStream(is);
				mArtImages[i] = new SVIImage();
				mArtImages[i].setBitmap(mArtBitmap[i], SVIImage.AlphaType.NORMAL);			
			}
			
			mCoverBitmap = new Bitmap[coverImage.length];
			mCoverImage = new SVIImage[coverImage.length];
			
			for(int i = 0; i < coverImage.length; i++) {				
				is = activity.getResources().openRawResource(coverImage[i]);
				mCoverBitmap[i] = BitmapFactory.decodeStream(is);
				mCoverImage[i] = new SVIImage();
				mCoverImage[i].setBitmap(mCoverBitmap[i], SVIImage.AlphaType.NORMAL);			
			}
			
			mIcons = new Bitmap[testImage.length];
			mImages = new SVIImage[testImage.length];
			
			for(int i=0; i<testImage.length; i++) {
				is = activity.getResources().openRawResource(testImage[i]);
				mIcons[i] = BitmapFactory.decodeStream(is);
				mImages[i] = new SVIImage();
				//mImages[i].setBitmap(mIcons[i], SVIImage.AlphaType.PREMULTIPLIED);
				mImages[i].setBitmap(mIcons[i], SVIImage.AlphaType.NORMAL);
			}
		}
		
		mRotation = activity.getWindowManager().getDefaultDisplay().getRotation();
		mPrevRotation = mRotation;
		mWidth = activity.getWindowManager().getDefaultDisplay().getWidth();
		mHeight= activity.getWindowManager().getDefaultDisplay().getHeight();
		mPrevWidth = mWidth;
		mPrevHeight = mHeight;
		
		mSlideManager.setSVISlideRequestRender(this);
		
		mContext = context;
	}

	
	public void release() {
		if( mIcons != null ) {
			for(int i = 0; i < mIcons.length; ++i) {
				mIcons[i] = null;
			}
			mIcons = null;
		}
		
		if( mImages != null ) {
			for(int i = 0; i < mImages.length; ++i) {
				mImages[i].setBitmap(null);
				mImages[i] = null;
			}
			mImages = null;
		}
		
		if( mArtBitmap != null ) {
			for(int i = 0; i < mArtBitmap.length; ++i) {
				mArtBitmap[i] = null;
			}
			mArtBitmap = null;
		}
		
		if( mArtImages != null ) {
			for(int i = 0; i < mArtImages.length; ++i) {
				mArtImages[i].setBitmap(null);
				mArtImages[i] = null;
			}
			mArtImages = null;
		}
		
		if( mCoverBitmap != null ) {
			for(int i = 0; i < mCoverBitmap.length; ++i) {
				mCoverBitmap[i] = null;
			}
			mCoverBitmap = null;
		}
		
		if( mCoverImage != null ) {
			for(int i = 0; i < mCoverImage.length; ++i) {
				mCoverImage[i].setBitmap(null);
				mCoverImage[i] = null;
			}
			mCoverImage = null;
		}
	}
	
	
	protected SVISlide		getRootSlide()		{ return mRootSlide; }
	protected SVISlide		getMainSlide()		{ return mMainSlide; }
	
	protected Bitmap		mIcons[];
	protected SVIImage		mImages[];
	protected Bitmap		mCoverBitmap[];
	protected SVIImage		mCoverImage[];
	protected Bitmap		mArtBitmap[];	
	protected SVIImage		mArtImages[];
	protected int			mRotation;
	protected int			mPrevRotation;
	protected int			mWidth;
	protected int			mHeight;
	protected int			mPrevWidth;
	protected int			mPrevHeight;
	protected Context		mContext;
	
	private SVISlide			mRootSlide = null;
	private SVISlide			mMainSlide = null;

	@Override
	public boolean requestSlideRender() {
		// TODO Auto-generated method stub
		swapAnimationSVIGLSurface();
		requestRender();
		//Log.i("SVI", "requestRender");
		
		return true;
	}

	@Override
	public void animationSlideNotify() {
		// TODO Auto-generated method stub
		
	}
}
