package com.github.sviengine.unittest;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.res.Configuration;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.LinearLayout;
import android.widget.TextView;

public class FilterSlideActivity extends UnitTestBaseActivity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		LinearLayout subLayout = new LinearLayout(this);
		
		subLayout.setGravity(Gravity.TOP | Gravity.RIGHT);
		
		subLayout.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT));
		
		mNameText = new TextView(this);
		mNameText.setText("Sketch Filter");
		
		mNameText.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, 1f));
		String color = "#222222";
		mNameText.setTextColor(Color.parseColor(color));
		
		mNameText.setTextSize(20.0f);
		subLayout.addView(mNameText);
		
		addContentView(subLayout, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT) );
		
		mDlg = new AlertDialog.Builder(this);
		mDlg.setTitle("SVI GPU Filter Test");
		mDlg.setItems(menuArray, new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				((FilterSlideWindow)mWindow).setFilter(which);
				if (which == 0){
					
				}else if (which == 1){
					((FilterSlideWindow)mWindow).changeImage();
				}else mNameText.setText(menuArray[which]);
				dialog.dismiss();
			}
		});
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
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		
		if(keyCode == KeyEvent.KEYCODE_MENU && event.getAction() == KeyEvent.ACTION_DOWN){
			mDlg.show();
		}
		return super.onKeyDown(keyCode, event);
	}
	
	@Override
	public boolean createWindow() {
		// TODO Auto-generated method stub
		mWindow = new FilterSlideWindow(this);
		
		if( mWindow == null )
			return false;
		
		return true;
	}
	
	protected String[] menuArray = {
			"Reset",
			"Change Image",
			"Sketch Filter",
			"Oil Filter",
			"Water Filter"
	};
	
	private AlertDialog.Builder mDlg;
	private TextView mNameText;
	
	@Override
	public boolean onOptionsItemSelected (MenuItem item) {
		mWindow.onOptionsItemSelected(item);
		return true;
	}
}
