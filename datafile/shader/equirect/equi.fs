in vec2 outuv;
out vec4 FragColor;
uniform sampler2D tex0;
void main(){
	FragColor = vec4(texture(tex0, outuv).bgr, 1.0);
	//"FragColor = vec4(texture(tex0, vec2(outuv.x, outuv.y)).bgr, 1.0);
	//"FragColor = vec4(outuv, 0.0, 1.0);
}