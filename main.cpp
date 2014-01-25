#include "initShaders.h"
#include <cstring>
#include <fstream>
#include <cstdlib>

#define NUM_OBJECTS 3

// My stupid object globals
int cycle = 0;
vector<void(*)(void)> objects;

// GL Globals
GLuint vaoID, vboID;
GLuint program;

// Courtesy of Austin Lund's OpenGL workshop
// TODO: Clean this up a bit
GLuint loadShader(string s,GLenum type)
{
    string shad;

    ifstream in(s.c_str());
    string line;
    while ( in.good() ) {
        getline (in,line);
        shad+=line+"\n";
    }
    in.close();

    GLchar* vShad = new char [shad.length()+1];
    strcpy (vShad, shad.c_str());

    GLuint vs;
    vs = glCreateShader(type);
    glShaderSource(vs, 1, (const GLchar**)(&vShad), NULL);
    glCompileShader(vs);

    delete[] vShad;

    return vs;
}


// More GL Prep
void init(void)
{
	program = glCreateProgram();
	GLuint vs = loadShader("vshader.glsl",GL_VERTEX_SHADER);
	GLuint fs = loadShader("fshader.glsl",GL_FRAGMENT_SHADER);

	glAttachShader(program,vs);
	glAttachShader(program,fs);

	glLinkProgram(program);

	glUseProgram(program);
}



void triangle(void)
{
	fprintf(stderr,"Triangling...\n");
	GLfloat vertices[] = {0.5f,-0.5f,0.0f,
						  0.0f,0.5f,0.0f,
						  -0.5f,-0.5f,0.0f};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*) 0);


	glDrawArrays(GL_TRIANGLES, 0, 3);

}

void square(void)
{
	fprintf(stderr,"Squaring...\n");
	GLfloat vertices[] = {-0.5f,-0.5f,0.0f,
						  0.5f ,-0.5f,0.0f,
						  0.5f ,0.5f ,0.0f,
						  -0.5f,0.5f ,0.0f};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*) 0);

	glDrawArrays(GL_QUADS, 0, 4);


}

void hex(void)
{
	fprintf(stderr,"Starring...\n");
	GLfloat vertices[] = {
		-0.25f,-0.5f,0.0f,	
		-0.5f,0.0f,0.0f,
		-0.25f,0.5f,0.0f,
		0.25f,0.5f,0.0f,
		0.5,0.0f,0.0f,
		0.25,-0.5f,0.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0,(void*) 0);

	glDrawArrays(GL_POLYGON, 0, sizeof(vertices)/sizeof(GLfloat));


}



void onMouse(int button, int state, int x, int y)
{
	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
		exit(0);

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
		cycle = (cycle + 1) % NUM_OBJECTS;

	glutPostRedisplay();
}

void showStuff(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);


	objects[cycle]();

	glFlush();
}


int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutCreateWindow("Lab 1");

	glutInitContextVersion(4,3);
	glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_COMPATIBILITY_PROFILE); // TODO: Look this up

	glewInit();
	init();
	
	objects.push_back(triangle);
	objects.push_back(square);
	objects.push_back(hex);



	glutDisplayFunc(showStuff);
	glutMouseFunc(onMouse);
	glutMainLoop();
	return 0;
}
