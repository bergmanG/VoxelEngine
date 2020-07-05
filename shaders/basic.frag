#version 400
layout (location = 0) out vec4 FragColor;

in vec3 color;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 LightPos;

void main() {

vec3 lightDir = normalize(LightPos-FragPos);  
float diff = max(dot(lightDir, Normal), 0.0);
FragColor = vec4(color*(diff + 0.5),1.0);
}
