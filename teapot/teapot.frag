// Fragment shader for normal mapping.
varying vec3 ec_vnormal, ec_vposition, ec_vtangent, ec_vbitangent,ec_reflect;
uniform sampler2D mytexture; 
uniform sampler2D mynormalmap;
uniform samplerCube cube_texture;
//varying vec4 tcoords;
void main()
{
	mat3 tform;
	vec3 P, N, L, V, H, R, mapN, tcolor,re_index;
	vec4 diffuse_color, specular_color; 
	float shininess = gl_FrontMaterial.shininess;
	float pi = 3.14159;
	//vec4 pccoords;

	// Create a 3x3 matrix from T, B, and N as columns:
	tform = mat3(ec_vtangent,ec_vbitangent,ec_vnormal);
	P = ec_vposition;
	L = normalize(gl_LightSource[0].position.xyz - P);
	V = normalize(-P);				
	H = normalize(L+V);
	R = normalize(ec_reflect);	
	re_index = 0.5*(R + vec3(1.0,-0.4,1.0));
	mapN = vec3(texture2D(mynormalmap,gl_TexCoord[0].st));
	// x, y, and z are in [0.0,1.0], but x and y should be in [-1.0,1.0].
	mapN.xy = 2.0*mapN.xy - vec2(1.0,1.0);
	N = normalize(tform*normalize(mapN));
//	N = normalize(ec_vnormal);

	tcolor = vec3(texture2D(mytexture,gl_TexCoord[0].st));
	diffuse_color = vec4(tcolor,1.0)*max(dot(N,L),0.0);
	specular_color = gl_FrontMaterial.specular*pow(max(dot(H,N),0.0),shininess);
	specular_color *= (shininess+2.0)/(8.0*pi)/5;
	gl_FragColor = 0.7*(diffuse_color + specular_color) + 0.3*vec4(textureCube(cube_texture,re_index));
}
