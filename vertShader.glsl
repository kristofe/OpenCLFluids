#version 330 core

in vec3 vert;
in vec3 color;
in vec2 uv;
uniform mat4 modelview;
out vec4 incolor;
out vec2 inuv;

void main() {
    // does not alter the vertices at all
    gl_Position = modelview * vec4(vert, 1);
    //gl_Color = vec4(color,1);
    incolor = vec4(color,1);
    inuv = uv;
}
