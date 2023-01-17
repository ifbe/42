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
    uint color = 0;
    float depth = 1000000000.0;
    for(uint j;j<datacount;j++){
        if(depth > pplldata[dataindex].depth){
            color = pplldata[dataindex].color;
            depth = pplldata[dataindex].depth;
        }
        dataindex = pplldata[dataindex].prev;
    }
    vec4 unpack0 = unpackUnorm4x8(color);
    FragColor = vec4(vec3(unpack0), 1.0);
}