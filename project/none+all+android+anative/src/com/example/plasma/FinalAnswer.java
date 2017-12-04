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
import android.graphics.PixelFormat;
import android.graphics.Canvas;
import android.graphics.Color;
//
//import javax.microedition.khronos.egl.EGLConfig;
//import javax.microedition.khronos.opengles.GL10;
//import android.opengl.GLES20;
//import android.opengl.GLSurfaceView;
//
import android.view.View;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Display;
import android.view.WindowManager;
import android.view.Surface;
import android.view.SurfaceView;
import android.view.SurfaceHolder;



/*
class MyRenderer implements GLSurfaceView.Renderer
{
	private static native void Start(Surface s);
	private static native void Stop();
	private static native void Read();
	private SurfaceHolder holder;

	public MyRenderer() {
		Log.i("finalanswer", "@MyRenderer");
	}

	public void setHolder(SurfaceHolder h)
	{
		holder = h;
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		Log.i("finalanswer", "@onSurfaceCreated");
		Start(holder.getSurface());
	}

	@Override
    public void onSurfaceChanged(GL10 gl, int w, int h) {
		Log.i("finalanswer", "@onSurfaceChanged");
        gl.glViewport(0, 0, w, h);
    }

	@Override
    public void onDrawFrame(GL10 gl) {
		//Log.i("finalanswer", "@onDrawFrame");
		Read();
    }
}
*/
class FinalAnswerView extends SurfaceView implements SurfaceHolder.Callback {
	private static native void Start(Surface s);
	private static native void Read();

	private SurfaceHolder holder;

	public FinalAnswerView(Context context) {
		super(context);
		Log.i("finalanswer", "@FinalAnswerView");

		holder = this.getHolder();
		holder.setFormat(PixelFormat.RGBA_8888);
        holder.addCallback(this);
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		Log.i("finalanswer", "@surfaceCreated");
		Start(holder.getSurface());
		Read();
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int fmt, int w, int h) {
		Log.i("finalanswer", "@surfaceChanged");
		Start(holder.getSurface());
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		Log.i("finalanswer", "@surfaceDestroyed");
	}

	public boolean onTouchEvent(MotionEvent event) {
		int action = event.getActionMasked();
		int index = event.getActionIndex();
		int count = event.getPointerCount();
		int x = (int)event.getX(index);
		int y = (int)event.getY(index);
		Log.i("finalanswer","("+action+","+index+","+count+")"+x+","+y);

		Read();
		return true;
	}
}
public class FinalAnswer extends Activity
{
	private FinalAnswerView view;
	static {
		System.loadLibrary("finalanswer");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.i("finalanswer", "@onCreate");

		view = new FinalAnswerView(this);
		setContentView(view);
	}

	@Override
	protected void onPause() {
		super.onPause();
		Log.i("finalanswer", "@onPause");
	}

	@Override
	protected void onResume() {
		super.onResume();
		Log.i("finalanswer", "@onResume");
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