#version 150

in vec3 vert;
in vec3 color;
uniform mat4 modelview;
out vec4 incolor;

void main() {
    // does not alter the vertices at all
    gl_Position = modelview * vec4(vert, 1);
    //gl_Color = vec4(color,1);
    incolor = vec4(color,1);
}
