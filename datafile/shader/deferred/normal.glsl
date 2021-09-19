#version 410 core
in mediump vec2 uvw;
out mediump vec4 FragColor;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
void main(){
	mediump vec3 normal = texture(tex1, uvw).rgb;
	FragColor = vec4(normal, 1.0);
}
