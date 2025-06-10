/*
 * *
 * Starter code for Project 2. Good luck!
 *
 * We recommending copy/pasting your HAL folder from Project 1
 * into this project.
 */
/* Standard Includes */



#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* HAL and Application includes */
#include <Application.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
extern const Graphics_Image image8BPP_UNCOMP;
extern const Graphics_Image flowerimage8BPP_UNCOMP;
extern const Graphics_Image homeimage8BPP_UNCOMP;





Application Application_construct()   // setting up construct
{     Application app;

// Initialize local application state variables here!
    app.baudChoice = BAUD_9600;
    app.firstCall = true;
        app.gamestates = startscreen;
        app.menuestates = show_menue_tsates;
        app.CircleX = (25);
        app.CircleY = (25);
        app.timer = SWTimer_construct(TITLE_SCREEN_WAIT);
        app.cursor = 0;
        app.up = 3;
        app.down = 3;
        app.left = 3;
        app.right =3;
        app.pollinspawner = SWTimer_construct(2000);
        app.flowerspawner = SWTimer_construct(2000);
        app.flowers[0].pollenated =false;
        app.flowers[1].pollenated =false;
        app.flowers[2].pollenated =false;
        app.pollen[0].active =true;
        app.pollen[1].active =true;
        app.pollen[2].active = true;
        app.Heart=3;
        int i=0;
        for (i=0;i<5;i++){
            app.Highscore[i]=0;
        }
        app.squarvalue=5;
        app.squartimer = SWTimer_construct(400);
        app.image1_draw = true;
        app.image2_draw = false;
        app.image3_draw = false;



        return app;

}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED()
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }

}

/*
 * This function is in charge of displaying the main game screen. and displayes the name of the game and my name as the creator on the screen
 */
void Application_showTitleScreen(Application* app_p, SWTimer* timer,SWTimer*squartimer,  GFX* gfx_p){
    GFX_clear(gfx_p);

    SWTimer_start(timer);
    SWTimer_start(squartimer);
}

/*
 * This function handles the title screen and handles the timer behind it that is set with the square at the bottom.
 */
void Application_handleTitleScreen(Application* app_p, HAL* hal_p)
    {
    if (app_p->image1_draw ==true){
        GFX_drawImage(&hal_p->gfx, image8BPP_UNCOMP, 0, 0);
        app_p->homescreen = SWTimer_construct(100);
        SWTimer_start(&app_p->homescreen);
        app_p->image2_draw = true;
        app_p->image1_draw = false;

    }
    if ((app_p->image2_draw == true) && (SWTimer_expired(&app_p->homescreen))){
        GFX_drawImage(&hal_p->gfx, flowerimage8BPP_UNCOMP, 0, 0);
        app_p->homescreen = SWTimer_construct(100);
        SWTimer_start(&app_p->homescreen);
        app_p->image3_draw = true;
        app_p->image2_draw = false;
    }
    if ((app_p->image3_draw ==true) && (SWTimer_expired(&app_p->homescreen))){
        GFX_drawImage(&hal_p->gfx, homeimage8BPP_UNCOMP, 0, 0);
        app_p->image3_draw = false;
        app_p->image2_draw = false;

        GFX_print(&hal_p->gfx, "ECE2564 project 2 ", 0, 0);
        GFX_print(&hal_p->gfx, "---------------------", 1, 0);
        GFX_print(&hal_p->gfx, "By: Amr Eldessouky", 2, 0);


    }


    if (SWTimer_expired(&app_p->squartimer))
     {
         GFX_drawSolidRectangle(&hal_p->gfx, app_p->squarvalue ,120, squarwidth, 5);
         app_p->squarvalue = (app_p->squarvalue + squarwidth);
         SWTimer_start(&app_p->squartimer);


     }
        if (SWTimer_expired(&app_p->timer))
        {
            app_p->gamestates = Menu_Screen;
            Application_transitionshowmenue(app_p, hal_p);
            //Application_showmenuScreen(app_p, &hal_p->gfx);
        }


    }

