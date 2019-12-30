#version 410 core
layout(location = 0)in mediump vec3 vert;
layout(location = 1)in mediump vec3 norm;
layout(location = 2)in mediump vec3 texc;
out mediump vec3 vertex;
out mediump vec3 normal;
out mediump vec3 texuvw;
out mediump vec3 depths;
uniform mat4 cammvp;
void main(){
	mediump vec4 tmp = cammvp * vec4(vert, 1.0);
	mediump float d = tmp.z/tmp.w;
	mediump float n = 1.0;
	mediump float f = 10000.0;
	mediump float c = (2.0 * n) / (f + n - d * (f - n));
	depths = vec3(c, c, c);
	texuvw = texc;
	normal = norm;
	vertex = vert;
	gl_Position = tmp;
}
