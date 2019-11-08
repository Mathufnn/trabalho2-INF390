#include <bits/stdc++.h>
#include <GL/glut.h> // compilar com -lGL -lGLU -lglut
using namespace std;

// Coisas do opengl

struct vertice
{
  int id;
  double x, y, z;
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
  int contadorDeErros = 0;

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

int rotacao = 0;
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

void rodarSubmarino(int tempo)
{
  // glLoadIdentity();
  rotacao += 1;

  glutPostRedisplay();
  glutTimerFunc(10, rodarSubmarino, 0);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -2.0f);
  
  glRotatef(rotacao, 1, 0, 0);

  glBegin(GL_TRIANGLES);
  for (int i = 0; i < obj.faces.size(); i++)
  {
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    int multiplicador = 5000;

    glVertex3f(obj.faces[i].vert1.x / (float)multiplicador, obj.faces[i].vert1.y / (float)multiplicador, obj.faces[i].vert1.z / (float)multiplicador);
    glVertex3f(obj.faces[i].vert2.x / (float)multiplicador, obj.faces[i].vert2.y / (float)multiplicador, obj.faces[i].vert2.z / (float)multiplicador);
    glVertex3f(obj.faces[i].vert3.x / (float)multiplicador, obj.faces[i].vert3.y / (float)multiplicador, obj.faces[i].vert3.z / (float)multiplicador);
  }
  glEnd();

  glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height)
{
  if (height == 0)
    height = 1;
  GLfloat aspect = (GLfloat)width / (GLfloat)height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

// Terminando coisas do opengl

int main(int argc, char **argv)
{
  // imprimeObjeto(obj);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(50, 50);
  glutCreateWindow(title);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  initGL();
  rodarSubmarino(100);
  glutMainLoop();

  return 0;
}