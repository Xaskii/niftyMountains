#include <bits/stdc++.h>
#include <vector>

using namespace std;

static const int ROWS = 480;
static const int COL = 844;

int allegroInit(int x, int y, int FPS);
void drawPixel(int x, int y,  ALLEGRO_COLOR c);
void drawMap(vector<vector<int> >& matrix, int mini, int maxi);
int drawLowestElevPath(vector<vector<int> >& m, int s, ALLEGRO_COLOR color);
int indexOfLowestElevPath(vector<vector<int> >& m);
bool closeDisplay();
int userChoice();
int keepLowestElevPath(vector<vector<int> >& m);
int showDijkstra(vector<vector<int> >& m, int s);
int welcomeText();
