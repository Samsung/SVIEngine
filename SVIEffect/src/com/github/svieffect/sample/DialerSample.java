/**
 * DialerSample.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.sample;

import java.io.InputStream;
import java.util.Random;

import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.util.TypedValue;
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
import android.widget.AbsoluteLayout.LayoutParams;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;
import android.widget.TextView;
import android.widget.Toast;

import com.github.svieffect.R;
import com.github.svieffect.dialer.SVIEffectDialer;
import com.github.svieffect.main.SVIEffectListener;
import com.github.svieffect.main.SVIEffectNode;
import com.github.svieffect.main.SVIEffectView;


//2012-12-04 masterkeaton27@gmail.com
//NOTE GRAPHICS: 
//should implements from SGIEffectListener interface for to check first render finished.
public class DialerSample extends Activity implements  SVIEffectListener{
	
	public static float RES_SCALE = 1.5f;
	public static SVIEffectDialer mDialerEffectView = null;
	
	//2012-12-13 phyuzion@gmail.com
  	//NOTE GRAPHICS: 
    //dial Button Resource ID
	private int[] mViewLandID ={
			R.id.no_1_land,
			R.id.no_2_land,
			R.id.no_3_land,
			R.id.no_4_land,
			R.id.no_5_land,
			R.id.no_6_land,
			R.id.no_7_land,
			R.id.no_8_land,
			R.id.no_9_land,
			R.id.no_10_land,
			R.id.no_11_land,
			R.id.no_12_land,
			R.id.no_13_land,
			R.id.no_14_land,
			R.id.no_15_land
	};
	
	private int[] mViewPortID ={
			R.id.no_1_port,
			R.id.no_2_port,
			R.id.no_3_port,
			R.id.no_4_port,
			R.id.no_5_port,
			R.id.no_6_port,
			R.id.no_7_port,
			R.id.no_8_port,
			R.id.no_9_port,
			R.id.no_10_port,
			R.id.no_11_port,
			R.id.no_12_port,
			R.id.no_13_port,
			R.id.no_14_port,
			R.id.no_15_port
	};
	 
	
	//2012-12-04 masterkeaton27@gmail.com
  	//NOTE GRAPHICS: 
    //dial names for logging.
	private String	[] mButtonName = new String[] {
		"DIAL_01", 		// 0
		"DIAL_02", 		// 1
		"DIAL_03", 		// 2
		"DIAL_04", 		// 3
		"DIAL_05", 		// 4
		"DIAL_06", 		// 5
		"DIAL_07", 		// 6
		"DIAL_08", 		// 7
		"DIAL_09", 		// 8
		"DIAL_Star", 	// 9
		"DIAL_00",  	//11
		"DIAL_Sharp",   //12
		"DIAL_VideoCall", //13
		"DIAL_Call",      //14
		"DIAL_Back",      //15
	};
	
	private String	[] mButtonString = new String[] {
			"1",
			"2",
			"3",
			"4",
			"5",
			"6",
			"7",
			"8",
			"9",
			"*",
			"0",
			"#",
			"",
			"",
			"",
		};
	
	private Bitmap	[] mButtonBitmap = new Bitmap [15];	
	private Bitmap	mPressDiffuseMapBitmap = null;		
	private Bitmap	mBackgroundBitmap = null;		
	private Bitmap	mPannelDiffuseMapBitmap = null;
	private Bitmap	mPannelNormalMapBitmap = null;
	private Bitmap	mPannelSpecularMapBitmap = null;
	
	private Bitmap	[] mButtonBitmapLand = new Bitmap [15];
	private Bitmap	mPressDiffuseMapBitmapLand = null;		
	private Bitmap	mPannelDiffuseMapBitmapLand = null;
	private Bitmap	mPannelNormalMapBitmapLand = null;
	private Bitmap	mPannelSpecularMapBitmapLand = null;
	
	private Bitmap  mHoverOverlapBitmap = null;	
	private Bitmap  mFocusOverlapBitmap = null;	
	
	private Bitmap  mRippleEnvBitmap = null;
	
	private float	mWidth = 720.0f;
	private float	mHeight = 1280.0f;
	
	
	private FrameLayout mLayout = null;
	
	//2012-12-21 masterkeaton27@gmail.com
	private AbsoluteLayout mLayoutLandScape = null;	
	private AbsoluteLayout mLayoutPortraits = null;	
	
	//2012-12-21 masterkeaton27@gmail.com
	private DialerView mView = null; 	//to process event
	private TextView mTextView = null;	//dummy text view
		
	
	//2012-12-04 masterkeaton27@gmail.com
	//NOTE GRAPHICS: 
	//please remind that this temporary DialerView for touch event handling 
	public class DialerView extends ImageView implements OnLongClickListener, OnKeyListener, OnHoverListener {
		
		private int mLastIndexOfButton = -1;
		
		public DialerView(Context context) {
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
			if( mDialerEffectView != null ){				
				SVIEffectView.RequestType requestType = SVIEffectView.RequestType.REQUEST_CLICK_RESULT;
				mDialerEffectView.validateOnTouch(requestType, event);				
			}
			return super.onTouchEvent(event);
		}

		@Override
		public boolean onHover(View v, MotionEvent event) {
			//2012-12-21 masterkeaton27@gmail.com
			if( mDialerEffectView != null ){				
			//	mDialerEffectView.validateOnHover(event);
			}
			// TODO Auto-generated method stub
			return false;
		}

		@Override
		public boolean onKey(View v, int keyCode, KeyEvent event) {
			//2012-12-21 masterkeaton27@gmail.com
			SVIEffectView.RequestType requestType = SVIEffectView.RequestType.REQUEST_KEY_RESULT;
				
			// TODO Auto-generated method stub
			return false;
		}

		@Override
		public boolean onLongClick(View v) {
			//2012-12-21 masterkeaton27@gmail.com
			SVIEffectView.RequestType requestType = SVIEffectView.RequestType.REQUEST_LONGPRESS_RESULT;
						
			//mDialerEffectView.validateOnLongClick();		
			
			// TODO Auto-generated method stub
			return false;
		}		
	}
	
	
	Random mRandom = new Random();
	//2012-12-04 masterkeaton27@gmail.com
	//NOTE GRAPHICS:
	//SEE RELATED: buildLayouts()
	//dialer button class (node for handling re-action with given touch event)
	//please look into class SGIEffectNode with it's methods.	
	public class DialerButton extends SVIEffectNode{
		DialerButton() {
			super();
		}

		@Override
		public boolean onClick(int idx, int type, MotionEvent inputEvent) {
			//2012-12-04 masterkeaton27@gmail.com
			//TODO 
			
			CharSequence str = mTextView.getText();
			
			boolean isUp = inputEvent.getActionMasked()==MotionEvent.ACTION_UP
				    || inputEvent.getActionMasked()==MotionEvent.ACTION_POINTER_UP;
			
			boolean isDown = inputEvent.getActionMasked()==MotionEvent.ACTION_DOWN
				    || inputEvent.getActionMasked()==MotionEvent.ACTION_POINTER_DOWN;
			
			boolean isMove = inputEvent.getActionMasked()==MotionEvent.ACTION_MOVE;
					
			boolean isPointer  = inputEvent.getPointerId((inputEvent.getActionIndex()& MotionEvent.ACTION_MASK)) == idx;
			
			
			if (isMove && isPointer){	
				if (mRandom.nextInt(3) == 1){
					float radius = 20.0f;
					float nextX = (mRandom.nextFloat() - 0.5f) * 2.0f * radius;
					float nextY = (mRandom.nextFloat() - 0.5f) * 2.0f * radius;
					mDialerEffectView.downRipple(nextX, nextY);
				}
			}
			
			if (isDown && isPointer){			
								
				//mDialerEffectView.onFocusedEnter(getIndex());
				
				mDialerEffectView.downRipple(inputEvent.getX(idx), inputEvent.getY(idx));
											
								
				Log.i("Dialer Sample", "Button<"+getIndex()+"> (" + mButtonName[getIndex()] + ") clicked.");
				
				if (getIndex() == 14){					
					if (str.length() >= 1){
						str = str.subSequence(0, str.length()-1);
						mTextView.setText(str);
					}
				}else {
					if (str.length() % 4 == 0 && str.length() > 3){
						mTextView.append("-");
					}
					mTextView.append(mButtonString[getIndex()]);
				}
				
				if (str.length() < 12){
					mTextView.setTextSize(TypedValue.COMPLEX_UNIT_SP, 60);
				}else if (str.length() < 30){
					mTextView.setTextSize(TypedValue.COMPLEX_UNIT_SP, 45);
				}else if (str.length() < 50){
					mTextView.setTextSize(TypedValue.COMPLEX_UNIT_SP, 40);
				}				
			}
			
			if (isUp && isPointer){
				//mDialerEffectView.onFocusedExit(getIndex());				
				
			}
			
			return false;
		}

		@Override
		public boolean onLongPress(int idx, int type, MotionEvent inputEvent) {
			//2012-12-04 masterkeaton27@gmail.com
			Log.i("Dialer Sample", "Button<"+idx+"> (" + mButtonName[idx] + ") long pressed.");
			return false;
		}

		@Override
		public boolean onHovering(int idx, int type, MotionEvent inputEvent) {
			//2012-12-04 masterkeaton27@gmail.com
			
			
			if (type == MotionEvent.ACTION_HOVER_ENTER){
				mDialerEffectView.showPopupView(null,null,null, getIndex());
				Log.i("Dialer Sample", "Button<"+idx+"> (" + mButtonName[idx] + ") hover - ENTER");
			}else if (type == MotionEvent.ACTION_HOVER_EXIT){
				mDialerEffectView.hidePopupView(getIndex());
				Log.i("Dialer Sample", "Button<"+idx+"> (" + mButtonName[idx] + ") hover - EXIT");
			}else if (type == MotionEvent.ACTION_HOVER_MOVE){
				Log.i("Dialer Sample", "Button<"+idx+"> (" + mButtonName[idx] + ") hover - MOVE");
				
			}
			
			return false;
		}
	}
	
	//2012-12-21 masterkeaton27@gmail.com
	public View buildViews_Portraits(Context context){		
		//2012-12-12 phyuzion@gmail.com
		//NOTE GRAPHICS: 
		//Create Layout use LayoutInflater with Layout ID
		LayoutInflater factory = LayoutInflater.from(context);
		View layoutView = factory.inflate(R.layout.dialer_layout_portraits, null);
			
		return layoutView;
	}	
	
	//2012-12-21 masterkeaton27@gmail.com
	public View buildViews_Landscape(Context context){	
		//2012-12-12 phyuzion@gmail.com
		//NOTE GRAPHICS: 
		//Create Layout use LayoutInflater with Layout ID
		LayoutInflater factory = LayoutInflater.from(context);
		View layoutView = factory.inflate(R.layout.dialer_layout_landscape, null);
		return layoutView;
	}	
	
	//2012-12-21 masterkeaton27@gmail.com
	//Build GUIS from layout 
	
	//2013-01-09 masterkeaton27@gmail.com
	//due to native crash (which releated with GL texture cycle)
	//build layout function seperated with both landscape and portraits.
	public void buildLayoutsPortraits(View layout, int pannelViewID, int[] viewsID){
		if (viewsID.length != mButtonBitmap.length){
			return;
		}
		
		//clear all attached nodes.
		mDialerEffectView.clearNodes();
		
		//add back ground pannel
		View pannelViewFromXML = (View)layout.findViewById(pannelViewID);
		if (pannelViewFromXML != null){
			float color = 230.0f / 255.0f;
			mDialerEffectView.addPannel(
					pannelViewFromXML.getPaddingLeft(),
					pannelViewFromXML.getPaddingTop(),
					pannelViewFromXML.getWidth(),
					pannelViewFromXML.getHeight(),
					color, color, color, 1.0f);
			/*
			mDialerEffectView.addPannel(
					pannelViewFromXML.getPaddingLeft(),
					pannelViewFromXML.getPaddingTop(),
					pannelViewFromXML.getWidth(),
					pannelViewFromXML.getHeight(), 				
					mPannelDiffuseMapBitmap, 
					null,//mPannelNormalMapBitmap,
					null,//mPannelSpecularMapBitmap, 	
					-1.0f);*/
		}	
		
		//add buttons
		for(int i = 0 ; i < viewsID.length; i++){			
			View buttonViewFromXML = (View)layout.findViewById(viewsID[i]);
			if (buttonViewFromXML != null){
				DialerButton btn = new DialerButton();
				btn.setIndex(i);
				btn.setContentGUI(mButtonBitmap[i]);
				btn.setColorGUI(mPressDiffuseMapBitmap);
				btn.setRegion(
						buttonViewFromXML.getPaddingLeft(), 
						buttonViewFromXML.getPaddingTop(), 
						buttonViewFromXML.getWidth(), 
						buttonViewFromXML.getHeight());
											
				
				if (i == 12 || i == 13){
					btn.setColor(mDialerEffectView.getDefaultGreen());
				}else if (i == 14){
					btn.setColor(mDialerEffectView.getDefaultBlue());
				}else{
					btn.setColor(mDialerEffectView.getDefaultDarkBlue());
				}
				
				mDialerEffectView.addNode(btn);
			}
		}
		
		mDialerEffectView.validateOnSensorChange(new float[]{-3,3,7}, 0);
	}
	
	//2013-01-09 masterkeaton27@gmail.com
	//due to native crash (which releated with GL texture cycle)
	//build layout function seperated with both landscape and portraits.
	public void buildLayoutsLandscape(View layout, int pannelViewID, int[] viewsID){
		if (viewsID.length != mButtonBitmapLand.length){
			return;
		}
		
		//clear all attached nodes.
		mDialerEffectView.clearNodes();
		
		//add back ground pannel
		View pannelViewFromXML = (View)layout.findViewById(pannelViewID);
		if (pannelViewFromXML != null){
			float color = 230.0f / 255.0f;
			mDialerEffectView.addPannel(
					pannelViewFromXML.getPaddingLeft(),
					pannelViewFromXML.getPaddingTop(),
					pannelViewFromXML.getWidth(),
					pannelViewFromXML.getHeight(),
					color, color, color, 1.0f);
			/*
			mDialerEffectView.addPannel(
					pannelViewFromXML.getPaddingLeft(),
					pannelViewFromXML.getPaddingTop(),
					pannelViewFromXML.getWidth(),
					pannelViewFromXML.getHeight(), 				
					mPannelDiffuseMapBitmapLand, 
					null,//mPannelNormalMapBitmapLand,
					null,//mPannelSpecularMapBitmapLand, 				
					-1.0f);
					*/
		}	
		
		//add buttons
		for(int i = 0 ; i < viewsID.length; i++){			
			View buttonViewFromXML = (View)layout.findViewById(viewsID[i]);
			if (buttonViewFromXML != null){
				DialerButton btn = new DialerButton();
				btn.setIndex(i);
				btn.setContentGUI(mButtonBitmapLand[i]);
				btn.setColorGUI(mPressDiffuseMapBitmapLand);
				btn.setRegion(
						buttonViewFromXML.getPaddingLeft(), 
						buttonViewFromXML.getPaddingTop(), 
						buttonViewFromXML.getWidth(), 
						buttonViewFromXML.getHeight());
				
				if (i == 12 || i == 13){
					btn.setColor(mDialerEffectView.getDefaultGreen());
				}else if (i == 14){
					btn.setColor(mDialerEffectView.getDefaultBlue());
				}else{
					btn.setColor(mDialerEffectView.getDefaultDarkBlue());
				}
				
				
				mDialerEffectView.addNode(btn);
			}
		}
		
		mDialerEffectView.validateOnSensorChange(new float[]{-3,3,7}, 0);
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
		
	    
	    mWidth = getWindowManager().getDefaultDisplay().getWidth();
		mHeight= getWindowManager().getDefaultDisplay().getHeight();
		
		//2013-01-17 masterkeaton27@gmail.com
		loadCommonGUI();
		
		Configuration startConfig = getResources().getConfiguration();		
		buildLayoutBasedOnConfiguration(startConfig);
	}	
		
	
	//2012-12-04 masterkeaton27@gmail.com
	//NOTE GRAPHICS: 
	//DON'T FORGET THIS!!
	@Override 
	protected void onDestroy(){
		super.onDestroy();
		if (mDialerEffectView != null){
			mDialerEffectView.finalize();
			
		}		
		
		releaseBitmaps();	
	}
	
	//2012-12-21 masterkeaton27@gmail.com
	public void buildViews(AbsoluteLayout layout){
				
		mLayout = new FrameLayout(this);		
		
		//2012-12-04 masterkeaton27@gmail.com
		//NOTE GRAPHICS: 
		//Create Front Backgound View
		mView = new DialerView(this);
		mView.setImageBitmap(mBackgroundBitmap);
		mView.setScaleType(ScaleType.FIT_XY);
		
			
		//2012-12-05 masterkeaton27@gmail.com
		//NOTE GRAPHICS:
		//Key Input Test TextView		
		float scale = RES_SCALE;
		
		final FrameLayout.LayoutParams flp = new FrameLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		flp.setMargins((int)(10.0f * scale), (int)(200.0f * scale), 0, 0);
		mTextView = new TextView(this);
		mTextView.setText("");
		mTextView.setTextColor(Color.rgb(30, 30, 100));
		mTextView.setLayoutParams(flp); 		
		mTextView.setTextSize(TypedValue.COMPLEX_UNIT_SP, 60);
								
		//2012-12-04 masterkeaton27@gmail.com
		//NOTE GRAPHICS: 
		//Create Front Effect View and add to frame layout.
		
				
		//2012-12-04 masterkeaton27@gmail.com
		//NOTE GRAPHICS: 
		//add dialer image view to layout.
		mLayout.addView(mView); 		
				
		mLayout.addView(mTextView);				
		mLayout.addView(layout);
		
		mLayout.addView(createEffectView());
				
		mDialerEffectView.setTouchDispatcher(mLayout);
		
		setContentView(mLayout);
	}
		
	//2012-12-21 masterkeaton27@gmail.com
	public void loadBitmapsForLandscape(){				
		releaseLandscapeBitmaps();
				
		InputStream is;		
		int demoImage[] = {
            /*
			R.drawable.dialer_land_text_001,
			R.drawable.dialer_land_text_002,
			R.drawable.dialer_land_text_003,
			R.drawable.dialer_land_text_004,
			R.drawable.dialer_land_text_005,
			R.drawable.dialer_land_text_006,
			R.drawable.dialer_land_text_007,
			R.drawable.dialer_land_text_008,
			R.drawable.dialer_land_text_009,
			R.drawable.dialer_land_text_010,
			R.drawable.dialer_land_text_011,
			R.drawable.dialer_land_text_012,
			R.drawable.dialer_land_text_013,
			R.drawable.dialer_land_text_014,
			R.drawable.dialer_land_text_015
			*/
				R.drawable.text_icon_white_fx_01,
				R.drawable.text_icon_white_fx_02,
				R.drawable.text_icon_white_fx_03,
				R.drawable.text_icon_white_fx_04,
				R.drawable.text_icon_white_fx_05,
				R.drawable.text_icon_white_fx_06,
				R.drawable.text_icon_white_fx_07,
				R.drawable.text_icon_white_fx_08,
				R.drawable.text_icon_white_fx_09,
				R.drawable.text_icon_white_fx_10,
				R.drawable.text_icon_white_fx_11,
				R.drawable.text_icon_white_fx_12,
				R.drawable.text_icon_white_fx_13,
				R.drawable.text_icon_white_fx_14,
				R.drawable.text_icon_white_fx_15
		};
		
				
		for (int n = 0; n < 15; n++){
			is = getResources().openRawResource(demoImage[n]);
			mButtonBitmapLand[n] = BitmapFactory.decodeStream(is);
			
			
		}						
						
		is = getResources().openRawResource(R.drawable.dialer_land_press_left);
		mPressDiffuseMapBitmapLand = BitmapFactory.decodeStream(is);
								
		is = getResources().openRawResource(R.drawable.dialer_land_bg);
		mBackgroundBitmap = BitmapFactory.decodeStream(is);		
				
		/*
		is = getResources().openRawResource(R.drawable.dialer_land_normal);
		mPannelNormalMapBitmapLand = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.dialer_land_weight);
		mPannelSpecularMapBitmapLand = BitmapFactory.decodeStream(is);
		
		
		is = getResources().openRawResource(R.drawable.dialer_land_diffuse);
		mPannelDiffuseMapBitmapLand = BitmapFactory.decodeStream(is);
		*/
	}
	
	//2012-12-21 masterkeaton27@gmail.com
	public void loadBitmapsForPortraits(){
		
		long start = System.currentTimeMillis();
		
		releasePortraitsBitmaps();
		
		InputStream is;		
		int demoImage[] = {
				/*
				R.drawable.dialer_btn_text_001,
				R.drawable.dialer_btn_text_002,
				R.drawable.dialer_btn_text_003,
				R.drawable.dialer_btn_text_004,
				R.drawable.dialer_btn_text_005,
				R.drawable.dialer_btn_text_006,
				R.drawable.dialer_btn_text_007,
				R.drawable.dialer_btn_text_008,
				R.drawable.dialer_btn_text_009,
				R.drawable.dialer_btn_text_010,
				R.drawable.dialer_btn_text_011,
				R.drawable.dialer_btn_text_012,
				R.drawable.dialer_btn_text_013,
				R.drawable.dialer_btn_text_014,
				R.drawable.dialer_btn_text_015
				*/
				R.drawable.text_icon_white_fx_01,
				R.drawable.text_icon_white_fx_02,
				R.drawable.text_icon_white_fx_03,
				R.drawable.text_icon_white_fx_04,
				R.drawable.text_icon_white_fx_05,
				R.drawable.text_icon_white_fx_06,
				R.drawable.text_icon_white_fx_07,
				R.drawable.text_icon_white_fx_08,
				R.drawable.text_icon_white_fx_09,
				R.drawable.text_icon_white_fx_10,
				R.drawable.text_icon_white_fx_11,
				R.drawable.text_icon_white_fx_12,
				R.drawable.text_icon_white_fx_13,
				R.drawable.text_icon_white_fx_14,
				R.drawable.text_icon_white_fx_15
				};
		
		for (int n = 0; n < 15; n++){
			is = getResources().openRawResource(demoImage[n]);
			mButtonBitmap[n] = BitmapFactory.decodeStream(is);
		}						
				
		is = getResources().openRawResource(R.drawable.dialer_btn_press_org);
		mPressDiffuseMapBitmap = BitmapFactory.decodeStream(is);
				
		is = getResources().openRawResource(R.drawable.dialer_bg_background);
		mBackgroundBitmap = BitmapFactory.decodeStream(is);
		
		/*
		is = getResources().openRawResource(R.drawable.dialer_specular_mid);
		mPannelSpecularMapBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.dialer_pannel_normal);
		mPannelNormalMapBitmap = BitmapFactory.decodeStream(is);
		
		is = getResources().openRawResource(R.drawable.dialer_pannel_diffuse);
		mPannelDiffuseMapBitmap = BitmapFactory.decodeStream(is);		
		*/
		long end = System.currentTimeMillis();
		
		long duration = end - start;
		
		Log.e("new-dialer","bitmap decorde:"+duration+"ms");
	}
	
	public void releaseBitmaps(){
		releasePortraitsBitmaps();
		releaseLandscapeBitmaps();
		
		//2013-01-17 masterkeaton27@gmail.com
		releaseCommonGUI();
	}
	
	public void releasePortraitsBitmaps(){
		for (int n = 0; n < 15; n++){
			if (mButtonBitmap[n] != null){
				mButtonBitmap[n].recycle();
				mButtonBitmap[n] = null;
			}
		}
		
		if (mBackgroundBitmap != null){
			mBackgroundBitmap.recycle();
			mBackgroundBitmap = null;
		}
		
		if (mPressDiffuseMapBitmap != null){
			mPressDiffuseMapBitmap.recycle();
			mPressDiffuseMapBitmap = null;
		}
		
		if (mPannelSpecularMapBitmap != null){
			mPannelSpecularMapBitmap.recycle();
			mPannelSpecularMapBitmap = null;
		}
		
		if (mPannelDiffuseMapBitmap != null){
			mPannelDiffuseMapBitmap.recycle();
			mPannelDiffuseMapBitmap = null;
		}
		
		if (mPannelNormalMapBitmap != null){
			mPannelNormalMapBitmap.recycle();
			mPannelNormalMapBitmap = null;
		}		
		
		if (mHoverOverlapBitmap != null){
			mHoverOverlapBitmap.recycle();
			mHoverOverlapBitmap = null;
		}		
	}
	
	public void releaseLandscapeBitmaps(){
		for (int n = 0; n < 15; n++){
			if (mButtonBitmapLand[n] != null){
				mButtonBitmapLand[n].recycle();
				mButtonBitmapLand[n] = null;
			}
		}
		
		if (mBackgroundBitmap != null){
			mBackgroundBitmap.recycle();
			mBackgroundBitmap = null;
		}
		
		if (mPressDiffuseMapBitmapLand != null){
			mPressDiffuseMapBitmapLand.recycle();
			mPressDiffuseMapBitmapLand = null;
		}
		
		if (mPannelSpecularMapBitmapLand != null){
			mPannelSpecularMapBitmapLand.recycle();
			mPannelSpecularMapBitmapLand = null;
		}
		
		if (mPannelDiffuseMapBitmapLand != null){
			mPannelDiffuseMapBitmapLand.recycle();
			mPannelDiffuseMapBitmapLand = null;
		}
		
		if (mPannelNormalMapBitmapLand != null){
			mPannelNormalMapBitmapLand.recycle();
			mPannelNormalMapBitmapLand = null;
		}
	}
		
	//2012-12-21 masterkeaton27@gmail.com
	public void buildLayoutForPortrait(){
		if (mLayout != null){
			mLayout.removeAllViews();
			mLayout.removeAllViewsInLayout();
			//setContentView(null);
		}
		
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
		if (mLayout != null){
			mLayout.removeAllViews();
			mLayout.removeAllViewsInLayout();
			//setContentView(null);
		}
		
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
		@Override
	  	public void onGlobalLayout() {		  		
			buildLayoutsPortraits(mLayoutPortraits, R.id.dial_pannel_portraits, mViewPortID);	  		
	  		ViewTreeObserver vto = mLayout.getViewTreeObserver();
	  		vto.removeGlobalOnLayoutListener(mListenerPortraits);
	  		//mLayout.removeView(mLayoutPortraits);
	  	}
	};
	
	//2012-12-21 masterkeaton27@gmail.com
	private OnGlobalLayoutListener mListenerLandscape = new OnGlobalLayoutListener(){
		@Override
	  	public void onGlobalLayout() {		  		
			buildLayoutsLandscape(mLayoutLandScape, R.id.dial_pannel_landscape, mViewLandID);	  
	  		ViewTreeObserver vto = mLayout.getViewTreeObserver();
	  		vto.removeGlobalOnLayoutListener(mListenerLandscape);
	  		//mLayout.removeView(mLayoutLandScape);
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
	    if( mDialerEffectView != null )
	    	mDialerEffectView.onPause();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	  
	
	    //2012-12-04 masterkeaton27@gmail.com
	  	//NOTE GRAPHICS: 
	    //DON'T FORGET THIS!!!!
	    if( mDialerEffectView != null )
	    	mDialerEffectView.onResume();
	}

	
	public SVIEffectDialer createEffectView() {
		// TODO Auto-generated method stub
		
		//2012-12-04 masterkeaton27@gmail.com
	  	//NOTE GRAPHICS: 
	    //Create effect view, set listener for check loaded.
		if (mDialerEffectView == null){
			mDialerEffectView = new SVIEffectDialer(this);		
						
			//2013-01-17 masterkeaton27@gmail.com
			mDialerEffectView.setHoverOverlapGUI(mHoverOverlapBitmap);
			mDialerEffectView.setFocusOverlapGUI(mFocusOverlapBitmap);
		}else{
			mDialerEffectView.finalize();
		}		
		
		mDialerEffectView.setListener(this);
		mDialerEffectView.setRippleEnvImage(mRippleEnvBitmap);
		mDialerEffectView.initialize();
		
		
		return mDialerEffectView;
	}
		
	
	//2013-01-17 masterkeaton27@gmail.com
	public void loadCommonGUI(){
		InputStream is;		
		is = getResources().openRawResource(R.drawable.dialer_hover);
		mHoverOverlapBitmap = BitmapFactory.decodeStream(is);			
		is = getResources().openRawResource(R.drawable.dialer_focus);
		mFocusOverlapBitmap = BitmapFactory.decodeStream(is);
				
		//2013-02-08 masterkeaton27@gmail.com
		is = getResources().openRawResource(R.drawable.dialer_envmap);
		mRippleEnvBitmap = BitmapFactory.decodeStream(is);		
	}
	
	//2013-01-17 masterkeaton27@gmail.com
	public void releaseCommonGUI(){
		//2013-02-08 masterkeaton27@gmail.com
		if (mRippleEnvBitmap != null){
			mRippleEnvBitmap.recycle();
			mRippleEnvBitmap = null;
		}	
		
		if (mHoverOverlapBitmap != null){
			mHoverOverlapBitmap.recycle();
			mHoverOverlapBitmap = null;
		}
		
		if (mFocusOverlapBitmap != null){
			mFocusOverlapBitmap.recycle();
			mFocusOverlapBitmap = null;
		}
	}
	
	@Override
	public boolean onCreateOptionsMenu (Menu menu) {
		super.onCreateOptionsMenu(menu);

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

		switch(item.getItemId())
        {
		case 101: mDialerEffectView.setRenderFPS(5); bFPS = true; break;
		case 102: mDialerEffectView.setRenderFPS(10); bFPS = true; break;
		case 103: mDialerEffectView.setRenderFPS(15); bFPS = true; break;
		case 104: mDialerEffectView.setRenderFPS(20); bFPS = true; break;
		case 105: mDialerEffectView.setRenderFPS(25); bFPS = true; break;
		case 106: mDialerEffectView.setRenderFPS(30); bFPS = true; break;
		case 107: mDialerEffectView.setRenderFPS(35); bFPS = true; break;
		case 108: mDialerEffectView.setRenderFPS(40); bFPS = true; break;
		case 109: mDialerEffectView.setRenderFPS(45); bFPS = true; break;
		case 110: mDialerEffectView.setRenderFPS(50); bFPS = true; break;
		case 111: mDialerEffectView.setRenderFPS(55); bFPS = true; break;
		case 112: mDialerEffectView.setRenderFPS(60); bFPS = true; break;

/*
		case 201: mDialerEffectView.setAnimationSpeed(0.1f); bAnimationDuration = true; break;
		case 202: mDialerEffectView.setAnimationSpeed(0.2f); bAnimationDuration = true; break;
		case 203: mDialerEffectView.setAnimationSpeed(0.3f); bAnimationDuration = true; break;
		case 204: mDialerEffectView.setAnimationSpeed(0.4f); bAnimationDuration = true; break;
		case 205: mDialerEffectView.setAnimationSpeed(0.5f); bAnimationDuration = true; break;
		case 206: mDialerEffectView.setAnimationSpeed(0.6f); bAnimationDuration = true; break;
		case 207: mDialerEffectView.setAnimationSpeed(0.7f); bAnimationDuration = true; break;
		case 208: mDialerEffectView.setAnimationSpeed(0.8f); bAnimationDuration = true; break;
		case 209: mDialerEffectView.setAnimationSpeed(0.9f); bAnimationDuration = true; break;
		case 210: mDialerEffectView.setAnimationSpeed(1.0f); bAnimationDuration = true; break;
		case 211: mDialerEffectView.setAnimationSpeed(2.0f); bAnimationDuration = true; break;
		case 212: mDialerEffectView.setAnimationSpeed(3.0f); bAnimationDuration = true; break;
		case 213: mDialerEffectView.setAnimationSpeed(4.0f); bAnimationDuration = true; break;
		case 214: mDialerEffectView.setAnimationSpeed(5.0f); bAnimationDuration = true; break;
		case 215: mDialerEffectView.setAnimationSpeed(6.0f); bAnimationDuration = true; break;
		case 216: mDialerEffectView.setAnimationSpeed(7.0f); bAnimationDuration = true; break;
		case 217: mDialerEffectView.setAnimationSpeed(8.0f); bAnimationDuration = true; break;
		case 218: mDialerEffectView.setAnimationSpeed(9.0f); bAnimationDuration = true; break;
		case 219: mDialerEffectView.setAnimationSpeed(10.0f); bAnimationDuration = true; break;
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
	public void onBackPressed(){
		super.onBackPressed();
		if (mLayout != null){
			mLayout.removeAllViews();
			mLayout.removeAllViewsInLayout();			
		}
	}	


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
		
	}
	
	
	
	//2013/01/25 rareboy0112@facebook.com
	//start support compressed texture
	//2013/01/23 rareboy0112@facebook.com
	//compressed texture buffer
	private byte[][] 	mButtonBuf = new byte[15][];	
	private byte[]		mPressDiffuseMapBuf = null;		
	private byte[]		mBackgroundBuf = null;		
	private byte[]		mPannelDiffuseMapBuf = null;
	private byte[]		mPannelNormalMapBuf = null;
	private byte[]		mPannelSpecularMapBuf = null;
	
	private byte[][] 	mButtonBufLand = new byte[15][];
	private byte[]		mPressDiffuseMapBufLand = null;		
	private byte[]		mPannelDiffuseMapBufLand = null;
	private byte[]		mPannelNormalMapBufpLand = null;
	private byte[]		mPannelSpecularMapBufLand = null;
	
	private byte[]  	mHoverOverlapBuf = null;	
	private byte[]  	mFocusOverlapBuf = null;
	
	public void buildLayoutsPortraits_compress(View layout, int pannelViewID, int[] viewsID){
		if (viewsID.length != mButtonBuf.length){
			return;
		}
		
		//clear all attached nodes.
		mDialerEffectView.clearNodes();
		
		//add back ground pannel
		View pannelViewFromXML = (View)layout.findViewById(pannelViewID);
		if (pannelViewFromXML != null){
			
			float color = 230.0f / 255.0f;
			mDialerEffectView.addPannel(
					pannelViewFromXML.getPaddingLeft(),
					pannelViewFromXML.getPaddingTop(),
					pannelViewFromXML.getWidth(),
					pannelViewFromXML.getHeight(),
					color, color, color, 1.0f);
			
				/*
			mDialerEffectView.addPannel(
					pannelViewFromXML.getPaddingLeft(),
					pannelViewFromXML.getPaddingTop(),
					pannelViewFromXML.getWidth(),
					pannelViewFromXML.getHeight(), 				
					mPannelDiffuseMapBitmap, 
					mPannelNormalMapBitmap,
					mPannelSpecularMapBitmap, 				
					-1.0f);
					*/
		}	
		
		//add buttons
		for(int i = 0 ; i < viewsID.length; i++){			
			View buttonViewFromXML = (View)layout.findViewById(viewsID[i]);
			if (buttonViewFromXML != null){
				DialerButton btn = new DialerButton();
				btn.setIndex(i);
				btn.setContentGUI(mButtonBitmap[i]);
				btn.setColorGUI(mPressDiffuseMapBitmap);
				btn.setRegion(
						buttonViewFromXML.getPaddingLeft(), 
						buttonViewFromXML.getPaddingTop(), 
						buttonViewFromXML.getWidth(), 
						buttonViewFromXML.getHeight());
				
				mDialerEffectView.addNode(btn);
			}
		}
		
		mDialerEffectView.validateOnSensorChange(new float[]{-3,3,7}, 0);
	}
	
	public void buildLayoutsLandscape__compress(View layout, int pannelViewID, int[] viewsID){
		if (viewsID.length != mButtonBitmapLand.length){
			return;
		}
		
		//clear all attached nodes.
		mDialerEffectView.clearNodes();
		
		//add back ground pannel
		View pannelViewFromXML = (View)layout.findViewById(pannelViewID);
		if (pannelViewFromXML != null){
			float color = 230.0f / 255.0f;
			mDialerEffectView.addPannel(
					pannelViewFromXML.getPaddingLeft(),
					pannelViewFromXML.getPaddingTop(),
					pannelViewFromXML.getWidth(),
					pannelViewFromXML.getHeight(),
					color, color, color, 1.0f);
			/*
			mDialerEffectView.addPannel(
					pannelViewFromXML.getPaddingLeft(),
					pannelViewFromXML.getPaddingTop(),
					pannelViewFromXML.getWidth(),
					pannelViewFromXML.getHeight(), 				
					mPannelDiffuseMapBitmapLand, 
					mPannelNormalMapBitmapLand,
					mPannelSpecularMapBitmapLand, 				
					-1.0f);
					*/
		}	
		
		//add buttons
		for(int i = 0 ; i < viewsID.length; i++){			
			View buttonViewFromXML = (View)layout.findViewById(viewsID[i]);
			if (buttonViewFromXML != null){
				DialerButton btn = new DialerButton();
				btn.setIndex(i);
				btn.setContentGUI(mButtonBitmapLand[i]);
				btn.setColorGUI(mPressDiffuseMapBitmapLand);
				btn.setRegion(
						buttonViewFromXML.getPaddingLeft(), 
						buttonViewFromXML.getPaddingTop(), 
						buttonViewFromXML.getWidth(), 
						buttonViewFromXML.getHeight());
				
				mDialerEffectView.addNode(btn);
			}
		}
		
		mDialerEffectView.validateOnSensorChange(new float[]{-3,3,7}, 0);
	}

}
