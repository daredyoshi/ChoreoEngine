#type vertex
#version 330 core

layout(location=0) in vec3 a_Position;

uniform mat4 u_viewProjection;
uniform mat4 u_xform;


void main(){
    gl_Position = u_viewProjection * u_xform * vec4(a_Position , 1.0); 
}


#type fragment
#version 330 core

layout(location=0) out vec4 color;

uniform vec3 u_Color;

void main(){

    color = vec4(u_Color, 1.0f);
}
