layout(location = 0)in vec3 v;
layout(location = 1)in vec3 n;
layout(location = 2)in vec3 t;
out vec3 objxyz;
out vec3 normal;
out vec3 texuvw;
//out vec3 uvw;
//out vec3 xyz;
uniform mat4 objmat;
uniform mat4 cammvp;
void main(){
	vec4 xyzw = objmat * vec4(v, 1.0);
	objxyz = vec3(xyzw);
	normal = n;
	texuvw = t;
	gl_Position = cammvp * xyzw;
}
