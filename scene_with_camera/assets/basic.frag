#version 430

uniform vec3 drawColor;
out vec4 outColor;

void main()
{
    /*
    vec3 pos;
    pos.x = gl_FragCoord.x / 800;
    pos.y = gl_FragCoord.y / 600;
    pos.z = gl_FragCoord.z;

    vec3 light = vec3(0.17, 0.77, 0.0);
    float intensity = dot(pos, light);

    outColor = vec4(intensity * drawColor, 1.0);
    */
    outColor = vec4(normalize(gl_FragCoord.xyz), 1.0);
}
