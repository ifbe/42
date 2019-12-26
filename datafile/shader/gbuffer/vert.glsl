#version 410 core
layout(location = 0)in mediump vec3 v;
layout(location = 1)in mediump vec3 n;
out mediump vec3 vertex;
out mediump vec3 normal;
uniform mat4 objmat;
uniform mat4 cammvp;
void main(){
	normal = n;
	vertex = vec3(objmat * vec4(v, 1.0));
	gl_Position = cammvp * objmat * vec4(v, 1.0);
}
