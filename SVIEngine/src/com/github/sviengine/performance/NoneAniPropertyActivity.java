package com.github.sviengine.performance;


import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
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
import android.widget.CheckBox;
import android.widget.LinearLayout;
import android.widget.TextView;

public class NoneAniPropertyActivity extends TestCaseBaseActivity implements OnClickListener{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		
		super.onCreate(savedInstanceState);		

		LinearLayout rootLayout = new LinearLayout(this);
		rootLayout.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT));
		rootLayout.setOrientation(LinearLayout.VERTICAL);
		
		LinearLayout subLayout = new LinearLayout(this);
		
		subLayout.setGravity(Gravity.BOTTOM);
		
		subLayout.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT));
		
		
		prevText = new TextView(this);
		currentText = new TextView(this);
		nextText = new TextView(this);
		mTextViewLog = new TextView(this);
		
		
		prevText.setText(menuArray[0]);
		currentText.setText(menuArray[1]);
		nextText.setText(menuArray[2]);
		mTextViewLog.setText("");
		
		prevText.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, 1f));
		currentText.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, 1f));
		nextText.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, 1f));
		mTextViewLog.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT, 1f));
		
		
		String sideColor = "#000000";
		prevText.setTextColor(Color.parseColor(sideColor));
		nextText.setTextColor(Color.parseColor(sideColor));
		mTextViewLog.setTextColor(Color.parseColor(sideColor));
		
		prevText.setTextSize(sideTextSize);
		nextText.setTextSize(sideTextSize);
		
		currentText.setTextSize(centerTextSize);
		mTextViewLog.setTextSize(sideTextSize);
		
		String centerColor = "#ff0000";
		currentText.setTextColor(Color.parseColor(centerColor));
		
		prevText.setGravity(Gravity.BOTTOM|Gravity.LEFT);
		nextText.setGravity(Gravity.BOTTOM|Gravity.RIGHT);
		currentText.setGravity(Gravity.BOTTOM|Gravity.CENTER);
		mTextViewLog.setGravity(Gravity.TOP|Gravity.RIGHT);
		
		prevText.setId(0);
		nextText.setId(1);
		currentText.setId(2);
			
		prevText.setOnClickListener(this);
		nextText.setOnClickListener(this);
		currentText.setOnClickListener(this);
		
		
		
		subLayout.addView(prevText);
		subLayout.addView(currentText);
		subLayout.addView(nextText);
		
		rootLayout.addView(mTextViewLog);
		rootLayout.addView(subLayout);
		
		addContentView(rootLayout, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT) );
		
		
		propertyText = new TextView(this);
		
		propertyText.setText("Explicit");
		propertyText.setTextSize(60.0f);
		addContentView(propertyText, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT));
		
		mCheckBoxList = new CheckBox[menuArray.length-2];		
		((NoneAniPropertyWindow)mWindow).setTextViewLog(mTextViewLog);
	}
	
	@Override
	public void onClick(View v) {
		int temp = (int)v.getId();
		
		originWindow.removeall();
		
		switch(temp){
		
		case 0:
			if(mIndex>0){
				nextText.setText(menuArray[mIndex+1]);
				currentText.setText(menuArray[mIndex]);
				prevText.setText(menuArray[mIndex-1]);
				mIndex--;
			}
			break;
			
		case 1:
			if(mIndex<menuArray.length-3){
				mIndex++;
				prevText.setText(menuArray[mIndex]);
				currentText.setText(menuArray[mIndex+1]);
				nextText.setText(menuArray[mIndex+2]);
			}
			break;
		case 2:
			Log.i("test",menuArray[mIndex+1]+"");
			break;
		default :
			Log.i("test",menuArray[mIndex+1]+"");
		}
		
		originWindow.mProperty = mIndex;
		
		originWindow.buildSubSlide();
		
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
	protected void onDestroy() {
		originWindow.release();
		originWindow = null;
		super.onDestroy();
	}
	

	@Override
	boolean createWindow() {
		
		mWindow = new NoneAniPropertyWindow(this);
		originWindow = (NoneAniPropertyWindow)mWindow;
		
		if(mWindow == null)
			return false;
		
		return true;
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// TODO Auto-generated method stub
		menu.add(0, COMBINATION, 0, "combination");
		return true;
	}

	@Override
	protected Dialog onCreateDialog(int id) {
		// TODO Auto-generated method stub
	    switch (id) {
	    case COMBINATION:
	    	LinearLayout linearLayout = new LinearLayout(this);
	    	linearLayout.setOrientation(LinearLayout.VERTICAL);
	    	for(int j=0; j<mCheckBoxList.length; j++) {
	    		mCheckBoxList[j] = new CheckBox(this);
	    		mCheckBoxList[j].setText(menuArray[j+1]);
	    		linearLayout.addView(mCheckBoxList[j]);
	    	}
	    	return new AlertDialog.Builder(this)
	    		.setTitle("Non-animatable property list")
	    		.setView(linearLayout)
	    		.setPositiveButton("OK", new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface arg0, int arg1) {
						// TODO Auto-generated method stub
						boolean[] checkedList = new boolean[menuArray.length-2];
				    	for(int i=0; i<checkedList.length; i++) {
				    		checkedList[i] = mCheckBoxList[i].isChecked();
				    	}
				    	((NoneAniPropertyWindow)mWindow).setCheckedList(checkedList);
				    	((NoneAniPropertyWindow)mWindow).setNonAnimatableProperties();
					}
	    			
	    		})
	    		.create();
	    	
	    	
	    default:
	    	return super.onCreateDialog(id);
	    }
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub
	    // Handle item selection
	    switch (item.getItemId()) {
	    case COMBINATION:
	    	showDialog(COMBINATION);
	        return true;
	    default:
	        return super.onOptionsItemSelected(item);
	    }
	}	
	
	
	
	protected TextView 			prevText;
	protected TextView 			currentText;
	protected TextView 			nextText;
	protected TextView			propertyText;
	protected TextView			mTextViewLog;
	
	
	
	protected int					mIndex =0;
	
	final protected float 				sideTextSize = 30.0f;
	final protected float 				centerTextSize = 50.0f;
	
	
	protected String[] menuArray = {
			"==",
			"CLIP_SUB_SLIDE", 									
			"ZORDER_TYPE",
			"ORTHOGONAL", 			
			"HOLD_OPACITY", 				
			"HOLD_SCALE",				
			"BLEND_TYPE", 				
			"LIGHT_TYPE", 				
			"IMAGESCALETYPE",
			"=="
	};
	
	NoneAniPropertyWindow 					originWindow = null;
	
	
	protected static final int 				COMBINATION = 0;	

	protected CheckBox[] 					mCheckBoxList;

}
