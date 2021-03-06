#version 410 core
layout(location = 0)in mediump vec3 vtx;
layout(location = 1)in mediump vec2 tex;
layout(location = 2)in mediump vec3 nrm;
layout(location = 3)in mediump vec3 tgt;
out mediump vec3 objxyz;
out mediump vec2 texcoo;
out mediump vec3 normal;
out mediump vec3 tangen;
uniform mat4 objmat;
uniform mat4 cammvp;
void main(){
	objxyz = vtx;
	texcoo = tex;
	normal = nrm;
	tangen = tgt;
	gl_Position = cammvp * objmat * vec4(vtx, 1.0);
}
