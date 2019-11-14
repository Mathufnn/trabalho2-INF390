#include <bits/stdc++.h>
#include <GL/glut.h> // compilar com -lGL -lGLU -lglut
using namespace std;
#define PI 3.14159265

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

vector<string> quebraString(string &str)
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

//-------------------------------------------------------------------------------------------------------
//
//
// FIM DO PARSER, INICIO DO OPENGL
//
//
//-------------------------------------------------------------------------------------------------------

GLint rotacao = 0;
bool rodarSentidoHorario = 0, rodarSentidoAntiHorario = 0;
bool andarPraFrente = 0, andarPraTras = 0;
bool subir = 0, descer = 0;
bool vistaDeFora = 1;
bool ajuda = 0;
char title[] = "Yellow submarine";
objeto3d submarino = leObjeto("submarine.obj");
objeto3d cavalo = leObjeto("cavalo.obj");
objeto3d navio = leObjeto("navio.obj");
objeto3d leao = leObjeto("leao_marinho.obj");
objeto3d peixe = leObjeto("fish.obj");
int x_peixe[100]; // variaveis para guardar posicao aleatoria dos peixes
int y_peixe[100];
int z_peixe[100];
int x_navio[10];
int z_navio[10];
int rot_navio[10];
GLdouble posx = 0.0; // posição que será usada para referência para câmera
GLdouble posy = 0.0;
GLdouble posz = 0.0;
GLdouble centroSubmarinoX = 0.0; // posição que será usada como referência para objetos no mapa
GLdouble centroSubmarinoY = 0.0;
GLdouble centroSubmarinoZ = 0.0;
GLfloat verticesQuadrado[8][3] = {{-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0}, {1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}};

GLdouble posicaoAtualSubmarinoX = 0.0;
// GLdouble posicaoAtualSubmarinoY = 0.0;
GLdouble posicaoAtualSubmarinoZ = 0.0;

vector<string> texto;

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

  float conty = 0.1;
  for (int j = 0; j < texto.size(); j++)
  {
    glPushMatrix();

    glColor3f(1, 0, 0);
    //  glTranslatef(posx-1, posy+1, posz);
    glTranslatef(posx - 1, posy + 1.1 - conty, posz);

    glScalef(1 / 3500.38, 1 / 3500.38, 1 / 3500.38);
    for (int i = 0; i < texto[j].size(); i++)
    {
      glutStrokeCharacter(GLUT_STROKE_ROMAN, texto[j][i]);
    }
    glPopMatrix();
    conty += 0.05;
  }
}

void iniciaPosicoesAleatorias()
{
  for (int i = 0; i < 100; i++)
  {
    x_peixe[i] = rand() % 100 - 50;
    y_peixe[i] = rand() % 100 - 100;
    z_peixe[i] = rand() % 100 - 50;
  }
  for (int i = 0; i < 10; i++)
  {
    x_navio[i] = rand() % 100 - 50;
    z_navio[i] = rand() % 100 - 50;
    rot_navio[i] = rand() % 360;
  }
}

