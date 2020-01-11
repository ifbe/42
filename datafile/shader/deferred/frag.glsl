#version 410 core
in mediump vec2 uvw;
out mediump vec4 FragColor;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
void main(){
	vec3 albedo = texture(tex0, uvw).rgb;
	vec3 depth0 = vec3(1.0-texture(tex3, uvw).r);
	FragColor = vec4(albedo*depth0, 1.0);
}