layout(location = 0)in vec3 vertex;
layout(location = 1)in vec2 texuvw;
uniform int camtype;
uniform mat4 cammv_;
uniform mat4 cammvp;
out vec2 outuv;
const float MYPI=3.1415926589793;
void main(){
	switch(camtype){
	case 30:
		vec4 tmp1 = cammv_ * vec4(vertex, 1.0);
		vec3 vec = tmp1.xyz;
		vec3 front = vec3(0,1,0);
		float len = length(vec);
		float angle = acos(dot(front, vec) / len);
		float range = MYPI / 2;
		vec2 uv = vec2(tmp1.x, tmp1.z);
		uv *= angle / range / length(uv);
		float w = (10000000-len) / (10000000-1);
		if(angle > range)w=99;
		gl_Position = vec4(uv, w, 1.0);
		break;
	case 20:
		vec4 tmp = cammv_ * vec4(vertex, 1.0);
		float sq2 = length(vec2(tmp.x, tmp.y));
		float sq3 = length(vec2(sq2, tmp.z));
		float tu = -atan(-tmp.x, tmp.y)/MYPI;
		float tv = atan(tmp.z, sq2)*2/MYPI;
		float tw = (10000000-sq3) / (10000000-1);
		if(texuvw.x<0.001||texuvw.x>0.999||texuvw.y<0.001||texuvw.y>0.999)tw=999;
		gl_Position = vec4(tu, tv, tw, 1.0);
		break;
	default:
		gl_Position = cammvp * vec4(vertex, 1.0);
		break;
	}
	outuv = texuvw;
}