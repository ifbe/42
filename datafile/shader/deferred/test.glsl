#version 410 core
in mediump vec2 uvw;
out mediump vec4 FragColor;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
void main(){
	mediump vec3 tmp;
	if((uvw.x < 0.5)&&(uvw.y < 0.5)){
		tmp = texture(tex0, uvw).rgb;
		FragColor = vec4(tmp, 1.0);
	}
	if((uvw.x > 0.5)&&(uvw.y < 0.5)){
		tmp = texture(tex1, uvw).rgb*2.0-1.0;
		FragColor = vec4(tmp, 1.0);
	}
	if((uvw.x < 0.5)&&(uvw.y > 0.5)){
		tmp = texture(tex2, uvw).rgb;
		FragColor = vec4(tmp, 1.0);
	}
	if((uvw.x > 0.5)&&(uvw.y > 0.5)){
		mediump float d = texture(tex3, uvw).b;
		tmp = vec3(d,d,d);
		FragColor = vec4(tmp, 1.0);
	}
}