/*
 * This function takes care of the logic behind the cursor and how it goes in wich ever direction the player guides it to.
 */
void Application_handleMENUScreen(Application* app_p, HAL* hal_p)
{
    if (Joystick_isPressedToUP(&hal_p->joystick)){
        if (app_p->cursor == 0)
            app_p->cursor = 2;
        else
            app_p->cursor--;
    }
    else if (Joystick_isPressedToDOWN(&hal_p->joystick)){
        app_p->cursor = (app_p->cursor+1)% 3;
    }
    if (Button_isTapped(&hal_p->boosterpackJS))
        {
            switch (app_p->cursor){
            case 0:
                Application_transitiongamescreen(app_p, hal_p);
                app_p-> menuestates = playGame;
                break;
            case 1:
                Application_showhighscores(app_p, hal_p);
                app_p->menuestates = Highscores;
                break;

            case 2:
                Application_showhowtoplay( app_p, hal_p);
                app_p->menuestates = howtoplay;
                break;
            }
        }
}

/*
 * This function does the transitioning from main game screen to menu screen it also set's the background color to red.
 */
void Application_transitionshowmenue(Application* app, HAL* hal_p)
{
    GFX_setBackground(&hal_p->gfx, GRAPHICS_COLOR_RED);
    GFX_clear(&hal_p->gfx);
    Application_showmenuScreen( app,&hal_p->gfx);
}

/*
 * This function is in charge of showing the main menu screen also displaying the cursor for each state such as "play game" , Highscores, and How to play
 */
void Application_showmenuScreen(Application* app_p, GFX* gfx_p)
{


       if (app_p->cursor == 0){
           GFX_print(gfx_p, "> Play Game", 2, 2);
       }
       else
       {
           GFX_print(gfx_p, "  Play Game", 2, 2);
       }
       if (app_p->cursor == 1){
                  GFX_print(gfx_p, "> High scores", 3, 2);
              }
              else
              {
                  GFX_print(gfx_p, "  High scores", 3, 2);
              }
       if (app_p->cursor == 2){
                  GFX_print(gfx_p, "> How to play", 4, 2);
              }
              else
              {
                  GFX_print(gfx_p, "  How to play", 4, 2);
              }

}
/*
 * In this function we clear the screen and proceed to the how to play game instruction screen, and the instructions are displayed.
 */
void Application_showhowtoplay(Application* app, HAL* hal_p)
    {
    GFX* gfx_p = &hal_p->gfx;
    GFX_clear(gfx_p);

       GFX_print(gfx_p, "Control bee with the", 0, 0);
       GFX_print(gfx_p, "Joystick. Collect the", 1, 0);
       GFX_print(gfx_p, "square pollen and use", 2, 0);
       GFX_print(gfx_p, "it to pollinate the", 3, 0);
       GFX_print(gfx_p, "circular plants. Fail", 4, 0);
       GFX_print(gfx_p, "To pollinate 3 plants", 5, 0);
       GFX_print(gfx_p, "before they disappear", 6, 0);
       GFX_print(gfx_p, "and it's game over.", 7, 0);
       GFX_print(gfx_p, "Press JS to return...", 12, 0);

}

/*
 * This function is in charge of handeling the how to play screen and how when teh joystick is pressed the screen goes back to main menu screen.
 */
void Application_handlehowtoplay(Application* app, HAL* hal_p){
    if (Button_isTapped(&hal_p->boosterpackJS))
    {
        Application_transitionshowmenue(app, hal_p);
        app->menuestates = show_menue_tsates;
    }
}

/*
 * This function is in charge of showing the Highscores screen.
 */
