in vec3 objxyz;
in vec3 normal;
in vec3 texuvw;
out vec4 FragColor;
uniform sampler2D rgbtex;
uniform vec3 camxyz;
uniform vec3 matter;

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
void main(){
	vec3 albedo = pow(texture(rgbtex, texuvw.xy).bgr, vec3(2.2));

	float metal = matter.x;
	float rough = matter.y;
	float amocc = matter.z;

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
	ocolor = pow(ocolor, vec3(1.0/2.2));
	FragColor = vec4(ocolor, 1.0);
}
