//
//  ex2.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include <time.h>

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

#define KEY_ESC                   ('\e') // Key used to terminate the program - ESC  //
#define KEY_QUIT                   ('q') // Key used to terminate the program        //
#define KEY_RESET                  ('r') // Key used to reset the applied TX's	      //
#define KEY_RELOAD                 ('l') // Key used to reload the shaders 	      //
#define KEY_WIREFRAME              ('w') // Key used to toggle wireframe rendering   //
#define KEY_PROJECTION             ('p') // Key used to toggle projection mode       //
#define KEY_NORMAL                 ('n') // Key used to toggle normal mode           //
#define KEY_INCREASE_SPEC          ('=') // Key used to increase specular coefficient//
#define KEY_DECREASE_SPEC          ('-') // Key used to decrease specular coefficient//
#define KEY_PHONG                  ('1') // Key used to switch to Phong shading mode //
#define KEY_GOURAUD                ('2') // Key used to switch to Gouraud shading mode //
#define KEY_COLOR                  ('3') // Key used to switch to pixel colors based on position //
#define KEY_INCREASE_TEXTURE_SCALE ('d')
#define KEY_DECREASE_TEXTURE_SCALE ('s')
#define KEY_INCREASE_TURB_COEFF    ('g')
#define KEY_DECREASE_TURB_COEFF    ('f')

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

// A global variable for our model (a better practice would be to use a singletone that holds all model):
Model _model;

/** main function */
int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: ./ex2 <input_file>" << std::endl;
        return 1;
    }

    srand((unsigned int)time(NULL));
    
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
    glutCreateWindow("CG ex2");
    
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
    
    // Init anything that can be done once and for all:
    _model.init(argv[1]);

    // Set clear color to black:
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
        
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
            _model.reset();
            break;
        case KEY_RELOAD:
            // Reload the shading programs of the object
            // For use in a future exercise
            break;
        case KEY_WIREFRAME:
            // Toggle wireframe mode
            _model.toggle_polygon_mode();
            break;
        case KEY_PROJECTION:
            // Toggle projection mode
            _model.toggle_projection_mode();
            break;
        case KEY_NORMAL:
            _model.toggle_normal_mode();
            break;
        case KEY_INCREASE_SPEC:
            _model.increase_spec();
            break;
        case KEY_DECREASE_SPEC:
            _model.decrease_spec();
            break;
        case KEY_PHONG:
            _model.set_shading_mode(SHADING_MODE_PHONG);
            break;
        case KEY_GOURAUD:
            _model.set_shading_mode(SHADING_MODE_GOURAUD);
            break;
        case KEY_COLOR:
            _model.set_shading_mode(SHADING_MODE_COLOR);
            break;
        case KEY_INCREASE_TEXTURE_SCALE:
            _model.increase_texture_scale();
            break;
        case KEY_DECREASE_TEXTURE_SCALE:
            _model.decrease_texture_scale();
            break;
        case KEY_INCREASE_TURB_COEFF:
            _model.increase_turb_coeff();
            break;
        case KEY_DECREASE_TURB_COEFF:
            _model.decrease_turb_coeff();
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

    glutPostRedisplay();
    
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
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            _model.begin_rotation(x, y);
        }
        else {
            _model.end_rotation();
        }
        break;
    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_DOWN) {
            _model.begin_zoom(x, y);
        }
        else {
            _model.end_zoom();
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN) {
            _model.begin_translation(x, y);
        }
        else {
            _model.end_translation();
        }
        break;
    }
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
    _model.update(x, y);

    glutPostRedisplay();
}