void Application_showhighscores(Application* app, HAL* hal_p)
    {
           GFX* gfx_p = &hal_p->gfx;
           GFX_clear(gfx_p);
           GFX_print(gfx_p, "High Scores", 0, 0);
           GFX_print(gfx_p, "-------------------------", 1, 0);
           GFX_print(gfx_p, "Press JS to return", 12, 0);
           char buffer [10]={0};
           int i;
           for(i=0;i<5;i++){
               snprintf (buffer,7,"%u: %u", i+1, app->Highscore[i]);
               GFX_print(gfx_p, buffer, 2+i, 3);
           }
    }

/*
 * This function is in charge of the handeling the score.
 */
void Application_handlescores(Application* app, HAL* hal_p){

if (Button_isTapped(&hal_p->boosterpackJS))
   {
       Application_transitionshowmenue(app, hal_p);
       app->menuestates = show_menue_tsates;
   }

}

/*
 * This function takes care of the transition into game screen so that when the
 * player presses play game the screen displays the player, the pollen and the flower. The stats on the bottom of the screen aslo displayed.
 */
void Application_transitiongamescreen(Application* app, HAL* hal_p){

    GFX* gfx_p = &hal_p->gfx;
        GFX_clear(gfx_p);

           GFX_print(gfx_p, "Play Game", 0, 0);
           GFX_print(gfx_p, "------------------------", 1, 0);
           SWTimer_start(&app->flowerspawner);
           SWTimer_start(&app->pollinspawner);
           app->LEFT = 0;
           app->RIGHT = 0;
           app->UP = 0;
           app->DOWN = 0;
           app->pollenscore=0;
           app->Heart = 3;
           app->Score =0;
           int i;
           for (i=0; i<3; i++){
               app->flowers[i].active=false;
               app->pollen[i].active=false;
           }

}
/*
 * This function is in charge of the showing the game screen once the play taps play game.
 * This function controls the player and how he moves the circle.it sets the boundaries so that the player does not go off screenand limits them to 110.
 */
void Application_showgamescreen(Application* app, HAL* hal_p){
    GFX_removeSolidCircle(&hal_p->gfx, app->CircleX, app->CircleY, 5);
  if (app->UP >= app->up &&app->CircleY>20){
      app->CircleY-=3;
      app->UP = 0;
  }
  else if (app->DOWN >= app->down&&app->CircleY<110){
      app->CircleY+=3;
      app->DOWN = 0;
  }
      if (app->LEFT >=app->left&&app->CircleX>6){
          app->CircleX-=3;
          app->LEFT = 0;
      }
      else if (app->RIGHT >= app->right &&app->CircleX<110){
            app->CircleX+=3;
            app->RIGHT = 0;

  }
  Application_showflower(app, hal_p);

  Application_showpollin(app, hal_p);

  GFX_drawSolidCircle(&hal_p->gfx, app->CircleX, app->CircleY, 5);

}

/*
 * This function is in charge of showing the flowers both pollinated and none pollinated.
 *  if the flower is not pollinated it will draw a hollow circle and if it is pollinated it will fill in that circle.
 */
void Application_showflower(Application* app, HAL* hal_p){
    int i = 0;
    for (i =0; i<3;  i++ ){
    if (app->flowers[i].active){

          Flower* flower = &app->flowers[i];

    if   ( flower->pollenated == false){
          GFX_drawHollowCircle(&hal_p->gfx, flower->X, flower->Y, 3);
    }

    if ( flower->pollenated){
        GFX_drawSolidCircle(&hal_p->gfx, flower->X, flower->Y, 3);
    }
    }
    }
}
/*
 * This application is in charge of displaying the pollen on screen and it does that by drawing a solid rectangle using the function that is mentioned in Graphics.H
 */
