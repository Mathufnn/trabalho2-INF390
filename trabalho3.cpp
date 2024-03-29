#include <bits/stdc++.h>
#include <GL/glut.h> // compilar com -lGL -lGLU -lglut
using namespace std;
#define PI 3.14159265

// -------------------------------    PARSER DE BITMAP (extraído do material da INF 390)  ---------------------------------------------------------

GLuint texture[3];

struct Image
{
  unsigned long sizeX;
  unsigned long sizeY;
  char *data;
};

typedef struct Image Image;

#define checkImageWidth 64
#define checkImageHeight 64

float xrot;
float yrot;
float zrot;
float ratio;

int ImageLoad(char *filename, Image *image)
{
  FILE *file;
  unsigned size;
  unsigned long i;
  unsigned short int planes;
  unsigned short int bpp;

  char temp;

  if ((file = fopen(filename, "rb")) == NULL)
  {
    printf("File Not Found : %s\n", filename);
    return 0;
  }

  fseek(file, 18, SEEK_CUR);

  if ((i = fread(&image->sizeX, 4, 1, file)) != 1)
  {
    printf("Error reading width from %s.\n", filename);
    return 0;
  }

  if ((i = fread(&image->sizeY, 4, 1, file)) != 1)
  {
    printf("Error reading height from %s.\n", filename);
    return 0;
  }

  size = image->sizeX * image->sizeY * 3;
  if ((fread(&planes, 2, 1, file)) != 1)
  {
    printf("Error reading planes from %s.\n", filename);
    return 0;
  }

  if (planes != 1)
  {
    printf("Planes from %s is not 1: %u\n", filename, planes);
    return 0;
  }

  if ((i = fread(&bpp, 2, 1, file)) != 1)
  {
    printf("Error reading bpp from %s.\n", filename);
    return 0;
  }

  if (bpp != 24)
  {
    printf("Bpp from %s is not 24: %u\n", filename, bpp);
    return 0;
  }

  fseek(file, 24, SEEK_CUR);

  image->data = (char *)malloc(size);
  if (image->data == NULL)
  {
    printf("Error allocating memory for color-corrected image data");
    return 0;
  }
  if ((i = fread(image->data, size, 1, file)) != 1)
  {
    printf("Error reading image data from %s.\n", filename);
    return 0;
  }
  for (i = 0; i < size; i += 3)
  {
    temp = image->data[i];
    image->data[i] = image->data[i + 2];
    image->data[i + 2] = temp;
  }
  return 1;
}

Image *loadTexture(char *file_name)
{
  Image *image_aux;
  image_aux = (Image *)malloc(sizeof(Image));
  if (image_aux == NULL)
  {
    printf("Error allocating space for image");
    exit(0);
  }

  if (!ImageLoad(file_name, image_aux))
  {
    exit(1);
  }

  return image_aux;
}

// -------------------------------   FIM DO PARSER DE BITMAP (extraído do material da INF 390)  ---------------------------------------------------------

//  -------------------------------   PARSER DOS OBJETOS   ----------------------------------------------------------------------------------------------

struct vertice // struct responsavel por armazenar pontos no espaco 3d
{              //
  double x, y, z;
};

struct textura
{
  double tx, ty;
};

struct normal
{
  double nx, ny, nz;
};

struct face // struct responsavel por armazenar um conjunto de 3 pontos que formam um triangulo
{
  vertice vert1, vert2, vert3;
  textura text1, text2, text3;
  normal norm1, norm2, norm3;
};

