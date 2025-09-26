#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <math.h>


float angle = 0.0f;
int isPaused = 0;
float zoom = 1.0f;

// Function to draw arrows
void drawArrow(float x1, float y1, float x2, float y2, float arrowHeadSize) {
    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();

    // Arrowhead
    float angle = atan2(y2 - y1, x2 - x1);
    float arrowAngle1 = angle + M_PI / 8;
    float arrowAngle2 = angle - M_PI / 8;

    float x3 = x2 - arrowHeadSize * cos(arrowAngle1);
    float y3 = y2 - arrowHeadSize * sin(arrowAngle1);
    float x4 = x2 - arrowHeadSize * cos(arrowAngle2);
    float y4 = y2 - arrowHeadSize * sin(arrowAngle2);

    glBegin(GL_TRIANGLES);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
        glVertex2f(x4, y4);
    glEnd();
}

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void drawCoil() {
    glColor3f(1.0f, 0.5f, 0.0f); // Orange coil
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.5f, -0.3f);
        glVertex2f( 0.5f, -0.3f);
        glVertex2f( 0.5f,  0.3f);
        glVertex2f(-0.5f,  0.3f);
    glEnd();

    // Current flow arrows (left to right on bottom, right to left on top)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow arrows
    drawArrow(-0.4f, -0.3f, 0.4f, -0.3f, 0.03f); // Bottom side
    drawArrow(0.4f, 0.3f, -0.4f, 0.3f, 0.03f);  // Top side

    // Lorentz force arrows
    glColor3f(0.0f, 1.0f, 0.0f); // Green force arrows
    drawArrow(-0.5f, 0.0f, -0.5f, 0.15f, 0.03f); // Left coil force up
    drawArrow(0.5f, 0.0f, 0.5f, -0.15f, 0.03f);  // Right coil force down
}

void drawMagnets() {
    // Left magnet - North Pole (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.9f, -0.5f);
        glVertex2f(-0.7f, -0.5f);
        glVertex2f(-0.7f,  0.5f);
        glVertex2f(-0.9f,  0.5f);
    glEnd();

    // Right magnet - South Pole (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(0.7f, -0.5f);
        glVertex2f(0.9f, -0.5f);
        glVertex2f(0.9f,  0.5f);
        glVertex2f(0.7f,  0.5f);
    glEnd();

    // Labels for poles
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-0.83f, 0.55f, "N");
    drawText(0.77f, 0.55f, "S");

    // Magnetic field lines (left to right)
    glColor3f(0.3f, 1.0f, 1.0f);
    for (float y = -0.4f; y <= 0.4f; y += 0.2f) {
        drawArrow(-0.65f, y, 0.65f, y, 0.02f);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glScalef(zoom, zoom, 1.0f);

    drawMagnets();

    glPushMatrix();
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    drawCoil();
    glPopMatrix();

    glutSwapBuffers();
}

void update(int value) {
    if (!isPaused) {
        angle += 2.0f;
        if (angle > 360)
            angle -= 360;
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

void menuHandler(int option) {
    switch(option) {
        case 1: isPaused = 1; break;
        case 2: isPaused = 0; break;
        case 3: zoom += 0.1f; break;
        case 4: zoom -= 0.1f; break;
    }
    glutPostRedisplay();
}

void createMenu() {
    glutCreateMenu(menuHandler);
    glutAddMenuEntry("Pause", 1);
    glutAddMenuEntry("Resume", 2);
    glutAddMenuEntry("Zoom In", 3);
    glutAddMenuEntry("Zoom Out", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Electric Motor Simulation");

    init();
    createMenu();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}


