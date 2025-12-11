#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 view;
uniform mat4 projection;

// Pixel width of the line when face-on
uniform float basePixelWidth;

// Screen height in pixels
uniform float viewportHeight;

// Orbit plane normal in world space
uniform vec3 orbitNormal;

// Camera world position
uniform vec3 cameraPos;

in VS_OUT {
    vec3 worldPos;
    vec3 color;
} gs_in[];

out vec3 fragColor;

void emitVertex(vec4 clipPos, vec3 color)
{
    gl_Position = clipPos;
    fragColor = color;
    EmitVertex();
}

void main()
{
    // Get world positions of line segment
    vec3 w0 = gs_in[0].worldPos;
    vec3 w1 = gs_in[1].worldPos;

    // Colors
    vec3 c0 = gs_in[0].color;
    vec3 c1 = gs_in[1].color;

    // View-space positions
    vec4 v0 = view * vec4(w0, 1.0);
    vec4 v1 = view * vec4(w1, 1.0);

    // View-space direction of the line
    vec3 dir = normalize(v1.xyz - v0.xyz);

    // Camera forward direction in view space is (0,0,-1)
    vec3 camForwardVS = vec3(0, 0, -1);

    // Perpendicular direction in view space
    vec3 perp = normalize(cross(dir, camForwardVS));

    // -----------------------------
    //  Visibility based on orbit plane angle
    // -----------------------------
    // Camera look direction (world space)
    vec3 cameraDir = normalize(-(mat3(view) * vec3(0,0,-1)));

    // Dot between orbit normal and camera direction
    float d = abs(dot(normalize(orbitNormal), normalize(cameraDir)));

    // d=0 → face-on → visible, d=1 → edge-on → invisible
    float visibility = 1.0 - d;

    float pixelWidth = basePixelWidth * visibility;

    // If too small, skip drawing
    if (pixelWidth < 0.01)
        return;

    // Convert pixel width to NDC offset (screen-space thickness)
    float ndcOffset = pixelWidth / viewportHeight;

    // View-space extrusion scaled by clip-space depth
    vec3 offset0 = perp * ndcOffset * v0.w;
    vec3 offset1 = perp * ndcOffset * v1.w;

    // Emit 4 vertices of the quad
    emitVertex(projection * vec4(v0.xyz + offset0, v0.w), c0);
    emitVertex(projection * vec4(v0.xyz - offset0, v0.w), c0);
    emitVertex(projection * vec4(v1.xyz + offset1, v1.w), c1);
    emitVertex(projection * vec4(v1.xyz - offset1, v1.w), c1);

    EndPrimitive();
}
