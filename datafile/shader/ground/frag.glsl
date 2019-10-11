#version 410 core
in mediump vec3 vertex;
out mediump vec4 FragColor;
uniform sampler2D suntex;
uniform sampler2D sunimg;
uniform sampler2D tex0;
uniform mat4 sunmvp;
vec3 shadow(){
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
void main(){
	mediump float x = mod(abs(vertex.x), 1000.0) / 1000.0;
	mediump float y = mod(abs(vertex.y), 1000.0) / 1000.0;
	vec3 bgr = texture(tex0, vec2(x,y)).bgr;
	FragColor = vec4(shadow() * bgr, 1.0);
}