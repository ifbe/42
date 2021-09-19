#version 410 core
in mediump vec2 uvw;
out mediump vec4 FragColor;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform mediump vec3 camxyz;
uniform mediump mat4 invwvp;
void main(){
	float d = texture(tex4, uvw).r;

	if(uvw.x < 0.5){
		d = 2.0 / (10000.0 - 9999.0*d);
		FragColor = vec4(vec3(d), 1.0);
	}
	else if(d < 0.0){
		FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else{
		mediump vec4 screenxyz = vec4(uvw.x*2.0-1.0, uvw.y*2.0-1.0, d*2.0-1.0, 1.0);
		mediump vec4 tmp = invwvp * screenxyz;
		tmp /= tmp.w;
		FragColor = tmp;
	}
}
