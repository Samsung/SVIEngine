/**
 * RemoteControllerSample.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.sample;

import java.io.InputStream;

import android.app.ActionBar.LayoutParams;
import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SubMenu;
import android.view.View;
import android.view.View.OnHoverListener;
import android.view.View.OnKeyListener;
import android.view.View.OnLongClickListener;
import android.view.ViewTreeObserver;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AbsoluteLayout;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.Toast;

import com.github.svieffect.R;
import com.github.svieffect.main.SVIEffectListener;
import com.github.svieffect.main.SVIEffectNode;
import com.github.svieffect.main.SVIEffectView;
import com.github.svieffect.remotecontroller.SVIEffectRemoteController;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.slide.SVISlide;


//2012-12-04 masterkeaton27@gmail.com
//NOTE GRAPHICS: 
//should implements from SGIEffectListener interface for to check first render finished.
public class RemoteControllerSample extends Activity implements SensorEventListener, SVIEffectListener{
	public static float RES_SCALE = 1.5f;
	public static SVIEffectRemoteController mEffectView = null;
	public static Sensor gravitymeter;	
	public static SensorManager mSensorManager;
	public static float[] mGravity = new float[]{0.0f, 0.0f, 0.0f};
	
	//2012-12-13 phyuzion@gmail.com
  	//NOTE GRAPHICS: 
    //dial Button Resource ID
	private int[] mViewLandID ={
			R.id.channel_btn_land,
			R.id.volume_btn_land,			
	};
	
	private int[] mViewPortID ={
			R.id.channel_btn_port,
			R.id.volume_btn_port,			
	};
	 
	
	private Bitmap	mChannelButtonUpBitmap = null;
	private Bitmap	mChannelButtonDownBitmap = null;
	private Bitmap	mVolumeButtonUpBitmap = null;
	private Bitmap	mVolumeButtonDownBitmap = null;
	
	private Bitmap	mBackgroundBitmap = null;	
	
	
	private float	mWidth = 720.0f;
	private float	mHeight = 1280.0f;
	
	
	private FrameLayout mLayout = null;
	
	//2012-12-21 masterkeaton27@gmail.com
	private AbsoluteLayout mLayoutLandScape = null;	
	private AbsoluteLayout mLayoutPortraits = null;	
	
	
	//2012-12-21 masterkeaton27@gmail.com
	private SmartRemoteView mView = null; 	//to process event (dummy view)
	
	//2013-02-23 jdev.nam
	private FrameLayout mEffectLayout ;

	//2013-02-23 jdev.nam
	private int mAddingYDelta =0;
	private int mPreviousScrollYDelta = 0;
	private int mScrollYDelta = 0;
	private int mTopOffset = 0;
	
	//2013-02-23 jdev.nam
	private SmartRemoteButton mChBtn = null;
	private SmartRemoteButton mChDownBtn = null;
	private SmartRemoteButton mVBtn = null;
	private SmartRemoteButton mVDownBtn = null;
		
	
	//2012-12-04 masterkeaton27@gmail.com
	//NOTE GRAPHICS: 
	//please remind that this temporary SmartRemote View for touch event handling 
	public class SmartRemoteView extends ImageView implements OnLongClickListener, OnKeyListener, OnHoverListener {
		
		private int mLastIndexOfButton = -1;
		
		public SmartRemoteView(Context context) {
			super(context);
			// TODO Auto-generated constructor stub
			
			//2012-12-21 masterkeaton27@gmail.com
			setLongClickable(true);
			setHovered(true);						
			setOnLongClickListener(this);
			setOnHoverListener(this);
			setOnKeyListener(this);
		}
		
		
		@Override 
		public boolean onTouchEvent(final MotionEvent event) {			
			//2012-12-21 masterkeaton27@gmail.com			
			if( mEffectView != null ){
				SVIEffectView.RequestType requestType = SVIEffectView.RequestType.REQUEST_CLICK_RESULT;
				
				//2013-02-23 jdev.nam
				//NOTE GRAPHICS: 
				//Passing mScrollYDelta
				mEffectView.setScrollPositionY(mScrollYDelta);
				
				//2013-02-23 jdev.nam
				//NOTE GRAPHICS: 
				//Passing offset of Top
				mEffectView.setTopOffset(mTopOffset);
				boolean result = mEffectView.validateOnTouch(requestType, event);
			}
			return super.onTouchEvent(event);
		}

		public boolean onHover(View v, MotionEvent event) {
			//2012-12-21 masterkeaton27@gmail.com
			SVIEffectView.RequestType requestType = SVIEffectView.RequestType.REQUEST_HOVER_RESULT;
			if( mEffectView != null ){				
				mEffectView.validateOnTouch(requestType, event);
			}
			// TODO Auto-generated method stub
			return false;
		}

		public boolean onKey(View v, int keyCode, KeyEvent event) {
			//2012-12-21 masterkeaton27@gmail.com
			SVIEffectView.RequestType requestType = SVIEffectView.RequestType.REQUEST_KEY_RESULT;
				
			// TODO Auto-generated method stub
			return false;
		}

		public boolean onLongClick(View v) {
			//2012-12-21 masterkeaton27@gmail.com
			SVIEffectView.RequestType requestType = SVIEffectView.RequestType.REQUEST_LONGPRESS_RESULT;
						
			mEffectView.validateOnLongClick();		
			
			// TODO Auto-generated method stub
			return false;
		}		
	}
	
	
	//2012-12-04 masterkeaton27@gmail.com
	//NOTE GRAPHICS:
	//SEE RELATED: buildLayouts()
	//smart remote button class (node for handling re-action with given touch event)
	//please look into class SGIEffectNode with it's methods.	
	public class SmartRemoteButton extends SVIEffectNode{
		SmartRemoteButton() {
			super();
		}

		@Override
		public boolean onClick(int idx, int type, MotionEvent inputEvent) {
			//2012-12-04 masterkeaton27@gmail.com
			//TODO 			
			if (inputEvent.getAction() == MotionEvent.ACTION_DOWN){
				if (type == 1) Log.i("Controller Sample", "Down Button<"+getIndex()+">  clicked.");
				else if (type == -1) Log.i("Controller Sample", "Up Button<"+getIndex()+">  clicked.");
				else if (type == 0) Log.i("Controller Sample", "Stay Button<"+getIndex()+">  clicked.");				
			}
			return false;
		}

		@Override
		public boolean onLongPress(int idx, int type, MotionEvent inputEvent) {
			return false;
		}

		@Override
		public boolean onHovering(int idx, int type, MotionEvent inputEvent) {
			return false;
		}
	}
	
	//2012-12-21 masterkeaton27@gmail.com
	public View buildViews_Portraits(Context context){		
		//2012-12-12 phyuzion@gmail.com
		//NOTE GRAPHICS: 
		//Create Layout use LayoutInflater with Layout ID
		LayoutInflater factory = LayoutInflater.from(context);
		View layoutView = factory.inflate(R.layout.smartremote_layout_portraits, null);
		return layoutView;
	}	
	
	//2012-12-21 masterkeaton27@gmail.com
	public View buildViews_Landscape(Context context){	
		//2012-12-12 phyuzion@gmail.com
		//NOTE GRAPHICS: 
		//Create Layout use LayoutInflater with Layout ID
		LayoutInflater factory = LayoutInflater.from(context);
		View layoutView = factory.inflate(R.layout.smartremote_layout_landscape, null);
		return layoutView;
	}	
	
	//2012-12-21 masterkeaton27@gmail.com
	//Build GUIS from layout 
	public void buildLayouts(View layout, int[] viewsID){
		if (viewsID.length != 2){
			return;
		}
		
		//clear all attached nodes.
		mEffectView.clearNodes();
						
		//add channel button
		View buttonChViewFromXML = (View)layout.findViewById(viewsID[0]);
		if (buttonChViewFromXML != null){
			mChBtn = new SmartRemoteButton();
			mChBtn.setIndex(0);
			mChBtn.setColorGUI(mChannelButtonUpBitmap);
			mChBtn.setRegion(
					buttonChViewFromXML.getPaddingLeft(), 
					buttonChViewFromXML.getPaddingTop()-mTopOffset, 
					buttonChViewFromXML.getWidth(), 
					buttonChViewFromXML.getHeight());
			
			mEffectView.addNode(mChBtn);
			
			mChDownBtn = new SmartRemoteButton();
			mChDownBtn.setIndex(1);
			mChDownBtn.setColorGUI(mChannelButtonDownBitmap);
			mChDownBtn.setRegion(
					buttonChViewFromXML.getPaddingLeft(), 
					buttonChViewFromXML.getPaddingTop()-mTopOffset, 
					buttonChViewFromXML.getWidth(), 
					buttonChViewFromXML.getHeight());
				
			mEffectView.addNode(mChDownBtn);
		}
		
		//add volume button
		View buttonVolViewFromXML = (View)layout.findViewById(viewsID[1]);
		if (buttonVolViewFromXML != null){
			mVBtn = new SmartRemoteButton();
			mVBtn.setIndex(2);
			mVBtn.setColorGUI(mVolumeButtonUpBitmap);
			mVBtn.setRegion(
					buttonVolViewFromXML.getPaddingLeft(), 
					buttonVolViewFromXML.getPaddingTop()-mTopOffset, 
					buttonVolViewFromXML.getWidth(), 
					buttonVolViewFromXML.getHeight());			
			mEffectView.addNode(mVBtn);
			
			mVDownBtn = new SmartRemoteButton();
			mVDownBtn.setIndex(3);
			mVDownBtn.setColorGUI(mVolumeButtonDownBitmap);
			mVDownBtn.setRegion(
					buttonVolViewFromXML.getPaddingLeft(), 
					buttonVolViewFromXML.getPaddingTop()-mTopOffset, 
					buttonVolViewFromXML.getWidth(), 
					buttonVolViewFromXML.getHeight());
				
			mEffectView.addNode(mVDownBtn);
		}
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		//2012-12-04 masterkeaton27@gmail.com
		//NOTE GRAPHICS: 
		//set screen size to full, only for the demo.		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		mSensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
	    gravitymeter = mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);
	    
	    mWidth = getWindowManager().getDefaultDisplay().getWidth();
		mHeight= getWindowManager().getDefaultDisplay().getHeight();
		
		Configuration startConfig = getResources().getConfiguration();		
		buildLayoutBasedOnConfiguration(startConfig);	
		

	}	
		
	
	//2012-12-04 masterkeaton27@gmail.com
	//NOTE GRAPHICS: 
	//DON'T FORGET THIS!!
	@Override 
	protected void onDestroy(){
		super.onDestroy();
		
		if (mEffectView != null){
			mEffectView.destroy();
		}		
		
		releaseBitmaps();	
	}
	
	
	//2012-12-21 masterkeaton27@gmail.com
	@SuppressWarnings("deprecation")
	public void buildViews(AbsoluteLayout layout){
		mLayout = new FrameLayout(this);		
		
		//2012-12-04 masterkeaton27@gmail.com
		//NOTE GRAPHICS: 
		//Create Front Backgound View
		mView = new SmartRemoteView(this);
		mView.setImageBitmap(mBackgroundBitmap);
		mView.setScaleType(ScaleType.FIT_XY);
		
		 
		//2013-02-23 jdev.nam
		//NOTE GRAPHICS: 
		//Create ScrollView for the test cause by Top menu bar.
		ImageView mCoverView = new ImageView(this);
		InputStream is;
		Bitmap	mBgDiffuse;
		is =getResources().openRawResource(R.drawable.menubar);
		mBgDiffuse = BitmapFactory.decodeStream(is);
		
		mCoverView.setImageBitmap(mBgDiffuse);
		mCoverView.setScaleType(ScaleType.FIT_START);
		
		//Figure out offset using image height.
		mTopOffset = mBgDiffuse.getHeight();
		
		ScrollView mainScrollView = new ScrollView(this){ 
			@Override
			protected void onScrollChanged(int l, int t, int oldl, int oldt) {
				// TODO Auto-generated method stub
				super.onScrollChanged(l, t, oldl, oldt);
				mPreviousScrollYDelta = mScrollYDelta;
				mScrollYDelta = t;
						
				//2013-02-23 jdev.nam
				//NOTE GRAPHICS: 
				//Always effect layout  have to keep position even scrolling.
				mEffectLayout.setY(mScrollYDelta+mTopOffset);
				
				SVISlide chUpSlide = mChBtn.getSlide();
				SVISlide chDownSlide  = mChDownBtn.getSlide();
				
				SVISlide vUpSidle = mVBtn.getSlide();
				SVISlide vDownSlide = mVDownBtn.getSlide();
				
				//2013-02-23 jdev.nam
				//NOTE GRAPHICS: 
				//To figure out delta that how many delta of scroll you need.
				mAddingYDelta = (mPreviousScrollYDelta - mScrollYDelta);
				
				//2013-02-23 jdev.nam
				//NOTE GRAPHICS: 
				//Moving Y position of button.
				SVIRect tempChBtnRect = chUpSlide.getRegion();
				chUpSlide.setRegion(
						tempChBtnRect.mOrigin.mX, 
						tempChBtnRect.mOrigin.mY +mAddingYDelta, 
						tempChBtnRect.mSize.mWidth, 
						tempChBtnRect.mSize.mHeight);
				
				SVIRect tempChBtnDownRect =chDownSlide.getRegion();
				chDownSlide.setRegion(
						tempChBtnDownRect.mOrigin.mX, 
						tempChBtnDownRect.mOrigin.mY +mAddingYDelta, 
						tempChBtnDownRect.mSize.mWidth, 
						tempChBtnDownRect.mSize.mHeight);
				
				SVIRect tempVBtnRect = vUpSidle.getRegion();
				vUpSidle.setRegion(
						tempVBtnRect.mOrigin.mX, 
						tempVBtnRect.mOrigin.mY +mAddingYDelta, 
						tempVBtnRect.mSize.mWidth, 
						tempVBtnRect.mSize.mHeight);
				
				SVIRect tempVBtnDownRect =vDownSlide.getRegion();
				vDownSlide.setRegion(
						tempVBtnDownRect.mOrigin.mX, 
						tempVBtnDownRect.mOrigin.mY +mAddingYDelta, 
						tempVBtnDownRect.mSize.mWidth, 
						tempVBtnDownRect.mSize.mHeight);
			}
		};
		FrameLayout fLayout = new FrameLayout(this);
		mEffectLayout = new FrameLayout(this);
		
		mainScrollView.setLayoutParams(new RelativeLayout.LayoutParams(
				LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
		
		mainScrollView.setScrollBarStyle(View.SCROLLBARS_INSIDE_INSET);
		
		fLayout.addView(mView);
		mEffectLayout.addView(createEffectView());
		mEffectLayout.addView(layout);
		mEffectLayout.setY(mTopOffset);
		fLayout.addView(mEffectLayout);
		
		mainScrollView.addView(fLayout);
		
		mLayout.addView(mainScrollView);

		mLayout.addView(mCoverView);
	
		setContentView(mLayout);
	}
		
	//2012-12-21 masterkeaton27@gmail.com
	public void loadBitmapsForLandscape(){				
		releaseBitmaps();
				
		InputStream is;		
		
		is = getResources().openRawResource(R.drawable.ch_pack_001);
		mChannelButtonUpBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.ch_pack_002);
		mChannelButtonDownBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.vol_pack_001);
		mVolumeButtonUpBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.vol_pack_002);
		mVolumeButtonDownBitmap = BitmapFactory.decodeStream(is);
								
		is = getResources().openRawResource(R.drawable.remote_bg);
		mBackgroundBitmap = BitmapFactory.decodeStream(is);		
	}
	
	//2012-12-21 masterkeaton27@gmail.com
	public void loadBitmapsForPortraits(){				
		releaseBitmaps();
		
		InputStream is;		
		
		is = getResources().openRawResource(R.drawable.ch_pack_001);
		mChannelButtonUpBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.ch_pack_002);
		mChannelButtonDownBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.vol_pack_001);
		mVolumeButtonUpBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.vol_pack_002);
		mVolumeButtonDownBitmap = BitmapFactory.decodeStream(is);
								
		is = getResources().openRawResource(R.drawable.remote_bg);
		mBackgroundBitmap = BitmapFactory.decodeStream(is);		
	}
	
	public void releaseBitmaps(){
		
		if (mBackgroundBitmap != null){
			mBackgroundBitmap.recycle();
			mBackgroundBitmap = null;
		}
		
		if (mChannelButtonUpBitmap != null){
			mChannelButtonUpBitmap.recycle();
			mChannelButtonUpBitmap = null;
		}
		
		if (mChannelButtonDownBitmap != null){
			mChannelButtonDownBitmap.recycle();
			mChannelButtonDownBitmap = null;
		}
		
		if (mVolumeButtonUpBitmap != null){
			mVolumeButtonUpBitmap.recycle();
			mVolumeButtonUpBitmap = null;
		}
		
		if (mVolumeButtonDownBitmap != null){
			mVolumeButtonDownBitmap.recycle();
			mVolumeButtonDownBitmap = null;
		}
		
	}
		
	//2012-12-21 masterkeaton27@gmail.com
	public void buildLayoutForPortrait(){
	
		//release previous bitmaps and load new 
		loadBitmapsForPortraits();
		
		//get layout view from xml
		mLayoutPortraits = (AbsoluteLayout)buildViews_Portraits(this);
		
		//build legacy views & inflate all.
		buildViews(mLayoutPortraits);
		
		//get GUI information from layout
		buildViewBasedOnXML_Portraits();
		
	}
	
	//2012-12-21 masterkeaton27@gmail.com
	public void buildLayoutForLandScape(){
		
		//release previous bitmaps and load new 
		loadBitmapsForLandscape();
		
		//get layout view from xml
		mLayoutLandScape = (AbsoluteLayout)buildViews_Landscape(this);
		
		//build legacy views & inflate all.
		buildViews(mLayoutLandScape);
		
		//get GUI information from layout
		buildViewBasedOnXML_Landscape();
		
	}		
	
	//2012-12-21 masterkeaton27@gmail.com
	private OnGlobalLayoutListener mListenerPortraits = new OnGlobalLayoutListener(){
	  	public void onGlobalLayout() {		  		
	  		buildLayouts(mLayoutPortraits, mViewPortID);	  		
	  		ViewTreeObserver vto = mLayout.getViewTreeObserver();
	  		vto.removeGlobalOnLayoutListener(mListenerPortraits);
	  		mLayout.removeView(mLayoutPortraits);
	  	}
	};
	
	//2012-12-21 masterkeaton27@gmail.com
	private OnGlobalLayoutListener mListenerLandscape = new OnGlobalLayoutListener(){
	  	public void onGlobalLayout() {		  		
	  		buildLayouts(mLayoutLandScape, mViewLandID);	  
	  		ViewTreeObserver vto = mLayout.getViewTreeObserver();
	  		vto.removeGlobalOnLayoutListener(mListenerLandscape);
	  		mLayout.removeView(mLayoutLandScape);
	  	}
	};
	
	
	//2012-12-21 masterkeaton27@gmail.com
	public void buildViewBasedOnXML_Landscape(){		
		if(mLayoutLandScape == null)
	  		return;
		
	  	//2012-12-12 phyuzion@gmail.com
		//NOTE GRAPHICS: need Calculate layout first Time.
		ViewTreeObserver vto = mLayout.getViewTreeObserver();
		vto.addOnGlobalLayoutListener(mListenerLandscape);
	}
	
	//2012-12-21 masterkeaton27@gmail.com
	public void buildViewBasedOnXML_Portraits(){		
	  	if(mLayoutPortraits == null)
	  		return;
	  	    
	  	//2012-12-12 phyuzion@gmail.com
		//NOTE GRAPHICS: need Calculate layout first Time.
		ViewTreeObserver vto = mLayout.getViewTreeObserver();
		vto.addOnGlobalLayoutListener(mListenerPortraits);	    	    
	}
	
	
	//2012-12-21 masterkeaton27@gmail.com
	public void buildLayoutBasedOnConfiguration(Configuration config){
		if (config.orientation == Configuration.ORIENTATION_PORTRAIT) {			
			buildLayoutForPortrait();
		} else if (config.orientation == Configuration.ORIENTATION_LANDSCAPE){			
			buildLayoutForLandScape();
		}
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
		
		//2012-12-21 masterkeaton27@gmail.com
		buildLayoutBasedOnConfiguration(newConfig);		
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
	    //2012-12-04 masterkeaton27@gmail.com
	  	//NOTE GRAPHICS: 
	    //DON'T FORGET THIS!!!!
	    if( mEffectView != null )
	    	mEffectView.onPause();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	
	    //2012-12-04 masterkeaton27@gmail.com
	  	//NOTE GRAPHICS: 
	    //DON'T FORGET THIS!!!!
	    if( mEffectView != null )
	    	mEffectView.onResume();
	}
	
	public SVIEffectRemoteController createEffectView() {
		// TODO Auto-generated method stub
		
		//2012-12-04 masterkeaton27@gmail.com
	  	//NOTE GRAPHICS: 
	    //Create effect view, set listener for check loaded.
		mEffectView = new SVIEffectRemoteController(this);
		mEffectView.setListener(this);
		mEffectView.initialize();

		return mEffectView;
	}
	
	
	
	@Override
	public boolean onCreateOptionsMenu (Menu menu) {				
		SubMenu subMenu = menu.addSubMenu("FPS");
		subMenu.add(0, 101, 0, "5-FPS");
		subMenu.add(0, 102, 0, "10-FPS");
		subMenu.add(0, 103, 0, "15-FPS");
		subMenu.add(0, 104, 0, "20-FPS");
		subMenu.add(0, 105, 0, "25-FPS");
		subMenu.add(0, 106, 0, "30-FPS");
		subMenu.add(0, 107, 0, "35-FPS");
		subMenu.add(0, 108, 0, "40-FPS");
		subMenu.add(0, 109, 0, "45-FPS");
		subMenu.add(0, 110, 0, "50-FPS");
		subMenu.add(0, 111, 0, "55-FPS");
		subMenu.add(0, 112, 0, "60-FPS");

		/*
		subMenu = menu.addSubMenu("Animation Speed");
		subMenu.add(0, 201, 0, "x0.1");
		subMenu.add(0, 202, 0, "x0.2");
		subMenu.add(0, 203, 0, "x0.3");
		subMenu.add(0, 204, 0, "x0.4");
		subMenu.add(0, 205, 0, "x0.5");
		subMenu.add(0, 206, 0, "x0.6");
		subMenu.add(0, 207, 0, "x0.7");
		subMenu.add(0, 208, 0, "x0.8");
		subMenu.add(0, 209, 0, "x0.9");
		subMenu.add(0, 210, 0, "x1.0");
		subMenu.add(0, 211, 0, "x2.0");
		subMenu.add(0, 212, 0, "x3.0");
		subMenu.add(0, 213, 0, "x4.0");
		subMenu.add(0, 214, 0, "x5.0");
		subMenu.add(0, 215, 0, "x6.0");
		subMenu.add(0, 216, 0, "x7.0");
		subMenu.add(0, 217, 0, "x8.0");
		subMenu.add(0, 218, 0, "x9.0");
		subMenu.add(0, 219, 0, "x10.0");
		*/
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected (MenuItem item) {	
		boolean bFPS = false;
		boolean bAnimationDuration = false;

		switch (item.getItemId()) {
		case 101: mEffectView.setRenderFPS(5); bFPS = true; break;
		case 102: mEffectView.setRenderFPS(10); bFPS = true; break;
		case 103: mEffectView.setRenderFPS(15); bFPS = true; break;
		case 104: mEffectView.setRenderFPS(20); bFPS = true; break;
		case 105: mEffectView.setRenderFPS(25); bFPS = true; break;
		case 106: mEffectView.setRenderFPS(30); bFPS = true; break;
		case 107: mEffectView.setRenderFPS(35); bFPS = true; break;
		case 108: mEffectView.setRenderFPS(40); bFPS = true; break;
		case 109: mEffectView.setRenderFPS(45); bFPS = true; break;
		case 110: mEffectView.setRenderFPS(50); bFPS = true; break;
		case 111: mEffectView.setRenderFPS(55); bFPS = true; break;
		case 112: mEffectView.setRenderFPS(60); bFPS = true; break;

		/*
		case 201: mEffectView.setAnimationSpeed(0.1f); bAnimationDuration = true; break;
		case 202: mEffectView.setAnimationSpeed(0.2f); bAnimationDuration = true; break;
		case 203: mEffectView.setAnimationSpeed(0.3f); bAnimationDuration = true; break;
		case 204: mEffectView.setAnimationSpeed(0.4f); bAnimationDuration = true; break;
		case 205: mEffectView.setAnimationSpeed(0.5f); bAnimationDuration = true; break;
		case 206: mEffectView.setAnimationSpeed(0.6f); bAnimationDuration = true; break;
		case 207: mEffectView.setAnimationSpeed(0.7f); bAnimationDuration = true; break;
		case 208: mEffectView.setAnimationSpeed(0.8f); bAnimationDuration = true; break;
		case 209: mEffectView.setAnimationSpeed(0.9f); bAnimationDuration = true; break;
		case 210: mEffectView.setAnimationSpeed(1.0f); bAnimationDuration = true; break;
		case 211: mEffectView.setAnimationSpeed(2.0f); bAnimationDuration = true; break;
		case 212: mEffectView.setAnimationSpeed(3.0f); bAnimationDuration = true; break;
		case 213: mEffectView.setAnimationSpeed(4.0f); bAnimationDuration = true; break;
		case 214: mEffectView.setAnimationSpeed(5.0f); bAnimationDuration = true; break;
		case 215: mEffectView.setAnimationSpeed(6.0f); bAnimationDuration = true; break;
		case 216: mEffectView.setAnimationSpeed(7.0f); bAnimationDuration = true; break;
		case 217: mEffectView.setAnimationSpeed(8.0f); bAnimationDuration = true; break;
		case 218: mEffectView.setAnimationSpeed(9.0f); bAnimationDuration = true; break;
		case 219: mEffectView.setAnimationSpeed(10.0f); bAnimationDuration = true; break;
		*/
		}

		if (bFPS) {
			Toast.makeText(this, item.getTitle(), Toast.LENGTH_SHORT).show();
		}
		else if (bAnimationDuration) {
			Toast.makeText(this, "Animation Speed: " + item.getTitle(), Toast.LENGTH_SHORT).show();
		}
		
		return true;
	}
	@Override
	public void onFirstRenderFinished() {
		
		//2013-02-23 jdev.nam
		//NOTE GRAPHICS: 
		//We need to hide Effect button while scrolling. 
		//If not, we can get after-image because Effect button need high CPU.
		mEffectView.setHideNode();
	}
	

	@Override
	public void onAnimateHideFinished() {
		// TODO Auto-generated method stub
		
		//2013-02-23 jdev.nam
		//NOTE GRAPHICS: 
		//Animation is done. Don't need show the effect button.
		mEffectView.setHideNode();
	}
	@Override
	public void onAnimateShowFinished() {
		// TODO Auto-generated method stub
	}

	@Override
	public void onAccuracyChanged(Sensor arg0, int arg1) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		// TODO Auto-generated method stub
		
	}

}
