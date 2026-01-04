#version 330 core

// Circle vertex: (cosθ, sinθ)
layout (location = 0) in vec2 aUnit;

// Per-instance data
layout (location = 1) in mat4 oModel;   // rotation + translation
layout (location = 5) in float oA;      // semi-major axis
layout (location = 6) in float oE;      // eccentricity
layout (location = 7) in vec4 oColor;

uniform mat4 uView;   // <-- REQUIRED

out VS_OUT {
    vec3 viewPos;    // <-- FIX: view-space position
    vec4 color;
} vs_out;

void main()
{
    // Map unit circle to your ν convention
    float sinNu = aUnit.x;
    float cosNu = aUnit.y;

    // Kepler radius
    float r = oA * (1.0 - oE * oE) / (1.0 + oE * cosNu);

    // Orbital plane position (X–Z plane)
    vec3 r_orbital = vec3(
        r * sinNu,
        0.0,
        r * cosNu
    );

    // World space
    vec4 world = oModel * vec4(r_orbital, 1.0);

    // View space (THIS is the key)
    vec4 view = uView * world;

    vs_out.viewPos = view.xyz;
    vs_out.color   = oColor;

    // Geometry shader will set final clip-space position
    gl_Position = vec4(0.0);
}
