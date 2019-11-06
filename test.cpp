#include <bits/stdc++.h>
using namespace std;

struct vertice
{
  int id;
  double x, y, z;
};

struct face
{
  int x, y, z;
};

struct objeto
{
  int id;
  vector<vertice> vertices;
  vector<face> faces;
  vertice centro;
};

int main()
{
  ifstream objeto;
  objeto.open("submarine.obj");
  string linha;

  while(!objeto.eof())
  {
    getline(objeto, linha);
    stringstream s(linha);
    string dado;

    while(s >> dado)
    {
      cout << dado << endl;
    }
    
    cout << linha << endl;
  }
}