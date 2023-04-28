layout(location = 0)in vec3 v;
layout(location = 1)in vec2 t;
out vec3 normal;
out vec2 texuvw;
out vec3 angle0;
uniform vec3 camxyz;
uniform mat4 cammvp;
void main(){
	texuvw = t;
	normal = vec3(0.0, 0.0, 1.0);
	angle0 = camxyz - v;
	gl_Position = cammvp * vec4(v, 1.0);
}