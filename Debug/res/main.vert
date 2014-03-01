
#version 330 core

uniform mat4 mM; //model matrix
uniform mat4 mR; //rotation part of matrix only
uniform mat4 mV; //view matrix
uniform mat4 mP; //perspec matrix

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec4 vColor;
layout(location = 3) in vec2 vUV;
layout(location = 4) in vec3 vTan;
layout(location = 5) in vec3 vBitan;

out vec3 vNormal_world;
out vec4 diffuseColor;
out mat3 invTBN;
out vec2 vecUV;

void main() {
	

	vec4 v4_position = vec4(vPosition, 1.0f);

	gl_Position = mP*mV*mM*v4_position;

	vec4 v4_normal = mR*vec4(vNormal, 0.0f);
	vNormal_world = vec3(v4_normal);
	diffuseColor = vColor;


	//	Normalmap prep

	//calc matrix to convert from tangentspace to worldspace
	//all vectors are orthagonal, therefore the inverse is also the transpose (and the transpose is faster)
	invTBN = transpose(mat3(
        vec3(mR*vec4(vTan, 0.0f)),
        vec3(mR*vec4(vBitan, 0.0f)),
        vec3(mR*vec4(vNormal, 0.0f))
    ));

	vecUV = vUV;	//pass through to fragment shader


}