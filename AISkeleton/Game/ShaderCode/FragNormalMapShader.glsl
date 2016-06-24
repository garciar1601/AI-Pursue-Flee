#version 400

in vec2 theUV;
in vec3 thePosition;
in mat3 TBN;

uniform sampler2D meTexture;
uniform sampler2D color;
uniform sampler2D diffuse;
uniform vec3 diffuseLightPos;
uniform vec3 diffuseColor;
uniform vec3 cameraPos;
uniform vec3 ambient;
uniform vec3 specularColor;
uniform float specularExponent;

out vec3 finalColor;

void main(){
	vec3 mapNormal = 2 * vec3(texture(meTexture, theUV)) - 1;

	vec3 textureNormal = normalize(TBN * mapNormal);

	vec3 texColor = vec3(texture(color, theUV));

	vec3 texDiffuse = vec3(texture(diffuse, theUV));

	vec3 lightVector = normalize(diffuseLightPos - thePosition);
	
	float brightness = clamp(dot(lightVector, textureNormal), 0, 1);

	vec3 reflectedLight = reflect(-lightVector, textureNormal);
	vec3 eyeVector = normalize(cameraPos - thePosition);
	float specular = clamp(dot(reflectedLight, eyeVector), 0, 1);
	specular = pow(specular, specularExponent);

	finalColor = texColor + texDiffuse * clamp(ambient + (brightness * diffuseColor) + (specular * specularColor), 0, 1);
}