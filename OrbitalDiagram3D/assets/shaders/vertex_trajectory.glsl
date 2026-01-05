#version 330 core

// Circle vertex: (cosθ, sinθ)
layout (location = 0) in vec2 aUnit;

// Per-instance data
layout (location = 1) in mat4 oModel;   // rotation + translation
layout (location = 5) in float oA;      // semi-major axis
layout (location = 6) in float oE;      // eccentricity
layout (location = 7) in float oV;      // nu true anomaly
layout (location = 8) in vec4 oColor;

uniform mat4 uView;   // <-- REQUIRED

out VS_OUT {
    vec3 viewPos;    // <-- FIX: view-space position
    vec4 color;
} vs_out;

void main()
{
    float M_PI = 3.14159265;
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
    float angle = atan(aUnit.x, aUnit.y);
    angle = angle < 0.0 ? angle + 2.0 * M_PI : angle;
    float rad = 75.0f * M_PI / 180.0f;
    float d = angle - oV;
    d = mod(d + M_PI, 2.0 * M_PI) - M_PI;
    if ((d < 0) && (abs(d) < rad)) {
      vs_out.color  = oColor;
    } else {
      vs_out.color  = vec4(oColor.rgb, oColor.a * 0.3);
    }
    

    // Geometry shader will set final clip-space position
    gl_Position = vec4(0.0);
}
