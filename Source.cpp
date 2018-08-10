#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;
//vertex has two values x,y and can be used to store the distances in x,y directions
class vertex {
  private:
  //x-coordinates
  int x = 0;
  //y-coordinates
  int y = 0;
  //difference between any two vertices on the same axis, a.k.a side length of the square
  int s = 1;  
public:  
  void setX(int);
  void setY(int);
  int getX(void);
  int getY(void);
  int getSide(void);
};
//set x-coordinate of the vertex
void vertex::setX(int i) { 
  vertex::x += i*vertex::s;
}
//set y-coordinate of the vertex
void vertex::setY(int i) {  
  vertex::y += i*vertex::s;
}
//get x value of the vertex 
int vertex::getX(void) {  
  return x;
}
//get y value of the vertex
int vertex::getY(void) {  
  return y;
}
//get the side length
int vertex::getSide(void) {
   return s;
}
//each square in the grid
class square:public vertex {  
private:
  //(down left)
  vertex v1;
  //(down right)
  vertex v2;
  //(up right)
  vertex v3;
  //(up left)
  vertex v4;  
public:
  //when one vertex of the square is known(v1) as well as its side length then all the vertices can be defined accordingly 
  void setV1(vertex);
  void setV2(void);
  void setV3(void);
  void setV4(void);
  vertex getV1(void);
  vertex getV2(void);
  vertex getV3(void);
  vertex getV4(void);
};
//set the first vertex (down left)
void square::setV1(vertex v) {  
  v1.setX(v.getX());
  v1.setY(v.getY());
}
//set the second vertex (down right)
void square::setV2(void) {
  v2.setX(v1.getX() + getSide());
  v2.setY(v1.getY());
}
//set the third vertex (up right)
void square::setV3(void) {
  v3.setX(v1.getX() + getSide());
  v3.setY(v1.getY() + getSide());
}
//set the fourth vertex (up left)
void square::setV4(void) {
  v4.setX(v1.getX());
  v4.setY(v1.getY() + getSide());
}
//get the first vertex (down left)
vertex square::getV1() {
  return v1;
}
//get the second vertex (down right)
vertex square::getV2() {
  return v2;
}
//get the third vertex (up right)
vertex square::getV3() {
  return v3;
}
//get the fourth vertex (up left)
vertex square::getV4() {
  return v4;
}
//the path between starting and ending squares
class path {
private:
  //vector to store the distance in x and y directions respectively
  vertex distVector;
  //starting square of the  path
  square startSquare;
  //ending square of the path
  square endSquare;     
public:
  vertex getDistVector(void);
  square getStartSquare(void);
  square getEndSquare(void);
  void setDistVector(vertex);
  void setStartSquare(square);
  void setEndSquare(square);
};
//get the distance vector
vertex path::getDistVector() {
  return distVector;
}
//get the starting square
square path::getStartSquare() {
  return startSquare;
}
//get the ending square
square path::getEndSquare() {
  return endSquare;
}
//set the distance vector
void path::setDistVector(vertex v) {
  distVector.setX(v.getX());
  distVector.setY(v.getY());
}
//set the starting square
void path::setStartSquare(square Square) {
  startSquare.setV1(Square.getV1());
  startSquare.setV2();
  startSquare.setV3();
  startSquare.setV4();
}
//set the ending square
void path::setEndSquare(square Square) {
  endSquare.setV1(Square.getV1());
  endSquare.setV2();
  endSquare.setV3();
  endSquare.setV4();
}
//Defining the global variables

// number of the vertices in both x and y direction
const int n = 21;
//Array containing all defined squares
square Squares[(n - 2)*(n - 2)];  
//the starting square to be entered by the user
int nStart;                       
//the ending square to be entered by the user
int nEnd;    

