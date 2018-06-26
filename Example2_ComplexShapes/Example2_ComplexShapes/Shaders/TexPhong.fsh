#version 330 core

uniform	vec4 mAmbient;
uniform	vec4 mDiffuse;
uniform	vec4 mSpecular;
uniform float mShininess;

uniform	vec4 lAmbient;
uniform	vec4 lDiffuse;
uniform	vec4 lSpecular;
uniform	vec4 lPosition;

uniform	sampler2D tex0;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

void main()
{
	vec3 vNormal = normalize(Normal);
	vec3 vToLight = normalize(vec3(lPosition) - Position);
	vec3 vToEye = normalize (vec3(0, 0 ,0) - Position);
	vec3 vReflect = normalize(reflect(-vToLight, vNormal));
	
	vec4 Ambient = mAmbient * lAmbient;  
	vec4 Diffuse = mDiffuse * lDiffuse * max(dot(vNormal, vToLight), 0.0f);
	vec4 Specular = mSpecular * lSpecular * pow(max(dot(vToEye, vReflect), 0.0f), mShininess);

	vec4 TexColor = texture(tex0, TexCoord);
	vec3 Color = vec3(Diffuse + Ambient + Specular) * vec3(TexColor); 
	
	gl_FragColor = vec4(Color, mDiffuse.a);
}