in vec3 objxyz;
in vec2 objuvw;
out vec4 FragColor;
subroutine vec3 passtype();
subroutine uniform passtype routine;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D suntex;
uniform sampler2D sunimg;
uniform vec3 sunrgb;
uniform vec3 sunxyz;
uniform vec3 sundir;
uniform mat4 sunmvp;
uniform vec3 camxyz;

const float PI = 3.1415926535897932384626433832795028841971693993151;
float getD(float v, float r){
    float a2 = r*r*r*r;
    float de = (v*v * (a2 - 1.0) + 1.0);
    return a2 / (PI * de * de);
}
float getG(float v, float r){
    float k = (r+1.0) * (r+1.0) / 8.0;
    return v / (v * (1.0 - k) + k);
}
subroutine (passtype) vec3 rawcolor(){
	vec3 albedo = pow(texture(tex0, objuvw).bgr, vec3(2.2));
	vec3 normal = texture(tex1, objuvw).bgr * 2.0 - vec3(1.0);
	vec3 matter = texture(tex2, objuvw).bgr;

	float metal = matter.x;
	float rough = matter.y;
	float amocc = pow(matter.z, 2.2);

	vec3 N = normalize(normal);
	vec3 E = normalize(camxyz - objxyz);
	vec3 F0 = mix(vec3(0.04), albedo, metal);

	vec3 litrgb = vec3(1.0, 1.0, 1.0);
	vec3 litdir[4];
	litdir[0] = vec3(-1.0, 0.0, 1.0);
	litdir[1] = vec3( 1.0, 0.0, 1.0);
	litdir[2] = vec3( 0.0,-1.0, 1.0);
	litdir[3] = vec3( 0.0, 1.0, 1.0);

	vec3 ocolor = vec3(0.03) * albedo * amocc;
	for(int j=0;j<4;j++){
		vec3 L = litdir[j];
		//float distance = length(L);
		//float attenuation = 1.0 / (distance * distance);
		//vec3 radiance = litrgb * attenuation;
		vec3 radiance = litrgb;

		L = normalize(L);
		vec3 H = normalize(E + L);
		float NdotL = max(dot(N, L), 0.0);
		float NdotE = max(dot(N, E), 0.0);
		float NdotH = max(dot(N, H), 0.0);
		float HdotE = max(dot(H, E), 0.0);

		float G = getG(NdotL, rough)*getG(NdotE, rough);
		float D = getD(NdotH, rough);
		vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotE, 5.0);

		vec3 kS = F;
		vec3 kD = (vec3(1.0) - kS) * (1.0 - metal);
		vec3 specular = (D * G * F) / max(4.0 * NdotE * NdotL, 0.001);
		ocolor += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	ocolor = ocolor / (ocolor + vec3(1.0));
	return pow(ocolor, vec3(1.0/2.2));
}
subroutine (passtype) vec3 dirlight(){
	vec3 albedo = texture(tex0, objuvw).bgr;

	vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(	(tmp.x < 0.0) || (tmp.x > 1.0) ||
		(tmp.y < 0.0) || (tmp.y > 1.0) ||
		(tmp.z - texture(suntex, tmp.xy).r > 0.0001) )return albedo*vec3(0.5);
	return albedo;
}
subroutine (passtype) vec3 spotlight(){
	vec3 albedo = texture(tex0, objuvw).bgr;

	vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	float val = tmp.x*tmp.x + tmp.y*tmp.y;
	if(val > 1.0)return albedo*vec3(0.2);

	tmp = (tmp+1.0)*0.5;
	if(tmp.z - texture(suntex, tmp.xy).r > 0.0001)return albedo*vec3(0.2);
	return albedo*mix(sunrgb, vec3(0.2), val);
}
subroutine (passtype) vec3 projector(){
	vec3 albedo = texture(tex0, objuvw).bgr;

	vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(	(tmp.x < 0.0) || (tmp.x > 1.0) ||
		(tmp.y < 0.0) || (tmp.y > 1.0) ||
		(tmp.z - texture(suntex, tmp.xy).r > 0.0001) )return albedo*vec3(0.5);
	return albedo*texture(sunimg, tmp.xy).bgr;
}
subroutine (passtype) vec3 pointlight(){
	vec3 albedo = texture(tex0, objuvw).bgr;

	float dx = (sunxyz-objxyz).x;
	float dy = (sunxyz-objxyz).y;
	float dz = (sunxyz-objxyz).z;
	float sq = 100000000.0 - (dx*dx+dy*dy+dz*dz);
	if(sq < 0.0)sq = 0.0;
	return albedo*sunrgb * sq/100000000.0;
}
void main(){
	FragColor = vec4(routine(), 1.0);
}
