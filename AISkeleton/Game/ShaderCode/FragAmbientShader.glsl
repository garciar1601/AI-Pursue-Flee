#version 400

in vec3 deColor;

uniform vec3 ambient;

out vec3 finalColor;

void main(){
	finalColor = deColor * ambient;
}