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
		tmp = texture(tex0, vec2(uvw.x*2.0-0.0, uvw.y*2.0-0.0)).rgb;
		FragColor = vec4(tmp, 1.0);
	}
	if((uvw.x > 0.5)&&(uvw.y < 0.5)){
		tmp = texture(tex1, vec2(uvw.x*2.0-1.0, uvw.y*2.0-0.0)).rgb;
		FragColor = vec4(tmp, 1.0);
	}
	if((uvw.x < 0.5)&&(uvw.y > 0.5)){
		tmp = texture(tex2, vec2(uvw.x*2.0-0.0, uvw.y*2.0-1.0)).rgb;
		FragColor = vec4(tmp, 1.0);
	}
	if((uvw.x > 0.5)&&(uvw.y > 0.5)){
		mediump float d = texture(tex3, vec2(uvw.x*2.0-1.0, uvw.y*2.0-1.0)).b;
		tmp = vec3(d,d,d);
		FragColor = vec4(tmp, 1.0);
	}
}
