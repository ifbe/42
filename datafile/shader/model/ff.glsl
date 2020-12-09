#version 410 core
in mediump vec3 objxyz;
in mediump vec3 normal;
out mediump vec4 FragColor;
subroutine vec3 passtype();
subroutine uniform passtype routine;

uniform mediump vec3 matter;
uniform mediump vec3 camxyz;
uniform mediump vec3 sunxyz;
uniform mediump vec3 sundir;
uniform mediump vec3 sunrgb;
uniform sampler2D shadowmap;
uniform sampler2D prjtormap;
uniform sampler2D iblenvmap;
uniform mat4 sunmvp;
mediump vec3 LA = sunrgb;
mediump vec3 LD = sunrgb;
mediump vec3 LS = sunrgb;
mediump vec3 KA = vec3(0.231250, 0.231250, 0.231250);
mediump vec3 KD = vec3(0.277500, 0.277500, 0.277500);
mediump vec3 KS = vec3(0.773911, 0.773911, 0.773911);

mediump vec3 albedo = vec3(1.0, 1.0, 1.0);
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
subroutine (passtype) vec3 pbrcolor(){
	mediump float metal = matter.x;
	mediump float rough = matter.y;
	mediump float amocc = matter.z;

	mediump vec3 N = normalize(normal);
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

		mediump vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotE, 5.0);
		mediump float D = getD(NdotH, rough);
		mediump float G = getG(NdotL, rough)*getG(NdotE, rough);

		mediump vec3 kS = F;
		mediump vec3 kD = (vec3(1.0) - kS) * (1.0 - metal);
		mediump vec3 specular = (D * G * F) / max(4.0 * NdotE * NdotL, 0.001);
		ocolor += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	ocolor = ocolor / (ocolor + vec3(1.0));
	return pow(ocolor, vec3(1.0/2.2));
}


mediump float iblG(mediump float v, mediump float r){
    float k = r*r / 2.0;
    return v / (v * (1.0 - k) + k);
}
subroutine (passtype) vec3 imagelight(){
	mediump float metal = matter.x;
	mediump float rough = matter.y;
	mediump float amocc = matter.z;

	mediump vec3 N = normalize(normal);
	mediump vec3 E = normalize(camxyz - objxyz);
	mediump vec3 R = reflect(E, N);
	mediump vec3 F0 = mix(vec3(0.04), albedo, metal);

	mediump float x = atan(R.y, R.x)/PI/2.0 + 0.5;
	mediump float y = 0.5 + asin(R.z)/PI;
	mediump vec3 litrgb = texture(iblenvmap, vec2(x,y)).bgr;
	mediump vec3 litdir = -R;

	mediump vec3 ocolor = vec3(0.03) * albedo * amocc;
	//for(int j=0;j<4;j++){
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

		mediump vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotE, 5.0);
		mediump float D = getD(NdotH, rough);
		mediump float G = iblG(NdotL, rough)*iblG(NdotE, rough);

		mediump vec3 kS = F;
		mediump vec3 kD = (vec3(1.0) - kS) * (1.0 - metal);
		mediump vec3 specular = (D * G * F) / max(4.0 * NdotE * NdotL, 0.001);
		ocolor += (kD * albedo / PI + specular) * radiance * NdotL;
	//}

	ocolor = ocolor / (ocolor + vec3(1.0));
	return pow(ocolor, vec3(1.0/2.2));
}


subroutine (passtype) vec3 dirlight(){
	mediump vec4 tmp = sunmvp * vec4(objxyz, 1.0);
	tmp /= tmp.w;
	tmp = (tmp+1.0)*0.5;

	if( (tmp.x < 0.0) || (tmp.x > 1.0) ||
		(tmp.y < 0.0) || (tmp.y > 1.0) )return vec3(0.1);

	if(tmp.z - texture(shadowmap, tmp.xy).r > 0.0001)return vec3(0.2);

	mediump vec3 N = normalize(normal);
	mediump vec3 L = normalize(-sundir);
	mediump float SN = dot(N, L);
	mediump vec3 ret = LA*KA + LD*KD*max(SN, 0.0);
	if(SN < 0.0)return ret;

	mediump vec3 E = normalize(camxyz - objxyz);
	mediump vec3 H = normalize(E + L);
	mediump float NH = max(dot(N, H), 0.0);
	return ret + LS*KS*pow(NH, 25.0);
}


subroutine (passtype) vec3 spotlight(){
	mediump vec3 N = normalize(normal);
	mediump vec3 L = normalize(sunxyz - objxyz);
	mediump float dotNL = dot(N, L);
	mediump vec3 colour = LA*KA + LD*KD*max(dotNL, 0.0);

	//out of light
	mediump vec4 tmp = sunmvp * vec4(objxyz, 1.0);
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
	mediump vec3 E = normalize(camxyz - objxyz);
	mediump vec3 H = normalize(E + L);
	mediump float NH = max(dot(N, H), 0.0);
	return colour + LS*KS*pow(NH, 25.0);
}


subroutine (passtype) vec3 projector(){
	mediump vec3 N = normalize(normal);
	mediump vec3 L = normalize(sunxyz - objxyz);
	mediump float dotNL = dot(N, L);
	mediump vec3 colour = LA*KA + LD*KD*max(dotNL, 0.0);

	//out of light
	mediump vec4 tmp = sunmvp * vec4(objxyz, 1.0);
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
	mediump vec3 E = normalize(camxyz - objxyz);
	mediump vec3 H = normalize(E + L);
	mediump float NH = max(dot(N, H), 0.0);
	return colour + LS*KS*pow(NH, 25.0);
}


subroutine (passtype) vec3 pointlight(){
	mediump vec3 N = normalize(normal);
	mediump vec3 L = normalize(sunxyz - objxyz);
	mediump float SN = dot(N, L);
	mediump vec3 ret = LA*KA + LD*KD*max(SN, 0.0);
	if(SN < 0.0)return ret;

	mediump vec3 E = normalize(camxyz - objxyz);
	mediump vec3 H = normalize(E + L);
	mediump float NH = max(dot(N, H), 0.0);
	return ret + LS*KS*pow(NH, 25.0);
}


void main(){
	FragColor = vec4(routine(), 1.0);
}
