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
package com.example.plasma;

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

public class Plasma extends Activity
{
	/* load our native library */
	static {
		System.loadLibrary("plasma");
	}

	/* Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		Display display = getWindowManager().getDefaultDisplay();
		setContentView(new PlasmaView(this,display.getWidth(),display.getHeight()));
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			Log.i("PlasmaView","KeyEvent.KEYCODE_BACK");
			finish();
			System.exit(0);
		}
		return super.onKeyDown(keyCode, event);
	}
}

class PlasmaView extends View {
	private Bitmap mBitmap;

	/* implementend by libplasma.so */
	private static native void renderPlasma(Bitmap  bitmap, long time_ms);
	private static native void ProcessEvent(long type, long value);

	public PlasmaView(Context context, int width, int height) {
		super(context);
		mBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		// canvas.drawColor(0xFFCCCCCC);
		canvas.drawBitmap(mBitmap, 0, 0, null);
		// force a redraw, with a different time-based pattern.
		invalidate();
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int action = event.getActionMasked();
		long x = (long)event.getX(0);
		long y = (long)event.getY(0);
		long z = (long)event.getPressure(0);
		long temp=x+(y<<16)+(z<<32);
		//Log.i("PlasmaView","<<<<<<<<<<<<<<<("+x+","+y+","+z+"),"+temp);

		if( action == MotionEvent.ACTION_DOWN )
		{
			ProcessEvent(0x6e776f64 , temp);
		}
		if( action == MotionEvent.ACTION_UP )
		{
			ProcessEvent(0x7075 , temp);
		}
		if( action == MotionEvent.ACTION_MOVE )
		{
			ProcessEvent(0x65766f6d , temp);
		}

		renderPlasma(mBitmap, 0);
		return true;
	}
}
