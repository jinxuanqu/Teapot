#define GL_GLEXT_PROTOTYPES 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


#define XRES 1280
#define YRES 720
void set_uniform(int p,char* name,int index);
void set_light();
void set_ground_material();
void set_teapot_material();
void save_matrix(float *ep, float *vp);
void draw_stuff();
void draw_ground();
GLuint p; //the teapot shader
GLuint g; //ground shader
GLuint tex_cube = 11;
float eye[]    = {1.0, 3.0, 4.0};
float viewpt[] = {0.0, 2.0, 0.0};
float up[] = {0.0, 1.0, 0.0};
float light0_position[] = {2.4, 2.2, 2.0, 1.0};

GLfloat vertex[13008][3];
GLfloat normal[13008][3];
GLfloat texcoord[13498][2];
GLfloat tangent[13008][3];
GLfloat bi_tangent[13008][3];
int face[13008][4][3];

struct point{
    float x,y,z;
    };
void view_volume(float *ep, float *vp)
{
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,1.0*w/h,0.1,40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(ep[0],ep[1],ep[2],vp[0],vp[1],vp[2],0.0,1.0,0.0);
}
int readFile()
{
    FILE *file1;
    file1 = fopen("teapot.605.obj","r");
    GLfloat a, b, c;
    int e,f,g, x,y,z;
    int vNum=0,txNum=0,vnNum=0,tanNum=0,btanNum=0,fNum=0;
    char v,n;
    while(!feof(file1))
    {
        v=fgetc(file1);
        switch(v)
        {
            case 'v':
                n = fgetc(file1);
                switch(n){
                    case ' ':
                        fscanf(file1,"%f%f%f",&a,&b,&c);
                        	vertex[vNum][0] = a;
                        	vertex[vNum][1] = b;
                        	vertex[vNum][2] = c;
                        	vNum++;
                        	break;
                    case 't':
                        fscanf(file1,"%f%f",&a,&b);
                        	texcoord[txNum][0] = a;
                        	texcoord[txNum][1] = b;
                        	txNum++;
                        	break;
                    case 'n':
                        fscanf(file1,"%f%f%f",&a,&b,&c);
                        	normal[vnNum][0] = a;
                        	normal[vnNum][1] = b;
                        	normal[vnNum][2] = c;
                        	vnNum++;
                        	break;
                    case 'x':
                        fscanf(file1,"%f%f%f",&a,&b,&c);
			if(a!=0 && b!=0 && c!=0){
                        	tangent[tanNum][0] = a;
                        	tangent[tanNum][1] = b;
                        	tangent[tanNum][2] = c;
                        	tanNum++;
			}
                        break;
                    case 'y':
                        fscanf(file1,"%f%f%f",&a,&b,&c);
			if(a!=0 && b!=0 && c!=0){
                        	bi_tangent[btanNum][0] = a;
                        	bi_tangent[btanNum][1] = b;
                        	bi_tangent[btanNum][2] = c;
                        	btanNum++;
			 }
			break;
                    default:
                        break;
                }
                break;
            case 'f':
                fscanf(file1," %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                        &face[fNum][0][0], &face[fNum][0][1], &face[fNum][0][2],
                        &face[fNum][1][0], &face[fNum][1][1], &face[fNum][1][2],
                        &face[fNum][2][0], &face[fNum][2][1], &face[fNum][2][2],
                        &face[fNum][3][0], &face[fNum][3][1], &face[fNum][3][2]);
                fNum++;
                break;
            default:
                break;

        }
    }
    fclose(file1);
    return 0; 
}
void draw_sky(){
    float sF = 6.0;
    float bT = -1.0;
    float tP = 7.0;
    struct point front[4]={{-sF,bT,sF},{sF,bT,sF},{sF,tP,sF},{-sF,tP,sF}};
    struct point back[4]={{-sF,bT,-sF},{-sF,tP,-sF},{sF,tP,-sF},{sF,bT,-sF}};
    struct point left[4]={{-sF,bT,-sF},{-sF,tP,-sF},{-sF,tP,sF},{-sF,bT,sF}};
    struct point right[4]={{sF,bT,-sF},{sF,tP,-sF},{sF,tP,sF},{sF,bT,sF}};
    struct point top[4]={{-sF,tP,-sF},{-sF,tP,sF},{sF,tP,sF},{sF,tP,-sF}};
    struct point bottom[4]={{-sF,bT,-sF},{-sF,bT,sF},{sF,bT,sF},{sF,bT,-sF}};
    float frontTexCoords[4][2] = {{1.0,1.0},{0.0,1.0},{0.0,0.0},{1.0,0.0}};
    float backTexCoords[4][2] = {{0.0,1.0},{0.0,0.0},{1.0,0.0},{1.0,1.0}};
    float leftTexCoords[4][2] = {{1.0,1.0},{1.0,0.0},{0.0,0.0},{0.0,1.0}};
    float rightTexCoords[4][2] = {{0.0,1.0},{0.0,0.0},{1.0,0.0},{1.0,1.0}};
    float topTexCoords[4][2] = {{0.0,1.0},{0.0,0.0},{1.0,0.0},{1.0,1.0}};
    float bottomTexCoords[4][2] = {{0.0,0.0},{0.0,1.0},{1.0,1.0},{1.0,0.0}};
    glUseProgram(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,4);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++){
        glTexCoord2fv(frontTexCoords[i]);
        glVertex3f(front[i].x,front[i].y,front[i].z);
        glNormal3f(0.0,0.0,-1.0);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D,5);
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++){
        glTexCoord2fv(backTexCoords[i]);
        glVertex3f(back[i].x,back[i].y,back[i].z);
        glNormal3f(0.0,0.0,1.0);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D,6);
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++){
        glTexCoord2fv(leftTexCoords[i]);
        glVertex3f(left[i].x,left[i].y,left[i].z);
        glNormal3f(1.0,0.0,0.0);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D,7);
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++){
        glTexCoord2fv(rightTexCoords[i]);
        glVertex3f(right[i].x,right[i].y,right[i].z);
        glNormal3f(-1.0,0.0,0.0);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D,8);
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++){
        glTexCoord2fv(topTexCoords[i]);
        glVertex3f(top[i].x,top[i].y,top[i].z);
        glNormal3f(0.0,-1.0,0.0);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D,9);
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++){
        glTexCoord2fv(bottomTexCoords[i]);
        glVertex3f(bottom[i].x,bottom[i].y,bottom[i].z);
        glNormal3f(0.0,1.0,0.0);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);  
    
}

