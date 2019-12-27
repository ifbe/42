#version 410 core
layout(location = 0)in mediump vec3 vert;
layout(location = 1)in mediump vec3 norm;
out mediump vec3 vertex;
out mediump vec3 normal;
out mediump vec3 albedo;
uniform mat4 objmat;
uniform mat4 cammvp;
void main(){
	mediump vec4 tmp = cammvp * objmat * vec4(vert, 1.0);
	mediump float d = tmp.z/tmp.w;
	mediump float n = 1.0;
	mediump float f = 3000.0;
	mediump float c = (2.0 * n) / (f + n - d * (f - n));
	albedo = vec3(c, c, c);
	normal = norm;
	vertex = vec3(objmat * vec4(vert, 1.0));
	gl_Position = tmp;
}
