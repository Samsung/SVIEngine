/**
 * PhotoEditorSample.java 
 * @author Wooram Seo
 * @email rareboy0112@facebook.com
 */

package com.github.svieffect.sample;

import java.io.InputStream;
import java.util.Vector;

import net.fxgear.android.util.drawpoints.DrawPointsView;
import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;

import com.github.svieffect.R;
import com.github.svieffect.copyandpaste.FXGCopyAndPasteView;
import com.github.svieffect.copyandpaste.SVICopyAndPasteCallback;

//NOTE GRAPHICS: 
//should implements from SGIEffectListener interface for to check first render finished.
public class FXGPhotoEditorSample extends Activity implements SVICopyAndPasteCallback{
	
	public static FXGCopyAndPasteView mCopyAndPasteView = null;

	//NOTE FXGear:
	//should add mDrawPointsView to see dot-lined contours.
	public DrawPointsView mDrawPointsView = null;
	
	private float	mWidth = 720.0f;
	private float	mHeight = 1280.0f;
	
	private FrameLayout mLayout = null;
	private DummyPhotoView mView = null;
	
	private Vector<Point> mPoints = new Vector<Point>();
	
	private Bitmap mBackgroundBitmap = null;
	private boolean mIsAnimating = false;
	private boolean mMorphingCopy = false;
	private boolean mMorphingDrag = false;
	
	//variables for customize demo
	private static final int		MORPHING_ANIMATION_DURATION = 700;
	private final int 				MIN_DISTANCE = 10;
	
	//2013-02-21 rareboy0112@facebook.com
	//splash view to avoid initialization blink
	private ImageView mSplashView = null;
	
	public class DummyPhotoView extends ImageView {
		
		public DummyPhotoView(Context context) {
			super(context);
			// TODO Auto-generated constructor stub			
		}
		
		@Override 
		public boolean onTouchEvent(final MotionEvent event) {
						
			//NOTE GRAPHICS: 
			//check animating state.
			if(mIsAnimating == true)
				return super.onTouchEvent(event);
			
			int x = (int)event.getRawX();
			int y = (int)event.getRawY();
			
			if(mMorphingCopy == false)
			{
				if(mMorphingDrag != true && event.getAction() == MotionEvent.ACTION_DOWN)
				{
					mMorphingDrag = true;
					
					mPoints.clear();
				}
				else if(mMorphingDrag == true && event.getAction() == MotionEvent.ACTION_MOVE)
				{
					if(mPoints.size() > 0)
					{
						boolean added = false;
						Point startPoint = mPoints.get(0);
						Point lastPoint = mPoints.get(mPoints.size() - 1);
						if( x > lastPoint.x + MIN_DISTANCE ||
							x < lastPoint.x - MIN_DISTANCE ||
							y > lastPoint.y + MIN_DISTANCE ||
							y < lastPoint.y - MIN_DISTANCE )
						{
							Point newPoint = new Point(x, y);
							mPoints.add(newPoint);
							
							added = true;
							
						}
						//NOTE GRAPHICS: 
						//FIRST_POINT == LAST_POINT : start Copy animation
						if( mPoints.size() > 3 &&
							x < startPoint.x + MIN_DISTANCE &&
							x > startPoint.x - MIN_DISTANCE && 
							y < startPoint.y + MIN_DISTANCE && 
							y > startPoint.y - MIN_DISTANCE )
						{
							if(added == false)
							{
								Point newPoint = new Point(x, y);
								mPoints.add(newPoint);
							}
							
							runMorphing();
							
							mPoints.clear();
							mIsAnimating = true;
							mMorphingDrag = false;
							mMorphingCopy = false;
							mView.setAlpha(0.7f);
						}
					}
					else
					{
						Point newPoint = new Point(x, y);
						mPoints.add(newPoint);
					}					
				}
				//NOTE GRAPHICS: 
				//ACTION_UP : start Copy animation
				else if(mMorphingDrag == true && event.getAction() == MotionEvent.ACTION_UP && mPoints.size() > 3)
				{
					runMorphing();
					
					mPoints.clear();
					mIsAnimating = true;
					mMorphingDrag = false;
					mMorphingCopy = false;
					mView.setAlpha(0.7f);
				}
			}

			//NOTE FXGear:
			//should add mDrawPointsView to see dot-lined contours.
			mDrawPointsView.setPoints(mPoints);
			
			return true;
		}
	}
	
