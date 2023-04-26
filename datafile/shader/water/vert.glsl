layout(location = 0)in mediump vec3 v;
layout(location = 1)in mediump vec2 t;
out mediump vec3 normal;
out mediump vec2 texuvw;
out mediump vec3 angle0;
uniform vec3 camxyz;
uniform mat4 cammvp;
void main(){
	texuvw = t;
	normal = vec3(0.0, 0.0, 1.0);
	angle0 = camxyz - v;
	gl_Position = cammvp * vec4(v, 1.0);
}