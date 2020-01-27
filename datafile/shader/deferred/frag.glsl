#version 410 core
in mediump vec2 uvw;
out mediump vec4 FragColor;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
void main(){
	mediump vec3 albedo = texture(tex0, uvw).rgb;
	mediump vec3 normal = texture(tex1, uvw).rgb;
	mediump float zdepth = 1.0-texture(tex3, uvw).b;
	FragColor = vec4(albedo*zdepth, 1.0);
}
