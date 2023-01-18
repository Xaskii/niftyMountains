#include <bits/stdc++.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <time.h>

#include "draw.h"

#define GREEN al_map_rgb(0, 255, 0)
#define RED al_map_rgb(255, 0, 0)

using namespace std;

ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_DISPLAY *display;
ALLEGRO_TIMER *timer;

// Distance, coordinates
typedef pair<int, pair<int, int> > intP;

// Initializes all allegro functions and variables
int allegroInit(int x, int y, int FPS) {
    srand(time(0));
    ALLEGRO_COLOR background = al_map_rgb(255, 255, 255);
    al_init();

    display = al_create_display(x, y);
    if (!display) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_set_window_title(display, "My Display");

    if (!al_init_primitives_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize primatives addon!",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    event_queue = al_create_event_queue();
	if (!event_queue) {
		al_show_native_message_box(display, "Error", "Error", "Failed to create event_queue!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
      	return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));

    al_clear_to_color(background);
    al_flip_display();
    return 0;
}

void drawMap(vector<vector<int> >& matrix, int mini, int maxi) {
    int n = 0;
    int c;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {
            n = matrix[i][j];
            c = (float(n - mini) / (maxi - mini) * 255);
            drawPixel(j, i, al_map_rgb(c, c, c));
        }
    }
    al_flip_display();
}

void drawPixel(int x, int y, ALLEGRO_COLOR c) {
    al_draw_filled_rectangle(x * 2, y * 2, (x * 2) + 2, (y * 2) + 2, c);
}

int drawLowestElevPath(vector<vector<int> >& m, int s, ALLEGRO_COLOR color) {
    int n = 0;
    int r = s;
    int fwd, up, down;
    for (int i = 0; i < COL; i++) {
        fwd = abs(m[r][i + 1] - m[r][i]);
        // Calculate elevation changes for each choice and prevent
        // access violation
        if (r == 0) {
            up = INT_MAX;
            down = abs(m[r + 1][i + 1] - m[r][i]);
        } else if (r == ROWS - 1) {
            up = abs(m[r - 1][i + 1] - m[r][i]);
            down = INT_MAX;
        } else {
            up = abs(m[r - 1][i + 1] - m[r][i]);
            down = abs(m[r + 1][i + 1] - m[r][i]);
        }
        // cout << "up: " << up << " fwd: " << fwd << " down: " << down << endl;
        // Decides how to change the row
        if (up < fwd && up < down) {
            n += up;
            r--;
            // cout << "went up" << endl;
        } else if (down < fwd && down < up) {
            n += down;
            r++;
            // cout << "went down" << endl;
        } else if (up == down && up < fwd) {
            if (rand() % 2 == 1) {
                n += down;
                r++;
            } else {
                n += up;
                r--;
            }
        } else {
            // cout << "went forward" << endl;
            n += fwd;
        }
        drawPixel(i, r, color);
    }
    return n;
}



int indexOfLowestElevPath(vector<vector<int> >& m) {
    int low = drawLowestElevPath(m, 0, RED);
    int index = 0;
    int temp;
    for (int i = 1; i < ROWS; i++) {
        temp = drawLowestElevPath(m, i, RED);
        if (temp < low) {
            low = temp;
            index = i;
        }
    }
    drawLowestElevPath(m, index, GREEN);
    cout << "Lowest elevation change is " << low << " from index " << index << endl;
    cout << endl;
    return index;
}

int keepLowestElevPath(vector<vector<int> > &m){
    return 0;
}

int showDijkstra(vector<vector<int> > &m, int s){
    vector<vector<bool> > V(ROWS);
    for (int i = 0; i < ROWS; i++){
        V[i].assign(COL, false);
    }

    vector<vector<int> > dist(ROWS);
    for (int i = 0; i < ROWS; i++){
        dist[i].assign(COL, INT_MAX);
    }
    cout << "distance to 0, 0 at start is: " << dist[0][0] << endl;

    priority_queue< intP, vector <intP> , greater<intP> > pq;

    pq.push(make_pair(0, make_pair(s, 0)));

    dist[s][0] = 0;

    int i = 0;

    while (!pq.empty()){
        pair<int, int> u = pq.top().second;
        pq.pop();

        int r = u.first;
        int c = u.second;

        int fwd, up, down;
        fwd = abs(m[r][c + 1] - m[r][c]);
        // Calculate elevation changes for each choice and prevent
        // access violation
        if (r == 0) {
            up = INT_MAX;
            down = abs(m[r + 1][c + 1] - m[r][c]);
        } else if (r == ROWS - 1) {
            up = abs(m[r - 1][c + 1] - m[r][c]);
            down = INT_MAX;
        } else {
            up = abs(m[r - 1][c + 1] - m[r][c]);
            down = abs(m[r + 1][c + 1] - m[r][c]);
        }
        // up
        if (u.first != 0){
            pair<int, int> v = u;
            v.first -= 1;
            v.second += 1;
            int weight = up;

            if (dist[v.first][v.second] > dist[u.first][u.second] + weight){
                dist[v.first][v.second] = dist[u.first][u.second] + weight;
                pq.push(make_pair(dist[v.first][v.second], make_pair(v.first, v.second)));
            }
        }

        if (true) {
            pair<int, int> v = u;
            v.second += 1;
            int weight = fwd;

            if (dist[v.first][v.second] > dist[u.first][u.second] + weight){
                dist[v.first][v.second] = dist[u.first][u.second] + weight;
                pq.push(make_pair(dist[v.first][v.second], make_pair(v.first, v.second)));
            }
        }

        // down
        if (u.first != ROWS - 1){
            pair<int, int> v = u;
            v.first += 1;
            v.second += 1;
            int weight = down;

            if (dist[v.first][v.second] > dist[u.first][u.second] + weight){
                dist[v.first][v.second] = dist[u.first][u.second] + weight;
                pq.push(make_pair(dist[v.first][v.second], make_pair(v.first, v.second)));
            }
        }
    }

    cout << s << " to other edge of graph is " << dist[s][COL - 1] << endl;
    return 0;
}


bool closeDisplay(){
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);
    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        return true;
    }

    return false;
}

int userChoice(){
    int x = 0;
    cout << "Choose an option: " << endl;
    cout << "[0]: Close program." << endl;
    cout << "[1]: Show greedy algorithm paths." << endl;
    cout << "[2]: Show lowest elevation paths" << endl;
    cout << "[3]: Show Dijkstra paths" << endl;
    cout << "> ";
    scanf("%d", &x);
    return x;
}

int welcomeText(){
    cout << "Welcome to \"Nifty Mountains\"  by Xavi Simpson" << endl;
    cout << "--------------------------------------------------------------" << endl;
    cout << "You can show paths from one end of the map to another using" << endl;
    cout << "the assortment of algorithms shown below. " << endl;
    cout << endl;
    cout << endl;
    return 0;
}
