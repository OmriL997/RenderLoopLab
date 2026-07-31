#version 330 core

// Per-vertex attributes (filled by dynamic vertex buffer)
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;

// Output to fragment shader
out vec2 v_TexCoord;
out vec4 v_Color;

// Orthographic projection matrix
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * vec4(a_Position, 0.0, 1.0);
    v_TexCoord  = a_TexCoord;
    v_Color     = a_Color;
}