void draw_ground(){
    struct point groundVet[4] = {{4.0,0.0,4.0},{4.0,0.0,-4.0},{-4.0,0.0,-4.0},{-4.0,0.0,4.0}};
    float groundTexCoords[4][2] = {{0.0,1.0},{1.0,1.0},{1.0,0.0},{0.0,0.0}};
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++){
        glNormal3f(0.0,1.0,0.0);
        glTexCoord2fv(groundTexCoords[i]);
        glVertex3f(groundVet[i].x,groundVet[i].y,groundVet[i].z);
    }
    glEnd();
}

void save_matrix(float *ep, float *vp)
{
glMatrixMode(GL_TEXTURE); 
glActiveTexture(GL_TEXTURE3);
glLoadIdentity();
glTranslatef(0.0,0.0,-0.002);
glScalef(0.5,0.5,0.5);
glTranslatef(1.0,1.0,1.0);
gluPerspective(45.0,(float)(XRES)/(float)(YRES),0.1,40.0);
gluLookAt(ep[0],ep[1],ep[2],vp[0],vp[1],vp[2],0.0,1.0,0.0);
}

void draw_stuff()
{   
    int i, j;
    int vi, ti, ni;

    int index_tangent = glGetAttribLocation(p,"tangent");
    int index_bitangent = glGetAttribLocation(p,"bitangent");
    glUseProgram(p);
    set_uniform(p,"mytexture",0);
    set_uniform(p,"mynormalmap",1);
    set_uniform(p,"cube_texture",5);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,3);
    glActiveTexture (GL_TEXTURE5);
    glBindTexture (GL_TEXTURE_CUBE_MAP, tex_cube);
    glBegin(GL_QUADS);
    for (i=0; i<13008; i++) {
        for (j=0; j<4; j++) {
                            
            vi = face[i][j][0] - 1; //note: the index in .obj starts from 1
            ti = face[i][j][1] - 1;
            ni = face[i][j][2] - 1;

            glNormal3f(normal[ni][0],normal[ni][1],normal[ni][2]);
            glTexCoord2f(texcoord[ti][0],texcoord[ti][1]);
            glVertexAttrib3f(index_tangent, tangent[vi][0],tangent[vi][1],tangent[vi][2]);
            glVertexAttrib3f(index_bitangent, bi_tangent[vi][0],bi_tangent[vi][1],bi_tangent[vi][2]);
            glVertex3f(vertex[vi][0],vertex[vi][1],vertex[vi][2]);
        }
    }
    glEnd();
}
void draw_teapot(){
    int i, j;
    int vi, ni;

    int index_tangent = glGetAttribLocation(p,"tangent");
    int index_bitangent = glGetAttribLocation(p,"bitangent");
    glUseProgram(0);
    glBegin(GL_QUADS);
    for (i=0; i<13008; i++) {
        for (j=0; j<4; j++) {
                            
            vi = face[i][j][0] - 1; //note: the index in .obj starts from 1
            ni = face[i][j][2] - 1;
            glNormal3f(normal[ni][0],normal[ni][1],normal[ni][2]);
            glVertex3f(vertex[vi][0],vertex[vi][1],vertex[vi][2]);
        }
    }
    glEnd();
}
void create_shadow_texture(){
   float eyepoint[3], viewpoint[3] = {0.0,0.0,0.0};
    glEnable(GL_DEPTH_TEST);
    glBindFramebufferEXT(GL_FRAMEBUFFER,1); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(0);
    for(int k=0;k<3;k++){ 
        eyepoint[k] = light0_position[k];
    }
    eyepoint[0] = light0_position[0];
    eyepoint[1] = light0_position[1]+0.4;
    eyepoint[2] = light0_position[2]-0.8;
    view_volume(eyepoint,viewpoint);
    glTranslatef(0.01,-0.2,-1.0);//0.14,-0.2,-1.0
    glRotatef(-20.0,0.4,0.4,0.0);
    set_light();
    draw_teapot();
    glBindFramebufferEXT(GL_FRAMEBUFFER,0);
    save_matrix(eyepoint,viewpoint);
}

