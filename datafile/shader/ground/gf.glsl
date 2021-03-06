in mediump vec2 texuvw;
in mediump vec3 depths;
layout(location = 0)out mediump vec4 o0;
layout(location = 1)out mediump vec4 o1;
layout(location = 2)out mediump vec4 o2;
layout(location = 3)out mediump vec4 o3;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

void main(){
	o0 = vec4(texture(tex0, texuvw).bgr, 1.0);
	o1 = vec4(texture(tex1, texuvw).bgr, 1.0);
	o2 = vec4(texture(tex2, texuvw).bgr, 1.0);
	o3 = vec4(depths, 1.0);
}
