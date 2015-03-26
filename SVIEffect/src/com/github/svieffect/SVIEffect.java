/**
 * SGIEffectMain.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect;

import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.github.svieffect.sample.CalculatorSample;
import com.github.svieffect.sample.FXGPhotoEditorSample;
import com.github.svieffect.sample.PhotoEditorSample;
import com.github.svieffect.sample.SpeakerSample;

public class SVIEffect  extends ListActivity {
	Intent mIntent = null;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        System.gc();
        
        //mIntent = new Intent(SVIEffect.this, SpeakerSample.class);
		
        
        final String [] mListCont = {
        		"Calculator Demo",
        		"GroupPlay Demo",
        		"PhotoEditor Demo - 1",
        		"PhotoEditor Demo - 2"
        		//"Smart Controller Demo",
        		//"Ripple - Dialer Demo",
        };
        
        setListAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, mListCont));

        ListView list = getListView();

        list.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3)
			{
				Intent intent = null;
				switch(arg2)
		        {
				
		        case 0:
					intent = new Intent(SVIEffect.this, CalculatorSample.class);
					break;
				
		        case 1:
					intent = new Intent(SVIEffect.this, SpeakerSample.class);
					break;

		        case 2:
					intent = new Intent(SVIEffect.this, PhotoEditorSample.class);
					break;

		        case 3:
					intent = new Intent(SVIEffect.this, FXGPhotoEditorSample.class);
					break;
					
		        //case 4:
				//	intent = new Intent(SVIEffect.this, RemoteControllerSample.class);
				//	break;
					
				//case 5:
				//	intent = new Intent(SVIEffect.this, DialerSample.class);
				//	break;
					
		        }
				
				if( intent != null )
					startActivity(intent);
			}
        });
        
    }

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		
		mIntent = null;
		
		System.gc();
	}
}