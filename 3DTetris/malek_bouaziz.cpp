#include <GL/gl.h>	 // Open Graphics Library (OpenGL) header
#include <GL/glut.h> // The GL Utility Toolkit (GLUT) Header
#include <GL/glu.h>

#include <iostream>
#include <list>
#include <queue>
#include <memory>
#include <vector>

// Tetromino class definition (as provided earlier)
class tetromino {
public:
    // Tetromino shapes
    enum Shape { I, J, L, O, S, T, Z };

    // Constructor
    tetromino(Shape shape) : shape(shape) {
        initializeShape();
    }

    // Get the shape of the tetromino
    Shape getShape() const {
        return shape;
    }

    // Get the blocks of the tetromino
    const std::vector<std::vector<std::vector<int>>>& getBlocks() const {
        return blocks;
    }

private:
    Shape shape;
    std::vector<std::vector<std::vector<int>>> blocks;

    // Initialize the blocks of the tetromino based on its shape
    void initializeShape() {
        switch (shape) {
            case I:
                blocks = {{{1}, {1}, {1}, {1}}};
                break;
            case J:
                blocks = {{{1, 0, 0}, {1, 1, 1}}};
                break;
            case L:
                blocks = {{{0, 0, 1}, {1, 1, 1}}};
                break;
            case O:
                blocks = {{{1, 1}, {1, 1}}};
                break;
            case S:
                blocks = {{{0, 1, 1}, {1, 1, 0}}};
                break;
            case T:
                blocks = {{{0, 1, 0}, {1, 1, 1}}};
                break;
            case Z:
                blocks = {{{1, 1, 0}, {0, 1, 1}}};
                break;
        }
    }
};

// Function to display a Tetromino
void displayTetromino(const tetromino& t, int x, int y, int z, int color) {
    const auto& blocks = t.getBlocks();

    // Set color based on the color parameter
    GLfloat r, g, b;
    switch (color) {
        case 0: r = 0xBA / 255.0f; g = 0x20 / 255.0f; b = 0x20 / 255.0f; break; // Red BA2020
        case 1: r = 0x62 / 255.0f; g = 0xC4 / 255.0f; b = 0x00 / 255.0f; break; // Green 62C400
        case 2: r = 0x12 / 255.0f; g = 0xAD / 255.0f; b = 0xD4 / 255.0f; break; // Blue 12ADD4
        case 3: r = 0xDE / 255.0f; g = 0xE9 / 255.0f; b = 0x3C / 255.0f; break; // Yellow DEE93C
        case 4: r = 0x9E / 255.0f; g = 0x13 / 255.0f; b = 0xE4 / 255.0f; break; // Purple 9E13E4
        case 5: r = 0x08 / 255.0f; g = 0x47 / 255.0f; b = 0xF3 / 255.0f; break; // Blue 0847F3
        default: r = 1; g = 1; b = 1; break; // White
    }

    // Draw each block of the Tetromino
    for (size_t i = 0; i < blocks.size(); ++i) {
        for (size_t j = 0; j < blocks[i].size(); ++j) {
            for (size_t k = 0; k < blocks[i][j].size(); ++k) {
                if (blocks[i][j][k] != 0) {
                    glPushMatrix();
                    glTranslatef(x + i, y + j, z + k);

                    // Set color for the solid cube
                    glColor3f(r, g, b);
                    glutSolidCube(1);

                    // Calculate darker shade for the wireframe
                    GLfloat darkR = r * 0.5f;
                    GLfloat darkG = g * 0.5f;
                    GLfloat darkB = b * 0.5f;

                    // Draw wireframe edges
                    glLineWidth(2.0f); // Set line width to 2.0
                    glColor3f(darkR, darkG, darkB); // Set edge color to darker shade
                    glutWireCube(1.01); // Slightly larger to avoid z-fighting

                    glPopMatrix();
                }
            }
        }
    }
}

// Global variables for mouse control
float angleX = 0.0f;
float angleY = 0.0f;
float zoom = 1.0f;
int lastMouseX = 0;
int lastMouseY = 0;
bool isDragging = false;

// Function to initialize OpenGL settings
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
    glEnable(GL_DEPTH_TEST);          // Enable depth testing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Function to display the grid
void displayGrid() {
	int n = 15;		// grid length

		/* white grid */
		glBegin(GL_LINES);
			for(int i=0; i<=n; i++) {
				// set colour
				glColor3f(1, 0, 0);

				// x-z axis
				glVertex3f(i, 0, 0);
				glVertex3f(i, 0, n);

				glVertex3f(0, 0, i);
				glVertex3f(n, 0, i);
				
				glColor3f(0, 1, 0);
				// y-z axis
				glVertex3f(0, i, 0);
				glVertex3f(0, i, n);

				glVertex3f(0, 0, i);
				glVertex3f(0, n, i);

				glColor3f(0, 1, 1);
				// x-y axis
				glVertex3f(i, 0, 0);
				glVertex3f(i, n, 0);

				glVertex3f(0, i, 0);
				glVertex3f(n, i, 0);
			}
		glEnd();
}

// Display function to render the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glScalef(zoom, zoom, zoom); // Apply zoom
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);

    displayGrid();

    // Create a Tetromino of shape 'T'
    tetromino t(tetromino::T);

    // Display the Tetromino at position (1, 2, 3) with color 0 (Red)
    displayTetromino(t, 1, 2, 3, 0);

    glPopMatrix();

    glutSwapBuffers();
    glFlush();
}

// Mouse button event handler
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

// Mouse motion event handler
void mouseMotion(int x, int y) {
    if (isDragging) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        angleX += dy * 0.5f;
        angleY += dx * 0.5f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

// Mouse wheel event handler
void mouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) {
        zoom *= 1.1f; // Zoom in
    } else {
        zoom /= 1.1f; // Zoom out
    }
    glutPostRedisplay();
}
void mouseFunc(int button, int state, int x, int y) {
    if (button == 3 || button == 4) { // Scroll up or down
        mouseWheel(button, (button == 3) ? 1 : -1, x, y);
    } else {
        mouseButton(button, state, x, y);
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);                                      // GLUT initialization
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);   // Display Mode
    glutInitWindowPosition(100, 250);
    glutInitWindowSize(1920, 1080);                             // Set window size
    glutCreateWindow("TETRIS-3D");                              // Create Window

    init();
    glutDisplayFunc(display);                                   // Register Display Function
    glutMouseFunc(mouseButton);                                 // Register Mouse Button Function
    glutMotionFunc(mouseMotion);                                // Register Mouse Motion Function
    glutMouseFunc(mouseFunc);                                   // Register Mouse Function
	
    glutMainLoop();                                             // Run GLUT main loop

    return 0;
}