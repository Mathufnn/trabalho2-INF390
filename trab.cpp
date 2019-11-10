#include <bits/stdc++.h>
#include <GL/glut.h> // compilar com -lGL -lGLU -lglut
using namespace std;

GLdouble posx = 0.0;
GLdouble posy = 0.0;
GLdouble posz = 0.0;
GLdouble centerX = 0.0;
GLdouble centerY = 0.0;
GLdouble centerZ = 0.0;
GLdouble upX = 0.0;
GLdouble upY = 0.0;
GLdouble upZ = 0.0;

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

      int v1 = atoi(tokens[1].c_str()) - 1; // transforma string em int
      int v2 = atoi(tokens[4].c_str()) - 1;
      int v3 = atoi(tokens[7].c_str()) - 1;

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

double rotacao = 1; // variaveis para testar se o submarino esta sendo mostrado corretamente
bool rotacionar = 0;
char title[] = "Psychedelic submarine";
objeto3d obj = leObjeto("submarine2.obj");
objeto3d obj1 = leObjeto("cavalo.obj");
objeto3d obj2 = leObjeto("navio.obj");
objeto3d obj3 = leObjeto("leao_marinho.obj");

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
  if (rotacionar)
  {
    rotacao += 1;
  }

  glutPostRedisplay();
  glutTimerFunc(10, rodarSubmarino, 0);
}

void keyboard(unsigned char key, int x, int y)
{
  if (key == 'R' || key == 'r') // R ou r
  {
    rotacionar = !rotacionar;
  }
  else if (key == 'W' || key == 'w')
  {
    posz -= 0.1;
  }
  else if (key == 'S' || key == 's')
  {
    posz += 0.1;
  }

  cout << posx << " / " << posy << " / " << posz << " / " << (int)rotacao % 360 << " graus" << endl;
}

void keyboard2(int key, int x, int y)
{

  if (key == GLUT_KEY_UP)
  {
    posy += 0.1;
  }
  else if (key == GLUT_KEY_DOWN)
  {
    posy -= 0.1;
  }
  else if (key == GLUT_KEY_RIGHT)
  {
    rotacao += 5;
  }
  else if (key == GLUT_KEY_LEFT)
  {
    rotacao -= 5;
  }

  cout << posx << " / " << posy << " / " << posz << " / " << (int)rotacao % 360 << " graus" << endl;
}

