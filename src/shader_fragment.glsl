#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da cor de cada vértice, definidas em "shader_vertex.glsl" e
// "main.cpp" (array color_coefficients).
in vec4 cor_interpolada_pelo_rasterizador;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

void main()
{
    color = cor_interpolada_pelo_rasterizador;
} 

