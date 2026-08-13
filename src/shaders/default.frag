#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;
uniform vec3 tint;

void main()
{
    vec4 tex = texture(tex0, texCoord);
    FragColor = vec4(tex.rgb * tint, tex.a);
}