#version 100
precision mediump float;

varying vec2 v_texCoord;
vec4 out_color;
void main() {
    gl_FragColor = vec4(v_texCoord, 0.0, 1.0); // Example color output
}