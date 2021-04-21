#type vertex
#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec2 a_Uv;

uniform mat4 u_viewProjection;

out vec2 v_Uv;
out vec4 v_Color;

void main(){
    v_Uv = a_Uv;
    v_Color = a_Color;
    gl_Position = u_viewProjection * vec4(a_Position , 1.0); 
}


#type fragment
#version 330 core

layout(location=0) out vec4 color;

in vec2 v_Uv;
in vec4 v_Color;
uniform float u_tilingFactor;
uniform sampler2D u_texture;
uniform vec4 u_color;

void main(){

    // color = texture(u_texture, v_Uv * u_tilingFactor) * v_Color;
    color = v_Color;
}