void Application_showpollin(Application* app, HAL* hal_p){

    int i = 0;
    for (i = 0; i<3; i++){
    if (app->pollen[i].active){


        Pollen* pollen = &app->pollen[i];

        GFX_drawSolidRectangle(&hal_p->gfx, pollen->X, pollen->Y, 8,8);

    }
    }
}
/*
 * This function handles the input of the Joystick it is in control of communicating with the joystick wether to go up Upr or go down wether to go tight or left.
 * the thresholds for the Joystick is also mentioned in Joystick.H
 */
void Application_handleinput(Application* app, HAL* hal_p){
    Joystick*joystick = &hal_p->joystick;
       uint_fast16_t x = joystick->x;
       uint_fast16_t y = joystick->y;

   if( y>UP_threshold){

       app->UP++;
}
   else if(y<DOWN_threshold){
       app->DOWN++;

   }
   if( x<LEFT_threshold){
       app->LEFT++;

   }
   if (x>RIGHT_threshold) {
           app->RIGHT++;
   }

}

Flower* Application_getnewflower(Application* app, HAL* hal_p){
    int i = 0;
    for (i =0; i<3;  i++ ){
        if (!app->flowers[i].active)
            return &app->flowers[i];
    }
    return NULL;
}

Pollen* Application_getnewpollen(Application* app, HAL* hal_p){
    int i = 0;
    for (i = 0; i<3; i++){
        if (!app->pollen[i].active)
            return &app->pollen[i];
    }
    return NULL;

}
/*
 *This function is in charge of where the flower particles are allocated on the screen, how fast they fall and when they disappear.
 */
void Application_allocateflower(Application* app, HAL* hal_p){
    Flower* flower = Application_getnewflower( app, hal_p);
    if (flower){
        flower->active = true;
        flower->Y = flowerYValue;
        flower->X = rand()%110;
        flower->DOWN = 5;
        flower->down =0;
        flower->pollenated =false;

    }
}

/*
 *This function is in charge of where the pollen particles are allocated on the screen, how fast they fall, and when they disappear.
 */
void Application_allocatepollen(Application* app, HAL* hal_p){
    Pollen* pollen = Application_getnewpollen( app, hal_p);
    if (pollen){
           pollen->active = true;
           pollen->Y = flowerYValue;
           pollen->X = rand()%110;
           pollen->DOWN = 5;
           pollen->down =0;
    }

}
/*
 *This function handles the logic that makes both pollen and flower particles to drop from the top of the screen to the bottom.
 */
void Application_handleflowerpollenlogic(Application* app, HAL* hal_p){

    int i = 0;
    for (i = 0; i<3; i++){
    if (app->pollen[i].active)
    {
         Pollen* pollen = &app->pollen[i];
         pollen->down++;

         if (pollen->Y>=110)
             pollen->active = false;
         if (pollen->down >= pollen->DOWN ){
                    pollen->down =0;
                    pollen->Y++;
         }
        }
    }

    for (i =0; i<3;  i++ ){
        if (app->flowers[i].active)
        {
         Flower* flower = &app->flowers[i];
         flower->down++;

         if (flower->Y>=110){
             flower->active = false;
             if (!flower->pollenated){
                 app->Heart--;
             }
             else {
                 app->Score+=3;
             }
         }
         if (flower->down >= flower->DOWN ){
             flower->down =0;
             flower->Y++;
         }
        }
    }
}
/*
 *this application is used to remove the pollen adn the marker it leaves which is a solid rectangle
 */
void Application_removepollin(Application* app, HAL* hal_p){
    int i = 0;
    for (i = 0; i<3; i++){
    if (app->pollen[i].active){


        Pollen* pollen = &app->pollen[i];

        GFX_removeSolidRectangle(&hal_p->gfx, pollen->X, pollen->Y, 8,8);
    }
}
}
/*
 *this function removes the unpollinated flower which is a hollow symbol and replaces it with a solid circle once it is pollenated.
 */
