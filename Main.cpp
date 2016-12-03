#include "cstdlib"
#include <cmath>
#include "glut.h"
#include "Simulation.h"
#include<iostream>
#include<fstream>
#include<set>
using namespace std;


Simulation Pedestrain;
ofstream myfile;
bool update= false;

void render()
{   
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	
	glPointSize(1.5);
	glColor3f(1,0,0);

	glBegin(GL_POINTS);

	std::vector<People*> &pv= Pedestrain.getPeople();
	std::size_t size= pv.size();
	
	for(unsigned int i=0; i <size; ++i)
	{
		if (pv[i]->get_type() == PEOP)
		{ 
			
			glColor3f(1,0,1);
			glVertex2f(pv[i]->get_pos().getx(), pv[i]->get_pos().gety());
			
		}
		else if (pv[i]->get_type() == WAL)
		{   glColor3f(1,0,0);
			glVertex2f(pv[i]->get_pos().getx(), pv[i]->get_pos().gety());
		}
	}	
	glEnd();

	glutSwapBuffers();


}

void idle()
{   
	if (update)
		Pedestrain.updateStatus();

	render();
}

void keyboard(unsigned char c, int x, int y)
{
  switch(c)
  {
    // Quit
    case 27:
    case 'q':
    case 'Q':
      exit(0);
      break;
    
    case ' ':
		update= !update;
		break;
    }
}


void init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(WIN_LEFT-30,WIN_RIGHT+30,WIN_BOTTOM-20,WIN_TOP+20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glEnable(GL_POINT_SMOOTH);
	
    Pedestrain.generatepts();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize(600,300);
	glutCreateWindow("Pedestrain");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);	

    init();	
	glutMainLoop();
	
}

