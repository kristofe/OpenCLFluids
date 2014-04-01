#version 330 core

in vec4 incolor;
in vec2 inuv;
out vec4 finalColor;
uniform sampler2D tex;

void main() {
    //set every drawn pixel to white
    //finalColor = vec4(1.0, 1.0, 1.0, 1.0);
    //finalColor = incolor;
    finalColor = vec4(abs(texture(tex, inuv)).xxx, 1.0);
    //finalColor = vec4(inuv.xy,0.0, 1.0);
}
