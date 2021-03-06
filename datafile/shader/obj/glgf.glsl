#version 410 core
in mediump vec3 normal;
in mediump vec3 texuvw;
in mediump vec3 depths;
layout(location = 0)out mediump vec4 o0;
layout(location = 1)out mediump vec4 o1;
layout(location = 2)out mediump vec4 o2;
layout(location = 3)out mediump vec4 o3;
uniform sampler2D albedomap;
uniform sampler2D mattermap;

void main(){
	o0 = vec4(texture(albedomap, texuvw.xy).bgr, 1.0);
	o1 = vec4(normal, 1.0);
	o2 = vec4(texture(mattermap, texuvw.xy).bgr, 1.0);
	o3 = vec4(depths, 1.0);
}
