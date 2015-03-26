package com.github.sviengine.performance;

import android.content.res.Configuration;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.LinearLayout;
import android.widget.TextView;

public class AnimationListenerTestActivity extends TestCaseBaseActivity implements OnClickListener {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		
		super.onCreate(savedInstanceState);
		
		LinearLayout subLayout = new LinearLayout(this);
		
		subLayout.setGravity(Gravity.BOTTOM);
		
		subLayout.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT));
		
		
		prevText = new TextView(this);
		currentText = new TextView(this);
		nextText = new TextView(this);
		
		
		prevText.setText(menuArray[0]);
		currentText.setText(menuArray[1]);
		nextText.setText(menuArray[2]);
		
		prevText.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, 1f));
		currentText.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, 1f));
		nextText.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, 1f));
		
		
		String color = "#000000";
		prevText.setTextColor(Color.parseColor(color));
		nextText.setTextColor(Color.parseColor(color));
		
		prevText.setTextSize(sideTextSize);
		nextText.setTextSize(sideTextSize);
		
		currentText.setTextSize(centerTextSize);
		
		color = "#ff0000";
		currentText.setTextColor(Color.parseColor(color));
		
		prevText.setGravity(Gravity.BOTTOM|Gravity.LEFT);
		nextText.setGravity(Gravity.BOTTOM|Gravity.RIGHT);
		currentText.setGravity(Gravity.BOTTOM|Gravity.CENTER);
		
		prevText.setId(0);
		nextText.setId(1);
		currentText.setId(2);
			
		prevText.setOnClickListener(this);
		nextText.setOnClickListener(this);
		currentText.setOnClickListener(this);
		
		
		
		subLayout.addView(prevText);
		subLayout.addView(currentText);
		subLayout.addView(nextText);
		
		addContentView(subLayout, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT) );
		
		
		mCallbackStatusText = new TextView(this);
		
		color = "#0000ff";
		mCallbackStatusText.setTextColor(Color.parseColor(color));
		mCallbackStatusText.setText("NONE");
		mCallbackStatusText.setTextSize(60.0f);
		addContentView(mCallbackStatusText, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT));
		
		mOriginWindow.setCallbackStatusView(mCallbackStatusText);
	}
	
	@Override
	public void onClick(View v) {
		int temp = (int)v.getId();
		
		switch(temp){
		case 0:
			if(mIndex>1){
				mIndex--;
				nextText.setText(menuArray[mIndex+1]);
				currentText.setText(menuArray[mIndex]);
				prevText.setText(menuArray[mIndex-1]);
			}
			break;
			
		case 1:
			if(mIndex<menuArray.length-2){
				mIndex++;
				prevText.setText(menuArray[mIndex-1]);
				currentText.setText(menuArray[mIndex]);
				nextText.setText(menuArray[mIndex+1]);
			}
			break;
		case 2:
			break;
		}	
		Log.i("SVI", "index:"+mIndex);
		mOriginWindow.setTestCase(mIndex-1);
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
		
		mWindow = new AnimationListenerTestWindow(this);
		mOriginWindow = (AnimationListenerTestWindow)mWindow;
		if(mWindow == null)
			return false;
	
		return true;
	}
	
	
	@Override
	public boolean onCreateOptionsMenu (Menu menu) {
		menu.add("Increase Repeat Count");
		menu.add("Decrease Repeat Count");
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected (MenuItem item) {
		if( item.getTitle() == "Increase Repeat Count" ) {
			mOriginWindow.increaseRepeatCount();
		} else if( item.getTitle() == "Decrease Repeat Count" ) {
			mOriginWindow.decreaseRepeatCount();
		}
		return true;
	}
	
	protected TextView 			prevText;
	protected TextView 			currentText;
	protected TextView 			nextText;
	protected TextView			mCallbackStatusText;
	
	
	
	protected int					mIndex =1;
	
	final protected float 				sideTextSize = 30.0f;
	final protected float 				centerTextSize = 50.0f;
	
	
	protected String[] menuArray = {
			"==",
			"BasicAnimation", 		
			"KeyFrameAnimation",
			"TransitionAnimation",
			"SpriteAnimation",
			"AnimationSet",		
			"ImplicitAnimation",
			"=="
	};
	
	AnimationListenerTestWindow 					mOriginWindow = null;
}
