#version 430 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 0) in vec3 a_normal;

uniform mat4 MVP;

out vec3 v_normal;

void main()
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    v_normal = a_normal;
}