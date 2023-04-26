in vec3 objxyz;
in vec3 normal;
out vec4 FragColor;

uniform int lighttype;
uniform vec3 matter;
uniform vec3 camxyz;
uniform vec3 sunxyz;
uniform vec3 sundir;
uniform vec3 sunrgb;
uniform sampler2D shadowmap;
uniform sampler2D prjtormap;
uniform sampler2D iblenvmap;
uniform mat4 sunmvp;
vec3 LA = sunrgb;
vec3 LD = sunrgb;
vec3 LS = sunrgb;
vec3 KA = vec3(0.231250, 0.231250, 0.231250);
vec3 KD = vec3(0.277500, 0.277500, 0.277500);
vec3 KS = vec3(0.773911, 0.773911, 0.773911);

vec3 albedo = vec3(1.0, 1.0, 1.0);
float PI = 3.1415926535897932384626433832795028841971693993151;
float getD(float v, float r){
    float a2 = r*r*r*r;
    float de = (v*v * (a2 - 1.0) + 1.0);
    return a2 / (PI * de * de);
}
float getG(float v, float r){
    float k = (r+1.0) * (r+1.0) / 8.0;
    return v / (v * (1.0 - k) + k);
}
vec3 pbrcolor(){
	float metal = matter.x;
	float rough = matter.y;
	float amocc = matter.z;

	vec3 N = normalize(normal);
	vec3 E = normalize(camxyz - objxyz);
	vec3 F0 = mix(vec3(0.04), albedo, metal);

	vec3 litrgb = vec3(3.0, 3.0, 3.0);
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

		vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotE, 5.0);
		float D = getD(NdotH, rough);
		float G = getG(NdotL, rough)*getG(NdotE, rough);

		vec3 kS = F;
		vec3 specular = (D * G * F) / (4.0 * NdotE * NdotL + 0.0001);

		vec3 kD = (vec3(1.0) - kS) * (1.0 - metal);
		vec3 diffuse = kD * albedo / PI;

		ocolor += (diffuse + specular) * radiance * NdotL;
	}

	ocolor = ocolor / (ocolor + vec3(1.0));
	return pow(ocolor, vec3(1.0/2.2));
}


float iblG(float v, float r){
    float k = r*r / 2.0;
    return v / (v * (1.0 - k) + k);
}
vec3 imagelight(){
	float metal = matter.x;
	float rough = matter.y;
	float amocc = matter.z;

	vec3 N = normalize(normal);
	vec3 E = normalize(camxyz - objxyz);
	vec3 R = reflect(E, N);
	vec3 F0 = mix(vec3(0.04), albedo, metal);

	float x = atan(R.y, R.x)/PI/2.0 + 0.5;
	float y = 0.5 + asin(R.z)/PI;
	vec3 litrgb = texture(iblenvmap, vec2(x,y)).bgr;
	vec3 litdir = -R;

	vec3 ocolor = vec3(0.03) * albedo * amocc;
	//for(int j=0;j<4;j++){
		vec3 L = litdir;
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

		vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotE, 5.0);
		float D = getD(NdotH, rough);
		float G = iblG(NdotL, rough)*iblG(NdotE, rough);

		vec3 kS = F;
		vec3 kD = (vec3(1.0) - kS) * (1.0 - metal);
		vec3 specular = (D * G * F) / max(4.0 * NdotE * NdotL, 0.001);
		ocolor += (kD * albedo / PI + specular) * radiance * NdotL;
	//}

	ocolor = ocolor / (ocolor + vec3(1.0));
	return pow(ocolor, vec3(1.0/2.2));
}

vec3 dirlight(){
	vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;

	if( (tmp.x < 0.0) || (tmp.x > 1.0) ||
		(tmp.y < 0.0) || (tmp.y > 1.0) )return vec3(0.1);

	if(tmp.z - texture(shadowmap, tmp.xy).r > 0.0001)return vec3(0.2);

	vec3 N = normalize(normal);
	vec3 L = normalize(-sundir);
	float SN = dot(N, L);
	vec3 ret = LA*KA + LD*KD*max(SN, 0.0);
	if(SN < 0.0)return ret;

	vec3 E = normalize(camxyz - objxyz);
	vec3 H = normalize(E + L);
	float NH = max(dot(N, H), 0.0);
	return ret + LS*KS*pow(NH, 25.0);
}

vec3 spotlight(){
	vec3 N = normalize(normal);
	vec3 L = normalize(sunxyz - objxyz);
	float dotNL = dot(N, L);
	vec3 colour = LA*KA + LD*KD*max(dotNL, 0.0);

	//out of light
	vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp.x /= tmp.w;
	tmp.y /= tmp.w;
	float val = tmp.x*tmp.x + tmp.y*tmp.y;
	if(val > 1.0)return 0.1*colour;

	//in the shadow
	tmp = (tmp+1.0)*0.5;
	if(tmp.z+0.45 > tmp.w*texture(shadowmap, tmp.xy).r)return 0.2*colour;

	//greater than 90 degree
	if(dotNL < 0.0)return colour;

	//have specular
	vec3 E = normalize(camxyz - objxyz);
	vec3 H = normalize(E + L);
	float NH = max(dot(N, H), 0.0);
	return colour + LS*KS*pow(NH, 25.0);
}

vec3 projector(){
	vec3 N = normalize(normal);
	vec3 L = normalize(sunxyz - objxyz);
	float dotNL = dot(N, L);
	vec3 colour = LA*KA + LD*KD*max(dotNL, 0.0);

	//out of light
	vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp.x /= tmp.w;
	tmp.y /= tmp.w;
	float val = tmp.x*tmp.x + tmp.y*tmp.y;
	if(val > 1.0)return 0.1*colour;

	//in the shadow
	tmp = (tmp+1.0)*0.5;
	if(tmp.z+0.45 > tmp.w*texture(shadowmap, tmp.xy).r)return 0.2*colour;

	//greater than 90 degree
	colour *= texture(prjtormap, tmp.xy).bgr;
	if(dotNL < 0.0)return colour;

	//have specular
	vec3 E = normalize(camxyz - objxyz);
	vec3 H = normalize(E + L);
	float NH = max(dot(N, H), 0.0);
	return colour + LS*KS*pow(NH, 25.0);
}

vec3 pointlight(){
	vec3 N = normalize(normal);
	vec3 L = normalize(sunxyz - objxyz);
	float SN = dot(N, L);
	vec3 ret = LA*KA + LD*KD*max(SN, 0.0);
	if(SN < 0.0)return ret;

	vec3 E = normalize(camxyz - objxyz);
	vec3 H = normalize(E + L);
	float NH = max(dot(N, H), 0.0);
	return ret + LS*KS*pow(NH, 25.0);
}


void main(){
	vec3 c = vec3(0.0);
	switch(lighttype){
	case 20:c=dirlight();break;
	case 21:c=spotlight();break;
	case 22:c=projector();break;
	case 23:c=pointlight();break;
	case 24:c=imagelight();break;
	default:c=pbrcolor();break;
	}
	FragColor = vec4(c, 1.0);
}
