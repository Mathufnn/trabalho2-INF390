#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <string>
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
    
    cout << linha << endl;
  }
}