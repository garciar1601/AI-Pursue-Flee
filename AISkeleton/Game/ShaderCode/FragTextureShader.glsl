#version 400

in vec2 theUV;
in vec3 thePosition;
in vec3 theNormal;

uniform sampler2D meTexture;
uniform vec3 diffuseLightPos;
uniform vec3 ambient;

out vec3 finalColor;

void main(){
	vec3 lightVector = normalize(diffuseLightPos - thePosition);
	float brightness = clamp(dot(lightVector, theNormal), 0, 1);

	finalColor = vec3(texture(meTexture, theUV)) * clamp(ambient + vec3(brightness, brightness, brightness), 0, 1);
}