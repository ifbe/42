#version 410 core
in mediump vec3 vertex;
in mediump vec3 normal;
layout(location = 0)out mediump vec4 vert;
layout(location = 1)out mediump vec4 norm;

void main(){
	vert = vec4(vertex, 1.0);
	norm = vec4(normal, 1.0);
}
