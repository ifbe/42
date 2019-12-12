#version 410 core
in mediump vec3 vertex;
out mediump vec4 FragColor;
subroutine vec3 passtype();
subroutine uniform passtype routine;

uniform sampler2D tex0;
uniform sampler2D suntex;
uniform sampler2D sunimg;
uniform vec3 sunrgb;
uniform vec3 sunxyz;
uniform vec3 sundir;
uniform mat4 sunmvp;

subroutine (passtype) vec3 rawcolor(){
	return vec3(1.0);
}
subroutine (passtype) vec3 dirlight(){
	mediump vec4 tmp = sunmvp * vec4(vertex, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(tmp.x < 0.0)return vec3(0.5);
	if(tmp.x > 1.0)return vec3(0.5);
	if(tmp.y < 0.0)return vec3(0.5);
	if(tmp.y > 1.0)return vec3(0.5);
	if(tmp.z - texture(suntex, tmp.xy).r > 0.0001)return vec3(0.5);
	return vec3(1.0);
}
subroutine (passtype) vec3 spotlight(){
	mediump vec4 tmp = sunmvp * vec4(vertex, 1.0);
	tmp /= tmp.w;
	mediump float val = tmp.x*tmp.x + tmp.y*tmp.y;
	if(val > 1.0)return vec3(0.2);
	tmp = (tmp+1.0)*0.5;
	if(tmp.z - texture(suntex, tmp.xy).r > 0.0001)return vec3(0.2);
	return mix(sunrgb, vec3(0.2), val);
}
subroutine (passtype) vec3 projector(){
	mediump vec4 tmp = sunmvp * vec4(vertex, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(tmp.x < 0.0)return vec3(0.5);
	if(tmp.x > 1.0)return vec3(0.5);
	if(tmp.y < 0.0)return vec3(0.5);
	if(tmp.y > 1.0)return vec3(0.5);
	if(tmp.z - texture(suntex, tmp.xy).r > 0.0001)return vec3(0.5);
	return texture(sunimg, tmp.xy).bgr;
}
subroutine (passtype) vec3 pointlight(){
	mediump float dx = (sunxyz-vertex).x;
	mediump float dy = (sunxyz-vertex).y;
	mediump float dz = (sunxyz-vertex).z;
	mediump float sq = 100000000.0 - (dx*dx+dy*dy+dz*dz);
	if(sq < 0.0)sq = 0.0;
	return sunrgb * sq/100000000.0;
}
void main(){
	mediump float x = mod(abs(vertex.x), 1000.0) / 1000.0;
	mediump float y = mod(abs(vertex.y), 1000.0) / 1000.0;
	vec3 bgr = texture(tex0, vec2(x,y)).bgr;
	FragColor = vec4(routine() * bgr, 1.0);
}
