#version 330 core

in vec4 FragPos;
out vec4 FragColor;

// Sunset parameters
uniform float sunsetTime = 0.3; // 0.0 = day, 1.0 = night
uniform vec3 sunPos = vec3(0.0, 1.0, 0.0);
uniform float cloudDensity = 0.3; // 0.0 = no clouds, 1.0 = full clouds

// Simple noise function for clouds
float hash(vec2 p)
{
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);
    f = f * f * (3.0 - 2.0 * f);
    
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    
    return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

// Fractional Brownian motion for clouds
float fbm(vec2 p)
{
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;
    
    for(int i = 0; i < 4; i++)
    {
        value += amplitude * noise(p * frequency);
        frequency *= 2.0;
        amplitude *= 0.5;
    }
    
    return value;
}

void main()
{
    // Normalize fragment position
    vec3 norm = normalize(FragPos.xyz);
    float y = norm.y;
    
    // Calculate horizon distance (0 at horizon, 1 at poles)
    float horizonDist = abs(y);
    
    // Day colors (clear blue sky)
    vec3 dayUpper = vec3(0.2, 0.6, 1.0);
    vec3 dayHorizon = vec3(0.7, 0.85, 1.0);
    
    // Sunset colors (vibrant orange/red)
    vec3 sunsetUpper = vec3(0.8, 0.3, 0.1);
    vec3 sunsetMid = vec3(1.0, 0.6, 0.2);
    vec3 sunsetHorizon = vec3(1.0, 0.4, 0.1);
    
    // Night colors (deep blue/black)
    vec3 nightUpper = vec3(0.05, 0.05, 0.15);
    vec3 nightHorizon = vec3(0.2, 0.15, 0.25);
    
    // Smooth transitions between phases
    float dayToSunset = smoothstep(0.0, 0.4, sunsetTime);
    float sunsetToNight = smoothstep(0.4, 1.0, sunsetTime);
    
    // Mix day to sunset colors
    vec3 upperColor = mix(dayUpper, sunsetUpper, dayToSunset);
    vec3 horizonColor = mix(dayHorizon, sunsetHorizon, dayToSunset);
    
    // Mix sunset to night colors
    upperColor = mix(upperColor, nightUpper, sunsetToNight);
    horizonColor = mix(horizonColor, nightHorizon, sunsetToNight);
    
    // Create smooth gradient from horizon to zenith
    vec3 skyColor = mix(horizonColor, upperColor, horizonDist);
    
    // Sun glow effect (stronger during sunset)
    vec3 sunDirection = normalize(sunPos);
    float sunDot = dot(norm, sunDirection);
    float sunInfluence = smoothstep(-0.2, 0.3, sunDot) * (1.0 - sunsetToNight * 0.8);
    float sunGlow = exp(-pow(1.0 - sunDot, 2.0) * 8.0) * sunInfluence;
    skyColor += vec3(1.0, 0.8, 0.4) * sunGlow * 0.8;
    
    // Sun disk
    float sunDisk = smoothstep(0.05, 0.03, distance(norm, sunDirection));
    vec3 sunColor = mix(vec3(1.0, 0.9, 0.5), vec3(1.0, 0.4, 0.1), sunsetTime);
    skyColor = mix(skyColor, sunColor, sunDisk);
    
    // Cloud effect (optional)
    if(cloudDensity > 0.0)
    {
        float cloudNoise = fbm(norm.xz * 3.0);
        float cloudPattern = smoothstep(0.4, 0.6, cloudNoise);
        
        // Cloud color responds to sunset phase
        vec3 cloudDay = vec3(1.0, 0.95, 0.9);
        vec3 cloudSunset = vec3(1.0, 0.6, 0.3);
        vec3 cloudNight = vec3(0.3, 0.3, 0.4);
        
        vec3 cloudColor = mix(cloudDay, cloudSunset, dayToSunset);
        cloudColor = mix(cloudColor, cloudNight, sunsetToNight);
        
        skyColor = mix(skyColor, cloudColor, cloudPattern * cloudDensity * 0.6);
    }
    
    FragColor = vec4(skyColor, 1.0);
}