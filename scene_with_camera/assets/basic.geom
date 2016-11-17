#version 430

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float time;

vec3 calcNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 0.30f;
    vec3 direction = normal * ((sin(time) + 1.0f) / 2.0f) * magnitude;
    return position + vec4(direction, 0.0f);
}

void main()
{
    vec3 normal = calcNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    EmitVertex();
    EndPrimitive();
}
