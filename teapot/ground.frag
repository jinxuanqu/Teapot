uniform sampler2D texture_shadow;
uniform sampler2D texture_map;
varying vec3 ec_vnormal, ec_vposition;
varying vec4 tcoords;

void main()
{
float pi = 3.14159;
vec3 P, N, L, V, H, tcolor2;
vec4 pccoords;
vec4 diffuse_color; //= gl_FrontMaterial.diffuse; 
vec4 specular_color; //= gl_FrontMaterial.specular; 
float shininess = gl_FrontMaterial.shininess;
float depthsample, clarity;
P = ec_vposition;
N = normalize(ec_vnormal);
L = normalize(gl_LightSource[0].position.xyz - P);
V = normalize(-P);				// eye position is (0,0,0)!
H = normalize(L+V);
	

pccoords = tcoords/tcoords.w ;
depthsample = texture2D(texture_shadow,pccoords.st).z;
clarity = 1.0;

tcolor2 = vec3(texture2D(texture_map,gl_TexCoord[0].st));
diffuse_color = vec4(tcolor2,1.0)*max(dot(N,L),0.0);
specular_color = gl_FrontMaterial.specular*pow(max(dot(H,N),0.0),shininess);
specular_color *= (shininess+2.0)/(8.0*pi);

// Simulated z is larger than value is saved depth map; point was in shadow.
if(depthsample < pccoords.z) clarity = 0.2;
diffuse_color *= clarity*max(dot(N,L),0.0);
specular_color *= clarity*pow(max(dot(H,N),0.0),shininess);
gl_FragColor = diffuse_color*2 + specular_color;	
//gl_FragColor = diffuse_color + specular_color;
//gl_FragColor = vec4(1.0,1.0,1.0,1.0)* depthsample;

/*	vec3 P, N, L, V, H, mapN, tcolor2;
	vec4 diffuse_color, specular_color; 
	float shininess = gl_FrontMaterial.shininess;
float pi = 3.14159;
	P = ec_vposition;
	L = normalize(gl_LightSource[0].position.xyz - P);
	V = normalize(-P);				
	H = normalize(L+V);
	N = normalize(ec_vnormal);

	tcolor2 = vec3(texture2D(texture_map,gl_TexCoord[0].st));
	diffuse_color = vec4(tcolor2,1.0)*max(dot(N,L),0.0);
	specular_color = gl_FrontMaterial.specular*pow(max(dot(H,N),0.0),shininess);
	specular_color *= (shininess+2.0)/(8.0*pi);
	gl_FragColor = diffuse_color;// + specular_color;	
	//gl_FragColor = vec4(tcolor2,1.0);
*/	
}
