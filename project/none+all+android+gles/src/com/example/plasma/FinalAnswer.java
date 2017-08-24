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
//
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
//
import android.view.View;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Display;
import android.view.WindowManager;
import android.view.Surface;




class MyRenderer implements GLSurfaceView.Renderer
{
	private static native void Read();

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		gl.glClearColor(0f, 1f, 0f, 0f);
	}
    public void onSurfaceChanged(GL10 gl, int w, int h) {
    }
    public void onDrawFrame(GL10 gl) {
		//Read();
    }
}
public class FinalAnswer extends Activity
{
	private static native void Start(GLSurfaceView surface);
	private static native void Stop();

	private GLSurfaceView surface;
	static {
		System.loadLibrary("finalanswer");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		surface = new GLSurfaceView(this);
		surface.setEGLContextClientVersion(2);
		surface.setRenderer(new MyRenderer());
		setContentView(surface);

		Start(surface);
		//SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface_view);
		//surfaceHolder = surfaceView.getHolder();
		//surfaceHolder.addCallback(this);
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