void build_shadowmap()
{
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,10);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,XRES,YRES, 0, 
	GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER,1);
    glDrawBuffer(GL_NONE);

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,10,0);
    glBindFramebufferEXT(GL_FRAMEBUFFER,0);
    glActiveTexture(GL_TEXTURE0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        printf("framebuffer failed!");
    }
}

int load_cube_map_side (
  GLuint texture, GLenum side_target, const char* filename
) {
  glBindTexture (GL_TEXTURE_CUBE_MAP, texture);

    FILE *fopen(), *fptr;
    char buf[512];
    int im_size, im_width, im_height, max_color;
    unsigned char* texture_bytes, *parse;

    fptr=fopen(filename, "r");
    if (fptr==NULL) printf("fptr=NULL\n");
    fgets(buf,512,fptr);
    do {        
    fgets(buf,512,fptr);
    } while (buf[0]=='#');
    if (buf==NULL) printf("buf=NULL\n");
    parse = strtok(buf, " \t");
    im_width = atoi(parse);

    parse = strtok(NULL, " \n");
    im_height = atoi(parse);

    fgets(buf, 512, fptr);
    parse = strtok(buf, " \n");
    max_color = atoi(parse);
    im_size = im_width*im_height;
    texture_bytes = (unsigned char*) calloc(3, im_size);
    fread(texture_bytes,3,im_size, fptr);
    fclose(fptr);
  
  glTexImage2D (side_target,0,GL_RGB,im_width,im_height,0,GL_RGB,GL_UNSIGNED_BYTE,texture_bytes);
  free (texture_bytes);
  return 1;
}


void create_cube_map (
  const char* front,
  const char* back,
  const char* top,
  const char* bottom,
  const char* left,
  const char* right,
  GLuint* tex_cube
) {
  // generate a cube-map texture to hold all the sides
  glActiveTexture (GL_TEXTURE5);
  //glGenTextures (1, tex_cube);

  // load each image and copy into a side of the cube-map texture

    load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);

    load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);

    load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);

    load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);

    load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);

    load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
  // format cube map texture
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}



void renderScene(void)
{
    glClearColor(0.35,0.35,0.35, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    create_cube_map("front.ppm","back.ppm","top.ppm","ground.ppm","left.ppm","right.ppm",&tex_cube);
    create_shadow_texture();

    view_volume(eye,viewpt);
    set_light();
    glUseProgram(g);    
    set_uniform(g,"texture_shadow",3);
    set_uniform(g,"texture_map",2);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,10);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,1);
    set_ground_material();    
    draw_ground();
    draw_sky();
    set_teapot_material();
    glTranslatef(0.0,0.0,-1.0);
    glRotatef(-40.0,0.0,1.0,0.0);
    draw_stuff();
    glutSwapBuffers();
}


char *read_shader_program(char const *filename)
{
    FILE *fp;
    FILE *fd;
    char *content = NULL;
    int count;
    fd = fopen(filename,"r");
    fseek(fd,0,SEEK_END);
    count = ftell(fd);
    fclose(fd);
    content = (char *)calloc(1,(count+1));
    fp = fopen(filename,"r");
    count = fread(content,sizeof(char),count,fp);
    content[count] = '\0';
    fclose(fp);
    return content; 
}

void set_light()
{
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
}

void set_teapot_material()
{

    float mat_diffuse[]  = {0.8, 0.4, 0.0, 1.0};
    float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    float mat_shiniess[] = {50.0};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiniess);
}

