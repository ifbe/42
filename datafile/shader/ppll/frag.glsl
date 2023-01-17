#version 430 core
precision mediump float;
in vec2 uvw;
out vec4 FragColor;
uniform mediump vec3 camxyz;
uniform mediump mat4 invwvp;
layout(binding = 0, offset = 0) uniform atomic_uint cur;
layout(binding = 0, offset = 4) uniform atomic_uint cnt;
struct ppllheaddef{
    uint prev;
    uint count;
};
layout (std430, binding = 0) buffer ppllheadbuf{
    ppllheaddef ppllhead[];
};
struct pplldatadef{
    uint prev;
    uint temp;
    uint color;
    float depth;
};
layout (std430, binding = 1) buffer pplldatabuf{
    pplldatadef pplldata[];
};
vec4 uinttocolor(uint count){
    vec4 tmp = vec4(1.0, 1.0, 1.0, 1.0);
    if(count==0)tmp = vec4(0.0, 0.0, 0.0, 1.0);
    else if(count==1)tmp = vec4(1.0, 0.0, 0.0, 1.0);
    else if(count==2)tmp = vec4(0.0, 1.0, 0.0, 1.0);
    else if(count==3)tmp = vec4(1.0, 1.0, 0.0, 1.0);
    else if(count==4)tmp = vec4(0.0, 0.0, 1.0, 1.0);
    else if(count==5)tmp = vec4(1.0, 0.0, 1.0, 1.0);
    else if(count==6)tmp = vec4(0.0, 1.0, 1.0, 1.0);
    return tmp;
};
void main(){
	uint x = uint(gl_FragCoord.x);
	uint y = uint(gl_FragCoord.y);
	uint headindex = y*1024 + x;
    uint datacount = ppllhead[headindex].count;
    if(datacount < 1)discard;
    if(datacount > 4)datacount = 4;
    //FragColor = uinttocolor(datacount);

    uint dataindex = ppllhead[headindex].prev;
    uint color[4] = {0,0,0,0};
    float depth[4] = {1000000000.0,1000000000.0,1000000000.0,1000000000.0};
    for(uint j;j<datacount;j++){
        for(uint k=0;k<4;k++){
            if(depth[k] > pplldata[dataindex].depth){
                for(uint t=3;t>k;t--){
                    color[t] = color[t-1];
                    depth[t] = depth[t-1];
                }
                color[k] = pplldata[dataindex].color;
                depth[k] = pplldata[dataindex].depth;
                break;
            }
        }
        dataindex = pplldata[dataindex].prev;
    }
    vec4 unpack0 = unpackUnorm4x8(color[0]);
    vec4 unpack1 = unpackUnorm4x8(color[1]);
    vec4 unpack2 = unpackUnorm4x8(color[2]);
    vec4 unpack3 = unpackUnorm4x8(color[3]);
    vec3 c = vec3(0.0);
    c = unpack3.xyz*unpack3.w + c*(1.0-unpack3.w);
    c = unpack2.xyz*unpack2.w + c*(1.0-unpack2.w);
    c = unpack1.xyz*unpack1.w + c*(1.0-unpack1.w);
    c = unpack0.xyz*unpack0.w + c*(1.0-unpack0.w);
    FragColor = vec4(c, 1.0);
}