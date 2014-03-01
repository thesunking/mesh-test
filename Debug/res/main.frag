
#version 330 core

uniform vec4 vLight;
uniform sampler2D NormalTextureSampler;

in vec3 vNormal_world;
in vec4 diffuseColor;
in vec2 vecUV;
in mat3 invTBN;


void main() {
	
	vec3 TextureNormal_tangentspace = normalize(texture2D( NormalTextureSampler, vecUV ).rgb*2.0 - 1.0);


	//float cosTheta = clamp( dot( vNormal_world, vLight.xyz ), 0.0, 1.0);
	//float bias = 0.005*tan(acos(cosTheta));
	//bias = clamp(bias, 0,0.01);

	float bias = 0.005;
	float visibility = 1.0;


	float d_i1 = max(dot(vNormal_world, normalize(vec3(vLight))), 0.0f); //lambertian diffuse lighting
	float d_i2 = max(dot(TextureNormal_tangentspace, invTBN * normalize(vec3(vLight))), 0.0f); //normalmap lambertian diffuse lighting

	//remap from [0, 1] to [0.25, 1]
	d_i1 = (d_i1 * 0.75) + 0.25;
	d_i2 = (d_i2 * 0.75) + 0.25;

	//gl_FragColor = vec4(d_i1 * d_i2 * visibility * diffuseColor.rgb, 1.0f);
	gl_FragColor = vec4(d_i1 * d_i2 * diffuseColor.rgb, 1.0f);






	//Debug lighting
	//gl_FragColor = d_i1 * vec4(0.5f, 0.5f, 0.5f, 1.0f);

	//Debug normals
	//gl_FragColor = vec4(vNormal_world, 1.0f);

	//Debug the normal map
	//gl_FragColor = texture2D( NormalTextureSampler, vecUV );
	//gl_FragColor = d_i2 * vec4(0.5f, 0.5f, 0.5f, 1.0f);


}