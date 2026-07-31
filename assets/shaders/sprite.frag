#version 330 core

in  vec2 v_TexCoord;
in  vec4 v_Color;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main()
{
    vec4 texSample = texture(u_Texture, v_TexCoord);
    FragColor = texSample * v_Color;
}
