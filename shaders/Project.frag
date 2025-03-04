#version 100
precision lowp float;

varying vec2 v_texCoord;
uniform sampler2D textureSampler;
vec4 out_color;
void main() {
    gl_FragColor = texture2D(textureSampler,v_texCoord); // Example color output
}