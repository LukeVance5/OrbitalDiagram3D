#version 330 core

// Circle vertex: (cosθ, sinθ)
layout (location = 0) in vec2 aUnit;

// Per-instance data
layout (location = 1) in mat4 oModel;   // rotation + translation
layout (location = 5) in float oA;      // semi-major axis
layout (location = 6) in float oE;      // eccentricity
layout (location = 7) in vec4 oColor;

out VS_OUT {
    vec3 worldPos;
    vec4 color;
} vs_out;

void main()
{
    // Map unit circle to your ν convention
    // aUnit = (cosθ, sinθ) → sinν, cosν
    float sinNu = aUnit.x;
    float cosNu = aUnit.y;

    // Kepler radius
    float r = oA * (1.0 - oE * oE) / (1.0 + oE * cosNu);

    // Orbital plane position (X–Z plane, Z reference)
    vec3 r_orbital = vec3(
        r * sinNu,   // X
        0.0,         // Y
        r * cosNu    // Z
    );

    // Apply full model matrix (rotation + translation)
    vec4 world = oModel * vec4(r_orbital, 1.0);

    vs_out.worldPos = world.xyz;
    vs_out.color = oColor;
    // GS will apply view/projection
    gl_Position = vec4(0.0);
}
