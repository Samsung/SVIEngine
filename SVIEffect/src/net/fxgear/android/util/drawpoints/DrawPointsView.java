package net.fxgear.android.util.drawpoints;

import java.util.Vector;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class DrawPointsView extends SurfaceView implements SurfaceHolder.Callback
{
	
	private Paint paint;
	private DrawPointsThread thread;
	private Vector<Point> points = null;
	private boolean draw = true;
	
	public DrawPointsView(Context context, int color)
	{
		super(context);
		paint = new Paint();
		paint.setColor(color);
		getHolder().addCallback(this);
		getHolder().setFormat(PixelFormat.TRANSPARENT);
	}
	
	public void setPoints(Vector<Point> points)
	{
		this.points = points;
	}
	
	public void setDrawing(boolean draw)
	{
		this.draw = draw;
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder)
	{
		setWillNotDraw(false);
		thread = new DrawPointsThread(getHolder(), this);
		thread.setRunning(true);
		thread.start();
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public void onDraw(Canvas canvas)
	{
		canvas.drawColor(0x00000000);
		if(draw && points != null)
		{
			for(int i = 0; i < points.size(); i++)
			{
				canvas.drawCircle(points.get(i).x, points.get(i).y, 5, paint);
			}
		}
	}
	
	@Override
	public void surfaceDestroyed(SurfaceHolder holder)
	{
		try
		{
			thread.setRunning(false);
			thread.join();
		}
		catch (InterruptedException e) {}
	}
	
	private class DrawPointsThread extends Thread
	{
	    private SurfaceHolder surfaceHolder;
	    private DrawPointsView panel;
	    private boolean run = false;


	    public DrawPointsThread(SurfaceHolder surfaceHolder, DrawPointsView panel)
	    {
	        this.surfaceHolder = surfaceHolder;
	        this.panel = panel;
	    }


	    public void setRunning(boolean run)
	    {
	        this.run = run;
	    }


	    @Override
	    public void run()
	    {
	        Canvas canvas;
	        while (run)
	        {
	            canvas = null;
	            try
	            {
	                canvas = surfaceHolder.lockCanvas(null);
	                synchronized (surfaceHolder)
	                {
	                	panel.postInvalidate();
	                }
	            } 
	            finally
	            {
		            if (canvas != null)
		            {
		                surfaceHolder.unlockCanvasAndPost(canvas);
		            }
	            }
	        }
	    }
	}
}
