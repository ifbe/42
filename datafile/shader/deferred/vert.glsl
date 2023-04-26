layout(location = 0)in mediump vec3 vertex;
layout(location = 1)in mediump vec2 texuvw;
out mediump vec2 uvw;
void main(){
	uvw = texuvw;
	gl_Position = vec4(vertex, 1.0);
}