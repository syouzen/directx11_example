Texture2D fireTexture : register(t0);
Texture2D noiseTexture : register(t1);
Texture2D alphaTexture : register(t2);
SamplerState SampleType;
SamplerState SampleType2;

cbuffer DistortionBuffer
{
	float2 distortion1;
	float2 distortion2;
	float2 distortion3;
	float distortionScale;
	float distortionBias;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
   	float2 texCoords1 : TEXCOORD1;
	float2 texCoords2 : TEXCOORD2;
	float2 texCoords3 : TEXCOORD3;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 noise1;
	float4 noise2;
	float4 noise3;
	float4 finalNoise;
	float perturb;
	float2 noiseCoords;
	float4 fireColor;
	float4 alphaColor;

    noise1 = noiseTexture.Sample(SampleType, input.texCoords1);
    noise2 = noiseTexture.Sample(SampleType, input.texCoords2);
	noise3 = noiseTexture.Sample(SampleType, input.texCoords3);

    noise1 = (noise1 - 0.5f) * 2.0f;
    noise2 = (noise2 - 0.5f) * 2.0f;
    noise3 = (noise3 - 0.5f) * 2.0f;

	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	finalNoise = noise1 + noise2 + noise3;

	perturb = ((1.0f - input.tex.y) * distortionScale) + distortionBias;

	noiseCoords.xy = (finalNoise.xy * perturb) + input.tex.xy;
    
	fireColor = fireTexture.Sample(SampleType2, noiseCoords.xy);

    alphaColor = alphaTexture.Sample(SampleType2, noiseCoords.xy);

	fireColor.a = alphaColor;
	
    return fireColor;
}
