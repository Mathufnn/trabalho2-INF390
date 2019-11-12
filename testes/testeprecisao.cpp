desenhaObjeto(submarino, 100, -90, 0, 0, 0, posy, posz);
desenhaObjeto(navio, 1, 0, -180, -180, 0, -10, 10);

// glTranslatef(0, posy, posz);
// glRotatef(-90, 1, 0, 0);
// glRotatef(rotacao, 0, 0, 1);

// submarino
// glColor3f(1, 0, 0);
// glBegin(GL_TRIANGLES);
// for (int i = 0; i < submarino.faces.size(); i++)
// {
//   // cor = (cor + 1) % 3;
//   // if (cor == 0)
//   //   glColor3f(1, 0, 0);
//   // else if (cor == 1)
//   //   glColor3f(0, 1, 0);
//   // else if (cor == 2)
//   //   glColor3f(0, 0, 1);
//   int multiplicador = 100;

//   // Desenha todos os triangulos carregados do arquivo de entrada
//   glVertex3f(submarino.faces[i].vert1.x / (float)multiplicador, submarino.faces[i].vert1.y / (float)multiplicador, submarino.faces[i].vert1.z / (float)multiplicador);
//   glVertex3f(submarino.faces[i].vert2.x / (float)multiplicador, submarino.faces[i].vert2.y / (float)multiplicador, submarino.faces[i].vert2.z / (float)multiplicador);
//   glVertex3f(submarino.faces[i].vert3.x / (float)multiplicador, submarino.faces[i].vert3.y / (float)multiplicador, submarino.faces[i].vert3.z / (float)multiplicador);
// }
// glEnd();
// glPopMatrix();