void Application_removeflower(Application* app, HAL* hal_p){
    int i = 0;
    for (i =0; i<3;  i++ ){
    if (app->flowers[i].active){

          Flower* flower = &app->flowers[i];

          if (app->flowers[i].pollenated ==false)
              GFX_removeHollowCircle(&hal_p->gfx, flower->X, flower->Y, 3);

          if (app->flowers[i].pollenated)
               GFX_removeSolidCircle(&hal_p->gfx, flower->X, flower->Y, 3);
    }
}
}

/*
 *This function handles the flower and pollen spawner.
 *This once the timer is satrted for the play game screen the allocate function is called and activated and oce the game is over it stops.
 */
void Applicationhandleflwowerpollinspawner(Application* app, HAL* hal_p){
    if(SWTimer_expired(&app->flowerspawner)){
       SWTimer_start(&app->flowerspawner);
       Application_allocateflower(app, hal_p);
    }
    if(SWTimer_expired(&app->pollinspawner)){
    (SWTimer_start(&app->pollinspawner));
    Application_allocatepollen(app, hal_p);
    }
}
/*
 * This function is in charge of the collision logic that explains when the flower circles disappear when the bee or the player comes in contact with it.
 * It calculated the position of the position of the player and the position of the flower and when they come in contact the flower disappears and the flower counter at
 * at bottom of the screen after a flower gets polinated the score for polination goes up by +3 and the number of pollen gets decreased by -1.
 */
void Applicationflowercollision(Application* app, HAL* hal_p){
    int i;
    for ( i=0;i<3;i++){
        double X=app->CircleX - app->flowers[i].X;
        double Y=app->CircleY - app->flowers[i].Y;
        double dist=(sqrt((X*X)+(Y*Y)));
        if (!app->flowers[i].pollenated && app->pollenscore>0&& dist<(8.0)){
            app->flowers[i].pollenated =true;
            app->pollenscore--;
        }
    }
}

/*
 * This function is in charge of the collision logic that explains when the pollen squares disappear when the bee or the player comes in contact with it.
 * It calculated the position of the position of the player and the postion of the pollen and when they come in contact the pollen disappears and the pollen counter at
 * the bottom of the screen increases and inthis function it is done in another function but pointed at here using a pointer.
 */
void Applicationpollencollision(Application* app, HAL* hal_p){

if
    (app->pollen[0].active&&(app->pollen[0].X+8> app->CircleX-8)&&(app->pollen[0].X< app->CircleX+8)&&(app->pollen[0].Y< app->CircleY+8)&&(app->pollen[0].Y+8> app->CircleY-8)){
     app->pollen[0].active =false;
     app->pollenscore++;
     app->Score++;
}

if
          (app->pollen[1].active&&(app->pollen[1].X+8> app->CircleX-8)&&(app->pollen[1].X< app->CircleX+8)&&(app->pollen[1].Y< app->CircleY+8)&&(app->pollen[1].Y+8> app->CircleY-8)){
    app->pollen[1].active =false;
    app->pollenscore++;
    app->Score++;
}

if
            (app->pollen[2].active&&(app->pollen[2].X+8> app->CircleX-8)&&(app->pollen[2].X< app->CircleX+8)&&(app->pollen[2].Y< app->CircleY+8)&&(app->pollen[2].Y+8> app->CircleY-8)){
    app->pollen[2].active =false;
    app->pollenscore++;
    app->Score++;
}
}

/*
 * This function is in charge of handling the game over transition.
 * it starts by clearing the screen and displaying the results of the score that they player achieved for a limited time.
 */
void ApplicationHandleGameovertransition(Application* app, HAL* hal_p){
        GFX* gfx_p = &hal_p->gfx;
        GFX_clear(gfx_p);
        char buffer [20]={0};
        snprintf(buffer,20, "you'r Score: %u",app->Score );
        GFX_print(gfx_p, "Game Over", 0, 0);
        GFX_print(gfx_p, "--------------------", 1, 0);


         GFX_print(gfx_p, buffer, 3, 2);
         SWTimer_start(&app->timer);

}
/*
 * This function handles the logic that ties the game over screen.
 * this function will display the game over screen after the player has run out of hearts.
 * it does that by executing the  ApplicationHandleGameovertransition
 */
