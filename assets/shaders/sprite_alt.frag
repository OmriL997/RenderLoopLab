#version 330 core

in  vec2 v_TexCoord;
in  vec4 v_Color;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main()
{
    vec4 c = texture(u_Texture, v_TexCoord) * v_Color;
    float lum = dot(c.rgb, vec3(0.299, 0.587, 0.114));
    FragColor = vec4(lum, lum, lum, c.a);
}