void set_ground_material()
{
     float mat_ambient[] = {0.0,0.0,0.0,1.0};
    float mat_diffuse[]  = {1.0, 0.0, 0.5, 1.0};
    float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    float mat_shiniess[] = {10.0};

    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiniess);
}



unsigned int set_shaders(char const *vert, char const *frag)
{
    GLint vertCompiled, fragCompiled;
    char *vs, *fs;
    GLuint v, f, p;
    GLint logLen;
    GLchar log[200];
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    vs = read_shader_program(vert);
    fs = read_shader_program(frag);
    if (vs == NULL || fs == NULL) {
    printf("Fail to load shaders. Rendering with fixed pipeline");
    return 0;
    }

    glShaderSource(v,1,(const char **)&vs,NULL);
    glShaderSource(f,1,(const char **)&fs,NULL);
    free(vs);
    free(fs); 
    glCompileShader(v);
    glCompileShader(f);

GLint success = 0;
GLint logSize = 0;
glGetShaderiv(v, GL_COMPILE_STATUS, &success);
if (success == GL_FALSE){
    printf("Can not compile the vertex shader\n");
    glGetShaderiv(v, GL_INFO_LOG_LENGTH, &logSize);
    if(logSize > 0) {
        glGetShaderInfoLog(v, logSize, &logSize, log);
        printf("Program Info Log:v %s\n", log);
    }
}
glGetShaderiv(f, GL_COMPILE_STATUS, &success);
if (success == GL_FALSE){
    printf("Can not compile the fragment shader\n");
    glGetShaderiv(f, GL_INFO_LOG_LENGTH, &logSize);
    if(logSize > 0) {
        glGetShaderInfoLog(f, logSize, &logSize, log);
        printf("Program Info Logf: %s\n", log);
    }
}

    p = glCreateProgram();
    glAttachShader(p,f);
    glAttachShader(p,v);
    glLinkProgram(p);

glValidateProgram(p);
glGetProgramiv(p, GL_LINK_STATUS, &success);
if (success == GL_FALSE){
    glGetProgramiv(p, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
        glGetProgramInfoLog(p, logLen, &logLen, log);
        printf("The log: %s\n", log);
    }
    return 0;
} else {
    printf("Shader succeeds.\n");
    return(p);
}

}



void load_texture(char * filename, int tid)
{
    FILE *fopen(), *fptr;
    char buf[512];
    int im_size, im_width, im_height, max_color;
    unsigned char* texture_bytes, *parse;

    fptr=fopen(filename, "r");
    if (fptr==NULL) printf("fptr=NULL\n");
    fgets(buf,512,fptr);
    do {        
    fgets(buf,512,fptr);
    } while (buf[0]=='#');
    if (buf==NULL) printf("buf=NULL\n");
    parse = strtok(buf, " \t");
    im_width = atoi(parse);

    parse = strtok(NULL, " \n");
    im_height = atoi(parse);

    fgets(buf, 512, fptr);
    parse = strtok(buf, " \n");
    max_color = atoi(parse);
    im_size = im_width*im_height;
    texture_bytes = (unsigned char*) calloc(3, im_size);
    fread(texture_bytes,3,im_size, fptr);
    fclose(fptr);

    glBindTexture(GL_TEXTURE_2D, tid);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, im_width, im_height,0, GL_RGB,
    GL_UNSIGNED_BYTE, texture_bytes);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    cfree(texture_bytes);
}

void set_uniform(int p,char* name,int index)
{
    int location;
    location = glGetUniformLocation(p,name);
    glUniform1i(location,index);
    
    location = glGetUniformLocation(p,name);
    glUniform1i(location,index);    
}

void getout(unsigned char key,int x, int y)
{
    switch(key){
        case 'q':
        case 'Q':
        case 27:
            exit(1);
        case 's':
            glRotatef(5.0,0.0,1.0,0.0);
            renderScene();
            break;
        default :
            break;
    }
}


int main (int argc, char ** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(XRES, YRES);
    glutCreateWindow("phong-shaded teapot");    

    readFile();

    p = set_shaders("teapot.vert", "teapot.frag");
    g = set_shaders("ground.vert", "ground.frag");
    load_texture("ground.ppm", 1);
    load_texture("teapotTex.ppm", 2);
    load_texture("normal.ppm", 3);
    load_texture("front.ppm", 4);
    load_texture("back.ppm", 5);
    load_texture("left.ppm", 6);
    load_texture("right.ppm", 7);
    load_texture("top.ppm", 8);
    load_texture("bottom.ppm", 9);
    build_shadowmap();

    glutDisplayFunc(renderScene);
    glutKeyboardFunc(getout);
    glutMainLoop();
    
    return 0;
}
        
