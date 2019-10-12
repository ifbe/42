#version 410 core
in mediump vec3 vertex;
in mediump vec3 normal;
out mediump vec4 FragColor;
subroutine vec3 passtype();
subroutine uniform passtype routine;

uniform mediump vec3 camxyz;
uniform mediump vec3 sunxyz;
uniform mediump vec3 sundir;
uniform mediump vec3 sunrgb;
uniform sampler2D suntex;
uniform sampler2D sunimg;
uniform mat4 sunmvp;
mediump vec3 LA = sunrgb;
mediump vec3 LD = sunrgb;
mediump vec3 LS = sunrgb;
mediump vec3 KA = vec3(0.231250, 0.231250, 0.231250);
mediump vec3 KD = vec3(0.277500, 0.277500, 0.277500);
mediump vec3 KS = vec3(0.773911, 0.773911, 0.773911);

subroutine (passtype) vec3 rawcolor(){
	return normalize(normal);
}
subroutine (passtype) vec3 dirlight(){
	mediump vec4 tmp = sunmvp * vec4(vertex, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(tmp.x < 0.0)return vec3(0.0);
	if(tmp.x > 1.0)return vec3(0.0);
	if(tmp.y < 0.0)return vec3(0.0);
	if(tmp.y > 1.0)return vec3(0.0);
	if(tmp.z - texture(suntex, tmp.xy).r > 0.0001)return vec3(0.0);

	mediump vec3 N = normalize(normal);
	mediump vec3 L = normalize(-sundir);
	mediump float SN = dot(N, L);
	mediump vec3 ret = LA*KA + LD*KD*max(SN, 0.0);
	if(SN < 0.0)return ret;

	mediump vec3 E = normalize(camxyz - vertex);
	mediump vec3 H = normalize(E + L);
	mediump float NH = max(dot(N, H), 0.0);
	return ret + LS*KS*pow(NH, 25.0);
}
subroutine (passtype) vec3 spotlight(){
	mediump vec4 tmp = sunmvp * vec4(vertex, 1.0);
	tmp /= tmp.w;
	if(tmp.x*tmp.x + tmp.y*tmp.y > 1.0)return vec3(0.0);
	tmp = (tmp+1.0)*0.5;
	if(tmp.z - texture(suntex, tmp.xy).r > 0.0001)return vec3(0.0);

	mediump vec3 N = normalize(normal);
	mediump vec3 L = normalize(sunxyz - vertex);
	mediump float SN = dot(N, L);
	mediump vec3 ret = LA*KA + LD*KD*max(SN, 0.0);
	if(SN < 0.0)return ret;

	mediump vec3 E = normalize(camxyz - vertex);
	mediump vec3 H = normalize(E + L);
	mediump float NH = max(dot(N, H), 0.0);
	return ret + LS*KS*pow(NH, 25.0);
}
subroutine (passtype) vec3 projector(){
	mediump vec4 tmp = sunmvp * vec4(vertex, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(tmp.x < 0.0)return vec3(0.0);
	if(tmp.x > 1.0)return vec3(0.0);
	if(tmp.y < 0.0)return vec3(0.0);
	if(tmp.y > 1.0)return vec3(0.0);
	if(tmp.z - texture(suntex, tmp.xy).r > 0.0001)return vec3(0.0);
	return texture(sunimg, tmp.xy).bgr;
}
subroutine (passtype) vec3 pointlight(){
	mediump vec3 N = normalize(normal);
	mediump vec3 L = normalize(sunxyz - vertex);
	mediump float SN = dot(N, L);
	mediump vec3 ret = LA*KA + LD*KD*max(SN, 0.0);
	if(SN < 0.0)return ret;

	mediump vec3 E = normalize(camxyz - vertex);
	mediump vec3 H = normalize(E + L);
	mediump float NH = max(dot(N, H), 0.0);
	return ret + LS*KS*pow(NH, 25.0);
}
void main(){
	mediump vec3 light = routine();
	FragColor = vec4(pow(light, vec3(1.0/2.2)), 1.0);
}