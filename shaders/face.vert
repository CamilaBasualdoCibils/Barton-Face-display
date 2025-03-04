#version 100
precision lowp float;

attribute vec2 a_position; // Position of each vertex
attribute vec2 a_texCoord; // Texture coordinate for each vertex
uniform float time;
varying vec2 v_texCoord;


void main() {
    gl_Position = vec4(a_position + vec2(sin(time*1.0),cos(time*1.0)) *0.5, 0.0, 1.0);
    v_texCoord = a_texCoord;
}