struct objeto3d // struct que armazena conjuntos de vertices e faces, que podem representar um objeto 3d
{
  vector<vertice> vertices;
  vector<textura> texturas;
  vector<normal> normais;

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

objeto3d leObjeto(string arquivo) // parser de arquivos .obj, separa cada linha do texto em
{                                 // varios tokens (palavras), usando espaco e '/' como
  ifstream objeto;                // delimitadores, e atribui os valores lidos a objetos do tipo
  objeto.open(arquivo);           // objeto 3d
  string linha;
  objeto3d obj;
  int count = 0;

  while (getline(objeto, linha))
  {
    vector<string> tokens;
    tokens = quebraString(linha);

    if (tokens[0] == "v") // indica que o primeiro token da linha e um 'v', ou seja, e um vertice
    {
      vertice v;

      v.x = atof(tokens[1].c_str()); // transforma string em float
      v.y = atof(tokens[2].c_str());
      v.z = atof(tokens[3].c_str());

      obj.vertices.push_back(v);
    }
    else if (tokens[0] == "vt")
    {
      textura t;

      t.tx = atof(tokens[1].c_str());
      t.ty = atof(tokens[2].c_str());

      obj.texturas.push_back(t);
    }
    else if (tokens[0] == "vn")
    {
      normal n;

      n.nx = atof(tokens[1].c_str());
      n.ny = atof(tokens[2].c_str());
      n.nz = atof(tokens[3].c_str());

      obj.normais.push_back(n);
    }
    else if (tokens[0] == "f") // indica que o primeiro token e um 'f', ou seja, e uma face
    {
      face f;

      int v1 = atoi(tokens[1].c_str()) - 1; // transforma string em int
      int v2 = atoi(tokens[4].c_str()) - 1;
      int v3 = atoi(tokens[7].c_str()) - 1;

      int t1 = atoi(tokens[2].c_str()) - 1;
      int t2 = atoi(tokens[5].c_str()) - 1;
      int t3 = atoi(tokens[8].c_str()) - 1;

      int n1 = atoi(tokens[3].c_str()) - 1;
      int n2 = atoi(tokens[6].c_str()) - 1;
      int n3 = atoi(tokens[9].c_str()) - 1;

      f.vert1 = obj.vertices[v1];
      f.vert2 = obj.vertices[v2];
      f.vert3 = obj.vertices[v3];

      f.text1 = obj.texturas[t1];
      f.text2 = obj.texturas[t2];
      f.text3 = obj.texturas[t3];

      f.norm1 = obj.normais[n1];
      f.norm2 = obj.normais[n2];
      f.norm3 = obj.normais[n3];

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

char title[] = "Psychedelic submarine";
GLint rotacao = 0;                                         // armazena rotacao atual do submarino
bool rodarSentidoHorario = 0, rodarSentidoAntiHorario = 0; // armazenam estado do botao de virar a visao
bool andarPraFrente = 0, andarPraTras = 0;                 // armazena estado do botao de frente/re
bool subir = 0, descer = 0;                                // armazena estado do botao de mover verticalmente
bool vistaDeFora = 1;                                      // ponto de vista de dentro ou fora do submarino
bool ajuda = 0;                                            // ligar/desligar menu de ajuda
bool iluminacao = 0;                                       // ligar/desligar iluminacao
bool shading = 0;                                          // ligar/desligar shading
bool trab3 = true;                                         // ligar/desligar informações sobre o trabalho 3

objeto3d submarino = leObjeto("Objects/submarine.obj"); // armazenam objetos lidos e retornados pelo parser
objeto3d oceano = leObjeto("Objects/cube.obj");
objeto3d navio = leObjeto("Objects/navio.obj");
objeto3d peixe = leObjeto("Objects/fish.obj");
// objeto3d cavalo = leObjeto("Objects/cavalo.obj");
// objeto3d leao = leObjeto("Objects/leao_marinho.obj");
// objeto3d peixe_espada = leObjeto("Objects/peixe_espada.obj");
// objeto3d tubarao_martelo = leObjeto("Objects/tubarao_martelo.obj");
int x_peixe[100]; // variaveis para guardar posicao aleatoria dos peixes
int y_peixe[100];
int z_peixe[100];
int x_navio[10]; // posicoes do navio
int z_navio[10];
int rot_navio[10];
GLfloat posx = 0.0; // posição que será usada para referência para câmera
GLfloat posy = 0.0;
GLfloat posz = 0.0;
GLdouble posicaoAtualSubmarinoX = 0.0;
GLdouble posicaoAtualSubmarinoZ = 0.0;
bool luz_1 = false;
bool luz_2 = false;

GLfloat lightpos0[] = {0., 0., 0, 1};
GLfloat lightpos1[] = {0, 0, 0, 1};
GLfloat lightpos2[] = {0, posy + 3, posz + 3, 1};

void escreve() // funcao responsavel por escrever texto na tela
{
  vector<string> texto; // vetor que armazena strings que aparecerao na tela
  texto.push_back("Comandos do simulador");
  texto.push_back("Up (tecla direcional)  -  Mover (verticalmente para cima)");
  texto.push_back("Down (tecla direcional)  -  Mover (verticalmente) para baixo");
  texto.push_back("Down (tecla direcional)  -  Mover (verticalmente) para baixo");
  texto.push_back("Left (tecla direcional)  -  Virar (aproximadamente) 5 graus para a direita");
  texto.push_back("Right (tecla direcional) -  Virar (aproximadamente) 5 graus para a esquerda");
  texto.push_back("S ou s  -  Ré");
  texto.push_back("F ou f  -   Ponto de vista de fora do submarino");
  texto.push_back("I ou i  -   Ponto de vista de dentro do submarino");
  texto.push_back("G ou g -  Ativa o modelo flat ou Gouraud");
  texto.push_back("L ou l - Ativa ou desativa a iluminação");
  texto.push_back("1 - Ativa o modo iluminação multidirecional (apenas com modo iluminação ligado)");
  texto.push_back("2 - Ativa o modo luz direcional (apenas com modo iluminação ligado)");
  texto.push_back("H ou h   -   Apresentar/Ocultar um menu de ajuda (descrevendo os comandos do simulador)");
    texto.push_back("X ou x   -   Apresentar/Ocultar um menu com dados do trabalho 3");


  float conty = 0;
  for (int j = 0; j < texto.size(); j++) // realiza as transformacoes necessarias para exibir o texto
  {
    glPushMatrix();
    glColor3f(1, 0, 0);
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

void escrevet3(int msg) // funcao responsavel por escrever texto na tela dados sobre o trabalho 3
{
  vector<string> texto; // vetor que armazena strings que aparecerao na tela
  float conty = 0;


  if (msg == 1)
  {  
    texto.push_back("Pressione X ou x para desativar informacoes sobre o trabalho 3");
    texto.push_back("Modo iluminacao ativado");

    conty += 0.15;
  }
  if (msg == 2){
    texto.push_back("Pressione X ou x para desativar informacoes sobre o trabalho 3");

    texto.push_back("Modo iluminacao desativado");
    conty += 0.15;
  }

  if (msg == 3)
  {
    texto.push_back("Modo iluminacao multidirecional ativado");
    conty += 0.25;
  }
  if (msg == 4)
  {
    texto.push_back("Modo iluminacao multidirecional desativado");
    conty += 0.25;
  }

  if (msg == 5)
  {
    texto.push_back("Modo iluminacao unidirecional ativado");
    conty += 0.3;
  }

  if (msg == 6)
  {
    texto.push_back("Modo iluminacao unidirecional desativado");
    conty += 0.3;
  }

  if (msg == 7)
  {
    texto.push_back("Modo flat ativado");
    conty += 0.35;
  }

  if (msg == 8)
  {
    texto.push_back("Modo Gouraud ativado");
    conty += 0.35;
  }

  for (int j = 0; j < texto.size(); j++) // realiza as transformacoes necessarias para exibir o texto
  {
    glPushMatrix();
    glColor3f(1, 0, 0);
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

void iniciaPosicoesAleatorias() // inicia posicoes aleatorias de objetos no mapa
{
  for (int i = 0; i < 100; i++)
  {
    x_peixe[i] = rand() % 100 - 50;
    y_peixe[i] = rand() % 100 - 100;
    z_peixe[i] = rand() % 100 - 50;
  }
  for (int i = 0; i < 10; i++)
  {
    x_navio[i] = rand() % 80 - 40;
    z_navio[i] = rand() % 80 - 40;
    rot_navio[i] = rand() % 360;
  }
}

void initGL()
{
  Image *image1 = loadTexture("Objects/Maps/psychedelic.bmp");
  Image *image2 = loadTexture("Objects/Maps/nemo.bmp");

  glGenTextures(3, texture); // define o numero de texturas

  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
  glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,
               GL_RGB, GL_UNSIGNED_BYTE, image1->data);
  glTexEnvf(GL_TEXTURE_ENV, GL_BLEND, GL_DECAL);

  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
  glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0,
               GL_RGB, GL_UNSIGNED_BYTE, image2->data);
  glTexEnvf(GL_TEXTURE_ENV, GL_BLEND, GL_DECAL);

  glClearColor(0, 1, 1, 0.3);  // cor para limpeza do buffer, céu
  glMatrixMode(GL_PROJECTION); //
  glLoadIdentity();            //
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND); // ativa a mistura de objetos quando estes se interceptam
  gluPerspective(45.0f, 1, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glClearDepth(1.0f);

  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);

  glEnable(GL_NORMALIZE);
  glEnable(GL_TEXTURE_2D);
}

void timerMovimentacaoSubmarino(int tempo) // funcao responsavel por pegar os sinais de teclado
{                                          // altera os valores das variaveis durante a movimentacao
  if (rodarSentidoHorario)                 // do submarino. essa funcao tem uma callback com timer de
  {                                        // 10 ms, que fica sujeito a performance da maquina
    rotacao += 1;
    rotacao %= 360;
  }
  else if (rodarSentidoAntiHorario)
  {
    rotacao -= 1;
    if (rotacao < 0)
      rotacao = 360;
  }
  if (andarPraFrente)
  {
    posz -= 0.1;
  }
  else if (andarPraTras)
  {
    posz += 0.1;
  }
  if (subir)
  {
    if (posy < 0)
      posy += 0.1;
  }
  if (descer)
  {
    posy -= 0.1;
  }

  glutPostRedisplay();
  glutTimerFunc(10, timerMovimentacaoSubmarino, 0);
}

void keyboardNormal(unsigned char key, int x, int y) // trata eventos do teclado normal, ou seja,
{                                                    // as teclas que nao sao setas
  if (key == 'W' || key == 'w')                      // note que so trata o evento de pressionar,
  {                                                  // e nao de soltar uma tecla
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

  else if (key == 'X' || key == 'x')
  {
    trab3 = !trab3;
    glutPostRedisplay();
  }

  else if (key == 'L' || key == 'l' || key == '1' || key == '2')
  {

    if (key == 'L' || key == 'l')
    {
      iluminacao = !iluminacao;
    }

    if (key == '1')
    {
      luz_1 = !luz_1;
    }

    if (key == '2')
    {
      luz_2 = !luz_2;
    }

    if (iluminacao)
    {
      glEnable(GL_LIGHTING);
      escrevet3(1);

      if (luz_1)
      {
        glEnable(GL_LIGHT0);
        escrevet3(3);
      }

      if (luz_2)
      {
        glEnable(GL_LIGHT3);
        escrevet3(5);
      }

      if (!luz_1)
      {
        glDisable(GL_LIGHT0);
        escrevet3(4);
      }

      if (!luz_2)
      {
        glDisable(GL_LIGHT3);
        escrevet3(6);
      }
    }
    else
    {
      escrevet3(2);
      glDisable(GL_LIGHTING);
    }

    glutPostRedisplay();
  }

  else if (key == 'G' || key == 'g')
  {
    shading = !shading;
    if (shading)
    {
      glShadeModel(GL_FLAT);
      escrevet3(7);
    }
    else
    {
      glShadeModel(GL_SMOOTH);
      escrevet3(8);
    }
    glutPostRedisplay();
  }
}

void keyboardEspecial(int key, int x, int y) // trata eventos de pressionar teclas de seta
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
}

void keyboardNormal_soltar(unsigned char key, int x, int y) // trata eventos de soltar uma tecla
{                                                           // para teclas normais
  if (key == 'W' || key == 'w')
  {
    andarPraFrente = false;
  }
  else if (key == 'S' || key == 's')
  {
    andarPraTras = false;
  }
}

void keyboardEspecial_soltar(int key, int x, int y) // trata eventos de soltar uma tecla de seta
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

void desenhaObjeto(objeto3d &obj, GLdouble multiplicador) // funcao responsavel por desenhar os triangulos
{                                                         // do objeto na tela, usando o vetor de vertices
  multiplicador = 1 / multiplicador;                      // e arestas no objeto "objeto3d"
  glScalef(multiplicador, multiplicador, multiplicador);
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < obj.faces.size(); i++)
  {
    glNormal3f(obj.faces[i].norm1.nx, obj.faces[i].norm1.ny, obj.faces[i].norm1.nz);
    glTexCoord2f(obj.faces[i].text1.tx, obj.faces[i].text1.ty);
    glVertex3f(obj.faces[i].vert1.x, obj.faces[i].vert1.y, obj.faces[i].vert1.z);

    glNormal3f(obj.faces[i].norm2.nx, obj.faces[i].norm2.ny, obj.faces[i].norm2.nz);
    glTexCoord2f(obj.faces[i].text2.tx, obj.faces[i].text2.ty);
    glVertex3f(obj.faces[i].vert2.x, obj.faces[i].vert2.y, obj.faces[i].vert2.z);

    glNormal3f(obj.faces[i].norm3.nx, obj.faces[i].norm3.ny, obj.faces[i].norm3.nz);
    glTexCoord2f(obj.faces[i].text3.tx, obj.faces[i].text3.ty);
    glVertex3f(obj.faces[i].vert3.x, obj.faces[i].vert3.y, obj.faces[i].vert3.z);
  }
  glEnd();
}

void display() // responsavel por exibir os elementos do jogo na tela
{              // e organizar os objetos na tela
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  GLfloat lightpos0[] = {0.1, 0.1, 0.1, 0};            // Informações da luz ambiente (multidirecional)
  GLfloat lightpos1[] = {1, 1, 1, 1};                  // Informações da luz unidirecional
  GLfloat lightpos2[] = {posx, posy + 1, posz + 1, 1}; // Posição da fonte de luz unidirecional
  GLfloat lightpos3[] = {posx, posy, posz};            // Posição a qual a luz unidirecional está voltada

  glLightfv(GL_LIGHT0, GL_AMBIENT, lightpos0); // Luz multidirecional

  // a camera do jogo segue o submarino nos seus movimentos de subir/descer e ir em frente/re,
  // ou seja, o acompanha no eixo y e eixo z

  if (vistaDeFora)                                              // enxerga o submarino por cima
    gluLookAt(0, posy + 1, posz + 3, 0, posy, posz, 0, 1, 0);   // e um pouco por trás
  else                                                          // ponto de vista da frente do submarino
    gluLookAt(0, posy, posz - 1, 0, posy, posz - 1.5, 0, 1, 0); //

  if (ajuda)   // exibe menu de ajuda
    escreve(); //

  else
  { // tratamento de display para informacoes sobre o trabalho 3

    if (trab3)
    {
      if (iluminacao)
      {
        escrevet3(1);

        if (luz_1)
        {
          escrevet3(3);
        }

        if (luz_2)
        {
          escrevet3(5);
        }

        if (!luz_1)
        {
          escrevet3(4);
        }

        if (!luz_2)
        {
          escrevet3(6);
        }
      }
      else
      {
        escrevet3(2);
      }

      if (shading)
      {
        escrevet3(7);
      }
      else
      {
        escrevet3(8);
      }
    }
  }

  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glPushMatrix();              // exibe o submarino, transladado no eixo y
  glColor4f(1, 1, 1, 1);       // e no eixo z para representar seu deslocamento
  glTranslatef(0, posy, posz); //
  glRotatef(-90, 1, 0, 0);
  desenhaObjeto(submarino, 100);
  glPopMatrix();

  glPushMatrix();                   // oceano, representado por um cubo com centro em -50
  glColor4f(0.0f, 1.0f, 1.0f, 0.3); //
  glTranslatef(0, -50, 0);
  glutSolidCube(100);
  glPopMatrix();

  glBindTexture(GL_TEXTURE_2D, texture[1]);
  for (int i = 0; i < 100; i++) // exibe os peixes que aparecem em regioes aleatorias
  {                             // no fundo do oceano
    glPushMatrix();
    glColor3f(1, 0.5, 0);
    glRotatef(rotacao, 0, 1, 0);
    glTranslatef(x_peixe[i], y_peixe[i], z_peixe[i]);
    desenhaObjeto(peixe, 1);
    glPopMatrix();
  }

  glColor4f((float)139 / 255, (float)69 / 255, (float)19 / 255, 1);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glPushMatrix(); // exibe navios em posicoes aleatorias na superficie do oceano
  glRotatef(rotacao, 0, 1, 0);
  glTranslatef(2, 0, -3);
  desenhaObjeto(navio, 0.3);
  glPopMatrix();

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

  // Inserção da luz unidimensional

  glPushMatrix();
  glLightfv(GL_LIGHT3, GL_DIFFUSE, lightpos1);
  glLightfv(GL_LIGHT3, GL_POSITION, lightpos2);
  glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lightpos3);
  GLfloat atenuacao[] = {0.1};
  glLightfv(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, atenuacao);
  glPopMatrix();

  glutSwapBuffers(); // troca o double buffer para exibir a imagem mais rapidamente na tela
}

void reshape(GLsizei width, GLsizei height)
{
  if (height == 0)
    height = 1;
  GLfloat aspect = (GLfloat)width / (GLfloat)height;

  glViewport(0, 0, width, height);

  gluPerspective(45.0f, aspect, 0.2f, 100.0f);
}

int main(int argc, char **argv)
{
  iniciaPosicoesAleatorias();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1200.0, 1080.0);
  glutInitWindowPosition(500, 500);
  glutCreateWindow(title);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
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