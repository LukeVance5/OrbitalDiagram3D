#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

// Uniforms (shared across stages)
uniform mat4 uView;
uniform mat4 uProj;
uniform vec2 uViewportSize;   // (width, height) in pixels
uniform float uLineWidthPx;   // thickness in pixels

// From vertex shader
in VS_OUT {
    vec3 worldPos;
    vec4 color;
} gs_in[];

// To fragment shader
flat out vec4 gColor;

void emitVertex(vec4 clipPos, vec4 color)
{
    gl_Position = clipPos;
    gColor = color;
    EmitVertex();
}

void main()
{
    // Transform endpoints to clip space
    vec4 clip0 = uProj * uView * vec4(gs_in[0].worldPos, 1.0);
    vec4 clip1 = uProj * uView * vec4(gs_in[1].worldPos, 1.0);

    // If behind the camera, skip (simple & safe).
    // (More advanced: proper clipping, but this is usually fine for orbits.)
    if (clip0.w <= 0.0 || clip1.w <= 0.0)
        return;

    // Convert to NDC for screen-space direction
    vec2 ndc0 = clip0.xy / clip0.w;
    vec2 ndc1 = clip1.xy / clip1.w;

    vec2 d = ndc1 - ndc0;
    float len2 = dot(d, d);
    if (len2 < 1e-12)
        return;

    vec2 dir = d * inversesqrt(len2);
    vec2 n = vec2(-dir.y, dir.x); // perpendicular in screen space

    // Pixel -> NDC scale
    vec2 pxToNdc = vec2(2.0 / uViewportSize.x, 2.0 / uViewportSize.y);
    vec2 offsetNdc = n * (uLineWidthPx * 0.5) * pxToNdc;

    // Convert NDC offsets back to clip offsets per endpoint (perspective-correct)
    vec4 off0 = vec4(offsetNdc * clip0.w, 0.0, 0.0);
    vec4 off1 = vec4(offsetNdc * clip1.w, 0.0, 0.0);

    vec4 color = gs_in[0].color; // per-orbit (flat)

    // Emit a quad as triangle strip:
    // 0: p0 + offset
    // 1: p0 - offset
    // 2: p1 + offset
    // 3: p1 - offset
    emitVertex(clip0 + off0, color);
    emitVertex(clip0 - off0, color);
    emitVertex(clip1 + off1, color);
    emitVertex(clip1 - off1, color);

    EndPrimitive();
}
