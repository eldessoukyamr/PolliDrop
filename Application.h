/*
 * Application.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 *  Supervisor: Leyla Nazhand-Ali
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_
#define Circle_min_x 0
#define Circle_max_x 100
#define Circle_min_y 0
#define Circle_max_y 100
#define TITLE_SCREEN_WAIT 2000
#define MOVE_WAIT 1000
#define squarwidth 30
#define flowerYValue 25
#include <HAL/HAL.h>

typedef enum {startscreen, Menu_Screen ,Gameover } gamestates_t;
typedef enum {playGame, howtoplay, Highscores, show_menue_tsates} menuestates_t;

struct _Flower
{
    unsigned int down;
    unsigned int DOWN;
    int X;
    int Y;
    bool active;
    bool pollenated;
};
struct _Pollen
{
    unsigned int down;
    unsigned int DOWN;
    int X;
    int Y;
    bool active;
};
typedef struct _Flower Flower;
typedef struct _Pollen Pollen;

struct _Application
{
    // Put your application members and FSM state variables here!
    // =========================================================================
    UART_Baudrate baudChoice;
    bool firstCall;
    gamestates_t gamestates;
    int energylebel;
    int movelabel;
    int experience;
    menuestates_t menuestates;
    int CircleX;
    int CircleY;
    SWTimer timer;
    int cursor;
    int UP;
    int DOWN;
    int RIGHT;
    int LEFT;
    SWTimer pollinspawner;
    SWTimer flowerspawner;
    int up;
    int down;
    int left;
    int right;
    int pollenscore;
    Flower flowers[3];
    Pollen pollen[3];
    int Heart;
    unsigned int Score;
    int Highscore[6];
    SWTimer squartimer;
    int squarvalue;
    SWTimer homescreen;
    bool image1_draw;
    bool image2_draw;
    bool image3_draw;
};

typedef struct _Application Application;

void Gamefsm(Application* app_p, HAL* hal_p);
void Menuefsm(Application* app_p, HAL* hal_p);

// Called only a single time - inside of main(), where the application is constructed
Application Application_construct();

// Called once per super-loop of the main application.
void Application_loop(Application* app, HAL* hal);

// Called whenever the UART module needs to be updated
void Application_updateCommunications(Application* app, HAL* hal);

void application_HW7(HAL*hal, char c);
// Interprets an incoming character and echoes back to terminal what kind of
// character was received (number, letter, or other)
char Application_interpretIncomingChar(char);

// Handle callback functions used for each state of the Application_loop()
// function. State transitions are embedded inside of each of these functions.

void Application_handleTitleScreen(Application* app_p, HAL* hal_p);

void Application_handleMENUScreen(Application* app_p, HAL* hal_p);

void Application_transitionshowmenue(Application* app, HAL* hal_p);

void Application_showhowtoplay(Application* app, HAL* hal_p);
void Application_handlehowtoplay(Application* app, HAL* hal_p);

void Application_showhighscores(Application* app, HAL* hal_p);
void Application_handlescores(Application* app, HAL* hal_p);

void Application_showgamescreen(Application* app, HAL* hal_p);

void Application_transitiongamescreen(Application* app, HAL* hal_p);

void Application_handlegamescreen(Application* app, HAL* hal_p);

void Application_handleinput(Application* app, HAL* hal_p);

void Application_showflower(Application* app, HAL* hal_p);

void Application_showpollin(Application* app, HAL* hal_p);
void Application_handlepollin(Application* app, HAL* hal_p);

Flower* Application_getnewflower(Application* app, HAL* hal_p);
Pollen* Application_getnewpollen(Application* app, HAL* hal_p);

void Application_allocateflower(Application* app, HAL* hal_p);
void Application_allocatepollen(Application* app, HAL* hal_p);

void Application_handleflowerpollenlogic(Application* app, HAL* hal_p);

void Application_removepollin(Application* app, HAL* hal_p);
void Application_removeflower(Application* app, HAL* hal_p);

void Applicationhandleflwowerpollinspawner(Application* app, HAL* hal_p);

void Applicationflowercollision(Application* app, HAL* hal_p);
void Applicationpollencollision(Application* app, HAL* hal_p);

void ApplicationHandleGameover(Application* app, HAL* hal_p);

void ApplicationHandleGamelogic(Application* app, HAL* hal_p);

void ApplicationHandleGameovertransition(Application* app, HAL* hal_p);

void Applicationshowstats(Application* app, HAL* hal_p);



// Helper functions which clear the screen and draw the text for each state
void Application_showTitleScreen(Application* app_p, SWTimer* timer,SWTimer*squartimer,  GFX* gfx_p);
void Application_showGameScreen(Application* app_p, GFX* gfx_p);
void Application_showmenuScreen(Application* app_p, GFX* gfx_p);




// Generic circular increment function
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);

#endif /* APPLICATION_H_ */
