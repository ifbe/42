#version 410 core
in mediump vec3 objxyz;
in mediump vec3 normal;
out mediump vec4 FragColor;
subroutine vec3 passtype(mediump vec3 albedo);
subroutine uniform passtype routine;

uniform sampler2D tex0;
uniform sampler2D suntex;
uniform sampler2D sunimg;
uniform mediump vec3 sunrgb;
uniform mediump vec3 sunxyz;
uniform mediump vec3 sundir;
uniform mediump mat4 sunmvp;
uniform mediump vec3 camxyz;

mediump float PI = 3.1415926535897932384626433832795028841971693993151;
mediump float getD(mediump float v, mediump float r){
    float a2 = r*r*r*r;
    float de = (v*v * (a2 - 1.0) + 1.0);
    return a2 / (PI * de * de);
}
mediump float getG(mediump float v, mediump float r){
    float k = (r+1.0) * (r+1.0) / 8.0;
    return v / (v * (1.0 - k) + k);
}
subroutine (passtype) vec3 rawcolor(mediump vec3 albedo){
	mediump vec3 litdir = vec3(1.0, 1.0, 1.0);
	mediump vec3 litrgb = vec3(1.0, 1.0, 1.0);
	mediump vec3 mtrfao = vec3(0.5, 0.5, 1.0);
	mediump float metal = mtrfao.x;
	mediump float rough = mtrfao.y;
	mediump float amocc = mtrfao.z;

	mediump vec3 N = normalize(normal);
	mediump vec3 E = normalize(camxyz - objxyz);
	mediump vec3 F0 = mix(vec3(0.04), albedo, metal);

	mediump vec3 ocolor = vec3(0.0);
	//for(){
		mediump vec3 L = litdir;
		//mediump float distance = length(L);
		//mediump float attenuation = 1.0 / (distance * distance);
		//mediump vec3 radiance = litrgb * attenuation;
		mediump vec3 radiance = litrgb;

		L = normalize(L);
		mediump vec3 H = normalize(E + L);
		mediump float NdotL = max(dot(N, L), 0.0);
		mediump float NdotE = max(dot(N, E), 0.0);
		mediump float NdotH = max(dot(N, H), 0.0);
		mediump float HdotE = max(dot(H, E), 0.0);

		mediump float G = getG(NdotL, rough)*getG(NdotE, rough);
		mediump float D = getD(NdotH, rough);
		mediump vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotE, 5.0);

		mediump vec3 kS = F;
		mediump vec3 kD = (vec3(1.0) - kS) * (1.0 - metal);
		mediump vec3 specular = (D * G * F) / max(4.0 * NdotE * NdotL, 0.001);
		ocolor += (kD * albedo / PI + specular) * radiance * NdotL;
	//}

	ocolor += vec3(0.03) * albedo * amocc;
	ocolor = ocolor / (ocolor + vec3(1.0));
	return pow(ocolor, vec3(1.0/2.2));
}
subroutine (passtype) vec3 dirlight(mediump vec3 albedo){
	mediump vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(	(tmp.x < 0.0) || (tmp.x > 1.0) ||
		(tmp.y < 0.0) || (tmp.y > 1.0) ||
		(tmp.z - texture(suntex, tmp.xy).r > 0.0001) )return albedo*vec3(0.5);
	return albedo;
}
subroutine (passtype) vec3 spotlight(mediump vec3 albedo){
	mediump vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	mediump float val = tmp.x*tmp.x + tmp.y*tmp.y;
	if(val > 1.0)return albedo*vec3(0.2);

	tmp = (tmp+1.0)*0.5;
	if(tmp.z - texture(suntex, tmp.xy).r > 0.0001)return albedo*vec3(0.2);
	return albedo*mix(sunrgb, vec3(0.2), val);
}
subroutine (passtype) vec3 projector(mediump vec3 albedo){
	mediump vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(	(tmp.x < 0.0) || (tmp.x > 1.0) ||
		(tmp.y < 0.0) || (tmp.y > 1.0) ||
		(tmp.z - texture(suntex, tmp.xy).r > 0.0001) )return albedo*vec3(0.5);
	return albedo*texture(sunimg, tmp.xy).bgr;
}
subroutine (passtype) vec3 pointlight(mediump vec3 albedo){
	mediump float dx = (sunxyz-objxyz).x;
	mediump float dy = (sunxyz-objxyz).y;
	mediump float dz = (sunxyz-objxyz).z;
	mediump float sq = 100000000.0 - (dx*dx+dy*dy+dz*dz);
	if(sq < 0.0)sq = 0.0;
	return albedo*sunrgb * sq/100000000.0;
}
void main(){
	mediump float x = mod(objxyz.x, 1000.0) / 1000.0;
	mediump float y = mod(-objxyz.y, 1000.0) / 1000.0;
	vec3 albedo = texture(tex0, vec2(x,y)).bgr;
	FragColor = vec4(routine(albedo), 1.0);
}
