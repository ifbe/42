#version 410 core
in mediump vec2 uvw;
out mediump vec4 FragColor;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
void main(){
	mediump vec3 albedo = pow(texture(tex0, uvw).rgb, vec3(2.2));
	mediump vec3 normal = texture(tex1, uvw).rgb*2.0-1.0;
	mediump vec3 matter = texture(tex2, uvw).bgr;
	mediump float zdepth = 1.0-texture(tex3, uvw).b;
	mediump vec3 final = normal;
	FragColor = vec4(pow(final, vec3(1.0/2.2)), 1.0);
}
