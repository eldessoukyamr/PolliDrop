
#ifndef PROJ2_APP_H_
#define PROJ2_APP_H_

#include <HAL/HAL.h>
#include <HAL/Graphics.h>
#include <HAL/Timer.h>

#define TITLE_SCREEN_WAIT   3000  //3 seconds
#define RESULT_SCREEN_WAIT  3000  // 3 seconds

#define NUM_TEST_OPTIONS    4
#define NUM_RANDOM_NUMBERS  5

enum _GameState
{
    TITLE_SCREEN, INSTRUCTIONS_SCREEN, GAME_SCREEN, RESULT_SCREEN
};
typedef enum _GameState GameState;

enum _Cursor
{
    CURSOR_0 = 0, CURSOR_1 = 1, CURSOR_2 = 2, CURSOR_3 = 3
};
typedef enum _Cursor Cursor;

/**
 * The top-level Application object, initialized in the main() entry point and
 * passed around to basically every other function. Holds state variables
 * which YOU define as part of your application's state.
 */
struct _App_proj2
{
    GameState state;  // Determines which screen is currently shown
    SWTimer timer;    // General-purpose timer for when screens must disappear

    // Booleans to determine which colors the user has picked
    bool redSelected;
    bool greenSelected;
    bool blueSelected;

    // Cursor tracker variable
    Cursor cursor;

    // List of predetermined random numbers
    int randomNumbers[NUM_RANDOM_NUMBERS];
    int randomNumberChoice;
};
typedef struct _App_proj2 App_proj2;

// Boilerplate constructor function for the Application
App_proj2 App_proj2_construct(HAL* hal_p);
void App_proj2_loop(App_proj2* app_p, HAL* hal_p);

// Handle callback functions used for each state of the App_proj2_loop()
// function. State transitions are embedded inside of each of these functions.
void App_proj2_handleTitleScreen(App_proj2* app, HAL* hal);
void App_proj2_handleInstructionsScreen(App_proj2* app, HAL* hal_p);
void App_proj2_handleGameScreen(App_proj2* app, HAL* hal_p);
void App_proj2_handleResultScreen(App_proj2* app, HAL* hal_p);

// Helper functions which clear the screen and draw the text for each state
void App_proj2_showTitleScreen(GFX* gfx_p);
void App_proj2_showInstructionsScreen(App_proj2* app_p, GFX* gfx_p);
void App_proj2_showGameScreen(App_proj2* app_p, GFX* gfx_p);
void App_proj2_showResultScreen(App_proj2* app_p, HAL* hal_p);

// Helper function which reduces the number of redraws required when moving the
// cursor.
void App_proj2_updateGameScreen(App_proj2* app_p, GFX* gfx_p);

// Used to reset the internal game variables
void App_proj2_initGameVariables(App_proj2* app_p, HAL* hal_p);

#endif /* PROJ2_APP_H_ */