	//NOTE GRAPHICS:
	//set property of morphing geometry.
	//should be called prior to morphing animation.
	private void runMorphing(){
		//set app window size.
		mCopyAndPasteView.setWindowRegion(0.0f, 0.0f, mWidth, mHeight);
		//set app screen-shot of app-window.
		mCopyAndPasteView.setWindowImage(mBackgroundBitmap);
		//circle radius
		mCopyAndPasteView.setMorphingRadius(50.0f);
		//set detail of morphing geometry.(1 ~ n)
		//mCopyAndPasteView.setMorphingGridCount(16);
		mCopyAndPasteView.setMorphingVertexCount(32);
		//set color of morphing geometry.
		float color[] = {0.2f, 0.3f, 0.8f, 0.0f};
		mCopyAndPasteView.setMorphingColor(color);
		//set point-list of morphing geometry.(tesselation point-list)
		mCopyAndPasteView.setIntPoints(mPoints);
		
		mCopyAndPasteView.show();
		mCopyAndPasteView.play(MORPHING_ANIMATION_DURATION);
	}
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		//NOTE GRAPHICS: 
		//set screen size to full, only for the demo.
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
				
	    mWidth = getWindowManager().getDefaultDisplay().getWidth();
		mHeight = getWindowManager().getDefaultDisplay().getHeight();
			
		
		buildViews();
	}	
	
	//NOTE GRAPHICS: 
	//DON'T FORGET THIS!!
	@Override 
	protected void onDestroy(){
		super.onDestroy();
		
		if (mCopyAndPasteView != null){
			mCopyAndPasteView.destroy();
		}		
				
		releaseBitmaps();
	}
	
	public void buildViews(){
		loadBitmaps();
		mLayout = new FrameLayout(this);
		
		//2013-02-21 rareboy0112@facebook.com
		//NOTE GRAPHICS: 
		//this is App View.
		mView = new DummyPhotoView(this);
		mView.setImageBitmap(mBackgroundBitmap);
		mView.setScaleType(ScaleType.FIT_XY);

		//NOTE FXGear:
		//Should call setZOrderOnTop(true) to make this effect opaque.
		mCopyAndPasteView = new FXGCopyAndPasteView(this);
		mCopyAndPasteView.setCallbackListener(this);
		mCopyAndPasteView.hide();
		mCopyAndPasteView.setZOrderOnTop(true);
		
		mLayout.addView(mCopyAndPasteView);
		
		//2013-02-21 rareboy0112@facebook.com
		mSplashView = new ImageView(this);
		mSplashView.setImageBitmap(mBackgroundBitmap);
		mSplashView.setScaleType(ScaleType.FIT_XY);
		mLayout.addView(mSplashView);
		
		mLayout.addView(mView);

		//NOTE FXGear:
		//should add mDrawPointsView to see dot-lined contours.
		mDrawPointsView = new DrawPointsView(this, 0xfff75d59);
		mDrawPointsView.setZOrderOnTop(true);
		
		mLayout.addView(mDrawPointsView);
		
		setContentView(mLayout);
	}
	
	protected void releaseBitmaps(){		
		if (mBackgroundBitmap != null){
			mBackgroundBitmap.recycle(); 
			mBackgroundBitmap = null;
		}
	}
	
	public void loadBitmaps(){
		InputStream is;			
							
		is = getResources().openRawResource(R.drawable.back);
			mBackgroundBitmap = BitmapFactory.decodeStream(is);
	}
		
		
		
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);				
	}
	
	
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
	    
	    //NOTE GRAPHICS: 
	    //DON'T FORGET THIS!!!!
	    if( mCopyAndPasteView != null )
	    	mCopyAndPasteView.onPause();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	
	    //NOTE GRAPHICS: 
	    //DON'T FORGET THIS!!!!
	    if( mCopyAndPasteView != null )
	    	mCopyAndPasteView.onResume();
	}
	

	//NOTE FXGear:
	//Added options.
	@Override
	public boolean onCreateOptionsMenu (Menu menu) {	
    	menu.add("Enable drawing dot-lined contours");
    	menu.add("Disable drawing dot-lined contours");
		
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected (MenuItem item) {
		if(item.getTitle() == "Enable drawing dot-lined contours")
		{
			if(mDrawPointsView != null)
			{
				mDrawPointsView.setDrawing(true);
			}
		}
		if(item.getTitle() == "Disable drawing dot-lined contours")
		{
			if(mDrawPointsView != null)
			{
				mDrawPointsView.setDrawing(false);
			}
		}
		
		return true;
	}

	//2013-02-21 rareboy0112@facebook.com
	//remove splash view after first rendering finished.
	private Handler mRemoveSplashHandler = new Handler(){
		@Override
		public void handleMessage(Message message) {
			if (mSplashView != null) mLayout.removeView(mSplashView);
		}
	};
	
	//2012-02-21 rareboy0112@facebook.com
	@Override
	public void onFirstRenderFinished() {
		//2013-02-21 rareboy0112@facebook.com
		//after get in this function, it's under control of GL Thread.
		//so if we want remove splash view, should send a remove message to main thread.
		mRemoveSplashHandler.sendEmptyMessage(0);
	}

	public void onAnimationEnd() {
		// TODO Auto-generated method stub
		mCopyAndPasteView.hide();
		mIsAnimating = false;
		
		mView.setAlpha(1.0f);
	}


}
