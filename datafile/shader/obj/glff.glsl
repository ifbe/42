in highp vec3 objxyz;
in highp vec2 texcoo;
in highp vec3 normal;
in highp vec3 tangen;
out highp vec4 FragColor;

uniform int lighttype;
uniform sampler2D albedomap;
uniform sampler2D normalmap;
uniform sampler2D mattermap;
uniform sampler2D shadowmap;
uniform sampler2D prjtormap;
uniform highp vec3 sunrgb;
uniform highp vec3 sunxyz;
uniform highp vec3 sundir;
uniform highp mat4 sunmvp;
uniform highp vec3 camxyz;

highp float PI = 3.1415926535897932384626433832795028841971693993151;
highp float getD(highp float v, highp float r){
    float a2 = r*r*r*r;
    float de = (v*v * (a2 - 1.0) + 1.0);
    return a2 / (PI * de * de);
}
highp float getG(highp float v, highp float r){
    float k = (r+1.0) * (r+1.0) / 8.0;
    return v / (v * (1.0 - k) + k);
}
highp vec3 getnormal(){
	vec3 N = normalize(normal);
	vec3 T = normalize(tangen);
	T = normalize(T - N * dot(T, N));
	vec3 B = cross(N, T);
	vec3 XYZ = texture(normalmap, texcoo).bgr * 2.0 - vec3(1.0);
	mat3 TBN = mat3(T, B, N);
	return TBN * XYZ;
}
vec3 pbrcolor(){
	highp vec3 albedo = pow(texture(albedomap, texcoo).bgr, vec3(2.2));
	highp vec3 matter = texture(mattermap, texcoo).bgr;

	highp float metal = matter.x;
	highp float rough = matter.y;
	highp float amocc = matter.z;

	highp vec3 N = getnormal();
	highp vec3 E = normalize(camxyz - objxyz);
	highp vec3 F0 = mix(vec3(0.04), albedo, metal);

	highp vec3 litrgb = vec3(3.0, 3.0, 3.0);
	highp vec3 litdir[4];
	litdir[0] = vec3(-1.0, 0.0, 1.0);
	litdir[1] = vec3( 1.0, 0.0, 1.0);
	litdir[2] = vec3( 0.0,-1.0, 1.0);
	litdir[3] = vec3( 0.0, 1.0, 1.0);

	highp vec3 ocolor = vec3(0.03) * albedo * amocc;
	for(int j=0;j<4;j++){
		highp vec3 L = litdir[j];
		//highp float distance = length(L);
		//highp float attenuation = 1.0 / (distance * distance);
		//highp vec3 radiance = litrgb * attenuation;
		highp vec3 radiance = litrgb;

		L = normalize(L);
		highp vec3 H = normalize(E + L);
		highp float NdotL = max(dot(N, L), 0.0);
		highp float NdotE = max(dot(N, E), 0.0);
		highp float NdotH = max(dot(N, H), 0.0);
		highp float HdotE = max(dot(H, E), 0.0);

		highp float G = getG(NdotL, rough)*getG(NdotE, rough);
		highp float D = getD(NdotH, rough);
		highp vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotE, 5.0);

		highp vec3 kS = F;
		vec3 specular = (D * G * F) / (4.0 * NdotE * NdotL + 0.0001);

		highp vec3 kD = (vec3(1.0) - kS) * (1.0 - metal);
		vec3 diffuse = kD * albedo / PI;

		ocolor += (diffuse + specular) * radiance * NdotL;
	}

	ocolor = ocolor / (ocolor + vec3(1.0));
	return pow(ocolor, vec3(1.0/2.2));
}
vec3 dirlight(){
	highp float x = mod(objxyz.x, 1000.0) / 1000.0;
	highp float y = mod(-objxyz.y, 1000.0) / 1000.0;
	highp vec3 albedo = texture(albedomap, vec2(x,y)).bgr;

	highp vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(	(tmp.x < 0.0) || (tmp.x > 1.0) ||
		(tmp.y < 0.0) || (tmp.y > 1.0) ||
		(tmp.z - texture(shadowmap, tmp.xy).r > 0.0001) )return albedo*vec3(0.5);
	return albedo;
}
vec3 spotlight(){
	highp float x = mod(objxyz.x, 1000.0) / 1000.0;
	highp float y = mod(-objxyz.y, 1000.0) / 1000.0;
	highp vec3 albedo = texture(albedomap, vec2(x,y)).bgr;

	highp vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	highp float val = tmp.x*tmp.x + tmp.y*tmp.y;
	if(val > 1.0)return albedo*vec3(0.2);

	tmp = (tmp+1.0)*0.5;
	if(tmp.z - texture(shadowmap, tmp.xy).r > 0.0001)return albedo*vec3(0.2);
	return albedo*mix(sunrgb, vec3(0.2), val);
}
vec3 projector(){
	highp float x = mod(objxyz.x, 1000.0) / 1000.0;
	highp float y = mod(-objxyz.y, 1000.0) / 1000.0;
	highp vec3 albedo = texture(albedomap, vec2(x,y)).bgr;

	highp vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;
	if(	(tmp.x < 0.0) || (tmp.x > 1.0) ||
		(tmp.y < 0.0) || (tmp.y > 1.0) ||
		(tmp.z - texture(shadowmap, tmp.xy).r > 0.0001) )return albedo*vec3(0.5);
	return albedo*texture(prjtormap, tmp.xy).bgr;
}
vec3 pointlight(){
	highp float x = mod(objxyz.x, 1000.0) / 1000.0;
	highp float y = mod(-objxyz.y, 1000.0) / 1000.0;
	highp vec3 albedo = texture(albedomap, vec2(x,y)).bgr;

	highp float dx = (sunxyz-objxyz).x;
	highp float dy = (sunxyz-objxyz).y;
	highp float dz = (sunxyz-objxyz).z;
	highp float sq = 100000000.0 - (dx*dx+dy*dy+dz*dz);
	if(sq < 0.0)sq = 0.0;
	return albedo*sunrgb * sq/100000000.0;
}
void main(){
	vec3 c = vec3(0.0);
	switch(lighttype){
	case 20:c=dirlight();break;
	case 21:c=spotlight();break;
	case 22:c=projector();break;
	case 23:c=pointlight();break;
	default:c=pbrcolor();break;
	}
	FragColor = vec4(c, 1.0);
}