//creates the squares
void createSquares() {
  //store the x-coordinates of all the vertices
  int xVertices[n];
  //store the y-coordinates of all the vertices
  int yVertices[n];
  //Array storing the lower left corner in all squares
  vertex Corners[(n - 2)*(n - 2)]; 
  //array containing all the vertices in the grid                                
  vertex Grid[n][n];
  //Loop through all vertices in the grid to set the Grid Matrix
  for (int y = 0; y < n; y++) {
    for (int x = 0; x < n; x++) {
      Grid[x][y].setX(x);
      Grid[x][y].setY(y);
      xVertices[x] = Grid[x][y].getX();
      yVertices[y] = Grid[x][y].getY();
    }
  }
  //iterator to loop through all the squares/ corners of squares
  int q = 0;
  //Loop through all the vertices to set the Squares/Corners Matricies
  for (int i = 0; i < n - 2; i++) {
    for (int j = 0; j < n - 2; j++) {
      Corners[q].setX(xVertices[j]);
      Corners[q].setY(yVertices[i]);
      Squares[q].setV1(Corners[q]);
      Squares[q].setV2();
      Squares[q].setV3();
      Squares[q].setV4();
      q++;
    }
  }
}
//draw the basic grid without the path
void displayGrid(int offset) {
  //loop and draw each square
  for (int i = 0; i < ((n - 2)*(n - 2)); i++) {
    /*start openGL drawing of the grid */
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(offset + Squares[i].getV1().getX(), offset + Squares[i].getV1().getY());
    glVertex2f(offset + Squares[i].getV2().getX(), offset + Squares[i].getV2().getY());
    glVertex2f(offset + Squares[i].getV3().getX(), offset + Squares[i].getV3().getY());
    glVertex2f(offset + Squares[i].getV4().getX(), offset + Squares[i].getV4().getY());
    glEnd();
    glFlush();
    /*end openGL drawing of the grid */
  }
}
//create the path 
path createPath() {
  //path containing the distance moved in x and y directions, starting square and ending square
  path requiredPath; 
  //vector containing the distances in x,y directions, don't be confused by the type vertex 
  vertex distVector;
  //the distance in the x-axis direction of the path
  distVector.setX(Squares[nEnd - 1].getV1().getX() - Squares[nStart - 1].getV1().getX());
  //the distance in the y-axis direction of the path
  distVector.setY(Squares[nEnd - 1].getV1().getY() - Squares[nStart - 1].getV1().getY()); 
  /*start setting all the properties for path */
  requiredPath.setDistVector(distVector);
  requiredPath.setStartSquare(Squares[nStart-1]);
  requiredPath.setEndSquare(Squares[nEnd-1]);
  /*end setting all the properties for path */
  return requiredPath;
}
//draw the path on the grid
void displayPath(int offset){
  //making an instance of the path 
  path reqPath = createPath();
  //getting the horizontal distance moved within this path
  int horDist = reqPath.getDistVector().getX(); 
  //getting the vertical distance moved within this path
  int verDist = reqPath.getDistVector().getY();
  int side = reqPath.getStartSquare().getSide();
  /*start openGL drawing of the horizontal movement*/
  glColor3f(0.5, 0.5, 0.0);
  glBegin(GL_QUADS);
  glVertex3f(offset + reqPath.getStartSquare().getV1().getX()+(horDist<0 ? horDist : 0), offset + reqPath.getStartSquare().getV1().getY(), 0);
  glVertex3f(offset + reqPath.getStartSquare().getV2().getX()+(horDist>0 ? horDist : 0), offset + reqPath.getStartSquare().getV2().getY(), 0);
  glVertex3f(offset + reqPath.getStartSquare().getV3().getX()+(horDist>0 ? horDist : 0), offset + reqPath.getStartSquare().getV3().getY(), 0);
  glVertex3f(offset + reqPath.getStartSquare().getV4().getX()+(horDist<0 ? horDist : 0), offset + reqPath.getStartSquare().getV4().getY(), 0);
  glEnd();
  glFlush();
  /*end openGL drawing of the horizontal movement*/

  /*start openGL drawing of the vertical movement*/
  glColor3f(0.5, 0.5, 0.0);
  glBegin(GL_QUADS);
  glVertex3f(offset + reqPath.getEndSquare().getV1().getX() , offset + reqPath.getEndSquare().getV1().getY()-(verDist<0 ? verDist : 0), 0);
  glVertex3f(offset + reqPath.getEndSquare().getV2().getX() , offset + reqPath.getEndSquare().getV2().getY()-(verDist<0 ? verDist : 0), 0);
  glVertex3f(offset + reqPath.getEndSquare().getV3().getX() , offset + reqPath.getEndSquare().getV3().getY()-(verDist>0 ? verDist : 0), 0);
  glVertex3f(offset + reqPath.getEndSquare().getV4().getX() , offset + reqPath.getEndSquare().getV4().getY()-(verDist>0 ? verDist : 0), 0);
  glEnd();
  glFlush();
  /*end openGL drawing of the vertical movement*/

  /*start openGl highlighting the starting Square with green*/
  glColor3f(0.0, 1.0, 0.0);
  glBegin(GL_QUADS);
  glVertex3f(offset + reqPath.getStartSquare().getV1().getX(), offset + reqPath.getStartSquare().getV1().getY(), 0);
  glVertex3f(offset + reqPath.getStartSquare().getV2().getX(), offset + reqPath.getStartSquare().getV2().getY(), 0);
  glVertex3f(offset + reqPath.getStartSquare().getV3().getX(), offset + reqPath.getStartSquare().getV3().getY(), 0);
  glVertex3f(offset + reqPath.getStartSquare().getV4().getX(), offset + reqPath.getStartSquare().getV4().getY(), 0);
  glEnd();
  glFlush();
  /*end openGl highlighting the starting Square with green*/

  /*start openGl highlighting the ending Square with red*/
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
  glVertex3f(offset + reqPath.getEndSquare().getV1().getX(), offset + reqPath.getEndSquare().getV1().getY(), 0);
  glVertex3f(offset + reqPath.getEndSquare().getV2().getX(), offset + reqPath.getEndSquare().getV2().getY(), 0);
  glVertex3f(offset + reqPath.getEndSquare().getV3().getX(), offset + reqPath.getEndSquare().getV3().getY(), 0);
  glVertex3f(offset + reqPath.getEndSquare().getV4().getX(), offset + reqPath.getEndSquare().getV4().getY(), 0);
  glEnd();
  glFlush();
  /*end openGl highlighting the ending Square with red*/
}
//displays both the grid and the path
void display(void) {
  //leave a space between the grid and the margin of the window
  int offset = 1;
  
  displayGrid(offset);
  displayPath(offset);
}
//draw using openGL
void draw(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Squares Grid");
  // black background
  glClearColor(0.0, 0.0, 0.0, 0.0);
  // setup viewing projection 
  glMatrixMode(GL_PROJECTION);                
  // start with identity matrix
  glLoadIdentity(); 
  // setup a 25x25x2 viewing world 
  glOrtho(0.0, 25.0, 0.0, 25.0, -1.0, 1.0);   
  glutDisplayFunc(display);
  glutMainLoop();
}
//take the input from the user and show some love 
void inputOfUser(void) {
  cout << "\t\t ,d88b.d88b," << endl;
  cout << "\t\t 88888888888" << endl;
  cout << "\t\t `Y8888888Y'" << endl;
  cout << "\t\t   `Y888Y'" << endl;
  cout << "\t\t     `Y' " << endl;
  cout << "enter the number of the starting square (1-"<< (n - 2)*(n - 2) <<") ";
  cin >> nStart;
  if (nStart > (n - 2)*(n - 2) || nStart < 0) {
    cout << "Please enter a number from (1-" << (n - 2)*(n - 2) << ") ";
    cin >> nStart;
  }
  cout << "enter the number of the ending square (1-" << (n - 2)*(n - 2) << ") ";
  cin >> nEnd;
  if (nEnd >(n - 2)*(n - 2) || nEnd < 0) {
    cout << "Please enter a number from (1-" << (n - 2)*(n - 2) << ") ";
    cin >> nEnd;
  }
}
int main(int argc, char* argv[]) {
  inputOfUser();
  createSquares();
  display();
  draw(argc, argv);
  return 0;
}