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



vector <string> texto;


void escreve()
{

vector<string> texto;
texto.push_back("Comandos do simulador");
  texto.push_back("Up (tecla direcional)  -  Mover (verticalmente para cima)");
texto.push_back("Down (tecla direcional)  -  Mover (verticalmente) para baixo");
texto.push_back("Down (tecla direcional)  -  Mover (verticalmente) para baixo");
texto.push_back("Left (tecla direcional)  -  Virar (aproximadamente) 5 graus para a direita");
texto.push_back("Right (tecla direcional) -  Virar (aproximadamente) 5 graus para a esquerda");
texto.push_back("S ou s  -  Ré");
texto.push_back("F ou f  -   Ponto de vista de fora do submarino");
texto.push_back("I ou i  -   Ponto de vista de dentro do submarino");
texto.push_back("H ou h   -   Apresentar/Ocultar um menu de ajuda (descrevendo os comandos do simulador)");


float conty= 0.1;
for (int j =0; j<texto.size(); j++){
  glPushMatrix();

    glColor3f(1,0,0);
  //  glTranslatef(posx-1, posy+1, posz);
        glTranslatef(posx-1, posy+1.1-conty, posz);

    glScalef(1/3500.38, 1/3500.38, 1/3500.38);
    for( int i=0; i<texto[j].size();i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, texto[j][i]);
    }
        glPopMatrix();
    conty+=0.05;

}
}

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
bool rodarSentidoHorario = 0, rodarSentidoAntiHorario = 0;
bool andarPraFrente = 0, andarPraTras = 0;
bool subir = 0, descer = 0;
bool vistaDeFora = 1;
bool ajuda=1;
char title[] = "Yellow submarine";
objeto3d submarino = leObjeto("submarine.obj");
objeto3d cavalo = leObjeto("cavalo.obj");
objeto3d navio = leObjeto("navio.obj");
objeto3d leao = leObjeto("leao_marinho.obj");

