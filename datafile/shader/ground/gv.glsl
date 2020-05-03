layout(location = 0)in mediump vec3 vert;
layout(location = 1)in mediump vec2 texc;
out mediump vec2 texuvw;
out mediump vec3 depths;
uniform mat4 cammv_;
uniform mat4 cammvp;
uniform vec3 camxyz;
void main(){
	vec4 tmp = cammv_ * vec4(vert, 1.0);
	depths.z = -tmp.z/10000.0;
	texuvw = texc;
	gl_Position = cammvp * vec4(vert, 1.0);
}
