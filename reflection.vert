#version 120

void main() {
    vec3 Normal = gl_NormalMatrix * gl_Normal;
    vec4 Position = gl_ModelViewMatrix * gl_Vertex;
    vec3 Reflection = reflect(Position.xyz, normalize(Normal));
    gl_TexCoord[0].stp = vec3(Reflection.x, Reflection.yz);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}