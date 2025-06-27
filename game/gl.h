static inline void glStart(void) {
    glKosInit();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

static inline void glHUDMode(void) {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE); // Optionally disable depth writes
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static inline void glDrawHUD(int average[]) {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(average[0] - 10, average[1] - 10);
    glVertex2f(average[0] + 10, average[1] - 10);
    glVertex2f(average[0] + 10, average[1] + 10);
    glVertex2f(average[0] - 10, average[1] + 10);
    glEnd();
}

static inline void glResume(void) {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
