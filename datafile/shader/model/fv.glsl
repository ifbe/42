layout(location = 0)in mediump vec3 v;
layout(location = 1)in mediump vec3 n;
out mediump vec3 objxyz;
out mediump vec3 normal;
uniform mat4 objmat;
uniform mat4 cammvp;
void main(){
	mediump vec4 xyzw = objmat * vec4(v, 1.0);
	objxyz = vec3(xyzw);
	normal = n;
	gl_Position = cammvp * xyzw;
}
