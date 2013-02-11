vec3 DirectionalLight(vec3 worldNormal, vec3 direction, vec3 colour, float intensity)
{
    return colour * (clamp(dot(direction, worldNormal), 0.0, 1.0) * intensity);
}