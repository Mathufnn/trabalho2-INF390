#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

//tamanho da janela em pixels
int largurajanela = 400, alturajanela = 300;

//posicao do observador (camera)
GLdouble viewer[] = {2.0, 2.0, 3.0};


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

objeto3d submarino = leObjeto("Objects/submarino_novo.obj");

void desenhaObjeto(objeto3d &obj, GLdouble multiplicador) // funcao responsavel por desenhar os triangulos
{                                                         // do objeto na tela, usando o vetor de vertices
  multiplicador = 1 / multiplicador;                      // e arestas no objeto "objeto3d"
  glScalef(multiplicador, multiplicador, multiplicador);
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < obj.faces.size(); i++)
  {
    glNormal3f(obj.faces[i].norm1.nx, obj.faces[i].norm1.ny, obj.faces[i].norm1.nz);
    // glTexCoord2f(obj.faces[i].text1.tx, obj.faces[i].text1.ty);
    glVertex3f(obj.faces[i].vert1.x, obj.faces[i].vert1.y, obj.faces[i].vert1.z);

    glNormal3f(obj.faces[i].norm2.nx, obj.faces[i].norm2.ny, obj.faces[i].norm2.nz);
    // glTexCoord2f(obj.faces[i].text2.tx, obj.faces[i].text2.ty);
    glVertex3f(obj.faces[i].vert2.x, obj.faces[i].vert2.y, obj.faces[i].vert2.z);

    glNormal3f(obj.faces[i].norm3.nx, obj.faces[i].norm3.ny, obj.faces[i].norm3.nz);
    // glTexCoord2f(obj.faces[i].text3.tx, obj.faces[i].text3.ty);
    glVertex3f(obj.faces[i].vert3.x, obj.faces[i].vert3.y, obj.faces[i].vert3.z);
  }
  glEnd();
}

void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0); //cor para limpeza do buffer
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); //proje��o perspectiva

  glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
  glLoadIdentity();
  gluLookAt(viewer[0], viewer[1], viewer[2], // define posicao do observador
            0.0, 0.0, 0.0,                   // ponto de interesse (foco)
            0.0, 1.0, 0.0);                  // vetor de "view up"

  desenhaObjeto(submarino,1);

  glFlush();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);                                    //inicializa a glut
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //tipo de buffer/cores/profundidade
  glutInitWindowSize(largurajanela, alturajanela);          //dimens�es da janela
  glutInitWindowPosition(200, 200);                         //posicao da janela
  glutCreateWindow("Visualizacao 3D - Exemplo 1");          //cria a janela
  init();
  glutDisplayFunc(display); //determina fun��o callback de desenho
  glEnable(GL_DEPTH_TEST);  //habilita remo��o de superf�cies ocultas
  glutMainLoop();
  return 0;
}