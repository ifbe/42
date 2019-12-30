#version 410 core
in mediump vec3 uvw;
out mediump vec4 FragColor;
float fun(float x, float y){
    return sin(x*x*x)+cos(y*y);
}
void main(){
    mediump float dx=0.01;
    mediump float dy=0.01;
    mediump float xx = fun(uvw.x-dx,uvw.y) * fun(uvw.x+dx,uvw.y);
    mediump float yy = fun(uvw.x,uvw.y-dy) * fun(uvw.x,uvw.y+dy);
    if(xx<0||yy<0)FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    else FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    //FragColor = vec4(texture(tex0, uvw).bgr, 1.0);
}