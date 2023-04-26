in mediump vec2 uvw;
out mediump vec4 FragColor;
uniform sampler2D tex0;
void main(){
	mediump float t = 4.0*(uvw.x-0.5)*(uvw.x-0.5) + 4.0*(uvw.y-0.5)*(uvw.y-0.5);
	mediump vec3 c = texture(tex0, uvw).rgb;
	if(t > 1.0)discard;
	if(t > 0.8)c += 25.0*(t-0.8)*(t-0.8)*vec3(1.0, 1.0, 1.0);
	FragColor = vec4(c, 1.0);
}
