void draw_room(float xrot, float yrot, float pos_x, float pos_y, float pos_z, float base, float width, float depth, float height) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();

    glRotatef(yrot, 1.0f, 0.0f, 0.0f);
    glRotatef(xrot*57.1666666667, 0.0f, 1.0f, 0.0f);
    glTranslatef(pos_x, pos_y, pos_z);

    //floor
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);

    glVertex3f(-width, base, -depth);
    glVertex3f(width, base, -depth);
    glVertex3f(width, base, 0.0f);
    glVertex3f(-width, base, 0.0f);
    glEnd();

    //far wall
    glBegin(GL_QUADS);
    glColor3f(0.65882352941f, 1.0f, 1.0f);

    glVertex3f(-width, base, -depth);
    glVertex3f(width, base, -depth);
    glVertex3f(width, base + height, -depth);
    glVertex3f(-width, base + height, -depth);
    glEnd();

    //ceiling
    glBegin(GL_QUADS);
    glColor3f(0.82745098039f, 0.82745098039f, 0.82745098039f);

    glVertex3f(-width, base + height, -depth);
    glVertex3f(width, base + height, -depth);
    glVertex3f(width, base + height, 0.0f);
    glVertex3f(-width, base + height, 0.0f);
    glEnd();

    //left wall
    glBegin(GL_QUADS);
    glColor3f(0.88235294117f, 1.0f, 0.88235294117f);

    glVertex3f(-width, base, 0.0f);
    glVertex3f(-width, base, -depth);
    glVertex3f(-width, base + height, -depth);
    glVertex3f(-width, base + height, 0.0f);
    glEnd();

    //right wall
    glBegin(GL_QUADS);
    glColor3f(0.88235294117f, 1.0f, 0.88235294117f);

    glVertex3f(width, base, 0.0f);
    glVertex3f(width, base, -depth);
    glVertex3f(width, base + height, -depth);
    glVertex3f(width, base + height, 0.0f);
    glEnd();

    //near wall    
    glBegin(GL_QUADS);
    glColor3f(0.65882352941f, 1.0f, 1.0f);

    glVertex3f(-width, base, 0.0f);
    glVertex3f(-width, base + height, 0.0f);
    glVertex3f(width, base + height, 0.0f);
    glVertex3f(width, base, 0.0f);

    glEnd();
}

void draw_cube(float x, float y, float z, float width, float height, float depth) {
    //bottom
    glBegin(GL_QUADS);
    glColor3f(0.36078431372f, 0.25098039215f, 0.2f);
    glVertex3f(x, y, z - depth);
    glVertex3f(x + width, y, z - depth);
    glVertex3f(x + width, y, z);
    glVertex3f(x, y, z);
    glEnd();

    //blue
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x, y, z);
    glVertex3f(x, y + height, z);
    glVertex3f(x + width, y + height, z);
    glVertex3f(x + width, y, z);
    glEnd();

    //red
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(x + width, y, z - depth);
    glVertex3f(x + width, y + height, z - depth);
    glVertex3f(x, y + height, z - depth);
    glVertex3f(x, y, z - depth);
    glEnd();

    //pink/purple
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(x, y, z);
    glVertex3f(x, y + height, z);
    glVertex3f(x, y + height, z - depth);
    glVertex3f(x, y, z - depth);
    glEnd();

    //blue/green
    glBegin(GL_QUADS);
    glColor3f(0.1f, 0.5f, 1.0f);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y + height, z);
    glVertex3f(x + width, y + height, z - depth);
    glVertex3f(x + width, y, z - depth);
    glEnd();

    //top
    glBegin(GL_QUADS);
    glColor3f(0.36078431372f, 0.25098039215f, 0.2f);
    glVertex3f(x, y + height, z - depth);
    glVertex3f(x + width, y + height, z - depth);
    glVertex3f(x + width, y + height, z);
    glVertex3f(x, y + height, z);
    glEnd();
}
