#version 130

struct LightInfo 
{
	vec3 position;
	vec3 color;
	float radius;
	float falloff;
};

const int MaxLights = 25;

uniform sampler2D texture;
uniform vec3 AmbientLight;
uniform int LightsCount;
uniform LightInfo Lights[MaxLights];void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	vec4 finalPixel = gl_Color * pixel;
	finalPixel *= vec4(AmbientLight, 1.0);
	
	for(int i = 0; i < LightsCount; ++i) 
	{
		vec3 L = Lights[i].position - gl_FragCoord.xyz;
		float distance = length(L);
		float d = max(distance - Lights[i].radius, 0);
		L /= distance;

		float attenuation = 1 / pow(d / Lights[i].radius + 1, 2);
		attenuation = (attenuation - Lights[i].falloff) / (1 - Lights[i].falloff);
		attenuation = max(attenuation, 0);
		finalPixel += pixel * (vec4(Lights[i].color, 1.0) * attenuation);
	}

	gl_FragColor = finalPixel;
}