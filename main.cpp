#include <string>
#include <time.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>

using namespace std;

//merge two elements in a 1d vector
void vmerge(vector<int>& v, vector<int>::iterator a)
{
  *a *= 2;
  v.erase(a - 1);
} 

//basic move function which merges and removes 0s
vector<int> vmove (vector<int>& row)
{
  row.erase(std::remove_if(row.begin(), row.end(), [](int a) -> bool {return a==0;}),row.end());
  for (int i = row.size() - 1; i > 0; i--)
  {
    if (row[i] == row[i - 1]){
      vmerge(row, row.begin() + i); 
    }  
  }
  while (row.size() < 4) row.insert(row.begin(), 0);
  return row;
}

//Handler class for board
class BoardHandler
{
  private:
    int r, c;
    vector<vector<int>> contents;
    vector<vector<int>> empty_indeces;
  public:
    int gameState = 1;
    BoardHandler(int _r, int _c) : r(_r), c(_c) 
    {
      contents.reserve(4);
      for (int i = 0; i < r; i++) {
        contents[i].resize(c,0);
      }

    }
    vector<vector<int>> getemptyindeces()
    {
      empty_indeces.erase(empty_indeces.begin(), empty_indeces.end());
      for (int i = 0; i < r; i++)
      {
        for (int j = 0; j < c; j++){
          vector<int> temp {i,j};
          if (contents[i][j] == 0) empty_indeces.emplace_back(temp);
        }
      }
      return empty_indeces;
    }
    vector<int> getRow(int index)
    {
      return contents[index];
    }
    vector<int> getColumn(int index)
    {
      vector<int> output{};
      for (int i = 0; i < 4; i++) output.emplace_back(contents[i][index]);
      return output;
    }
    void setRow(int index, vector<int> data) 
    {
      contents[index] = data;
    }
    void setColumn (int index, vector<int> data)
    {
      for (int i = 0; i < 4; i++) contents[i][index] = data[i];
    } 
    bool hasMerge (int r, int c)
    {
      vector<int> neighbors; 
      if (c < 3) neighbors.emplace_back(contents[r][c+1]);
      if (r < 3) neighbors.emplace_back(contents[r+1][c]);
      if (c > 0) neighbors.emplace_back(contents[r][c-1]);
      if (r > 0) neighbors.emplace_back(contents[r-1][c]);
      sort(neighbors.begin(), neighbors.end());
      for (auto i : neighbors)
      {
        if (contents[r][c] == i || i == 0) return true;
      }
      return false;
    }
    int checkSpaceLeft () {
      for (int _r = 0; _r < r; _r++)
      {
        for (int _c = 0; _c < c; _c++)
        {
          if (hasMerge(_r,_c)) return 1;
        }
      }
      return 0;
    }
    int checkWin () {
      for (int _r = 0; _r < r; _r++)
      {
        for (int _c = 0; _c < c; _c++)
        {
          if (contents[_r][_c] == 2048) return 2;
        }
      }
      return gameState;
    }
    void resolveState () 
    {
      gameState = checkSpaceLeft();
      gameState = checkWin();
    }
    void moveBoardRight(){
      for (int i = 0; i < r; i++) 
      {
        vector<int> row (getRow(i));
        setRow(i, vmove(row));
      }
    }
    void moveBoardLeft(){
      for (int i = 0; i < r; i++) 
      {
        vector<int> row (getRow(i));
        reverse(row.begin(),row.end());
        vector<int> _row (vmove(row));
        reverse(_row.begin(), _row.end());
        setRow(i, _row);
      }
    }
    void moveBoardUp(){
      for (int i = 0; i < c; i++) 
      {
        vector<int> column (getColumn(i));
        reverse(column.begin(),column.end());
        vector<int> _column (vmove(column));
        reverse(_column.begin(), _column.end());
        setColumn(i, _column);
      }
    }
    void moveBoardDown() {
      for (int i = 0; i < c; i++) 
      {
        vector<int> column (getColumn(i));
        setColumn(i, vmove(column));
      }
    }
    void printBoard() {
      for (int i = 0; i < r; i++) 
      {
        cout << contents[i][0] << "        " << contents [i][1] << "        " << contents[i][2] << "        " << contents[i][3] << "\n\n\n\n\n";
      }
      cout << "---" << endl;
    }
    void insertRandomNumber() {
      srand(time(0));
      getemptyindeces();
      if (empty_indeces.size() == 0) return;
      vector<int> randomnums {2, 4};
      random_shuffle(empty_indeces.begin(), empty_indeces.end());
      random_shuffle(randomnums.begin(), randomnums.end());
      contents[empty_indeces[0][0]][empty_indeces[0][1]] = randomnums[0];
    }
    void HandleInput() {
      char s;
      cin >> s;
      switch (s){
        case 'w': moveBoardUp(); break;
        case 'a': moveBoardLeft(); break;
        case 's': moveBoardDown(); break;
        case 'd': moveBoardRight(); break;
      }
    }
};

//main driver function 
int main () 
{
  BoardHandler board(4,4);
  while (board.gameState == 1){
    system("clear");
    board.insertRandomNumber();
    board.printBoard();
    board.HandleInput();
    board.resolveState();
  } 
  if (board.gameState == 2) cout << "You win" << endl;
  else if (board.gameState == 0) cout << "You lose";
}
