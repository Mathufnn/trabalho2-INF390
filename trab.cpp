#include <bits/stdc++.h>
using namespace std;

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

int main()
{
  ifstream objeto;
  objeto.open("submarine_triangulated.obj");
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

      v.x = atof(tokens[1].c_str());
      v.y = atof(tokens[2].c_str());
      v.z = atof(tokens[3].c_str());
      v.id = contadorVertices;
      contadorVertices++;

      obj.vertices.push_back(v);
    }
    else if (tokens[0] == "f")
    {
      face f;

      float v1 = atoi(tokens[1].c_str());
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

  // for(int i = 0; i < 10; i++) // imprime os vertices
  // {
  //   cout << obj.vertices[i].id << " " << obj.vertices[i].x << " " << obj.vertices[i].y 
  //   << " " << obj.vertices[i].x << endl;
  // }

  // for(int i = 0; i < 10; i++) // imprime as faces
  // {
  //   cout << obj.faces[i].vert1.id << " " << obj.faces[i].vert1.x << " " 
  //   << obj.faces[i].vert1.y << " " << obj.faces[i].vert1.z << "  //  "
  //   << obj.faces[i].vert2.id << " " << obj.faces[i].vert2.x << " " 
  //   << obj.faces[i].vert2.y << " " << obj.faces[i].vert2.z << "  //  "
  //   << obj.faces[i].vert3.id << " " << obj.faces[i].vert3.x << " " 
  //   << obj.faces[i].vert3.y << " " << obj.faces[i].vert3.z << endl;
  // }
}