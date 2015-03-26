/**
 * CalculatorSample.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.sample;

import java.io.InputStream;

import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
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
import android.widget.Toast;

import com.github.svieffect.R;
import com.github.svieffect.calculator.SVIEffectCalculator;
import com.github.svieffect.main.SVIEffectListener;
import com.github.svieffect.main.SVIEffectNode;
import com.github.svieffect.main.SVIEffectView;
import com.github.sviengine.basetype.SVIRect;

//2012-12-04 masterkeaton27@gmail.com
//NOTE GRAPHICS: 
//should implements from SGIEffectListener interface for to check first render finished.
public class CalculatorSample extends Activity implements /*SensorEventListener, */SVIEffectListener{
	
	public static float RES_SCALE = 1.5f;
	public static int BUTTON_COUNT = 20;
	public static SVIEffectCalculator mCalculatorEffectView = null;
//	public static Sensor gravitymeter;	
//	public static SensorManager mSensorManager;
//	public static float[] mGravity = new float[]{0.0f, 0.0f, 0.0f};
	
	
	//2012-12-13 phyuzion@gmail.com
  	//NOTE GRAPHICS: 
    //dial Button Resource ID
	private int[] mViewLandID ={
			R.id.cno_1_land,
			R.id.cno_2_land,
			R.id.cno_3_land,
			R.id.cno_4_land,
			R.id.cno_5_land,
			R.id.cno_6_land,
			R.id.cno_7_land,
			R.id.cno_8_land,
			R.id.cno_9_land,
			R.id.cno_10_land,
			R.id.cno_11_land,
			R.id.cno_12_land,
			R.id.cno_13_land,
			R.id.cno_14_land,
			R.id.cno_15_land,
			R.id.cno_16_land,
			R.id.cno_17_land,
			R.id.cno_18_land,
			R.id.cno_19_land,
			R.id.cno_20_land,
			R.id.cno_21_land,
			R.id.cno_22_land,
			R.id.cno_23_land,
			R.id.cno_24_land,
			R.id.cno_25_land,
			R.id.cno_26_land,
			R.id.cno_27_land,
			R.id.cno_28_land,
			R.id.cno_29_land,
			R.id.cno_30_land,
			R.id.cno_31_land,
			R.id.cno_32_land,
			R.id.cno_33_land,
			R.id.cno_34_land,
			R.id.cno_35_land,
	};
	
	private int[] mViewPortID ={
			R.id.cno_1_port,
			R.id.cno_2_port,
			R.id.cno_3_port,
			R.id.cno_4_port,
			R.id.cno_5_port,
			R.id.cno_6_port,
			R.id.cno_7_port,
			R.id.cno_8_port,
			R.id.cno_9_port,
			R.id.cno_10_port,
			R.id.cno_11_port,
			R.id.cno_12_port,
			R.id.cno_13_port,
			R.id.cno_14_port,
			R.id.cno_15_port,
			R.id.cno_16_port,
			R.id.cno_17_port,
			R.id.cno_18_port,
			R.id.cno_19_port,
			R.id.cno_20_port,
	};
		
	private Bitmap	[] mButtonBitmap = null;	
	private Bitmap	mBackgroundBitmap = null;
	private Bitmap	mBackgroundHistoryBitmap = null;
	
	private Bitmap	mPressDiffuseMapBitmap = null;
		
	private Bitmap	mPannelDiffuseMapBitmap = null;
	private Bitmap	mPannelNormalMapBitmap = null;
	private Bitmap	mPannelSpecularMapBitmap = null;
	
	
	private Bitmap	[] mButtonBitmapLand = null;	
	private Bitmap	mBackgroundBitmapLand = null;
	private Bitmap	mBackgroundHistoryBitmapLand = null;	
	private Bitmap	mPressDiffuseMapBitmapLand = null;		
	private Bitmap	mPannelDiffuseMapBitmapLand = null;
	private Bitmap	mPannelNormalMapBitmapLand = null;
	private Bitmap	mPannelSpecularMapBitmapLand = null;
	
	
	
	private Bitmap	mHandleDiffuseMapBitmap = null;
	private Bitmap	mHandleNormalMapBitmap = null;
	private Bitmap	mHandleSpecularMapBitmap = null;
	
	//2012-12-26 masterkeaton27@gmail.com
	//TO-SEL-INDIA : specity distance for hide (handle should be shows up even at hide state)
	private float 	mHideDistance;
	
	//2012-12-28 masterkeaton27@gmail.com
	//TO-SEL-INDIA : region information for handler
	private SVIRect  mHandlerHideRect = new SVIRect();
	private SVIRect  mHandlerShowRect = new SVIRect();
	private boolean mIsHidden = false;
	
	private float	mWidth = 720.0f;
	private float	mHeight = 1280.0f;
	
