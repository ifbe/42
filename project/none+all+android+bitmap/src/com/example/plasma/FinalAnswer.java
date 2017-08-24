/*
 * Copyright (C) 2010 The Android Open Source Project
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
package com.example.finalanswer;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.view.View;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Display;
import android.view.WindowManager;




class FinalAnswerView extends View {
	private static native void Start(Bitmap bitmap);
	private static native void Stop();
	private static native void Read(Bitmap bitmap);
	private static native void Write(long type, long value);

	private Bitmap mBitmap;
	private int w = getResources().getDisplayMetrics().widthPixels;
	private int h = getResources().getDisplayMetrics().heightPixels;

	public FinalAnswerView(Context context, int width, int height) {
		super(context);
		mBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
		Start(mBitmap);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		//
		Read(mBitmap);

		//canvas.drawColor(0xFFCCCCCC);
		canvas.drawBitmap(mBitmap, 0, 0, null);

		//force a redraw, with a different time-based pattern.
		invalidate();
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int action = event.getActionMasked();
		int index = event.getActionIndex();
		int count = event.getPointerCount();
		//Log.i("<<<<<<<<","("+action+","+index+","+count+")");

		//point move:	'p@'=0x4070
		if( action == MotionEvent.ACTION_MOVE )
		{
			for(index=0;index<count;index++)
			{
				long temp=(long)event.getPointerId(index);
				temp=(temp<<16)+(long)event.getPressure(index);
				temp=(temp<<16)+(long)(event.getY(index) * 65536 / h);
				temp=(temp<<16)+(long)(event.getX(index) * 65536 / w);
				Write(0x4070 , temp);
			}
		}

		//point sensed:	'p+'=0x2d70
		else if((action == MotionEvent.ACTION_DOWN) |
			(action == MotionEvent.ACTION_POINTER_DOWN) )
		{
			long temp=(long)event.getPointerId(index);
			temp=(temp<<16)+(long)event.getPressure(index);
			temp=(temp<<16)+(long)(event.getY(index) * 65536 / h);
			temp=(temp<<16)+(long)(event.getX(index) * 65536 / w);
			Write(0x2b70 , temp);
		}

		//point gone:	'p-'=0x2d70
		else if((action == MotionEvent.ACTION_UP) |
			(action == MotionEvent.ACTION_POINTER_UP) )
		{
			long temp=(long)event.getPointerId(index);
			temp=(temp<<16)+(long)event.getPressure(index);
			temp=(temp<<16)+(long)(event.getY(index) * 65536 / h);
			temp=(temp<<16)+(long)(event.getX(index) * 65536 / w);
			Write(0x2d70 , temp);
		}

		return true;
	}
}
public class FinalAnswer extends Activity
{
	static {
		System.loadLibrary("finalanswer");
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Display display = getWindowManager().getDefaultDisplay();
		setContentView(
			new FinalAnswerView(this, display.getWidth(), display.getHeight())
		);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			Log.i("finalanswer","KeyEvent.KEYCODE_BACK");
			finish();
			System.exit(0);
		}
		return super.onKeyDown(keyCode, event);
	}
}