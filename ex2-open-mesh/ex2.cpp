//
//  ex0.cpp
//  cg-projects
//

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "ex2.h"
#include "Model.h"
#include "ShaderIO.h"


/** Internal Definitions */

#define	WINDOW_SIZE         (600) // initial size of the window               //
#define	WINDOW_POS_X        (100) // initial X position of the window         //
#define	WINDOW_POS_Y        (100) // initial Y position of the window         //

#define RC_OK                 (0) // Everything went ok                       //
#define RC_INVALID_ARGUMENTS  (1) // Invalid arguments given to the program   //
#define RC_INPUT_ERROR        (2) // Invalid input to the program             //

#define	ARGUMENTS_PROGRAM     (0) // program name position on argv            //
#define	ARGUMENTS_INPUTFILE   (1) // given input file position on argv        //
#define	ARGUMENTS_REQUIRED    (2) // number of required arguments             //

/** Key definitions */

#define KEY_ANIMATE						('a') // Key used to start animation              //
#define KEY_ESC							('\e') // Key used to terminate the program - ESC  //
#define KEY_QUIT						('q') // Key used to terminate the program        //
#define KEY_RESET						('r') // Key used to reset the applied TX's	      //
#define KEY_RELOAD						('l') // Key used to reload the shaders 	      //
#define KEY_WIREFRAME					('w') // Key used to toggle wireframe rendering   //
#define KEY_PROJECTION					('p') // Key used to toggle projection rendering   //
#define KEY_FACE						('n') // Key used to toggle face normal calculation //
#define KEY_PHONG_COLOR					('1') // Key used to toggle Phong shading model   //
#define KEY_GOURAUD_COLOR				('2') // Key used to toggle Gouraud shading model   //
#define KEY_BOX_COLOR					('3') // Key used to toggle bounding box coloring   //
#define KEY_INCREASE_ALPHA				('=') // Key used increase alpha in shading model   //
#define KEY_DECREASE_ALPHA				('-') // Key used decrease alpha in shading model   //
#define KEY_TEXTURE_STATE				('t') // Key used change textures in phong shading model   //
#define KEY_DECREASE_TEXTURE_SCALE		('s') // Key used to decrease the scaling of the texture coordinates in phong shading model   //
#define KEY_INECREASE_TEXTURE_SCALE		('d') // Key used to inecrease the scaling of the texture coordinates in phong shading model   //
#define KEY_DECREASE_TURB_SCALE			('f') // Key used to decrease the turbulence magnitude in phong shading model   //
#define KEY_INECREASE_TURB_SCALE		('g') // Key used to inecrease the turbulence magnitude in phong shading model   //
#define KEY_GLOW_MODE					('v') // Key used to change the glow mode
#define KEY_DECREASE_KERNEL_SIZE		('h') // Key used to decrease the gaussian kernel size
#define KEY_INCREASE_KERNEL_SIZE		('j') // Key used to increase the gaussian kernel size

/** display callback */
void display(void);

/** window reshape callback  */
void windowResize(int width, int height);

/** keyboard callback  */
void keyboard(unsigned char key, int x, int y);

/** mouse click callback */
void mouse(int button, int state, int x, int y) ;

/** mouse dragging callback  */
void motion(int x, int y) ;

/** mouse scralling callback */
void mouseWheel(int button, int dir, int x, int y);

/** Global variables */

int     g_nFPS = 0, g_nFrames = 0;              // FPS and FPS Counter
int     g_dwLastFPS = 0;                        // Last FPS Check Time
bool    g_startAnimation = false;
bool    g_duringAnimation = true;

// A global variable for our model (a better practice would be to use a singletone that holds all model):
Model _model;

/** main function */
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ex2b <input-file>" << std::endl;
		return 1;
	}

	std::cout << "Starting ex2b..." << std::endl;

	// Initialize GLUT
    glutInit(&argc, argv) ;
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE) ;
#else
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#endif
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
    glutCreateWindow("CGEx2: Go figure");

	// Initialize GLEW
    glewExperimental = GL_TRUE;
    int glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit(1);
    }

#ifdef __APPLE__
    GLint sync = 1;
    CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
#endif

	// Set callback functions:
    glutDisplayFunc(display) ;
    glutReshapeFunc(windowResize) ;
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
	glutMouseWheelFunc(mouseWheel);

	// Init anything that can be done once and for all:
	_model.init(argv[1]); //dragon_20k, bunny_10k, m89,

	// Set clear color to black:
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Start events/drawing loop
	glutMainLoop();

	return 0;
}

void display(void)
{
	// Clear the screen buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Let the model to draw itself...
	_model.draw();

	// Swap those buffers so someone will actually see the results... //
    glutSwapBuffers();
}

// This method is called when the window is resized
void windowResize(int w, int h)
{
    // Update model to fit the new resolution
	_model.resize(w, h);

    // set the new viewport //
    glViewport(0, 0, w, h);

    // Refresh the display //
    glutPostRedisplay();
}

