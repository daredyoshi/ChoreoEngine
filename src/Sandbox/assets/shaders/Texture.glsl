
#type vertex
#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_Uv;

uniform mat4 u_viewProjection;
uniform mat4 u_xform;

out vec2 v_Uv;

void main(){
    v_Uv = a_Uv;

    gl_Position = u_viewProjection * u_xform * vec4(a_Position , 1.0); 
}


#type fragment
#version 330 core

layout(location=0) out vec4 color;

in vec2 v_Uv;
uniform sampler2D u_texture;

void main(){

    color = texture(u_texture, v_Uv);
}
