#version 100
precision lowp float;

varying vec2 v_texCoord;
vec4 out_color;
void main() {
    vec2 uv = (v_texCoord -0.5)*2.0;
    bool v = dot(uv,uv) < 0.5*0.5;
    gl_FragColor = vec4(mix(vec3(0.0),vec3(1.0),float(v)), 1.0); // Example color output
}