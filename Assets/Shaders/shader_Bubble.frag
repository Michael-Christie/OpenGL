#version 440 core
out vec4 vertColour;	//output colour of vertex
in vec2 textureCoordinate; //tex coords from vertex shader
in vec3 normals;
in vec3 fragmentPosition;
in vec3 lightColour1;
in vec3 lightPosition1;
in vec3 lightColour2;
in vec3 lightPosition2;
in vec3 lightColour3;
in vec3 lightPosition3;
in vec3 position;

in vec3 viewPosition;

uniform sampler2D aTex;		//uniform holding texture info from main programme


void main()
{
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPosition - fragmentPosition);

	vec3 result = vec3(0.0);
//light 1
	//ambient component
	//********************************
	//set the ambient coeff from material
	float lightAmbientStrength1 = 0.01f;
	vec3 objectAmbientReflectionCoeff1 = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient1 = (lightAmbientStrength1 * objectAmbientReflectionCoeff1) * lightColour1;

	
	//diffuse component
	//********************************
	//normalise normal vectors (reset them as unit vectors)
	vec3 nNormal1 = normalize(normals);
	//calculate the light direction from the light position and the fragment position
    vec3 lightDirection1 = normalize(lightPosition1 - fragmentPosition);

	//determine the dot product of normal direction and light direction
	float diffuseStrength1 = max(dot(nNormal1, lightDirection1), 0.0f);
	
	//spector
	
	vec3 reflectDir1 = reflect(-lightDirection1, nNormal1);
	float spec1 = pow(max(dot(viewDir,reflectDir1), 0.0), 256);
	vec3 specular1 = specularStrength * spec1 * lightColour1;

	//combine this with the light colour
	//set the diffuse coeff from material
	vec3 objectDiffuseReflectionCoeff1 = vec3(1.0f, 1.0f, 1.0f);
    vec3 diffuse1 = (diffuseStrength1 * objectDiffuseReflectionCoeff1) * lightColour1;
	
	result += ambient1 + 2 * (diffuse1 + specular1);

//light 2
	//ambient component
	//********************************
	//set the ambient coeff from material
	float lightAmbientStrength2 = 0.01f;
	vec3 objectAmbientReflectionCoeff2 = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient2 = (lightAmbientStrength2 * objectAmbientReflectionCoeff2) * lightColour2;

	
	//diffuse component
	//********************************
	//normalise normal vectors (reset them as unit vectors)
	vec3 nNormal2 = normalize(normals);
	//calculate the light direction from the light position and the fragment position
    vec3 lightDirection2 = normalize(lightPosition2 - fragmentPosition);

	//determine the dot product of normal direction and light direction
	float diffuseStrength2 = max(dot(nNormal2, lightDirection2), 0.0f);
	
	//combine this with the light colour
	//set the diffuse coeff from material
	vec3 objectDiffuseReflectionCoeff2 = vec3(1.0f, 1.0f, 1.0f);
    vec3 diffuse2 = (diffuseStrength2 * objectDiffuseReflectionCoeff2) * lightColour2;
	

	vec3 reflectDir2 = reflect(-lightDirection2, nNormal2);
	float spec2 = pow(max(dot(viewDir,reflectDir2), 0.0), 256);
	vec3 specular2 = specularStrength * spec2 * lightColour2;

	result += ambient2 + 2 * (diffuse2 + specular2);

//light 3
	//ambient component
	//********************************
	//set the ambient coeff from material
	float lightAmbientStrength3 = 0.2f;
	vec3 objectAmbientReflectionCoeff3 = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient3 = (lightAmbientStrength3 * objectAmbientReflectionCoeff3) * lightColour3;

	
	//diffuse component
	//********************************
	//normalise normal vectors (reset them as unit vectors)
	vec3 nNormal3 = normalize(normals);
	//calculate the light direction from the light position and the fragment position
    vec3 lightDirection3 = normalize(lightPosition3 - fragmentPosition);

	//determine the dot product of normal direction and light direction
	float diffuseStrength3 = max(dot(nNormal3, lightDirection3), 0.0f);

	vec3 reflectDir3 = reflect(-lightDirection3, nNormal3);
	float spec3 = pow(max(dot(viewDir,reflectDir3), 0.0), 32);
	vec3 specular3 = specularStrength * spec3 * lightColour3;

	//combine this with the light colour
	//set the diffuse coeff from material
	vec3 objectDiffuseReflectionCoeff3 = vec3(1.0f, 1.0f, 1.0f);
    vec3 diffuse3= (diffuseStrength3 * objectDiffuseReflectionCoeff3) * lightColour3;
	

	result +=  ambient3 + 2 * (diffuse3 + specular3);
	
	//vec4 textureColour = texture(aTex, textureCoordinate) * vec4(sin(fragmentPosition.x * 20),tan(fragmentPosition.z * 20),cos(fragmentPosition.y * 20),1);
	vec4 textureColour = texture(aTex, textureCoordinate);
	//apply no lighting, ambient and diffuse components with colour contributed by texture
	//vertColour = (vec4((lightColour), 1.0) * textureColour);
	//vertColour = (vec4((ambient),1.0) * textureColour);
	vertColour = (vec4(result,1.0) * textureColour) * vec4(sin(position.x) * 2, cos(position.y) * 5, tan(position.z) * 6, 1.0f);
	
	
}