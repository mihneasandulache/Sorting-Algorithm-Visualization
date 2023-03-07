#include "raylib.h"
#include <vector>

#define ScreenWidth 1200
#define ScreenHeight 800
#define MinWindowWidth 500
#define MinWindowHeight 800
#define FPS 120

#define NORMAL 0
#define SORTED 1
#define SELECTED 2
/*bar states*/

bool ShouldRandomizeArray = true;
bool ShouldShowMenuScreen = true;
bool BubbleSortPressed = false;
bool ShouldShowStartOptions = false;
bool ShouldStartSorting = false;
bool addSpeed = false;
bool subSpeed = false;
bool addSize = false;
bool subSize = false;
bool NormalSize = false;
bool NormalSpeed = false;
bool ShouldStartGame = false;
bool ShouldShowEndingScreen = false;
int n = 100;
int SortingSpeed = -1000;

std::vector<std::pair<int, int>> a(n);
/*initializing the array and its size*/

void SortArray();
void DrawArray(std::vector<std::pair<int, int>> a);
void RandomizeArray(std::vector<std::pair<int, int>> &a);
Color FindColorForBars(int state);
void ShowMenuScreen();
void BubbleSortButton(float size, char *a);
void Button(float x, float y, char *a, Color color, bool &state);
void ShowStartOptions();
void Start_Button(float size, float font, char Start[]);
void ChangeSpeed(char operation, int &value);
void ChangeSize(char operation, int &value);
void Bubble_Sort(std::vector<std::pair<int, int>> &a);

void DrawArray(std::vector<std::pair<int, int>> a) {
    float BarWidth = (float) (GetScreenWidth() / n);
    /*computing the witdh of bars*/
    int i;
    for(i = 0 ; i < n; i++) {
        Color color = FindColorForBars(a[i].second);
        /*each element of the array of pairs has two elements:
        actual value and state, which means if it is sorted or not*/
        DrawRectangleV(Vector2{(float) i * BarWidth, (float) GetScreenHeight()
        - a[i].first}, Vector2{BarWidth, (float)a[i].first}, color);
        /*drawing each bar with a vector function which takes the x and y,
        the witdh and the color*/
    }
}

Color FindColorForBars(int state) {
    switch(state) {
        case SELECTED:
            return LIGHTGRAY;
            break;
        case SORTED:
            return GREEN;
            break;
        default:
            return GOLD;
            break;
    }
    /*coloring the bars by their state*/
}

void RandomizeArray(std::vector<std::pair<int, int>> &a) {
    int i;
    for(i = 0 ; i < n; i++) {
        a[i] = {GetRandomValue(40, MinWindowWidth - 100), NORMAL};
    }
    ShouldRandomizeArray = false;
}

void ShowMenuScreen() {
    float font = (2.5 * GetScreenWidth()) / 100;
    float tmp = MeasureText("Bubble Sort!", font) / 2;
    /*setting the button to a central position*/
    BubbleSortButton(tmp, "Bubble Sort!");

    DrawArray(a);
}

void BubbleSortButton(float size, char *a) {
    Color color;
    if(BubbleSortPressed) {
        color = BLUE;
    } else { 
        color = GREEN;
    }
    Button(GetScreenWidth() / 2 - size, GetScreenHeight() / 20, "Bubble Sort!",
    color, BubbleSortPressed);
}

void Button(float x, float y, char *a, Color color, bool &state) {
    float font = (2.5 * GetScreenWidth()) / 100;
    Rectangle r1 = {
        .x = x,
        .y = y,
        .width = (float)MeasureText(a, font),
        .height = (float) font,
    };
    /*creating a rectangle for the button with two coordinates and two values*/
    if(CheckCollisionPointRec(GetMousePosition(), r1)) {
        DrawText(a, x, y, font, RED);
        if(IsMouseButtonPressed(0)) {
            state = !state;
            return;
        }
    } else {
        DrawText(a, x, y, font, color);
    }
    /*checking whether the mouse is on the button or not*/
}

