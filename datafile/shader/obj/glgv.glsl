layout(location = 0)in mediump vec3 vert;
layout(location = 1)in mediump vec3 texc;
layout(location = 2)in mediump vec3 norm;
layout(location = 3)in mediump vec3 tang;
out mediump vec3 texuvw;
out mediump vec3 normal;
out mediump vec3 tangen;
uniform mat4 objmat;
uniform mat4 cammv_;
uniform mat4 cammvp;
uniform vec3 camxyz;


void main(){
	texuvw = texc;
	normal = vec3(objmat * vec4(norm, 0.0));
	tangen = vec3(objmat * vec4(tang, 0.0));
	gl_Position = cammvp * objmat * vec4(vert, 1.0);
}
