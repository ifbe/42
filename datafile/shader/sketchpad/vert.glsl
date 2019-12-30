#version 410 core
layout(location = 0)in mediump vec3 vertex;
layout(location = 1)in mediump vec3 texuvw;
uniform mat4 cammvp;
out mediump vec3 uvw;
void main(){
    uvw = texuvw;
    gl_Position = cammvp * vec4(vertex, 1.0);
}