	private FrameLayout mLayout = null;	
	private CalculatorView mView = null;
	private CalculatorView mHistoryView = null;
	
	//2012-12-26 masterkeaton27@gmail.com
	//TO-SEL-INDIA: two layouts for orientation change.
	private AbsoluteLayout mLayoutLandScape = null;	
	private AbsoluteLayout mLayoutPortraits = null;	
	
	
	//2012-12-04 masterkeaton27@gmail.com
	//NOTE GRAPHICS: 
	//please remind that this temporary CalculatorView for touch event handling 
	public class CalculatorView extends ImageView  implements OnLongClickListener, OnKeyListener, OnHoverListener {
		
		public CalculatorView(Context context) {
			super(context);
			// TODO Auto-generated constructor stub
			
			//2012-12-26 masterkeaton27@gmail.com
			setLongClickable(true);
			setHovered(true);						
			setOnLongClickListener(this);
			setOnHoverListener(this);
			setOnKeyListener(this);
		}
		
		@Override 
		public boolean onTouchEvent(final MotionEvent event) {
			//2012-12-26 masterkeaton27@gmail.com
			//TO-SEL-INDIA :
			//should be placed on typical event processing routine for calculator application
			
			if( mCalculatorEffectView != null ){				
				SVIEffectView.RequestType requestType = SVIEffectView.RequestType.REQUEST_CLICK_RESULT;
				mCalculatorEffectView.validateOnTouch(requestType, event);				
			}
			
			
			//2012-12-28 masterkeaton27@gmail.com
			//TO-SEL-INDIA :
			if (event.getAction() == MotionEvent.ACTION_DOWN){
				float x = event.getX();
				float y = event.getY();
				SVIRect handlerRect = mIsHidden ? mHandlerShowRect : mHandlerHideRect;
				boolean isHit = mCalculatorEffectView.getHit(x, y, handlerRect);
				
				if (isHit){
					if (mIsHidden){
						mIsHidden = false;
						mCalculatorEffectView.animateShow(500);
					}else{
						mIsHidden = true;
						mCalculatorEffectView.animateHide(500, mHideDistance);
						mView.setVisibility(View.INVISIBLE);
						mHistoryView.setVisibility(View.VISIBLE);
					}
				}
			}else if (event.getAction() == MotionEvent.ACTION_MOVE){
				//TO-SEL-INDIA : 
				//use following function set position
				//mCalculatorEffectView.setPannelPosition(x, y)
			}
			
			return super.onTouchEvent(event);
		}

		@Override
		public boolean onHover(View arg0, MotionEvent event) {
			// TODO Auto-generated method stub
			SVIEffectView.RequestType requestType = SVIEffectView.RequestType.REQUEST_HOVER_RESULT;
			if( mCalculatorEffectView != null ){				
				mCalculatorEffectView.validateOnTouch(requestType, event);
			}
			
			return false;
		}

		@Override
		public boolean onKey(View v, int keyCode, KeyEvent event) {
			// TODO Auto-generated method stub
			
			return false;
		}

		@Override
		public boolean onLongClick(View v) {
			
			//2012-12-21 masterkeaton27@gmail.com
			SVIEffectView.RequestType requestType = SVIEffectView.RequestType.REQUEST_LONGPRESS_RESULT;
						
			mCalculatorEffectView.validateOnLongClick();		
			
			// TODO Auto-generated method stub
			return false;
		}		
	}
	
	
	//2012-12-04 masterkeaton27@gmail.com
	//NOTE GRAPHICS:
	//SEE RELATED: buildLayouts()
	//dialer button class (node for handling re-action with given touch event)
	//please look into class SGIEffectNode with it's methods.	
	public class CalculatorButton extends SVIEffectNode{
		CalculatorButton() {
			super();			
		}

		@Override
		public boolean onClick(int idx, int type, MotionEvent inputEvent) {
			//2012-12-04 masterkeaton27@gmail.com
			//TODO 
			if (inputEvent.getAction() == MotionEvent.ACTION_DOWN)
				Log.i("Calculator Sample", "Button<"+idx+"> clicked.");
			return false;
		}

		@Override
		public boolean onLongPress(int idx, int type, MotionEvent inputEvent) {
			//2012-12-04 masterkeaton27@gmail.com
			//TODO
			Log.i("Calculator Sample", "Button<"+idx+"> long pressed.");
			return false;
		}

		@Override
		public boolean onHovering(int idx, int type, MotionEvent inputEvent) {
			//2012-12-04 masterkeaton27@gmail.com
			//TODO
			Log.i("Calculator Sample", "Button<"+idx+"> hovering.");
			return false;
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
		
//		mSensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
//	    gravitymeter = mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);
	    
	    mWidth = getWindowManager().getDefaultDisplay().getWidth();
		mHeight = getWindowManager().getDefaultDisplay().getHeight();
		
		//2012-12-26 masterkeaton27@gmail.com
		//TO-SEL-INDIA : initialize layout based on initial orientation 
		Configuration startConfig = getResources().getConfiguration();		
		buildLayoutBasedOnConfiguration(startConfig);	
	}	
	
