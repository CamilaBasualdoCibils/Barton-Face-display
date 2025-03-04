#version 100
precision lowp float;

attribute vec2 a_position; // Position of each vertex
attribute vec2 a_texCoord; // Texture coordinate for each vertex

varying vec2 v_texCoord;


void main() {
    gl_Position = vec4(a_position, 0.0, 1.0);
    v_texCoord = a_texCoord;
}
