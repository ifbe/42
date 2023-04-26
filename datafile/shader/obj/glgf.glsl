in mediump vec3 texuvw;
in mediump vec3 normal;
in mediump vec3 tangen;
layout(location = 0)out mediump vec4 o0;
layout(location = 1)out mediump vec4 o1;
layout(location = 2)out mediump vec4 o2;
layout(location = 3)out mediump vec4 o3;
uniform sampler2D albedomap;
uniform sampler2D normalmap;
uniform sampler2D mattermap;

mediump vec3 getnormal(){
	vec3 XYZ = texture(normalmap, texuvw.xy).bgr * 2.0 - vec3(1.0);

	vec3 N = normalize(normal);
	vec3 T = normalize(tangen);
	T = normalize(T - N * dot(T, N));
	vec3 B = cross(N, T);
	mat3 TBN = mat3(T, B, N);
	return TBN * XYZ;
}
void main(){
	o0 = vec4(texture(albedomap, texuvw.xy).bgr, 1.0);
	o1 = vec4(getnormal()/2.0+vec3(0.5), 1.0);
	o2 = vec4(texture(mattermap, texuvw.xy).bgr, 1.0);
	o3 = vec4(0.5, 0.3, 0.7, 1.0);
}
