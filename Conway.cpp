#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>
#include <thread> 
#include <chrono> 

class cell;
class grid;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class cell
{
    int x_coord;
    int y_coord;
    
  public:
    bool state;
    cell(int x_coord,int y_coord, bool state) : state(state), x_coord(x_coord), y_coord(y_coord) {};
    cell(int x_coord,int y_coord) : state(0), x_coord(x_coord), y_coord(y_coord) {};

    int NumOfNeighbours(grid);
    bool life(grid);
    bool birth(grid);
    void updateState(grid);
    void flipState();
    friend int operator+(const cell &lhs, const cell &rhs);
    friend std::ostream &operator<<(std::ostream &os, cell &c);
    
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::vector<cell>> createVector(int x, int y)
{
  std::vector<cell> temp;
  std::vector<std::vector<cell>> vec;
  for (int i = 0; i < x; ++i)
  {
    for (int j = 0; j < y; ++j)
    {
      temp.push_back(cell(i,j));
    }
    vec.push_back(temp);
    temp.clear();
  }
  return vec;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class grid
{ 
  std::vector<std::vector<cell>> allCells;
  int delta_x;
  int delta_y;

  public:
    grid(int x, int y) : delta_x(x), delta_y(y), allCells(createVector(x,y)) {};
    void updateGrid();
    friend std::ostream &operator<<(std::ostream &os, grid &g);
    friend int cell::NumOfNeighbours(grid);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int operator+(const cell &lhs, const cell &rhs)
{
  return lhs.state + rhs.state;
}

std::ostream &operator<<(std::ostream &os, cell &c)
{
  if (c.state)
    os << "*";
  else
    os << " ";
  return os;
}

int cell::NumOfNeighbours(grid g)
{
  int x = this -> x_coord;
  int y = this -> y_coord;
  return (g.allCells[(x+1)%g.delta_x][(y-1+g.delta_y)%g.delta_y] + g.allCells[(x+1)%g.delta_x][(y)%g.delta_y]) + (g.allCells[(x+1)%g.delta_x][(y+1)%g.delta_y] + g.allCells[(x)%g.delta_x][(y-1+g.delta_y)%g.delta_y]) + (g.allCells[(x)%g.delta_x][(y+1)%g.delta_y] + g.allCells[(x-1+g.delta_x)%g.delta_x][(y-1+g.delta_y)%g.delta_y]) + (g.allCells[(x-1+g.delta_x)%g.delta_x][(y)%g.delta_y] + g.allCells[(x-1+g.delta_x)%g.delta_x][(y+1)%g.delta_y]); 
}


bool cell::life(grid g)
{
  return ((this -> state) && (this -> NumOfNeighbours(g) == 2 || this -> NumOfNeighbours(g) == 3));
}

bool cell::birth(grid g)
{
  return (!(this -> state) && (this -> NumOfNeighbours(g) == 3));
}


void cell::updateState(grid g)
{
  if (this -> life(g) || this -> birth(g))
    this -> state = 1;
  else
    this -> state = 0;
}


void cell::flipState()
{
  bool prevState = this -> state;
  this -> state = !prevState;
}

void grid::updateGrid()
{
  std::vector<std::vector<cell>> tempVector = (this -> allCells);
  for (int i = 0; i < delta_y; ++i)
  {
    for (int j = 0; j < delta_x; ++j)
    {
      tempVector[j][i].updateState(*this);
    }
  }
  (this -> allCells) = tempVector;
}

std::ostream &operator<<(std::ostream &os, grid &g)
{
  for (int i = 0; i < g.delta_x; ++i)
  {
    for (int j = 0; j < g.delta_y; ++j)
    {
      os << g.allCells[i][j];      
    }
    os << std::endl;
  }
  return os;
}


int main()
{
  int x, y, numCells, rounds;
  std::cout << "Enter the number of rows:" << std::endl;
  std::cin >> y;
  std::cout << "Enter the number of columns:" << std::endl;
  std::cin >> x;
  grid g = grid(x,y);
  int m = std::max(x,y);
  std::cout << "Enter the number of cells you wish to activate:" << std::endl;
  std::cin >> numCells;
  for (int i = 0; i < numCells; ++i)
  {
    std::cout << "Enter the coordinates of the cell you wish to activate:" << std::endl;
    char c1, c2, c3;
    std::cin >> c1 >> x >> c2 >> y >> c3;
    g.allCells[x][y].flipState();
  }
  std::cout << "Enter the number of rounds you wish to be executed:" << std::endl;
  std::cin >> rounds;
  for (int i = 0; i < rounds; ++i)
  {
    system("clear");
    std::this_thread::sleep_for (std::chrono::milliseconds(1000/m));
    std::cout << g;
    g.updateGrid();
  }

  return 0;
} 