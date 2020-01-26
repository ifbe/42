#version 410 core
layout(location = 0)in mediump vec3 vert;
layout(location = 1)in mediump vec3 norm;
layout(location = 2)in mediump vec3 texc;
out mediump vec3 vertex;
out mediump vec3 normal;
out mediump vec3 texuvw;
out mediump vec3 depths;
uniform mat4 cammvp;
uniform mat4 viewmat;
void main(){
	vec4 tmp = viewmat * vec4(vert, 1.0);
	depths.z = -tmp.z/10000.0;
	texuvw = texc;
	normal = norm;
	vertex = vert;
	gl_Position = cammvp * vec4(vert, 1.0);
}
