package com.github.sviengine.testcase;


import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.pm.ApplicationInfo;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.KeyEvent;
import android.widget.TextView;

public class TransitionAnimationActivity extends TestCaseBaseActivity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		
		super.onCreate(savedInstanceState);
		/*
		LinearLayout subLayout = new LinearLayout(this);
		
		subLayout.setGravity(Gravity.BOTTOM);
		
		subLayout.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT));
		
		
		mNameText = new TextView(this);

		mNameText.setText(menuArray[0]);
		
		mNameText.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, 1f));
		String color = "#ff0000";
		mNameText.setTextColor(Color.parseColor(color));
		
		mNameText.setTextSize(30.0f);

		subLayout.addView(mNameText);
		
		addContentView(subLayout, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT) );
		*/
		
		mDlg = new AlertDialog.Builder(this);
		mDlg.setTitle("Test Case");
		mDlg.setItems(menuArray, new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				mOriginWindow.setTestCase(which);
				//mNameText.setText(menuArray[which]);
				dialog.dismiss();
				mOriginWindow.startAni();
			}
		});
		
		
	}
	

	@Override
	protected void onDestroy() {
		mOriginWindow.release();
		mOriginWindow = null;
		super.onDestroy();
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
		
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		
	}

	
	

	@Override
	boolean createWindow() {
		mWindow = new TransitionAnimationWindow(this);
		mOriginWindow = (TransitionAnimationWindow)mWindow;
		if(mWindow == null)
			return false;
		return true;
	}
	
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		
		if(keyCode == KeyEvent.KEYCODE_MENU && event.getAction() == KeyEvent.ACTION_DOWN){
			mDlg.show();
		}
		return super.onKeyDown(keyCode, event);
	}
	
	
	
	private AlertDialog.Builder mDlg;
	private TextView mNameText;
	
	
	protected int mIndex =1;
	
	
	
	protected String[] menuArray = {
			"BLIND",
			"BREAK",
			"CURTAIN",
			"DOMINOFLIP",
			"SNAIL",
			"THROUGH",
			"TORNADO",
			"TUMBLE",
			"WIZZLE",
			"ROTATE",
			"ROTATE_CUBE",
			"SIDEWINDOW",
			"SLIDE",
			"WAVE",
			"ZOOM_IN",
			"EXPLOID",
			"FLIP",
			"SCALE",
			"SMOOTH",
			"TWIST",
			"BOX",
			"CHECKER_BOARD",
			"COVER",
			"FERRIS_WHEEL",
			"FLASH",
			"FLIP_2",
			"FLY_THROUGH",
			"GALLERY",
			"HONEYCOMB",
			"REVEAL",
			"SHAPE",
			"SHRED",
			"SPLIT",
			"SWITCH",
			"UNCOVER",
			"WIPE",
			"CENTER_BLIND",
			"FADE_THROUGH_COLOR",
			"FALL",
			"MOVE_IN",
			"REVOLVING_DOOR",
			"SWAP",
			"SWOOSH",
			"TWIRL",
			"BRICK_CUBE",
			"CUBE_2PIECES",
			"CUBE_4PIECES",
			"FOLDING_SCREEN",
			"GALLERY_2",
			"SPACE"
			
	};
	
	TransitionAnimationWindow 					mOriginWindow = null;

}
