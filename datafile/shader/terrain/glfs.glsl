#version 410 core
in mediump vec3 objxyz;
in mediump vec3 normal;
in mediump vec3 texuvw;
out mediump vec4 FragColor;
uniform sampler2D rgbtex;
uniform mediump vec3 camxyz;
uniform mediump vec3 matter;

mediump float metal = matter.x;
mediump float rough = matter.y;
mediump float amocc = matter.z;

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
void main(){
	mediump vec3 albedo = pow(texture(rgbtex, texuvw.xy).bgr, vec3(2.2));

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

		mediump float G = getG(NdotL, rough)*getG(NdotE, rough);
		mediump float D = getD(NdotH, rough);
		mediump vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotE, 5.0);

		mediump vec3 kS = F;
		mediump vec3 kD = (vec3(1.0) - kS) * (1.0 - metal);
		mediump vec3 specular = (D * G * F) / max(4.0 * NdotE * NdotL, 0.001);
		ocolor += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	ocolor = ocolor / (ocolor + vec3(1.0));
	ocolor = pow(ocolor, vec3(1.0/2.2));
	FragColor = vec4(ocolor, 1.0);
}