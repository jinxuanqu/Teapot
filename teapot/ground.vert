
varying vec3 ec_vnormal, ec_vposition;
varying vec4 tcoords;

void main()
{
ec_vnormal = gl_NormalMatrix*gl_Normal;
ec_vposition = (gl_ModelViewMatrix*gl_Vertex).xyz;
gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
gl_TexCoord[0] = gl_MultiTexCoord0;
// This is the result of the simulated render from the light source.
tcoords = gl_TextureMatrix[3]*gl_Vertex;
}