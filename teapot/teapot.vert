
// Vertex shader for normal mapping.
varying vec3 ec_vnormal, ec_vposition, ec_vtangent, ec_vbitangent,ec_reflect;
attribute vec3 tangent, bitangent;

void main()
{	
	vec3 ec_eyedir;
	ec_vnormal = gl_NormalMatrix * gl_Normal;
	ec_vtangent = gl_NormalMatrix * tangent;
	ec_vbitangent = gl_NormalMatrix * bitangent;
	ec_vposition = (gl_ModelViewMatrix*gl_Vertex).xyz;
	ec_eyedir = normalize(-ec_vposition);
	ec_reflect = -ec_eyedir  + 2.0*dot(ec_eyedir,ec_vnormal)*ec_vnormal;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