void initGL()
{
  glClearColor(0, 1, 1, 0.3); // cor para limpeza do buffer
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  gluPerspective(45.0f, (GLdouble)1200 / 1080, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glClearDepth(1.0f);
}

void timerMovimentacaoSubmarino(int tempo)
{
  // GLint temp = - rotacao + 90;

  if (rodarSentidoHorario)
  {
    rotacao += 1;
    rotacao %= 360;
    cout << "rotacao: " << rotacao << " graus" << endl;
  }
  else if (rodarSentidoAntiHorario)
  {
    rotacao -= 1;
    if (rotacao < 0)
      rotacao = 360;
    cout << "rotacao: " << rotacao << " graus" << endl;
  }
  else if (andarPraFrente)
  {
    // posx -= sin(rotacao) / 0.1;
    // posz -= cos(rotacao) / 0.1;
    posz -= 0.05;
    posicaoAtualSubmarinoX += sin(rotacao * PI / 180) * 0.05;
    posicaoAtualSubmarinoZ -= cos(rotacao * PI / 180) * 0.05;
    cout << fixed << setprecision(3) << " /// posx = " << posicaoAtualSubmarinoX << "  posz = " << posicaoAtualSubmarinoZ << endl;
  }
  else if (andarPraTras)
  {
    posz += 0.05;
    posicaoAtualSubmarinoX -= sin(rotacao * PI / 180) * 0.05;
    posicaoAtualSubmarinoZ += cos(rotacao * PI / 180) * 0.05;
    cout << fixed << setprecision(3) << " /// posx = " << posicaoAtualSubmarinoX << "  posz = " << posicaoAtualSubmarinoZ << endl;
  }
  else if (subir)
  {
    if (posy <= 0)
      posy += 0.05;
  }
  else if (descer)
  {
    posy -= 0.05;
  }

  glutPostRedisplay();
  glutTimerFunc(10, timerMovimentacaoSubmarino, 0);
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

void quad(int a, int b, int c, int d, int ncolor)
{
  // if (ncolor == 4)
  glColor4f(0, 0, 1, 0.9);
  // else
  //   glColor4f(1, 0, 1, 0.5);

  glBegin(GL_QUADS);
  glVertex3fv(verticesQuadrado[a]);
  glVertex3fv(verticesQuadrado[b]);
  glVertex3fv(verticesQuadrado[c]);
  glVertex3fv(verticesQuadrado[d]);
  glEnd();
}

void desenhaObjeto(objeto3d &obj, GLdouble multiplicador)
{
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < obj.faces.size(); i++)
  {
    glVertex3f(obj.faces[i].vert2.x / multiplicador, obj.faces[i].vert2.y / multiplicador, obj.faces[i].vert2.z / multiplicador);
    glVertex3f(obj.faces[i].vert1.x / multiplicador, obj.faces[i].vert1.y / multiplicador, obj.faces[i].vert1.z / multiplicador);
    glVertex3f(obj.faces[i].vert3.x / multiplicador, obj.faces[i].vert3.y / multiplicador, obj.faces[i].vert3.z / multiplicador);
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

  // submarino
  glPushMatrix();
  glColor4f(1, 1, 0, 1);
  glTranslatef(0, posy, posz);
  glRotatef(-90, 1, 0, 0);
  desenhaObjeto(submarino, 100);
  glPopMatrix();

  // oceano
  glPushMatrix();
  glColor4f(0.0f, 0.0f, 1.0f, 0.3);
  glTranslatef(0, -50, 0);
  glutSolidCube(100);
  glPopMatrix();

  //--------------------------------------------------------
  // peixes
  for (int i = 0; i < 100; i++)
  {
    glPushMatrix();
    glColor3f(1, 0, 0);
    glRotatef(rotacao, 0, 1, 0);
    glTranslatef(x_peixe[i], y_peixe[i], z_peixe[i]);
    desenhaObjeto(peixe, 1);
    glPopMatrix();
  }

  //--------------------------------------------------------

  // navios
  glPushMatrix();
  glColor4f((float)139 / 255, (float)69 / 255, (float)19 / 255, 1);
  glRotatef(rotacao, 0, 1, 0);
  glTranslatef(2, 0, -3);
  desenhaObjeto(navio, 0.3);
  glPopMatrix();

  glPushMatrix();
  for (int i = 0; i < 10; i++)
  {
    glPushMatrix();
    glColor4f((float)139 / 255, (float)69 / 255, (float)19 / 255, 1);
    glRotatef(rotacao, 0, 1, 0);
    glTranslatef(x_navio[i], 0, z_navio[i]);
    glRotatef(rot_navio[i], 0, 1, 0);
    desenhaObjeto(navio, 1);
    glPopMatrix();
  }
  glPopMatrix();

  //--------------------------------------------------------

  // leão marinho
  // glPushMatrix();
  // glColor3f(1, 0, 1);
  // glRotatef(rotacao, 0, 1, 0);
  // glTranslatef(5, -5, 5);
  // desenhaObjeto(leao, 400);
  // glPopMatrix();

  //--------------------------------------------------------
  //cavalo marinho
  // glPushMatrix();
  // glColor3f(1, 0, 1);
  // glRotatef(rotacao, 0, 1, 0);
  // glTranslatef(2, -2, 2);
  // desenhaObjeto(cavalo, 200);
  // glPopMatrix();

  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  iniciaPosicoesAleatorias();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1200.0 / 2.0, 1080.0 / 2.0);
  glutInitWindowPosition(800, 50);
  glutCreateWindow(title);
  glutDisplayFunc(display);
  glEnable(GL_DEPTH_TEST);
  glutKeyboardFunc(keyboardNormal);
  glutSpecialFunc(keyboardEspecial);
  glutKeyboardUpFunc(keyboardNormal_soltar);
  glutSpecialUpFunc(keyboardEspecial_soltar);
  initGL();

  glutTimerFunc(10, timerMovimentacaoSubmarino, 0);
  glutMainLoop();

  return 0;
}