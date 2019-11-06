#include <bits/stdc++.h>
using namespace std;

struct vertice
{
  // int id;
  float x, y, z;
};

struct face
{
  int x, y, z;
};

struct objeto3d
{
  // int id;
  // int numVertices = 0;
  // int numFaces = 0;
  vector<vertice> vertices;
  vector<face> faces;
  // vertice centro;
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

      // cout << "atof: " << atof(tokens[1].c_str()) << endl;

      obj.vertices.push_back(v);
    }
    else if (tokens[0] == "f")
    {
      face f;

      f.x = atof(tokens[1].c_str());
      f.y = atof(tokens[4].c_str());
      f.z = atof(tokens[7].c_str());
    }
  }

  for(int i = 0; i < 10; i++)
  {
    cout << obj.vertices[i].x << " " << obj.vertices[i].y << " " << obj.vertices[i].x << endl;
  }
}