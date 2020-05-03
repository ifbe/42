layout(location = 0)in vec3 v;
layout(location = 1)in vec2 t;
out vec3 objxyz;
uniform mat4 cammvp;
void main(){
	objxyz = v;
	gl_Position = cammvp * vec4(v, 1.0);
}
