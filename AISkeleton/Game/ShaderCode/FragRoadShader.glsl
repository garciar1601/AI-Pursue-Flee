#version 400

in vec2 theUV;
in vec3 theNormal;
in vec3 thePosition;
in mat3 TBN;

uniform sampler2D meTexture;
uniform sampler2D diffuse;
uniform vec3 diffuseLightPos;
uniform vec3 diffuseColor;
uniform vec3 cameraPos;
uniform vec3 ambient;
uniform vec3 specularColor;
uniform float specularExponent;

out vec3 finalColor;

void main(){
	vec3 texColor = vec3(texture(meTexture, theUV));

	vec3 lightVector = normalize(diffuseLightPos - thePosition);
	
	float brightness = clamp(dot(lightVector, theNormal), 0, 1);

	vec3 reflectedLight = reflect(-lightVector, theNormal);
	vec3 eyeVector = normalize(cameraPos - thePosition);
	float specular = clamp(dot(reflectedLight, eyeVector), 0, 1);
	specular = pow(specular, specularExponent);

	finalColor = clamp(ambient + (brightness * diffuseColor) + (specular * specularColor), 0, 1);
}