	//2012-12-04 masterkeaton27@gmail.com
	//NOTE GRAPHICS: 
	//DON'T FORGET THIS!!
	@Override 
	protected void onDestroy(){
		super.onDestroy();
		
		if (mCalculatorEffectView != null){
			mCalculatorEffectView.destroy();
		}		
				
		releaseBitmaps();
	}
	
	//2012-12-26 masterkeaton27@gmail.com
	//TO-SEL-INDIA : build android view with given layout	
	public void buildViews(AbsoluteLayout layout){
		mLayout = new FrameLayout(this);		
		
		mView = new CalculatorView(this);
		mView.setImageBitmap(mBackgroundBitmap);
		mView.setScaleType(ScaleType.FIT_XY);
		
		//2012-12-28 masterkeaton27@gmail.com
		mHistoryView = new CalculatorView(this);
		mHistoryView.setImageBitmap(mBackgroundHistoryBitmap);
		mHistoryView.setScaleType(ScaleType.FIT_XY);
		mHistoryView.setVisibility(View.INVISIBLE);
		
		float scale = RES_SCALE;
		
		//add effect view first.
		mLayout.addView(createEffectView());
				
		//add android view(focused) to take touch events and so on.
		mLayout.addView(mHistoryView);
		mLayout.addView(mView); 			
		
		//add layout for virtual-inflating.
		mLayout.addView(layout);
		
		setContentView(mLayout);
	}
	
	protected void releaseBitmaps(){
		releaseBitmapsPortraits();
		releaseBitmapsLandscape();
		
		if (mHandleDiffuseMapBitmap != null){
			mHandleDiffuseMapBitmap.recycle(); mHandleDiffuseMapBitmap = null;
		}
		
		if (mHandleNormalMapBitmap != null){
			mHandleNormalMapBitmap.recycle(); mHandleNormalMapBitmap = null;
		}
		
		if (mHandleSpecularMapBitmap != null){
			mHandleSpecularMapBitmap.recycle(); mHandleSpecularMapBitmap = null;
		}
	}
	
	protected void releaseBitmapsPortraits(){
		
		
		if (mButtonBitmap != null){
			for (int n =0; n < mButtonBitmap.length; n++) {
				if (mButtonBitmap[n] != null){
					mButtonBitmap[n].recycle();
					mButtonBitmap[n] = null;
				}
			}		
			mButtonBitmap = null;
		}
		
		if (mPressDiffuseMapBitmap != null){
			mPressDiffuseMapBitmap.recycle(); mPressDiffuseMapBitmap = null;
		}
		
		if (mBackgroundBitmap != null){
			mBackgroundBitmap.recycle(); mBackgroundBitmap = null;
		}
		
		if (mBackgroundHistoryBitmap != null){
			mBackgroundHistoryBitmap.recycle(); mBackgroundHistoryBitmap = null;
		}
		
		if (mPannelDiffuseMapBitmap != null){
			mPannelDiffuseMapBitmap.recycle(); mPannelDiffuseMapBitmap = null;
		}
		
		if (mPannelNormalMapBitmap != null){
			mPannelNormalMapBitmap.recycle(); mPannelNormalMapBitmap = null;
		}
		
		if (mPannelSpecularMapBitmap != null){
			mPannelSpecularMapBitmap.recycle(); mPannelSpecularMapBitmap = null;
		}	
	}
	
	protected void releaseBitmapsLandscape(){
		
		
		if (mButtonBitmapLand != null){
			for (int n =0; n < mButtonBitmapLand.length; n++) {
				if (mButtonBitmapLand[n] != null){
					mButtonBitmapLand[n].recycle();
					mButtonBitmapLand[n] = null;
				}
			}		
			mButtonBitmapLand = null;
		}
		
		if (mPressDiffuseMapBitmapLand != null){
			mPressDiffuseMapBitmapLand.recycle(); mPressDiffuseMapBitmapLand = null;
		}
		
		if (mBackgroundBitmap != null){
			mBackgroundBitmap.recycle(); mBackgroundBitmap = null;
		}
		
		if (mBackgroundHistoryBitmapLand != null){
			mBackgroundHistoryBitmapLand.recycle(); mBackgroundHistoryBitmapLand = null;
		}
		
		if (mPannelDiffuseMapBitmapLand != null){
			mPannelDiffuseMapBitmapLand.recycle(); mPannelDiffuseMapBitmapLand = null;
		}
		
		if (mPannelNormalMapBitmapLand != null){
			mPannelNormalMapBitmapLand.recycle(); mPannelNormalMapBitmapLand = null;
		}
		
		if (mPannelSpecularMapBitmapLand != null){
			mPannelSpecularMapBitmapLand.recycle(); mPannelSpecularMapBitmapLand = null;
		}		
	}
	
