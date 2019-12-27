#version 410 core
in mediump vec3 vertex;
in mediump vec3 normal;
in mediump vec3 albedo;
layout(location = 0)out mediump vec4 o0;
layout(location = 1)out mediump vec4 o1;
layout(location = 2)out mediump vec4 o2;
layout(location = 3)out mediump vec4 o3;

void main(){
	o0 = vec4(1.0, 0.0, 1.0, 1.0);
	o1 = vec4(albedo, 1.0);
	o2 = vec4(vertex, 1.0);
	o3 = vec4(normal, 1.0);
}
