#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 uProj;
uniform vec2 uViewportSize;
uniform float uLineWidthPx;

in VS_OUT {
    vec3 viewPos;
    vec4 color;
} gs_in[];

flat out vec4 gColor;

void emitVertex(vec4 clipPos, vec4 color)
{
    gl_Position = clipPos;
    gColor = color;
    EmitVertex();
}

void main()
{
    // View-space endpoints
    vec3 v0 = gs_in[0].viewPos;
    vec3 v1 = gs_in[1].viewPos;

    // Clip-space positions
    vec4 clip0 = uProj * vec4(v0, 1.0);
    vec4 clip1 = uProj * vec4(v1, 1.0);

    // Screen-space direction
    vec2 ndc0 = clip0.xy / clip0.w;
    vec2 ndc1 = clip1.xy / clip1.w;

    vec2 d = ndc1 - ndc0;
    float len2 = dot(d, d);

    if (len2 < 1e-10)
    {
        d = vec2(0.0, 1.0);
        len2 = 1.0;
    }

    vec2 dir = d * inversesqrt(len2);
    vec2 n = vec2(-dir.y, dir.x);

    // View-space depth (positive forward)
    float z0 = -v0.z;
    float z1 = -v1.z;

    // Prevent blow-up
    z0 = max(z0, 0.1);
    z1 = max(z1, 0.1);

    // Convert pixels → NDC using depth
    vec2 pxToNdc = vec2(
        2.0 / uViewportSize.x,
        2.0 / uViewportSize.y
    );

    vec2 offset0 = n * (uLineWidthPx * 0.5) * pxToNdc * z0;
    vec2 offset1 = n * (uLineWidthPx * 0.5) * pxToNdc * z1;

    vec4 off0 = vec4(offset0, 0.0, 0.0);
    vec4 off1 = vec4(offset1, 0.0, 0.0);

    vec4 color = gs_in[0].color;

    emitVertex(clip0 + off0, color);
    emitVertex(clip0 - off0, color);
    emitVertex(clip1 + off1, color);
    emitVertex(clip1 - off1, color);

    EndPrimitive();
}