	//2012-12-21 masterkeaton27@gmail.com
	public void loadBitmapsForLandscape(){
		
		InputStream is;		
		
		is = getResources().openRawResource(R.drawable.cal_land_bg);
		mBackgroundBitmap = BitmapFactory.decodeStream(is);		
				
		is = getResources().openRawResource(R.drawable.cal_land_history_bg);
		mBackgroundHistoryBitmap = BitmapFactory.decodeStream(is);	
		
		if (mPressDiffuseMapBitmapLand != null) return;
		//releaseBitmapsLandscape();
				
		
		int demoImage[] = { 				
				R.drawable.cal_land_btn_num_01,
				R.drawable.cal_land_btn_num_02,
				R.drawable.cal_land_btn_num_03,
				R.drawable.cal_land_btn_num_04,
				R.drawable.cal_land_btn_num_05,
				R.drawable.cal_land_btn_num_06,
				R.drawable.cal_land_btn_num_07,
				R.drawable.cal_land_btn_num_08,
				R.drawable.cal_land_btn_num_09,
				R.drawable.cal_land_btn_num_10,
				R.drawable.cal_land_btn_num_11,
				R.drawable.cal_land_btn_num_12,
				R.drawable.cal_land_btn_num_13,
				R.drawable.cal_land_btn_num_14,
				R.drawable.cal_land_btn_num_15,
				R.drawable.cal_land_btn_num_16,
				R.drawable.cal_land_btn_num_17,
				R.drawable.cal_land_btn_num_18,
				R.drawable.cal_land_btn_num_19,
				R.drawable.cal_land_btn_num_20,
				R.drawable.cal_land_btn_num_21,
				R.drawable.cal_land_btn_num_22,
				R.drawable.cal_land_btn_num_23,
				R.drawable.cal_land_btn_num_24,
				R.drawable.cal_land_btn_num_25,
				R.drawable.cal_land_btn_num_26,
				R.drawable.cal_land_btn_num_27,
				R.drawable.cal_land_btn_num_28,
				R.drawable.cal_land_btn_num_29,
				R.drawable.cal_land_btn_num_30,
				R.drawable.cal_land_btn_num_31,
				R.drawable.cal_land_btn_num_32,
				R.drawable.cal_land_btn_num_33,
				R.drawable.cal_land_btn_num_34,
				R.drawable.cal_land_btn_num_35,
				};
		
		mButtonBitmapLand = new Bitmap[mViewLandID.length];
		
		for (int n = 0; n < mButtonBitmapLand.length; n++){
			is = getResources().openRawResource(demoImage[n]);
			mButtonBitmapLand[n] = BitmapFactory.decodeStream(is);
		}						
				
		is = getResources().openRawResource(R.drawable.cal_land_btn_press);
		mPressDiffuseMapBitmapLand = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.cal_land_diffuse);
		mPannelDiffuseMapBitmapLand = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.cal_land_normals);
		mPannelNormalMapBitmapLand = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.cal_land_weight);
		mPannelSpecularMapBitmapLand = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.cal_land_handle_diffuse);
		mHandleDiffuseMapBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.cal_land_handle_normals);
		mHandleNormalMapBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.cal_land_handle_weight);
		mHandleSpecularMapBitmap = BitmapFactory.decodeStream(is);
									
					
	}
		
	//2012-12-21 masterkeaton27@gmail.com
	public void loadBitmapsForPortraits(){		
		InputStream is;		
		
		is = getResources().openRawResource(R.drawable.calculator_bg);
		mBackgroundBitmap = BitmapFactory.decodeStream(is);		
		
		is = getResources().openRawResource(R.drawable.calculator_history_bg);
		mBackgroundHistoryBitmap = BitmapFactory.decodeStream(is);		
		
		if (mPressDiffuseMapBitmap != null) return;
		//releaseBitmapsPortraits();
		
		//2012-12-04 masterkeaton27@gmail.com
		//NOTE GRAPHICS: 
		//load bitmaps.
		
		
		int demoImage[] = { 				
				R.drawable.calculator_btn_num_01,
				R.drawable.calculator_btn_num_02,
				R.drawable.calculator_btn_num_03,
				R.drawable.calculator_btn_num_04,
				
				R.drawable.calculator_btn_num_05,
				R.drawable.calculator_btn_num_06,
				R.drawable.calculator_btn_num_07,
				R.drawable.calculator_btn_num_08,
				
				R.drawable.calculator_btn_num_09,
				R.drawable.calculator_btn_num_10,
				R.drawable.calculator_btn_num_11,
				R.drawable.calculator_btn_num_12,
				
				R.drawable.calculator_btn_num_13,
				R.drawable.calculator_btn_num_14,
				R.drawable.calculator_btn_num_15,
				R.drawable.calculator_btn_num_16,
				
				R.drawable.calculator_btn_num_17,
				R.drawable.calculator_btn_num_18,
				R.drawable.calculator_btn_num_19,
				R.drawable.calculator_btn_num_20,
				};
		
		mButtonBitmap = new Bitmap[mViewPortID.length];
		
		for (int n = 0; n < mButtonBitmap.length; n++){
			is = getResources().openRawResource(demoImage[n]);
			mButtonBitmap[n] = BitmapFactory.decodeStream(is);
		}						
				
		is = getResources().openRawResource(R.drawable.calculator_btn_press);
		mPressDiffuseMapBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.calculator_diffuse);
		mPannelDiffuseMapBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.calculator_normals);
		mPannelNormalMapBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.calculator_weight);
		mPannelSpecularMapBitmap = BitmapFactory.decodeStream(is);
							
		
	}
	
	
	//2012-12-26 masterkeaton27@gmail.com
	//TO-SEL-INDIA : following steps are very important.
	public void buildLayoutForPortrait(){
		if (mLayout != null){
			mLayout.removeAllViews();
			mLayout.removeAllViewsInLayout();
		}
		//release previous bitmaps and load new 
		loadBitmapsForPortraits();
		
		//get layout view from xml
		mLayoutPortraits = (AbsoluteLayout)buildViews_Portraits(this);
			
		//build legacy views & inflate all.
		//setContentView(view) of activity called here (all child views are actually inflated here) 
		buildViews(mLayoutPortraits);
			
		//get GUI information from layout
		buildViewBasedOnXML_Portraits();
	}
		
	//2012-12-26 masterkeaton27@gmail.com
		//TO-SEL-INDIA : following steps are very important.
	public void buildLayoutForLandScape(){
		if (mLayout != null){
			mLayout.removeAllViews();
			mLayout.removeAllViewsInLayout();
		}
		
		//release previous bitmaps and load new 
		loadBitmapsForLandscape();
		
		//get layout view from xml
		mLayoutLandScape = (AbsoluteLayout)buildViews_Landscape(this);
		
		//build legacy views & inflate all.
		//setContentView(view) of activity called here (all child views are actually inflated here)
		buildViews(mLayoutLandScape);
		
		//get GUI information from layout
		buildViewBasedOnXML_Landscape();
	}		
	
	//2012-12-26 masterkeaton27@gmail.com
	//TO-SEL-INDIA : because of GC, the listener should be form of member or static. 
	private OnGlobalLayoutListener mListenerPortraits = new OnGlobalLayoutListener(){
		@Override
	  	public void onGlobalLayout() {		  		
			//TO-SEL-INDIA : build actual effect layout from virtually inflated views.
			buildLayoutsPortraits(mLayoutPortraits, R.id.calculator_pannel_portraits, R.id.calculator_handle_portraits, mViewPortID);	  		
	  		
			//I need this call-back only once when orientation changed, so need to remove from observer.
			ViewTreeObserver vto = mLayout.getViewTreeObserver();  		
	  		vto.removeGlobalOnLayoutListener(mListenerPortraits);
	  			  		
	  		//remove virtual-layout from parent layout.
	  		mLayout.removeView(mLayoutPortraits);
	  	}
	};
		
	//2012-12-26 masterkeaton27@gmail.com
	//TO-SEL-INDIA : because of GC, the listener should be form of member or static.
	private OnGlobalLayoutListener mListenerLandscape = new OnGlobalLayoutListener(){
		@Override
	  	public void onGlobalLayout() {		  		
			//TO-SEL-INDIA : build actual effect layout from virtually inflated views.
	  		buildLayoutsLandscape(mLayoutLandScape, R.id.calculator_pannel_landscape, R.id.calculator_handle_landscape, mViewLandID);
	  		
	  		//I need this call-back only once when orientation changed, so need to remove from observer.
	  		ViewTreeObserver vto = mLayout.getViewTreeObserver();
	  		vto.removeGlobalOnLayoutListener(mListenerLandscape);
	  		
	  		//remove virtual-layout from parent layout.
	  		mLayout.removeView(mLayoutLandScape);
	  	}
	};
		
	//2012-12-26 masterkeaton27@gmail.com
	//TO-SEL-INDIA : add listener to know when inflation done.
	public void buildViewBasedOnXML_Landscape(){		
		if(mLayoutLandScape == null)
			return;			
		ViewTreeObserver vto = mLayout.getViewTreeObserver();
		vto.addOnGlobalLayoutListener(mListenerLandscape);
	}
		
	//2012-12-26 masterkeaton27@gmail.com
	//TO-SEL-INDIA : add listener to know when inflation done.
	public void buildViewBasedOnXML_Portraits(){		
	  	if(mLayoutPortraits == null)
	  		return;		 	    	  	
		ViewTreeObserver vto = mLayout.getViewTreeObserver();
		vto.addOnGlobalLayoutListener(mListenerPortraits);	    	    
	}
		
		
	//2012-12-26 masterkeaton27@gmail.com
	//TO-SEL-INDIA :
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
		//2012-12-26 masterkeaton27@gmail.com
		//TO-SEL-INDIA : Re-Build new layout with given orientation information
		buildLayoutBasedOnConfiguration(newConfig);		
	}
			
	//2012-12-26 masterkeaton27@gmail.com	
	public View buildViews_Portraits(Context context){		
		//Create Layout use LayoutInflater with Layout ID
		LayoutInflater factory = LayoutInflater.from(context);
		View layoutView = factory.inflate(R.layout.calculator_layout_portraits, null);
		return layoutView;
	}	
		
	//2012-12-26 masterkeaton27@gmail.com
	public View buildViews_Landscape(Context context){	
		//Create Layout use LayoutInflater with Layout ID
		LayoutInflater factory = LayoutInflater.from(context);
		View layoutView = factory.inflate(R.layout.calculator_layout_landscape, null);
		return layoutView;
	}	
		
	//2012-12-26 masterkeaton27@gmail.com
	//TO-SEL-INDIA : Build GUIS from layout 
	public void buildLayoutsPortraits(View layout, int pannelViewID, int handleViewID, int[] viewsID){
		if (viewsID.length != mButtonBitmap.length){
			return;
		}
		
		//clear all attached nodes.
		mCalculatorEffectView.clearNodes();
		
		//add back ground pannel
		View pannelViewFromXML = (View)layout.findViewById(pannelViewID);
		if (pannelViewFromXML != null){
			mCalculatorEffectView.addPannel(
					pannelViewFromXML.getPaddingLeft(),
					pannelViewFromXML.getPaddingTop(),
					pannelViewFromXML.getWidth(),
					pannelViewFromXML.getHeight(), 				
					mPannelDiffuseMapBitmap, 
					mPannelNormalMapBitmap,
					mPannelSpecularMapBitmap, 				
					-1.0f);
			mHideDistance = pannelViewFromXML.getHeight();			
		}
		
		//add handle pannel (only in landscape mode, view will be null when portraits mode)
		View handleViewFromXML = (View)layout.findViewById(handleViewID);
		if (handleViewFromXML != null){
			mHideDistance -= handleViewFromXML.getHeight();
						
			//2012-12-28 masterkeaton27@gmail.com
			//TO-SEL-INDIA : should store handler information to check hit.
			mHandlerHideRect.mOrigin.mX = handleViewFromXML.getPaddingLeft();
			mHandlerHideRect.mOrigin.mY = handleViewFromXML.getPaddingTop();
			mHandlerHideRect.mSize.mWidth = handleViewFromXML.getWidth();
			mHandlerHideRect.mSize.mHeight = handleViewFromXML.getHeight();
			
			mHandlerShowRect.mOrigin.mX = handleViewFromXML.getPaddingLeft();
			mHandlerShowRect.mOrigin.mY = 
					(pannelViewFromXML.getPaddingTop() + 
					pannelViewFromXML.getHeight()) - 
					handleViewFromXML.getHeight();						
			mHandlerShowRect.mSize.mWidth = handleViewFromXML.getWidth();
			mHandlerShowRect.mSize.mHeight = handleViewFromXML.getHeight();
		}
		
		//add buttons
		for(int i = 0 ; i < viewsID.length; i++){			
			View buttonViewFromXML = (View)layout.findViewById(viewsID[i]);
			if (buttonViewFromXML != null){
				CalculatorButton btn = new CalculatorButton();
				btn.setIndex(i);
				btn.setContentGUI(mButtonBitmap[i]);
				btn.setColorGUI(mPressDiffuseMapBitmap);
				btn.setRegion(
						buttonViewFromXML.getPaddingLeft(), 
						buttonViewFromXML.getPaddingTop(), 
						buttonViewFromXML.getWidth(), 
						buttonViewFromXML.getHeight());
				
				mCalculatorEffectView.addNode(btn);
			}
		}
		
		mCalculatorEffectView.validateOnSensorChange(new float[]{-3,3,7}, 0);
	}
	
	//2012-12-26 masterkeaton27@gmail.com
	//TO-SEL-INDIA : Build GUIS from layout 
	public void buildLayoutsLandscape(View layout, int pannelViewID, int handleViewID, int[] viewsID){
		if (viewsID.length != mButtonBitmapLand.length){
			return;
		}
		
		//clear all attached nodes.
		mCalculatorEffectView.clearNodes();
		
		//add back ground pannel
		View pannelViewFromXML = (View)layout.findViewById(pannelViewID);
		if (pannelViewFromXML != null){
			mCalculatorEffectView.addPannel(
					pannelViewFromXML.getPaddingLeft(),
					pannelViewFromXML.getPaddingTop(),
					pannelViewFromXML.getWidth(),
					pannelViewFromXML.getHeight(), 				
					mPannelDiffuseMapBitmapLand, 
					mPannelNormalMapBitmapLand,
					mPannelSpecularMapBitmapLand, 				
					-1.0f);
			mHideDistance = pannelViewFromXML.getHeight();			
		}
		
			//add handle pannel (only in landscape mode, view will be null when portraits mode)
		View handleViewFromXML = (View)layout.findViewById(handleViewID);
		if (handleViewFromXML != null){
						
			//2012-12-28 masterkeaton27@gmail.com
			//only landscape has gui for handle.
			mCalculatorEffectView.addPannel(
				handleViewFromXML.getPaddingLeft(),
				handleViewFromXML.getPaddingTop(),
				handleViewFromXML.getWidth(),
				handleViewFromXML.getHeight(), 				
				mHandleDiffuseMapBitmap, 
				mHandleNormalMapBitmap,
				mHandleSpecularMapBitmap, 				
				-1.0f);
			
			
				
			//2012-12-28 masterkeaton27@gmail.com
			//TO-SEL-INDIA : should store handler information to check hit.
			mHandlerHideRect.mOrigin.mX = handleViewFromXML.getPaddingLeft();
			mHandlerHideRect.mOrigin.mY = handleViewFromXML.getPaddingTop();
			mHandlerHideRect.mSize.mWidth = handleViewFromXML.getWidth();
			mHandlerHideRect.mSize.mHeight = handleViewFromXML.getHeight();
			
			mHandlerShowRect.mOrigin.mX = handleViewFromXML.getPaddingLeft();
			mHandlerShowRect.mOrigin.mY = 
					(pannelViewFromXML.getPaddingTop() + 
					pannelViewFromXML.getHeight()) - 
					handleViewFromXML.getHeight();						
			mHandlerShowRect.mSize.mWidth = handleViewFromXML.getWidth();
			mHandlerShowRect.mSize.mHeight = handleViewFromXML.getHeight();
		}
			
		//add buttons
		for(int i = 0 ; i < viewsID.length; i++){			
			View buttonViewFromXML = (View)layout.findViewById(viewsID[i]);
			if (buttonViewFromXML != null){
				CalculatorButton btn = new CalculatorButton();
				btn.setIndex(i);
				btn.setContentGUI(mButtonBitmapLand[i]);
				btn.setColorGUI(mPressDiffuseMapBitmapLand);
				btn.setRegion(
						buttonViewFromXML.getPaddingLeft(), 
						buttonViewFromXML.getPaddingTop(), 
						buttonViewFromXML.getWidth(), 
						buttonViewFromXML.getHeight());
				
				mCalculatorEffectView.addNode(btn);
			}
		}
		
		mCalculatorEffectView.validateOnSensorChange(new float[]{-3,3,7}, 0);
	}
		
	
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
//	    mSensorManager.unregisterListener(this, gravitymeter);
	    
	    //2012-12-04 masterkeaton27@gmail.com
	  	//NOTE GRAPHICS: 
	    //DON'T FORGET THIS!!!!
	    if( mCalculatorEffectView != null )
	    	mCalculatorEffectView.onPause();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
