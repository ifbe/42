#version 410 core
layout(location = 0)in mediump vec3 vert;
layout(location = 1)in mediump vec3 norm;
layout(location = 2)in mediump vec3 texc;
out mediump vec3 normal;
out mediump vec3 texuvw;
uniform mat4 objmat;
uniform mat4 cammv_;
uniform mat4 cammvp;
uniform vec3 camxyz;
void main(){
	vec4 obj = objmat * vec4(vert, 1.0);
	vec4 tmp = cammv_ * obj;
	texuvw = texc;
	normal = norm;
	gl_Position = cammvp * obj;
}
