#type vertex
#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec2 a_Uv;
layout(location=3) in float a_TexID;
layout(location=4) in float a_tilingFactor;

uniform mat4 u_viewProjection;

out vec2 v_Uv;
out vec4 v_Color;
out float v_TexID;
out float v_tilingFactor;

void main(){
    v_Uv = a_Uv;
    v_Color = a_Color;
    v_TexID = a_TexID;
    v_tilingFactor= a_tilingFactor;
    gl_Position = u_viewProjection * vec4(a_Position , 1.0); 
}


#type fragment
#version 330 core

layout(location=0) out vec4 color;

in vec2 v_Uv;
in vec4 v_Color;
in float v_TexID;
in float v_tilingFactor;

uniform sampler2D u_textures[32];

void main(){
    color = texture(u_textures[int(v_TexID)], v_Uv * v_tilingFactor) * v_Color;
}
