#version 400 core

uniform	vec4 mAmbient;
uniform	vec4 mDiffuse;
uniform	vec4 mSpecular;
uniform float mShininess;

uniform	vec4 lAmbient;
uniform	vec4 lDiffuse;
uniform	vec4 lSpecular;
uniform	vec4 lPosition;

in vec3 Position;
in vec3 Normal;

void main ()
{
	vec3 vToLight = normalize(vec3(lPosition) - Position);
	vec3 vToEye = normalize(vec3(0,0,0) - Position);
	vec3 vReflect = normalize(reflect(-vToLight, Normal));
	
	vec4 Ambient = mAmbient * lAmbient;  
	vec4 Diffuse = mDiffuse * lDiffuse * max(dot(Normal, vToLight), 0.0f);
	vec4 Specular = mSpecular * lSpecular * pow(max(dot(vToEye, vReflect), 0.0f), mShininess);

	vec3 Color = vec3(Ambient + Diffuse + Specular); 
	
	gl_FragColor = vec4(Color, 1.0);
}