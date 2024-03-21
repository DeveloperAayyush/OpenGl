#Shader Vertex
#version 330 core

layout(location = 0)in vec4 positions;

void main()
{
    gl_Position = positions;
};

#Shader Fragment
#version 330 core

layout(location = 0)out vec4 color;

void main()
{
    int num = 2;
    if(num %2 == 0)
    {
        color = vec4(1.0,0.33,0.76,0.17);
        num --;
    }
    else
    {
        color = vec4(1.0,0.79,0.16,0.23);
        num ++;
    }
};