void desenhaCubo()
{
  glBegin(GL_QUADS);               // Draw The Cube Using quads
  glColor3f(0.0f, 1.0f, 0.0f);     // Color Blue
  glVertex3f(1.0f, 1.0f, -1.0f);   // Top Right Of The Quad (Top)
  glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Left Of The Quad (Top)
  glVertex3f(-1.0f, 1.0f, 1.0f);   // Bottom Left Of The Quad (Top)
  glVertex3f(1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)
  glColor3f(1.0f, 0.5f, 0.0f);     // Color Orange
  glVertex3f(1.0f, -1.0f, 1.0f);   // Top Right Of The Quad (Bottom)
  glVertex3f(-1.0f, -1.0f, 1.0f);  // Top Left Of The Quad (Bottom)
  glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Quad (Bottom)
  glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom Right Of The Quad (Bottom)
  glColor3f(1.0f, 0.0f, 0.0f);     // Color Red
  glVertex3f(1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
  glVertex3f(-1.0f, 1.0f, 1.0f);   // Top Left Of The Quad (Front)
  glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom Left Of The Quad (Front)
  glVertex3f(1.0f, -1.0f, 1.0f);   // Bottom Right Of The Quad (Front)
  glColor3f(1.0f, 1.0f, 0.0f);     // Color Yellow
  glVertex3f(1.0f, -1.0f, -1.0f);  // Top Right Of The Quad (Back)
  glVertex3f(-1.0f, -1.0f, -1.0f); // Top Left Of The Quad (Back)
  glVertex3f(-1.0f, 1.0f, -1.0f);  // Bottom Left Of The Quad (Back)
  glVertex3f(1.0f, 1.0f, -1.0f);   // Bottom Right Of The Quad (Back)
  glColor3f(0.0f, 0.0f, 1.0f);     // Color Blue
  glVertex3f(-1.0f, 1.0f, 1.0f);   // Top Right Of The Quad (Left)
  glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Left Of The Quad (Left)
  glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Quad (Left)
  glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom Right Of The Quad (Left)
  glColor3f(1.0f, 0.0f, 1.0f);     // Color Violet
  glVertex3f(1.0f, 1.0f, -1.0f);   // Top Right Of The Quad (Right)
  glVertex3f(1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
  glVertex3f(1.0f, -1.0f, 1.0f);   // Bottom Left Of The Quad (Right)
  glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom Right Of The Quad (Right)
  glEnd();

  return;
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(1, posy + 1, posz + 1, 0, posy, posz, 0, 1, 0);
  //desenhaCubo();
  glColor3f(0.0f, 0.0f, 1.0f); // Color Blue

  glutSolidCube(20);
  glTranslatef(0, posy, posz);
  glRotatef(-90, 1, 0, 0);
  glRotatef(rotacao, 0, 0, 1);

  int cor = 0;



// submarino
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < obj.faces.size(); i++)
  {
    cor = (cor + 1) % 3;
    if (cor == 0)
      glColor3f(1, 0, 0);
    else if (cor == 1)
      glColor3f(0, 1, 0);
    else if (cor == 2)
      glColor3f(0, 0, 1);
    int multiplicador = 100;

    // Desenha todos os triangulos carregados do arquivo de entrada
    glVertex3f(obj.faces[i].vert1.x / (float)multiplicador, obj.faces[i].vert1.y / (float)multiplicador, obj.faces[i].vert1.z / (float)multiplicador);
    glVertex3f(obj.faces[i].vert2.x / (float)multiplicador, obj.faces[i].vert2.y / (float)multiplicador, obj.faces[i].vert2.z / (float)multiplicador);
    glVertex3f(obj.faces[i].vert3.x / (float)multiplicador, obj.faces[i].vert3.y / (float)multiplicador, obj.faces[i].vert3.z / (float)multiplicador);
  }
  glEnd();



// cavalo marinho
  glColor3f(1, 0, 0);

  glRotatef(180, 0, -1, -1);
  glTranslatef(0, 0, 2);
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < obj1.faces.size(); i++)
  {
    int multiplicador = 200;
    glVertex3f(obj1.faces[i].vert1.x / (float)multiplicador, obj1.faces[i].vert1.y / (float)multiplicador, obj1.faces[i].vert1.z / (float)multiplicador);
    glVertex3f(obj1.faces[i].vert2.x / (float)multiplicador, obj1.faces[i].vert2.y / (float)multiplicador, obj1.faces[i].vert2.z / (float)multiplicador);
    glVertex3f(obj1.faces[i].vert3.x / (float)multiplicador, obj1.faces[i].vert3.y / (float)multiplicador, obj1.faces[i].vert3.z / (float)multiplicador);
  }
  glEnd();



// navio
  glColor3f(0, 1, 1);

  glRotatef(0, 0, 1, 1);
  glTranslatef(1, 0, -1);

  glBegin(GL_TRIANGLES);
  for (int i = 0; i < obj2.faces.size(); i++)
  {
    int multiplicador = 1;
    glVertex3f(obj2.faces[i].vert1.x / (float)multiplicador, obj2.faces[i].vert1.y / (float)multiplicador, obj2.faces[i].vert1.z / (float)multiplicador);
    glVertex3f(obj2.faces[i].vert2.x / (float)multiplicador, obj2.faces[i].vert2.y / (float)multiplicador, obj2.faces[i].vert2.z / (float)multiplicador);
    glVertex3f(obj2.faces[i].vert3.x / (float)multiplicador, obj2.faces[i].vert3.y / (float)multiplicador, obj2.faces[i].vert3.z / (float)multiplicador);
  }
  glEnd();



// leão marinho
  glColor3f(1, 0, 1);
  glRotatef(0, 0, 1, 1);
  glTranslatef(0, 0, 1);

  glBegin(GL_TRIANGLES);
  for (int i = 0; i < obj3.faces.size(); i++)
  {
    int multiplicador = 400;
    glVertex3f(obj3.faces[i].vert1.x / (float)multiplicador, obj3.faces[i].vert1.y / (float)multiplicador, obj3.faces[i].vert1.z / (float)multiplicador);
    glVertex3f(obj3.faces[i].vert2.x / (float)multiplicador, obj3.faces[i].vert2.y / (float)multiplicador, obj3.faces[i].vert2.z / (float)multiplicador);
    glVertex3f(obj3.faces[i].vert3.x / (float)multiplicador, obj3.faces[i].vert3.y / (float)multiplicador, obj3.faces[i].vert3.z / (float)multiplicador);
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
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(keyboard2);
  initGL();
  glutTimerFunc(100, rodarSubmarino, 0);
  glutMainLoop();

  return 0;
}