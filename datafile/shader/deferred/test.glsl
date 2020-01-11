#version 410 core
in mediump vec2 uvw;
out mediump vec4 FragColor;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
void main(){
	if((uvw.x < 0.5)&&(uvw.y < 0.5))FragColor = vec4(texture(tex0, vec2(uvw.x*2.0-0.0, uvw.y*2.0-0.0)).rgb, 1.0);
	if((uvw.x > 0.5)&&(uvw.y < 0.5))FragColor = vec4(texture(tex1, vec2(uvw.x*2.0-1.0, uvw.y*2.0-0.0)).rgb, 1.0);
	if((uvw.x < 0.5)&&(uvw.y > 0.5))FragColor = vec4(texture(tex2, vec2(uvw.x*2.0-0.0, uvw.y*2.0-1.0)).rgb, 1.0);
	if((uvw.x > 0.5)&&(uvw.y > 0.5))FragColor = vec4(texture(tex3, vec2(uvw.x*2.0-1.0, uvw.y*2.0-1.0)).rgb, 1.0);
}