void ApplicationHandleGamelogic(Application* app, HAL* hal_p){
    if (app->Heart<=0){
        app->gamestates=Gameover;
        ApplicationHandleGameovertransition( app, hal_p);
    }
}
/*
 * This function is in charge of desplaying the stats or the final results the player gets after the game over screen.
 */
void Applicationshowstats(Application* app, HAL* hal_p){
    char buffer[20]={0};
    snprintf(buffer,20,  "s: %u h: %u p: %u", app->Score, app->Heart, app->pollenscore);
    GFX* gfx_p = &hal_p->gfx;
    GFX_print(gfx_p, buffer, 15, 0);
}

/*
 * This is used for the Highscores screen this will get what ever score the player got and
 *  store it in ordere from largest to beggist. this is also the last step to qsort function.
 */
int compare(const void *a, const void *b) {
       return -(*(int *)a + *(int *)b);
   }
/*
 * This function is in charge of handeling the game over screen when the timer is expired the transition
 * menue application is activated and we are back to the main menue screen after showing the player his score.
 */
void ApplicationHandleGameover(Application* app, HAL* hal_p){
    if (SWTimer_expired(&app->timer)){
        Application_transitionshowmenue(app, hal_p);
        app->gamestates=  Menu_Screen;
        app->menuestates= show_menue_tsates;
        app->Highscore[5]=app->Score;
        qsort(app->Highscore,6,sizeof ( int ), compare);
    }
}

int main()
{

    WDT_A_holdTimer();

    InitSystemTiming();

    HAL hal = HAL_construct();
    Application app = Application_construct();

    Application_showTitleScreen(&app, &app.timer,&app.squartimer, &hal.gfx);
    // Do not remove this line. This is your non-blocking check.
    InitNonBlockingLED();
    while (1)
    {
        HAL_refresh(&hal);

        // Do not remove this line. This is your non-blocking check.

        PollNonBlockingLED();



        Gamefsm(&app, &hal);

    }
}

    /*
     * This is where the Main Finite State Machines or the MAIN FSM of the whole game is located.
     * Gamefsm is in charge of the start screen the MenuScreen and the GameOver screen.
     * Menufsm is in charge of showing the options inside Menu_Screen case in Game FSM.
     * it is in control of howtoplay,Highscores, and playGame screens.
     */
    void Gamefsm(Application* app_p, HAL* hal_p){
        switch(app_p->gamestates){
            case startscreen:
                Application_handleTitleScreen(app_p, hal_p);
                break;
            case Menu_Screen:
                Menuefsm(app_p, hal_p);
                break;
            case Gameover:
                ApplicationHandleGameover(app_p, hal_p);
                break;
        }
    }
    void Menuefsm(Application* app_p, HAL* hal_p){
        switch(app_p->menuestates){
        case show_menue_tsates:

            Application_showmenuScreen(app_p, &hal_p->gfx);
            Application_handleMENUScreen(app_p, hal_p);
            break;
        case howtoplay:
             Application_handlehowtoplay( app_p, hal_p);
             break;
        case Highscores:
            Application_handlescores( app_p,hal_p);
            break;
        case playGame:
            Application_removepollin(app_p,hal_p);
            Application_removeflower(app_p,hal_p);
            Applicationhandleflwowerpollinspawner(app_p,hal_p);
            Applicationpollencollision(app_p,hal_p);
            Applicationflowercollision(app_p,hal_p);
            Application_handleflowerpollenlogic(app_p,hal_p);
            Application_handleinput(app_p,hal_p);
            Application_showgamescreen(app_p,hal_p);
            Applicationshowstats(app_p,hal_p);
            ApplicationHandleGamelogic(app_p,hal_p);
            break;
        }

        }

