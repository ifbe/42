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
	if(uvw.y < 0.333333333){
		if(uvw.x < 0.333333333){	//albedo
			tmp = texture(tex0, uvw).rgb;
			FragColor = vec4(tmp, 1.0);
		}
		else if(uvw.x < 0.666666666){	//normal
			tmp = texture(tex1, uvw).rgb*2.0-1.0;
			FragColor = vec4(tmp, 1.0);
		}
	}
	else if(uvw.y < 0.666666666){
		if(uvw.x < 0.333333333){	//metal
			float c = texture(tex2, uvw).r;
			FragColor = vec4(vec3(c), 1.0);
		}
		else if(uvw.x < 0.666666666){	//rough
			float c = texture(tex2, uvw).g;
			FragColor = vec4(vec3(c), 1.0);
		}
		else if(uvw.x < 0.999999999){	//ambie
			float c = texture(tex2, uvw).b;
			FragColor = vec4(vec3(c), 1.0);
		}
	}
	else{
		if(uvw.x < 0.333333333){	//depth0
			float d = texture(tex3, uvw).b;
			tmp = vec3(d,d,d);
			FragColor = vec4(tmp, 1.0);
		}
		else if(uvw.x < 0.666666666){	//depth
			float d = texture(tex4, uvw).r;
			d = 2.0 / (10000.0 - 9999.0*d);
			tmp = vec3(d,d,d);
			FragColor = vec4(tmp, 1.0);
		}
	}
}
