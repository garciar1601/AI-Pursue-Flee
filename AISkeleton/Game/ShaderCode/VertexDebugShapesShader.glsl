#version 400

in layout(location = 0) vec3 position;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec2 uv;

uniform mat4 mvp;
uniform vec3 uniformColor;

out vec3 deColor;

void main(){
	gl_Position = mvp * vec4(position, 1);
	deColor = uniformColor;
}