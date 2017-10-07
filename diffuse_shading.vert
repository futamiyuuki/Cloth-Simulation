
varying vec4 diffuse,ambientGlobal,ambient, ecPos;
varying vec3 normal,halfVector;

void main()
{
    vec3 aux;
    
    normal = normalize(gl_NormalMatrix * gl_Normal);
    
    ecPos = gl_ModelViewMatrix * gl_Vertex;
    
    halfVector = gl_LightSource[0].halfVector.xyz;
    
    diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;
    
    
    gl_Position = ftransform();
}