#version 400

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 color;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec2 uv;

uniform mat4 mvp;
uniform mat4 modelToWorld;
uniform mat4 rotation;

out vec2 theUV;
out vec3 thePosition;
out vec3 theNormal;

void main(){
	gl_Position = mvp * vec4(position, 1);
	vec4 lightNormal = rotation * vec4(normal, 1);
	vec4 lightPosition = modelToWorld * vec4(position, 1);
	theNormal = vec3(lightNormal);
	thePosition = vec3(lightPosition);
	theUV = uv;
}