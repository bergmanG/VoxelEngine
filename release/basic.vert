#version 400
layout (location = 0)in uint vertex_info;
layout (location = 1)in uint block_color;


uniform vec3 world_pos;
uniform mat4 Model;
uniform mat4 MVP;

out vec3 color;
out vec3 FragPos;
out vec3 Normal;

vec3 normals[6] = vec3[6](
	vec3(0,-1,0),
	vec3(0,1,0),
	vec3(1,0,0),
	vec3(-1,0,0),
	vec3(0,0,-1),
	vec3(0,0,1)
);

void main() {

	
	float side = float((vertex_info & 0x78000) >> 15u);
	float x = float((vertex_info & 0x7C00) >> 10u);
	float y = float((vertex_info & 0x3E0) >> 5u);
	float z = float(vertex_info & 0x1F);
	
	x+= world_pos.x;
	y+= world_pos.y;
	z+= world_pos.z;

	float b = float((block_color & 0xFF0000)>>16u)/255;
	float g = float((block_color & 0xFF00)>>8u)/255;
	float r = float(block_color & 0XFF)/255;
	if(side == 0) {
        color = 0.4*vec3(r,g,b);
    } else if(side == 1) {
        color = vec3(r,g,b);
    } else if(side == 2 || side == 3) {
        color = 0.8*vec3(r,g,b);
    } else if(side == 4||side == 5) {
        color = 0.6*vec3(r,g,b);
    }else{
		color = vec3(1,0,0);
	}
	Normal = normals[int(side)];
	FragPos = vec3(Model*vec4(x,y,z,1.0));
    gl_Position = MVP*vec4(x,y,z,1.0);
}