/********************************************************************
 * Function  :	keyboard
 * Arguments :	key : the key that was pressed
 *              x   : x value of the current mouse location
 *              y   : y value of the current mouse location
 * Returns   : n/a
 * Throws    : n/a
 *
 * Purpose   : This function handles all the keyboard input from the user.
 *             It supports terminating the application when the KEY_QUIT is pressed.
 *
 \******************************************************************/
void keyboard(unsigned char key, int x, int y)
{
    unsigned int lower_key = tolower(key);

    switch(lower_key)
    {
        case KEY_RESET:
            // reset to initial view of the object
            // For use in a future exercise
			_model.reset();
			glutPostRedisplay();
            break;
		case KEY_PROJECTION:
			_model.toggleperspectiveMode();
			glutPostRedisplay();
			break;
        case KEY_RELOAD:
            // Reload the shading programs of the object
            // For use in a future exercise
            break;
        case KEY_WIREFRAME:
            // Toggle wireframe mode
			_model.toggleWireframeMode();
			glutPostRedisplay();
            break;
        case KEY_ANIMATE:
            if (!g_duringAnimation) {
                g_startAnimation = true;
            }
            break;
		case KEY_FACE:
			_model.toggleFaceNormal();
            glutPostRedisplay();
			break;

		case KEY_PHONG_COLOR:
			_model.setShadingMode(Model::SHADE_PHONG);
			glutPostRedisplay();
			break;

        case KEY_GOURAUD_COLOR:
			_model.setShadingMode(Model::SHADE_GOURAUD);
			glutPostRedisplay();
			break;

        case KEY_BOX_COLOR:
            _model.setShadingMode(Model::SHADE_RGB);
			glutPostRedisplay();
			break;

        case KEY_INCREASE_ALPHA:
            _model.scaleAlpha(true);
			glutPostRedisplay();
			break;

        case KEY_DECREASE_ALPHA:
            _model.scaleAlpha(false);
			glutPostRedisplay();
			break;
		case KEY_TEXTURE_STATE:
			_model.changeTextureMode();
			glutPostRedisplay();
			break;
		case KEY_DECREASE_TEXTURE_SCALE:
			_model.scaleTexture(false);
			glutPostRedisplay();
			break;
		case KEY_INECREASE_TEXTURE_SCALE:
			_model.scaleTexture(true);
			glutPostRedisplay();
			break;
		case KEY_DECREASE_TURB_SCALE:
			_model.scaleTurbulence(false);
			glutPostRedisplay();
			break;
		case KEY_INECREASE_TURB_SCALE:
			_model.scaleTurbulence(true);
			glutPostRedisplay();
			break;
		case KEY_GLOW_MODE:
			_model.changeGlowMode();
			glutPostRedisplay();
			break;
		case KEY_DECREASE_KERNEL_SIZE:
			_model.changeKernelSize(false);
			glutPostRedisplay();
			break;
		case KEY_INCREASE_KERNEL_SIZE:
			_model.changeKernelSize(true);
			glutPostRedisplay();
			break;
        case KEY_QUIT:
        case KEY_ESC:
            // Terminate the program:
            exit(RC_OK);
            break;

        default:
            std::cerr << "Key " << lower_key << " undefined\n";
            break;
    }

    return;
}

/********************************************************************
 * Function  :   mouse
 * Arguments :   button  : the button that was engaged in some action
 *               state   : the new state of that button
 *               x       : x value of the current mouse location
 *               y       : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse actions.
 *
 \******************************************************************/
void mouse(int button, int state, int x, int y)
{
	_button = button;

	if (state == GLUT_DOWN)
	{
			_startX = x;
			_startY = y;
	}
	else if(button == GLUT_LEFT_BUTTON)
    {
		_model.updateFigurePos(_startX, _startY, x, y, true);
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
		_model.translate((float)_startX, (float)_startY, (float)x, (float)y, true);
    }
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		_model.zoom((float)_startX, (float)_startY, (float)x, (float)y, true);
	}

	glutPostRedisplay();

    return;
}

void mouseWheel(int button, int dir, int x, int y)
{
}

/********************************************************************
 * Function  :   motion
 * Arguments :   x   : x value of the current mouse location
 *               y   : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse dragging events.
 *
 \******************************************************************/
void motion(int x, int y)
{
	if (_button == GLUT_LEFT_BUTTON)
	{
		_model.updateFigurePos(_startX, _startY, x, y, false);
	}
	else if (_button == GLUT_RIGHT_BUTTON)
	{
		_model.translate((float)_startX, (float)_startY, (float)x, (float)y, false);
	}
	else if (_button == GLUT_MIDDLE_BUTTON)
	{
		_model.zoom((float)_startX, (float)_startY, (float)x, (float)y, false);
	}

	glutPostRedisplay();
}
