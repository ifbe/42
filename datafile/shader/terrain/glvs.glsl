#version 410 core
layout(location = 0)in mediump vec3 v;
layout(location = 1)in mediump vec3 n;
layout(location = 2)in mediump vec3 t;
out mediump vec3 objxyz;
out mediump vec3 normal;
out mediump vec3 texuvw;
//out mediump vec3 uvw;
//out mediump vec3 xyz;
uniform mat4 objmat;
uniform mat4 cammvp;
void main(){
	mediump vec4 xyzw = objmat * vec4(v, 1.0);
	objxyz = vec3(xyzw);
	normal = n;
	texuvw = t;
	gl_Position = cammvp * xyzw;
}