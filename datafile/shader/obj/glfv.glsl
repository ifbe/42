#version 410 core
layout(location = 0)in mediump vec3 v;
layout(location = 1)in mediump vec3 n;
layout(location = 2)in mediump vec2 t;
out mediump vec3 objxyz;
out mediump vec3 normal;
out mediump vec2 texcoo;
uniform mat4 objmat;
uniform mat4 cammvp;
void main(){
	objxyz = v;
	normal = n;
	texcoo = t;
	gl_Position = cammvp * objmat * vec4(v, 1.0);
}
