#version 430 core

layout( location = 0 ) in vec3 aPosition;
layout( location = 1 ) in vec3 aColor;
layout( location = 2 ) in vec2 aUV;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

out vec4 vColor;
out vec2 vUV;

void main()
{
    vec4 aPos4 = vec4(aPosition, 1.0f);
	vec4 aCol4 = vec4(aColor, 1.0f);

	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * aPos4;
    vColor = aCol4;
	vUV = aUV;
}
