#include <bits/stdc++.h>
#include <GL/glut.h> // GLUT, include glu.h and gl.h
using namespace std;

// Coisas do opengl

struct vertice
{
  int id;
  float x, y, z;
};

struct face
{
  int idvertice1, idvertice2, idvertice3;
  vertice vert1, vert2, vert3;
};

struct objeto3d
{
  vector<vertice> vertices;
  vector<face> faces;
};

vector<string> quebraString(string str)
{
  vector<string> tokens;
  string temp = "";

  for (int i = 0; i < str.size() + 1; i++)
  {
    if (str[i] != ' ' && str[i] != '/' && str[i] != '\0') // caractere valido
    {
      temp += str[i];
    }
    else // caractere delimitador
    {
      if (!temp.empty())
        tokens.push_back(temp);
      temp = "";
    }
  }
  return tokens;
}

objeto3d leObjeto(string arquivo)
{
  ifstream objeto;
  objeto.open(arquivo);
  string linha;
  objeto3d obj;
  int contadorVertices = 0; // numero do vertice, usado para indexar vertices

  while (getline(objeto, linha))
  {
    vector<string> tokens;
    tokens = quebraString(linha);

    if (tokens[0] == "v")
    {
      vertice v;

      v.x = atof(tokens[1].c_str()); // transforma string em float
      v.y = atof(tokens[2].c_str());
      v.z = atof(tokens[3].c_str());
      v.id = contadorVertices;
      contadorVertices++;

      obj.vertices.push_back(v);
    }
    else if (tokens[0] == "f")
    {
      face f;

      float v1 = atoi(tokens[1].c_str()); // transforma string em int
      float v2 = atoi(tokens[4].c_str());
      float v3 = atoi(tokens[7].c_str());

      f.idvertice1 = v1;
      f.idvertice2 = v2;
      f.idvertice3 = v3;
      f.vert1 = obj.vertices[v1];
      f.vert2 = obj.vertices[v2];
      f.vert3 = obj.vertices[v3];

      obj.faces.push_back(f);
    }
  }

  return obj;
}

void imprimeObjeto(objeto3d obj)
{
  for (int i = 0; i < 10; i++) // imprime os vertices
  {
    cout << obj.vertices[i].id << " " << obj.vertices[i].x << " " << obj.vertices[i].y
         << " " << obj.vertices[i].x << endl;
  }

  for (int i = 0; i < 10; i++) // imprime as faces
  {
    cout << obj.faces[i].vert1.id << " " << obj.faces[i].vert1.x << " "
         << obj.faces[i].vert1.y << " " << obj.faces[i].vert1.z << "  //  "
         << obj.faces[i].vert2.id << " " << obj.faces[i].vert2.x << " "
         << obj.faces[i].vert2.y << " " << obj.faces[i].vert2.z << "  //  "
         << obj.faces[i].vert3.id << " " << obj.faces[i].vert3.x << " "
         << obj.faces[i].vert3.y << " " << obj.faces[i].vert3.z << endl;
  }
}

char title[] = "3D Shapes";
objeto3d obj = leObjeto("submarine_triangulated.obj");

void initGL()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);              // Set background color to black and opaque
  glClearDepth(1.0f);                                // Set background depth to farthest
  glEnable(GL_DEPTH_TEST);                           // Enable depth testing for z-culling
  glDepthFunc(GL_LEQUAL);                            // Set the type of depth-test
  glShadeModel(GL_SMOOTH);                           // Enable smooth shading
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice perspective corrections
}



void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);                         // To operate on model-view matrix

  glLoadIdentity();                // Reset the model-view matrix
  glTranslatef(0.0f, 0.0f, -6.0f); // Move left and into the screen

  glRotatef(45,1,0,0);
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < obj.faces.size(); i++)
  {
    glColor3f(1.0f, 0.0f, 0.0f); // Red

    glVertex3f(obj.faces[i].vert1.x / 5000, obj.faces[i].vert1.y / 5000, obj.faces[i].vert1.z / 5000);
    glVertex3f(obj.faces[i].vert2.x / 5000, obj.faces[i].vert2.y / 5000, obj.faces[i].vert2.z / 5000);
    glVertex3f(obj.faces[i].vert3.x / 5000, obj.faces[i].vert3.y / 5000, obj.faces[i].vert3.z / 5000);
  }

  glEnd();

  glutSwapBuffers(); // Swap the front and back frame buffers (double buffering)
}

void reshape(GLsizei width, GLsizei height)
{ // GLsizei for non-negative integer
  // Compute aspect ratio of the new window
  if (height == 0)
    height = 1; // To prevent divide by 0
  GLfloat aspect = (GLfloat)width / (GLfloat)height;

  // Set the viewport to cover the new window
  glViewport(0, 0, width, height);

  // Set the aspect ratio of the clipping volume to match the viewport
  glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
  glLoadIdentity();            // Reset
  // Enable perspective projection with fovy, aspect, zNear and zFar
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

// Terminando coisas do opengl

int main(int argc, char **argv)
{
  glutInit(&argc, argv);            // Initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
  glutInitWindowSize(640, 480);     // Set the window's initial width & height
  glutInitWindowPosition(50, 50);   // Position the window's initial top-left corner
  glutCreateWindow(title);          // Create window with the given title
  glutDisplayFunc(display);         // Register callback handler for window re-paint event
  glutReshapeFunc(reshape);         // Register callback handler for window re-size event
  initGL();                         // Our own OpenGL initialization
  glutMainLoop();                   // Enter the infinite event-processing loop
  return 0;
  //
  //imprimeObjeto(obj);
}