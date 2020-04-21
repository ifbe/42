#version 410 core
layout(location = 0)in mediump vec3 v;
layout(location = 1)in mediump vec2 n;
layout(location = 2)in mediump vec2 t;
out mediump vec3 objxyz;
out mediump vec3 normal;
uniform mat4 cammvp;
void main(){
	objxyz = v;
	normal = vec3(0.0, 0.0, 1.0);
	gl_Position = cammvp * vec4(v, 1.0);
}
