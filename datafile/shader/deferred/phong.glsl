in mediump vec2 uvw;
out mediump vec4 FragColor;
uniform sampler2D tex0;	//albedo
uniform sampler2D tex1;	//normal
uniform sampler2D tex2;	//metal, rough
uniform sampler2D tex3;	//thermal, velocity
uniform sampler2D tex4;	//depth
uniform mediump vec3 camxyz;
uniform mediump mat4 invwvp;
void main(){
	//posxyz
	mediump float d = texture(tex4, uvw).r;
	mediump vec4 screenxyzw = vec4(uvw.x*2.0-1.0, uvw.y*2.0-1.0, d*2.0-1.0, 1.0);
	mediump vec4 worldxyzw = invwvp * screenxyzw;
	mediump vec3 posxyz = worldxyzw.xyz / worldxyzw.w;
	//normal
	mediump vec3 normal = normalize(texture(tex1, uvw).rgb*2.0-1.0);
	//ambient
	mediump vec3 final = pow(texture(tex0, uvw).rgb, vec3(2.2));
	//diffuse
	mediump vec3 N = normalize(normal);
	mediump vec3 L = vec3(0.0, 0.0, 1.0);
	mediump float NL = dot(N, L);
	final += vec3(0.1) * max(NL, 0.0);
	//specular
	if(NL > 0.0){
		vec3 E = normalize(camxyz - posxyz);
		vec3 H = normalize(E + L);
		float NH = clamp(dot(N, H), 0.0, 1.0);
		final += vec3(0.1)*pow(NH, 189.6);
	}
	final = pow(final, vec3(1.0/2.2));
	FragColor = vec4(final, 1.0);
}
