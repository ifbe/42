#version 410 core
in vec2 uvw;
out vec4 FragColor;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
void main(){
	vec3 tmp;
	if(uvw.y < 0.25){
		if(uvw.x < 0.25){	//albedo.r
			float c = texture(tex0, uvw).r;
			FragColor = vec4(c, 0.0, 0.0, 1.0);
		}
		else if(uvw.x < 0.5){	//albedo.g
			float c = texture(tex0, uvw).g;
			FragColor = vec4(0.0, c, 0.0, 1.0);
		}
		else if(uvw.x < 0.75){	//albedo.b
			float c = texture(tex0, uvw).b;
			FragColor = vec4(0.0, 0.0, c, 1.0);
		}
	}
	else if(uvw.y < 0.5){	//normal
		tmp = texture(tex1, uvw).rgb*2.0-1.0;
		FragColor = vec4(tmp, 1.0);
	}
	else if(uvw.y < 0.75){
		if(uvw.x < 0.25){	//metal
			float c = texture(tex2, uvw).r;
			FragColor = vec4(vec3(c), 1.0);
		}
		else if(uvw.x < 0.5){	//rough
			float c = texture(tex2, uvw).g;
			FragColor = vec4(vec3(c), 1.0);
		}
		else if(uvw.x < 0.75){	//ambie
			float c = texture(tex2, uvw).b;
			FragColor = vec4(vec3(c), 1.0);
		}
	}
	else{
		if(uvw.x < 0.75){	//unlit
			tmp = texture(tex3, uvw).rgb;
			FragColor = vec4(tmp, 1.0);
		}
		else{	//depth
			float d = texture(tex4, uvw).r;
			d = 2.0 / (10000.0 - 9999.0*d);
			tmp = vec3(d,d,d);
			FragColor = vec4(tmp, 1.0);
		}
	}
}