void initGL()
{


      glClearColor(0, 1, 1, 0.3); // cor para limpeza do buffer
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    


      gluPerspective(45.0f, 1, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
  glClearDepth(1.0f);         // Set background depth to farthest
  // glEnable(GL_DEPTH_TEST);    // Enable depth testing for z-culling
  //glDepthFunc(GL_NEVER); // Set the type of depth-test


  //glShadeModel(GL_SMOOTH);                           // Enable smooth shading
  //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice perspective corrections
}

void rodarSubmarino(int tempo)
{
  // glLoadIdentity();
  if (rodarSentidoHorario)
  {
    rotacao += 1;
  }
  else if (rodarSentidoAntiHorario)
  {
    rotacao -= 1;
  }
  else if (andarPraFrente)
  {
    posz -= 0.05;
  }
  else if (andarPraTras)
  {
    posz += 0.05;
  }
  else if (subir)
  {
    posy += 0.05;
  }
  else if (descer)
  {
    posy -= 0.05;
  }

  glutPostRedisplay();
  glutTimerFunc(10, rodarSubmarino, 0);
}

void keyboardNormal(unsigned char key, int x, int y)
{
  if (key == 'W' || key == 'w')
  {
    andarPraFrente = true;
  }
  else if (key == 'S' || key == 's')
  {
    andarPraTras = true;
  }
  else if (key == 'F' || key == 'f')
  {
    vistaDeFora = true;
    glutPostRedisplay();
  }
  else if (key == 'I' || key == 'i')
  {
    vistaDeFora = false;
    glutPostRedisplay();
  }
  else if (key == 'H' || key == 'h')
  {
    ajuda = !ajuda;
    glutPostRedisplay();
  }



}

void keyboardEspecial(int key, int x, int y)
{

  if (key == GLUT_KEY_UP)
  {
    subir = true;
  }
  else if (key == GLUT_KEY_DOWN)
  {
    descer = true;
  }
  else if (key == GLUT_KEY_RIGHT)
  {
    rodarSentidoHorario = true;
  }
  else if (key == GLUT_KEY_LEFT)
  {
    rodarSentidoAntiHorario = true;
  }

  // cout << posx << " / " << posy << " / " << posz << " / " << (int)rotacao % 360 << " graus" << endl;
}

void keyboardNormal_soltar(unsigned char key, int x, int y)
{
  if (key == 'W' || key == 'w')
  {
    andarPraFrente = false;
  }
  else if (key == 'S' || key == 's')
  {
    andarPraTras = false;
  }
}

void keyboardEspecial_soltar(int key, int x, int y)
{
  if (key == GLUT_KEY_UP)
  {
    subir = false;
  }
  else if (key == GLUT_KEY_DOWN)
  {
    descer = false;
  }
  else if (key == GLUT_KEY_RIGHT)
  {
    rodarSentidoHorario = false;
  }
  else if (key == GLUT_KEY_LEFT)
  {
    rodarSentidoAntiHorario = false;
  }
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

void desenhaObjeto(objeto3d obj, int multiplicador)
{
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < obj.faces.size(); i++)
  {
    glVertex3f(obj.faces[i].vert2.x / (float)multiplicador, obj.faces[i].vert2.y / (float)multiplicador, obj.faces[i].vert2.z / (float)multiplicador);
    glVertex3f(obj.faces[i].vert1.x / (float)multiplicador, obj.faces[i].vert1.y / (float)multiplicador, obj.faces[i].vert1.z / (float)multiplicador);
    glVertex3f(obj.faces[i].vert3.x / (float)multiplicador, obj.faces[i].vert3.y / (float)multiplicador, obj.faces[i].vert3.z / (float)multiplicador);
  }
  glEnd();
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  if (vistaDeFora)
      gluLookAt(0, posy + 1, posz + 3, 0, posy, posz, 0, 1, 0);
  else
    gluLookAt(0, posy, posz - 1, 0, posy, posz - 1.5, 0, 1, 0);


if (ajuda)
  escreve();


  glPushMatrix();
  glColor3f(0.0f, 0.0f, 1.0f); // Color Blue
  glTranslatef(0, -50, 0);
  glutSolidCube(100);
  glPopMatrix();

  //--------------------------------------------------------
  // navio
  glPushMatrix();
  glColor3f((float)139 / 255, (float)69 / 255, (float)19 / 255);

  glRotatef(rotacao, 0, 1, 0);
  glTranslatef(2, 0, -3);

  desenhaObjeto(navio, 1);
  glPopMatrix();

  //--------------------------------------------------------
  // submarino
  glPushMatrix();

  glTranslatef(0, posy, posz);
  glRotatef(-90, 1, 0, 0);

  glColor3f(1, 1, 0);
  desenhaObjeto(submarino, 100);
  glPopMatrix();

  //--------------------------------------------------------

  // peixes
  // glPushMatrix();
  // glColor3f(1, 0, 0);
  // for (int i = 0; i < 100; i++)
  // {
  //   int x_peixe = rand() % 200 - 100;
  //   int y_peixe = rand() % 200 - 200;
  //   int z_peixe = rand() % 200 - 100;
  //   glTranslatef(x_peixe,y_peixe,z_peixe);
  //   desenhaObjeto(peixe, 1);
  // }
  // glPopMatrix();

  //--------------------------------------------------------

  // leão marinho
  glPushMatrix();
   glColor3f(1, 0, 1);
   glRotatef(rotacao, 0, 1, 0);
   glTranslatef(5, -5, 5);

   desenhaObjeto(leao, 400);
   glPopMatrix();


//cavalo marinho
 glPushMatrix();
   glColor3f(1, 0, 1);
   glRotatef(rotacao, 0, 1, 0);
   glTranslatef(2, -2, 2);

   desenhaObjeto(cavalo, 200);
   glPopMatrix();




  glutSwapBuffers();
}



// Terminando coisas do opengl

int main(int argc, char **argv)
{



  // imprimeObjeto(obj);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1200,1080);
  glutInitWindowPosition(50, 50);
  glutCreateWindow(title);
  glutDisplayFunc(display);
  glEnable(GL_DEPTH_TEST);
  glutKeyboardFunc(keyboardNormal);
  glutSpecialFunc(keyboardEspecial);
  glutKeyboardUpFunc(keyboardNormal_soltar);
  glutSpecialUpFunc(keyboardEspecial_soltar);
  initGL();

  glutTimerFunc(100, rodarSubmarino, 0);
  glutMainLoop();

  return 0;
}