//	    mSensorManager.registerListener(this, gravitymeter, SensorManager.SENSOR_DELAY_GAME);
	
	    //2012-12-04 masterkeaton27@gmail.com
	  	//NOTE GRAPHICS: 
	    //DON'T FORGET THIS!!!!
	    if( mCalculatorEffectView != null )
	    	mCalculatorEffectView.onResume();
	}
	
	//2013-01-09 masterkeaton27@gmail.com
	@Override
	public void onBackPressed(){
		super.onBackPressed();
		
		mCalculatorEffectView.onPause();
		
		if (mLayout != null){
			mLayout.removeAllViews();
			mLayout.removeAllViewsInLayout();			
		}
		
		
	}
	
	public SVIEffectCalculator createEffectView() {
		// TODO Auto-generated method stub
				
		if (mCalculatorEffectView == null){
			mCalculatorEffectView = new SVIEffectCalculator(this);		
			mCalculatorEffectView.setListener(this);			
		}else{
			mCalculatorEffectView.finalize();
		}		
		mCalculatorEffectView.initialize();
		
		return mCalculatorEffectView;
	}
	
	
	
	@Override
	public boolean onCreateOptionsMenu (Menu menu) {				
		
		menu.add(0, 1, 0, "Show Pannel");
		menu.add(0, 2, 0, "Hide Pannel");

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
		case 1:
			mCalculatorEffectView.animateShow(500);
			return true;
			
		case 2:
			mCalculatorEffectView.animateHide(500, mHideDistance);
			mView.setVisibility(View.INVISIBLE);
			return true;
			
		case 101: mCalculatorEffectView.setRenderFPS(5); bFPS = true; break;
		case 102: mCalculatorEffectView.setRenderFPS(10); bFPS = true; break;
		case 103: mCalculatorEffectView.setRenderFPS(15); bFPS = true; break;
		case 104: mCalculatorEffectView.setRenderFPS(20); bFPS = true; break;
		case 105: mCalculatorEffectView.setRenderFPS(25); bFPS = true; break;
		case 106: mCalculatorEffectView.setRenderFPS(30); bFPS = true; break;
		case 107: mCalculatorEffectView.setRenderFPS(35); bFPS = true; break;
		case 108: mCalculatorEffectView.setRenderFPS(40); bFPS = true; break;
		case 109: mCalculatorEffectView.setRenderFPS(45); bFPS = true; break;
		case 110: mCalculatorEffectView.setRenderFPS(50); bFPS = true; break;
		case 111: mCalculatorEffectView.setRenderFPS(55); bFPS = true; break;
		case 112: mCalculatorEffectView.setRenderFPS(60); bFPS = true; break;

		/*
		case 201: mCalculatorEffectView.setAnimationSpeed(0.1f); bAnimationDuration = true; break;
		case 202: mCalculatorEffectView.setAnimationSpeed(0.2f); bAnimationDuration = true; break;
		case 203: mCalculatorEffectView.setAnimationSpeed(0.3f); bAnimationDuration = true; break;
		case 204: mCalculatorEffectView.setAnimationSpeed(0.4f); bAnimationDuration = true; break;
		case 205: mCalculatorEffectView.setAnimationSpeed(0.5f); bAnimationDuration = true; break;
		case 206: mCalculatorEffectView.setAnimationSpeed(0.6f); bAnimationDuration = true; break;
		case 207: mCalculatorEffectView.setAnimationSpeed(0.7f); bAnimationDuration = true; break;
		case 208: mCalculatorEffectView.setAnimationSpeed(0.8f); bAnimationDuration = true; break;
		case 209: mCalculatorEffectView.setAnimationSpeed(0.9f); bAnimationDuration = true; break;
		case 210: mCalculatorEffectView.setAnimationSpeed(1.0f); bAnimationDuration = true; break;
		case 211: mCalculatorEffectView.setAnimationSpeed(2.0f); bAnimationDuration = true; break;
		case 212: mCalculatorEffectView.setAnimationSpeed(3.0f); bAnimationDuration = true; break;
		case 213: mCalculatorEffectView.setAnimationSpeed(4.0f); bAnimationDuration = true; break;
		case 214: mCalculatorEffectView.setAnimationSpeed(5.0f); bAnimationDuration = true; break;
		case 215: mCalculatorEffectView.setAnimationSpeed(6.0f); bAnimationDuration = true; break;
		case 216: mCalculatorEffectView.setAnimationSpeed(7.0f); bAnimationDuration = true; break;
		case 217: mCalculatorEffectView.setAnimationSpeed(8.0f); bAnimationDuration = true; break;
		case 218: mCalculatorEffectView.setAnimationSpeed(9.0f); bAnimationDuration = true; break;
		case 219: mCalculatorEffectView.setAnimationSpeed(10.0f); bAnimationDuration = true; break;
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

/*	@Override
	public void onAccuracyChanged(Sensor arg0, int arg1) {
		// TODO Auto-generated method stub		
	}*/

	
/*	@Override
	public void onSensorChanged(SensorEvent event) {		
		if (event.sensor.getType() == Sensor.TYPE_GRAVITY) {	       	        
	        float fDistance = 
					(event.values[0] - mGravity[0]) * (event.values[0] - mGravity[0]) +
					(event.values[1] - mGravity[1]) * (event.values[1] - mGravity[1]) +
					(event.values[2] - mGravity[2]) * (event.values[2] - mGravity[2]);
			
			if (Math.sqrt(fDistance) > 0.5f){
				mGravity[0] = event.values[0];
				mGravity[1] = event.values[1];
				mGravity[2] = event.values[2];
				
				
				//2012-12-04 masterkeaton27@gmail.com
			  	//NOTE GRAPHICS: 
			    //send gravity sensor data to sgi effect view for calculating lights and so on.
				if( mCalculatorEffectView != null ){
					mCalculatorEffectView.validateOnSensorChange(new float[]{-mGravity[0],mGravity[1],mGravity[2]}, 0);
				}
			}			
	        return;
	    }			    
	    
	}*/


	@Override
	public void onFirstRenderFinished() {
		
	}


	@Override
	public void onAnimateHideFinished() {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onAnimateShowFinished() {
		// TODO Auto-generated method stub
		mView.setVisibility(View.VISIBLE);
		mHistoryView.setVisibility(View.INVISIBLE);
	}

}
