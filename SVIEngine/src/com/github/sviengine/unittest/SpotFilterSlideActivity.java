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

public class SpotFilterSlideActivity extends UnitTestBaseActivity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
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
	public boolean onCreateOptionsMenu (Menu menu) {
		menu.add("Change Image");
		menu.add("Inverse Color");
		menu.add("En/Disable Hover");
		return true;
	}
	
	@Override
	public boolean createWindow() {
		// TODO Auto-generated method stub
		mWindow = new SpotFilterSlideWindow(this);
		
		if( mWindow == null )
			return false;
		
		return true;
	}
	
	
	@Override
	public boolean onOptionsItemSelected (MenuItem item) {
		mWindow.onOptionsItemSelected(item);
		return true;
	}
}
