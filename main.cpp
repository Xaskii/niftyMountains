#include <bits/stdc++.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include "draw.h"


using namespace std;

vector<vector<int>> readFile(const char str[]){
    vector<vector<int> > matrix(ROWS);
    ifstream inFile(str);
    int n;

    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COL; j++){
            inFile >> n;
            matrix[i].push_back(n);
        }
    }
    return matrix;
}

int findMin(const vector<vector<int> >& a){
    int m = a[0][0];
    for (int i = 0; i < ROWS; i++){
        for (int j = 1; j < COL; j++){
            if (a[i][j] < m){
                m = a[i][j];
            }
        }
    }
    return m;
}

int findMax(const vector<vector<int> >& a){
    int m = a[0][0];
    for (int i = 0; i < ROWS; i++){
        for (int j = 1; j < COL; j++){
            if (a[i][j] > m){
                m = a[i][j];
            }
        }
    }
    return m;
}


int main() {
    bool escape = false;
    int choice = 0;
    vector<vector<int> > matrix = readFile("Colorado.dat");
    allegroInit(COL * 2, ROWS * 2, 60);
    welcomeText();
    drawMap(matrix, findMin(matrix), findMax(matrix));

    while (!escape) {
        choice = userChoice();
        drawMap(matrix, findMin(matrix), findMax(matrix));
        switch (choice){
            case 0:
                escape = true;
                break;
            case 1:
                indexOfLowestElevPath(matrix);
                break;
            case 2:
                keepLowestElevPath(matrix);
                break;
            case 3:
                showDijkstra(matrix, 100);
                break;
            default:
                cout << "Please choose a valid option. " << endl;
        }
        al_flip_display();
    }
    return 0;
}
