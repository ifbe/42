#version 410 core
in mediump vec3 objxyz;
in mediump vec2 texcoo;
in mediump vec3 normal;
in mediump vec3 tangen;
out mediump vec4 FragColor;
subroutine vec3 passtype();
subroutine uniform passtype routine;

uniform sampler2D albedomap;
uniform sampler2D normalmap;
uniform sampler2D mattermap;
uniform sampler2D shadowmap;
uniform sampler2D prjtormap;
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
mediump vec3 getnormal(){
	vec3 N = normalize(normal);
	vec3 T = normalize(tangen);
	T = normalize(T - N * dot(T, N));
	vec3 B = cross(N, T);
	vec3 XYZ = texture(normalmap, texcoo).bgr * 2.0 - vec3(1.0);
	mat3 TBN = mat3(T, B, N);
	return TBN * XYZ;
}
subroutine (passtype) vec3 pbrcolor(){
	mediump vec3 albedo = pow(texture(albedomap, texcoo).bgr, vec3(2.2));
	mediump vec3 matter = texture(mattermap, texcoo).bgr;

	mediump float metal = matter.x;
	mediump float rough = matter.y;
	mediump float amocc = matter.z;

	mediump vec3 N = getnormal();
	mediump vec3 E = normalize(camxyz - objxyz);
	mediump vec3 F0 = mix(vec3(0.04), albedo, metal);

	mediump vec3 litrgb = vec3(1.0, 1.0, 1.0);
	mediump vec3 litdir[4];
	litdir[0] = vec3(-1.0, 0.0, 1.0);
	litdir[1] = vec3( 1.0, 0.0, 1.0);
	litdir[2] = vec3( 0.0,-1.0, 1.0);
	litdir[3] = vec3( 0.0, 1.0, 1.0);

	mediump vec3 ocolor = vec3(0.03) * albedo * amocc;
	for(int j=0;j<4;j++){
		mediump vec3 L = litdir[j];
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
	}

	ocolor = ocolor / (ocolor + vec3(1.0));
	return pow(ocolor, vec3(1.0/2.2));
}
subroutine (passtype) vec3 dirlight(){
	mediump float x = mod(objxyz.x, 1000.0) / 1000.0;
	mediump float y = mod(-objxyz.y, 1000.0) / 1000.0;
	mediump vec3 albedo = texture(albedomap, vec2(x,y)).bgr;

	mediump vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(	(tmp.x < 0.0) || (tmp.x > 1.0) ||
		(tmp.y < 0.0) || (tmp.y > 1.0) ||
		(tmp.z - texture(shadowmap, tmp.xy).r > 0.0001) )return albedo*vec3(0.5);
	return albedo;
}
subroutine (passtype) vec3 spotlight(){
	mediump float x = mod(objxyz.x, 1000.0) / 1000.0;
	mediump float y = mod(-objxyz.y, 1000.0) / 1000.0;
	mediump vec3 albedo = texture(albedomap, vec2(x,y)).bgr;

	mediump vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	mediump float val = tmp.x*tmp.x + tmp.y*tmp.y;
	if(val > 1.0)return albedo*vec3(0.2);

	tmp = (tmp+1.0)*0.5;
	if(tmp.z - texture(shadowmap, tmp.xy).r > 0.0001)return albedo*vec3(0.2);
	return albedo*mix(sunrgb, vec3(0.2), val);
}
subroutine (passtype) vec3 projector(){
	mediump float x = mod(objxyz.x, 1000.0) / 1000.0;
	mediump float y = mod(-objxyz.y, 1000.0) / 1000.0;
	mediump vec3 albedo = texture(albedomap, vec2(x,y)).bgr;

	mediump vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(	(tmp.x < 0.0) || (tmp.x > 1.0) ||
		(tmp.y < 0.0) || (tmp.y > 1.0) ||
		(tmp.z - texture(shadowmap, tmp.xy).r > 0.0001) )return albedo*vec3(0.5);
	return albedo*texture(prjtormap, tmp.xy).bgr;
}
subroutine (passtype) vec3 pointlight(){
	mediump float x = mod(objxyz.x, 1000.0) / 1000.0;
	mediump float y = mod(-objxyz.y, 1000.0) / 1000.0;
	mediump vec3 albedo = texture(albedomap, vec2(x,y)).bgr;

	mediump float dx = (sunxyz-objxyz).x;
	mediump float dy = (sunxyz-objxyz).y;
	mediump float dz = (sunxyz-objxyz).z;
	mediump float sq = 100000000.0 - (dx*dx+dy*dy+dz*dz);
	if(sq < 0.0)sq = 0.0;
	return albedo*sunrgb * sq/100000000.0;
}
void main(){
	FragColor = vec4(routine(), 1.0);
}