void ShowStartOptions() {
    float font = (2.5 * GetScreenWidth()) / 100;
    char StartText[] = "Start Sorting";
    float tmp = (27 * GetScreenWidth()) / 100;
    Start_Button(tmp, font, StartText);
    tmp = tmp + MeasureText(StartText, font) + 75;
    char RandomizeArrayText[] = "Randomize Array!";
    Button(tmp, GetScreenHeight() / 20 + font * 2, RandomizeArrayText,
    DARKGRAY, ShouldRandomizeArray);
    addSpeed = false;
    subSpeed = false;
    addSize = false;
    subSize = false;
    NormalSize = false;
    NormalSpeed = false;
    tmp = (84.2 * GetScreenWidth()) / 100;
    char TimeButtonText[] = "Speed";
    Button(tmp, GetScreenHeight() / 20 + font * 2, TimeButtonText, BLUE,
    NormalSpeed);
    if (NormalSpeed){
        ChangeSpeed('/', SortingSpeed);
        return;
    }
    tmp = tmp + MeasureText(TimeButtonText, font) + 20;
    char SpeedPlusButtonText[] = "+";
    Button(tmp, GetScreenHeight() / 20 + font * 2, SpeedPlusButtonText, ORANGE,
    addSpeed); 
    if (addSpeed){
        ChangeSpeed('+', SortingSpeed);
        return;
    }
    tmp = tmp + MeasureText(SpeedPlusButtonText, font) + 20;
    char SpeedMinusButtonText[] = "-";
    Button(tmp, GetScreenHeight() / 20 + font * 2, SpeedMinusButtonText,
    ORANGE, subSpeed);
    if (subSpeed){
        ChangeSpeed('-', SortingSpeed);
        return;
    }
    tmp = (5 * GetScreenWidth()) / 100;
    char SizeButtonText[] = "Size";
    Button(tmp, GetScreenHeight()/20 + font * 2, SizeButtonText, BLUE,
    NormalSize);
    if (NormalSize){
        ChangeSize('/', n);
        return;
    }
    tmp = tmp + MeasureText(SizeButtonText, font) + 20;
    char SizePlusButton[] = "+";
    Button(tmp, GetScreenHeight() / 20 + font * 2, SizePlusButton, ORANGE,
    addSize);
    if (addSize){
        ChangeSize('+', n);
        return;
    }
    tmp = tmp + MeasureText(SizePlusButton, font) + 20;
    char SizeMinusButton[] = "-";
    Button(tmp, GetScreenHeight() / 20 + font * 2, SizeMinusButton, ORANGE,
    subSize);
    if (subSize){
        ChangeSize('-', n);
        return;
    } 
}
/*creating the initial menu which gives 4 options:changing the size of the bars
and the sorting speed,  randomizing the array and starting the sorting of
the array*/

void Start_Button(float size, float font, char Start[]){
    Button(size, GetScreenHeight() / 20 + font * 2, Start, DARKGRAY,
    ShouldStartSorting);
}

void ChangeSpeed(char operation, int &value) {
    switch(operation) {
        case '-':
            if(value - 10 > -10000) {
                value = value - 10;
            }
            break;
        case '+':
            value = value + 10;
            break;
        default:
            value = -1000;
            break;
    }
}
/*changing speed without crossing bellow 0*/

void ChangeSize(char operation, int &value) {
    int i;
    switch (operation) {
    case '-':
        if(value > 5) {
            value = value - 5;
            for(i = 0 ; i < 5; i++) {
                a.pop_back();
                /*deleting the last element of the array and increasing the
                size of bars*/
            }
        }
        break;
    case '+':
        value = value + 5;
        for(i = 0 ; i < 5; i++) {
            a.push_back({GetRandomValue(40, MinWindowWidth - 100), NORMAL});
        }
        /*adding the last element of the array and decreasing the size of
        bars*/
        break;
    default:
    while(n > 100) {
        n--;
        a.pop_back();
    }
    while(n < 100) {
        n++;
        a.push_back({GetRandomValue(40, MinWindowWidth - 100), NORMAL});
    }
        break;
    }
    for(i = 0; i < 100; i++) {
        a[i].second = NORMAL;
    }
    /*after the first sort, we want to make sure that the color of the bars
    becomes gold*/
    DrawArray(a);
}

void Bubble_Sort(std::vector<std::pair<int, int>> &a) {
    int i, k;
    int m = n;
    bool swapped;
    do {
        swapped = false;
        for(i = 0 ; i < m - 1; i++) {
            a[i].second = SELECTED;
            /*changing the color for the selected element*/
            if(a[i] > a[i + 1]) {
                std::swap(a[i], a[i + 1]);
                swapped = true;
            }
            BeginDrawing();
            ClearBackground(PURPLE);  
            for(k = n - 1; k >= m; k--) {
                a[k].second = SORTED;
            }
            /*coloring the sorted elements*/
            DrawArray(a);
            for(k = i ; k >= 0; k--) {
                a[k].second = NORMAL;
            }  
            EndDrawing();
        }
        m--;
    } while(swapped);
    for(i = n - 1; i >= 0; i--) {
        a[i].second = SORTED;
    }
}
void SortArray() {
    Bubble_Sort(a);
    ShouldStartSorting = false;
    DrawArray(a);
    /*creating a function in order to develope the application and add more
    sorting methods where to call these functions*/
}

void ShowEndingScreen(){
    DrawArray(a);
    ShouldShowMenuScreen = true;
    ShouldShowStartOptions = false;
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(ScreenWidth, ScreenHeight, "Sorting Algorithm Visualization");
    SetWindowMinSize(MinWindowWidth, MinWindowHeight);
    /*window confguration*/

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(PURPLE);
        if(ShouldShowMenuScreen) {
            ShowMenuScreen();
        }
        ShouldShowStartOptions = false;
        if(BubbleSortPressed) {
            ShouldShowStartOptions = true;
        }
        if(ShouldShowStartOptions) {
            ShowStartOptions();
        }
        if(ShouldRandomizeArray) {
            RandomizeArray(a);
        }
        if (ShouldStartSorting){
            ShouldShowMenuScreen = false;
            ShouldShowStartOptions = false;
            SortArray();
            ShouldStartSorting = false;
            ShouldShowEndingScreen = true;
        }
        if (ShouldShowEndingScreen) {
            ShowEndingScreen();
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
