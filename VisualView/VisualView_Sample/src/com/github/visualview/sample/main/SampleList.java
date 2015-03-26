/*
 * Copyright (C) 2013 SVV.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.github.visualview.sample.main;


import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.github.visualview.sample.list.AnimationSetActivity;
import com.github.visualview.sample.list.BasicAnimationActivity;
import com.github.visualview.sample.list.EnableLogActivity;
import com.github.visualview.sample.list.ExplicitAnimationListenerActivity;
import com.github.visualview.sample.list.HelloVisualViewActivity;
import com.github.visualview.sample.list.ImageHandleActivity;
import com.github.visualview.sample.list.ImplicitAnimationListenerActivity;
import com.github.visualview.sample.list.ImplicitAnimation_One_Activity;
import com.github.visualview.sample.list.ImplicitAnimation_Two_Activity;
import com.github.visualview.sample.list.KeyframeAnimationActivity;
import com.github.visualview.sample.list.PropertySetActivity;
import com.github.visualview.sample.list.SpriteAnimationActivity;
import com.github.visualview.sample.list.TransitionAnimationActivity;

public class SampleList extends ListActivity {
    /** Called when the activity is first created. */
	public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        final String[] mList = {"Hello Visual View", 				//0
        						"Enable Log",						//1
        						"Property Set",						//2
        						"Image Handle",						//3
        						"Implicit Animation : Scenario 1",	//4
        						"Implicit Animation : Scenario 2",	//5
        						"Implicit Animation Listener",		//6
        						"Basic Animation",					//7
        						"Keyframe Animation",				//8
        						"Sprite Animation",					//9
        						"TransitionAnimation",				//10
        						"Animation Set",					//11
        						"Explicit Animation Listener"};		//12
        
        setListAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, mList));
        
        ListView list = getListView();
        
        list.setOnItemClickListener(new OnItemClickListener(){

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
					long arg3) {
				Intent intent = null;
				switch (arg2) {
				case 0:
					intent = new Intent(SampleList.this, HelloVisualViewActivity.class);
					break;
				case 1:
					intent = new Intent(SampleList.this, EnableLogActivity.class);
					break;
				case 2:
					intent = new Intent(SampleList.this, PropertySetActivity.class);
					break;
				case 3:
					intent = new Intent(SampleList.this, ImageHandleActivity.class);
					break;
				case 4:
					intent = new Intent(SampleList.this, ImplicitAnimation_One_Activity.class);
					break;
				case 5:
					intent = new Intent(SampleList.this, ImplicitAnimation_Two_Activity.class);
					break;
				case 6:
					intent = new Intent(SampleList.this, ImplicitAnimationListenerActivity.class);
					break;
				case 7:
					intent = new Intent(SampleList.this, BasicAnimationActivity.class);
					break;
				case 8:
					intent = new Intent(SampleList.this, KeyframeAnimationActivity.class);
					break;
				case 9:
					intent = new Intent(SampleList.this, SpriteAnimationActivity.class);
					break;
				case 10:
					intent = new Intent(SampleList.this, TransitionAnimationActivity.class);
					break;
				case 11:
					intent = new Intent(SampleList.this, AnimationSetActivity.class);
					break;
				case 12:
					intent = new Intent(SampleList.this, ExplicitAnimationListenerActivity.class);
					break;
				}
				
				if(intent != null)
					startActivity(intent);
			}
        	
        });
        
        
	}
}