/**
 * Starter code for Project 2. Good luck!
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <HAL/HAL.h>
#include <HAL/Timer.h>
#include <Application.h>
#include <HAL/Graphics.h>

////////////the following const are for my images
extern const Graphics_Image Cake_Cooking8BPP_UNCOMP;
extern const Graphics_Image Cake_making8BPP_UNCOMP;
extern const Graphics_Image Crust_pic8BPP_UNCOMP;
extern const Graphics_Image Cream_cooking8BPP_UNCOMP;
extern const Graphics_Image Chocolate_make8BPP_UNCOMP;
extern const Graphics_Image Berry_pick8BPP_UNCOMP;
extern const Graphics_Image Kiwi_pick8BPP_UNCOMP;
extern const Graphics_Image Mango_pick8BPP_UNCOMP;
extern const Graphics_Image Win_result8BPP_UNCOMP;
extern const Graphics_Image bad_result8BPP_UNCOMP;


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



int main()
 {
    WDT_A_holdTimer();

    // Do not remove this line. This is your non-blocking check.
    InitNonBlockingLED();
    InitSystemTiming();

    HAL hal_p = HAL_construct();
    Application app_p = Application_construct(&hal_p);
    Application_showTitleScreen(&hal_p.gfx);
    while (true)
    {
        // Do not remove this line. This is your non-blocking check.
        Application_loop(&app_p, &hal_p, &hal_p.gfx);
        HAL_refresh(&hal_p);
        PollNonBlockingLED();
    }
}


Application Application_construct(HAL* hal_p)
{
    // The App_GuessTheColor object to initialize
    Application app_p;

    // Predetermined random numbers for this application. In an actual project,
    // you should probably use some form of noise generator instead, like the
    // noise from your ADC.
    app_p.countTimer = SWTimer_construct(BACK_TO_BEGIN);// set the countdown timer to decrease from start by 1 second
    SWTimer_expired(&app_p.countTimer);//start to expire time
    SWTimer_start(&app_p.countTimer);//count down again from the current time
    app_p.time = TIME_START;//initialize the start time from 30 seconds

    //////////////////////////initialize all my macros and states in following
    app_p.error = 0;
    app_p.income = 0;
    app_p.crust = 0;
    app_p.cream = 0;
    app_p.berry = 0;
    app_p.kiwi = 0;
    app_p.mango = 0;
    app_p.choco = 0;
    app_p.randomnumber = 0;
    app_p.remainder = 0;
    app_p.sc1 = 0;
    app_p.sc2 = 0;
    app_p.sc3 = 0;
    app_p.sc4 = 0;
    app_p.result = 0;
    // Initialization of FSM variables
    app_p.creamState = select_ing1;
    app_p.crustState = crust1;
    app_p.berryState = berry1;
    app_p.kiwiState = kiwi1;
    app_p.mangoState = mango1;
    app_p.chocoState = choco1;

    app_p.state = TITLE_SCREEN;

    app_p.matchState = match1;
    app_p.dishState = dish1;


    //app_p.errorState = error1;

    app_p.timer = SWTimer_construct(TITLE_SCREEN_WAIT); //this is fro the title screen to expire in three seconds to next screen
    SWTimer_start(&app_p.timer);

    Application_initGameVariables(&app_p, hal_p);

    app_p.cursor = CURSOR_0;

    // Return the completed Application struct to the user
    return app_p;
}






void Application_loop(Application* app_p, HAL* hal_p, GFX* gfx_p)
{



    if (SWTimer_expired(&app_p->countTimer) && (app_p->state == GAME_SCREEN)){///////////this is the count down timer function
            app_p->time--;
            SWTimer_start(&app_p->countTimer);

        }





    switch (app_p->state)////////////my loop
    {
        case TITLE_SCREEN:
            Application_handleTitleScreen(app_p, hal_p);
            break;

        case MENU_SCREEN:
            Application_handleMenuScreen(app_p, hal_p);
            break;

        case HOWTOPLAY_SCREEN:
            Application_handleHowToPlayScreen(app_p, hal_p);
            break;

        case HIGHSCORES_SCREEN:
            Application_handleHighScoresScreen(app_p, hal_p);
            break;

        case GAME_SCREEN:
            Application_handleGameScreen(app_p, hal_p);
            break;

        case WIN_SCREEN:
            Application_handlewinScreen(app_p, hal_p);
            break;

        case LOSE_SCREEN:
                    Application_handleloseScreen(app_p, hal_p);
                    break;

        default:
            break;
    }
}


void Application_showTitleScreen(GFX* gfx_p)/////////////my title screen show all my contents
{

    GFX_clear(gfx_p);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &Cake_Cooking8BPP_UNCOMP, 0, 0);

    GFX_print(gfx_p, "Cooking tale  ", 0, 0);
    GFX_print(gfx_p, "By: Qianhui Xie", 1, 0);
    GFX_print(gfx_p, "ECE2564", 2, 0);

}



void Application_initGameVariables(Application* app_p, HAL* hal_p)
{
    // Reset the cursor
    app_p->cursor = CURSOR_0;

    // Deselect each option
    app_p->caketime = false;
    app_p->highscores = false;
    app_p->howtocook = false;

    // Turn off all LEDs - they don't turn on until a random number is generated
    LED_turnOff(&hal_p->boosterpackRed);
    LED_turnOff(&hal_p->boosterpackGreen);
    LED_turnOff(&hal_p->boosterpackBlue);


}




void Application_handleTitleScreen(Application* app_p, HAL* hal_p)
{
    if (SWTimer_expired(&app_p->timer))///////////////control title screen, in three seconds, the title screen will turn to menu screen
    {
        app_p->state = MENU_SCREEN;
        Application_showMenuScreen(app_p, &hal_p->gfx);

    }
}


void Application_handleMenuScreen(Application* app_p, HAL* hal_p)////////////////////
{
    if (Joystick_isTappedToUp(&hal_p->joystick) && ((int) app_p->cursor) > 0) {////////////tapp the joystick to up to move the cursor
            app_p->cursor = (Cursor) (((int) app_p->cursor - 1) % NUM_TEST_OPTIONS);
            Application_updateMenuScreen(app_p, &hal_p->gfx);
        }
    if (Joystick_isTappedToDown(&hal_p->joystick)) {/////////////////////tap the joystick to down to move the cursor
                app_p->cursor = (Cursor) (((int) app_p->cursor + 1) % NUM_TEST_OPTIONS);
                Application_updateMenuScreen(app_p, &hal_p->gfx);
            }

        // If B1 is pressed, either add a selection to the proper color choice OR
        // transition to the SHOW_RESULT state if the user chooses to end the test.
        if (Joystick_isTappedToDown(&hal_p->joystick))
        {
            switch (app_p->cursor)
            {
                // In the first three choices, we need to re-display the game screen
                // to reflect updated choices.
                // -----------------------------------------------------------------
                case CURSOR_0: // Red choice
                    app_p->caketime = true;/////////////at cursor 0, point to cake time option in menu screen
                    Application_updateMenuScreen(app_p, &hal_p->gfx);
                    break;

                case CURSOR_1: // Green choice
                    app_p->highscores = true;//////////////at cursor 1, point to highscores option in menu screen
                    Application_updateMenuScreen(app_p, &hal_p->gfx);
                    break;

                case CURSOR_2: // Blue choice
                    app_p->howtocook = true;//////////////at cursor 2, point to howtowork option in menu screen
                    Application_updateMenuScreen(app_p, &hal_p->gfx);
                    break;



                // In the final choice, we must setup a transition to RESULT_SCREEN
                // by starting a timer and calling the proper draw function.
                // -----------------------------------------------------------------

                default:
                    break;
            }
        }

        if (Button_isTapped(&hal_p->boosterpackJS) && app_p->cursor == CURSOR_2){//////////////at cursor 2, tap joystick will turn the menu to howtoplay screen
            app_p->state = HOWTOPLAY_SCREEN;
            Application_showHowToPlayScreen(app_p, &hal_p->gfx);
        }

        if (Button_isTapped(&hal_p->boosterpackJS) && app_p->cursor == CURSOR_1){//////////////at cursor 1, tap joystick will turn the menu to jighscores screen
            app_p->state = HIGHSCORES_SCREEN;
            Application_showHighScoresScreen(app_p, &hal_p->gfx);

        }

        if (Button_isTapped(&hal_p->boosterpackJS) && app_p->cursor == CURSOR_0){//////////////at cursor 0, tap joystick will turn the menu to the game screen

            app_p->state = GAME_SCREEN;
            Application_showGameScreen(app_p, &hal_p->gfx);
        }
}





void Application_handleHowToPlayScreen(Application* app_p, HAL* hal_p)
{
    if (Button_isTapped(&hal_p->boosterpackJS)) {//at how to play screen, tap joystick button will turn to menu screen
        app_p->state = MENU_SCREEN;
        Application_showMenuScreen(app_p, &hal_p->gfx);
    }
}


void Application_handleHighScoresScreen(Application* app_p, HAL* hal_p) {////////////////////////

//////////the follwoing three buffers is to print out the three highest scores.
    char buffer1[15];
    snprintf(buffer1, 15, "%02d", app_p->sc1);
    GFX_print(&hal_p->gfx, buffer1, 3, 0);

    char buffer2[15];
    snprintf(buffer2, 15, "%02d", app_p->sc2);
    GFX_print(&hal_p->gfx, buffer2, 4, 0);

    char buffer3[15];
    snprintf(buffer3, 15, "%02d", app_p->sc3);
    GFX_print(&hal_p->gfx, buffer3, 5, 0);

    if (Button_isTapped(&hal_p->boosterpackJS)) {//tap joystick button will turn to menu screen

            app_p->state = MENU_SCREEN;

            Application_showMenuScreen(app_p, &hal_p->gfx);

        }
}



////////////////////////////////////////////////////////////////////////////////draw the rectangles which will contain all my ingredients. and remove them
//////////in the following, I listed the rectangle locations
void basic_one(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tr1;
    tr1.xMax=86;
    tr1.xMin=44;
    tr1.yMax=92;
    tr1.yMin=65;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tr1);
}

void basic_two(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tr1;
    tr1.xMax=86;
    tr1.xMin=44;
    tr1.yMax=92;
    tr1.yMin=65;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLUE);
    Graphics_drawRectangle(&gfx_p->context, &tr1);
}
////////////////////////////////////////////////////////////////////////////////////////////////push to left
void Rectangle_movel(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=43;
    tRectangle.xMin=0;
    tRectangle.yMax=92;
    tRectangle.yMin=65;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLUE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle);


}

void Rectangle_movell(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=43;
    tRectangle.xMin=0;
    tRectangle.yMax=92;
    tRectangle.yMin=65;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle);


}

/////////////////////////////////////////////////////////////////////////////////////push to right
void Rectangle_mover(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=126;
    tRectangle.xMin=87;
    tRectangle.yMax=92;
    tRectangle.yMin=65;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLUE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle);


}

void Rectangle_moverr(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=126;
    tRectangle.xMin=87;
    tRectangle.yMax=92;
    tRectangle.yMin=65;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle);


}


//////////////////////////////////////////////////////////////////////////////////////////move to up

void Rectangle_moveu(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=86;
    tRectangle.xMin=43;
    tRectangle.yMax=45;
    tRectangle.yMin=18;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLUE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle);



}

void Rectangle_moveuu(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=86;
    tRectangle.xMin=43;
    tRectangle.yMax=45;
    tRectangle.yMin=18;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle);


}

//////////////////////////////////////////////////////////////upleft

void move_upleft(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=43;
    tRectangle.xMin=0;
    tRectangle.yMax=45;
    tRectangle.yMin=18;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLUE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle);
}

void move_upleftt(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=43;
    tRectangle.xMin=0;
    tRectangle.yMax=45;
    tRectangle.yMin=18;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle);
}

//////////////////////////////////////////////////////////////////////////upright
void move_upright(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=126;
    tRectangle.xMin=86;
    tRectangle.yMax=45;
    tRectangle.yMin=18;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLUE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle);
}

void move_uprightt(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=126;
    tRectangle.xMin=86;
    tRectangle.yMax=45;
    tRectangle.yMin=18;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Application_handleGameScreen(Application* app_p, HAL* hal_p)
{

    char buffer8[15];///print  out my count down time
          snprintf(buffer8, 15, "%02d", app_p->time);
          GFX_print(&hal_p->gfx, buffer8, 0, 10);

          //////////////////////////////////////////////////in the following are my joystick operations in my game.
          //////////////////////move joystick to different location will highlight differetnt rectangles.
   if (Joystick_isStill(&hal_p->joystick)) {

        ingredient_select2(app_p, hal_p);




       basic_two(app_p, &hal_p->gfx);
   }


    if (Joystick_isPushedToLeft(&hal_p->joystick)){
        Rectangle_movel(app_p, &hal_p->gfx);
        basic_one(app_p, &hal_p->gfx);
        ingredient_select3(app_p, hal_p, &hal_p->gfx);
    }
    else
        Rectangle_movell(app_p, &hal_p->gfx);




     if (Joystick_isPushedToRight(&hal_p->joystick)){
        Rectangle_mover(app_p, &hal_p->gfx);
        basic_one(app_p, &hal_p->gfx);
        ingredient_select1(app_p, hal_p);
    }
    else
        Rectangle_moverr(app_p, &hal_p->gfx);


     if (Joystick_isPushedToUp(&hal_p->joystick)){
        Rectangle_moveu(app_p, &hal_p->gfx);
        basic_one(app_p, &hal_p->gfx);
        ingredient_select5(app_p, hal_p, &hal_p->gfx);
    }
    else
        Rectangle_moveuu(app_p, &hal_p->gfx);


     if (Joystick_isPushedToUpleft(&hal_p->joystick)){
        move_upleft(app_p, &hal_p->gfx);
        basic_one(app_p, &hal_p->gfx);
        ingredient_select4(app_p, hal_p, &hal_p->gfx);
    }
    else
        move_upleftt(app_p, &hal_p->gfx);



     if (Joystick_isPushedToUpright(&hal_p->joystick)){
        move_upright(app_p, &hal_p->gfx);
        basic_one(app_p, &hal_p->gfx);
        ingredient_select6(app_p, hal_p, &hal_p->gfx);
    }

    else {

        move_uprightt(app_p, &hal_p->gfx);

    }



          menu_dish(app_p, hal_p, &hal_p->gfx);//////////////this function listed all my ingredients anf logic of selecting them



}






void Application_handlewinScreen(Application* app_p, HAL* hal_p)
{

    if (Button_isTapped(&hal_p->boosterpackJS)){//in my result screen, tap joystick button will turn to the menu screen and refresh everything of the game
        highscore(app_p, hal_p);
    app_p->time = 30;

    app_p->income = 0;
    app_p->error = 0;
    reset_uing(app_p, hal_p);




        app_p->state = MENU_SCREEN;
                   Application_showMenuScreen(app_p, &hal_p->gfx);
    }
}

void Application_handleloseScreen(Application* app_p, HAL* hal_p)
{
    if (Button_isTapped(&hal_p->boosterpackJS)){//in my result screen, tap joystick button will turn to the menu screen and refresh everything of the game
        highscore(app_p, hal_p);
        app_p->time = 30;

        app_p->income = 0;
        app_p->error = 0;
        reset_uing(app_p, hal_p);




            app_p->state = MENU_SCREEN;
                       Application_showMenuScreen(app_p, &hal_p->gfx);
        }
}


void Application_showMenuScreen(Application* app_p, GFX* gfx_p)
{
    // Clear the screen from any old text state and print out all my texts.
    GFX_clear(gfx_p);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &Cake_making8BPP_UNCOMP, 1, 0);

    // Display the text
    GFX_print(gfx_p, "Cooking Tale         ", 0, 0);
    GFX_print(gfx_p, "----Main Menu----", 12, 3);
    GFX_print(gfx_p, "Cake time   ", 13, 5);
    GFX_print(gfx_p, "High scores", 14, 5);

    GFX_print(gfx_p, "How to cook    ", 15, 5);

    GFX_print(gfx_p, ">", 13 + app_p->cursor, 3);

    // Draw the stars for LED guesses


}



////////////////////////////////////////////////////////////////////////////////////////////////////////
void Application_showHowToPlayScreen(Application* app_p, GFX* gfx_p)
{

    ///////////////in this function listed below are all the texts that i need to show in the show how to play screen
    //the content is the description of how to play this game
    GFX_clear(gfx_p);
    GFX_print(gfx_p, "How to play", 0, 0);
    GFX_print(gfx_p, "-----------", 1, 0);
    GFX_print(gfx_p, "Move joystick towards", 2, 0);
    GFX_print(gfx_p, "an ingredient to", 3, 0);
    GFX_print(gfx_p, "highlight it", 4, 0);

    GFX_print(gfx_p, "Press BoostS1 to add", 6, 0);
    GFX_print(gfx_p, "one serving to dish.", 7, 0);
    GFX_print(gfx_p, "Press BoostS2 to", 8, 0);
    GFX_print(gfx_p, "reset dish.", 9, 0);
    GFX_print(gfx_p, "Press joystick in", 10, 0);
    GFX_print(gfx_p, "to serve dish.", 11, 0);

    GFX_print(gfx_p, "Follow the recipe", 13, 0);
    GFX_print(gfx_p, "ticket carefully!", 14, 0);

}


void Application_showHighScoresScreen(Application* app_p, GFX* gfx_p)
{
    GFX_clear(gfx_p);
    GFX_print(gfx_p, "High Scores", 0, 0);////////////add a head of this screen
    GFX_print(gfx_p, "-----------", 1, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////


void Application_showGameScreen(Application* app_p, GFX* gfx_p)
{
    // Clear the screen from any old text state
    GFX_clear(gfx_p);
    GFX_print(gfx_p, "$       0:", 0, 0);
    GFX_print(gfx_p, "kiwi   mango  choco", 1, 1);
    GFX_print(gfx_p, "berry  crust  cream", 7, 1);



    ///////////////////////the Graphics_drawimage is to display all the customed image
    Graphics_drawImage((const Graphics_Context *)gfx_p, &Crust_pic8BPP_UNCOMP, 46, 65);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &Cream_cooking8BPP_UNCOMP, 88, 65);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &Chocolate_make8BPP_UNCOMP, 88, 18);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &Berry_pick8BPP_UNCOMP, 3, 65);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &Kiwi_pick8BPP_UNCOMP, 3, 19);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &Mango_pick8BPP_UNCOMP, 46, 19);


    /////////////the rectangles listed below is forall the rectangles containing my ingredients.
    Graphics_Rectangle tRectangle1, tRectangle2, tRectangle3, tRectangle4, tRectangle5, tRectangle6, tRectangle7, tRectangle8,
    tRectangle9, tRectangle10, tRectangle11, tRectangle12;



    //Graphics_Rectangle tRectangle3;
    tRectangle3.xMax=43;
    tRectangle3.xMin=0;
    tRectangle3.yMax=45;
    tRectangle3.yMin=18;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle3);

    //Graphics_Rectangle tRectangle4;
    tRectangle4.xMax=86;
    tRectangle4.xMin=43;
    tRectangle4.yMax=45;
    tRectangle4.yMin=18;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle4);

    //Graphics_Rectangle tRectangle5;
    tRectangle5.xMax=126;
    tRectangle5.xMin=86;
    tRectangle5.yMax=45;
    tRectangle5.yMin=18;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle5);
    //////////////////////////////////////////////////////////////////////////////////////////
    tRectangle6.xMax=43;
    tRectangle6.xMin=0;
    tRectangle6.yMax=52;
    tRectangle6.yMin=45;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle6);

    tRectangle7.xMax=86;
    tRectangle7.xMin=43;
    tRectangle7.yMax=52;
    tRectangle7.yMin=45;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle7);

    tRectangle8.xMax=126;
    tRectangle8.xMin=86;
    tRectangle8.yMax=52;
    tRectangle8.yMin=45;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle8);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    tRectangle9.xMax=43;
    tRectangle9.xMin=0;
    tRectangle9.yMax=92;
    tRectangle9.yMin=65;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle9);

    tRectangle10.xMax=86;
    tRectangle10.xMin=44;
    tRectangle10.yMax=92;
    tRectangle10.yMin=65;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLUE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle10);

    tRectangle11.xMax=126;
    tRectangle11.xMin=87;
    tRectangle11.yMax=92;
    tRectangle11.yMin=65;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle11);
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    tRectangle1.xMax=43;
    tRectangle1.xMin=0;
    tRectangle1.yMax=100;
    tRectangle1.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle1);

    tRectangle2.xMax=87;
    tRectangle2.xMin=43;
    tRectangle2.yMax=100;
    tRectangle2.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle2);

    tRectangle12.xMax=126;
    tRectangle12.xMin=87;
    tRectangle12.yMax=100;
    tRectangle12.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle12);


}



void Application_updateMenuScreen(Application* app_p, GFX* gfx_p){

    GFX_print(gfx_p, "  ", 13, 3);
    GFX_print(gfx_p, "  ", 14, 3);
    GFX_print(gfx_p, "  ", 15, 3);

    GFX_print(gfx_p, ">", 13 + app_p->cursor, 3);///set the cursor's start location

    // Draw the stars for LED guesses

}


void Application_winScreen(Application* app_p, HAL* hal_p, GFX* gfx_p){

    ////////display all required image and texts in this screen
    GFX_clear(gfx_p);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &Win_result8BPP_UNCOMP, 4, 0);
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    GFX_print(gfx_p, "Score:", 0, 0);
    char buffer9[15];
    snprintf(buffer9, 15, "%02d", app_p->income);
    GFX_print(&hal_p->gfx, buffer9, 0, 6);





}


void Application_loseScreen(Application* app_p, GFX* gfx_p)
{

    ////////display all required image and texts in this screen
    GFX_clear(gfx_p);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &bad_result8BPP_UNCOMP, 3, 0);
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    GFX_print(gfx_p, "No score achieved", 0, 0);



}






/////////////////////////////each ingredients can be selected for three times, so the below are small rectangles for ingredients.
/////////each ingredients will have three small rectangles. the rectangles has different colors which can be used to deleted selected rectangles
void select_cream1(Application* app_p, GFX* gfx_p){

    Graphics_Rectangle tRectangle;
    tRectangle.xMax=98;
    tRectangle.xMin=87;
    tRectangle.yMax=99;
    tRectangle.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&gfx_p->context, &tRectangle);
}

void select_cream2(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=112;
    tRectangle.xMin=100;
    tRectangle.yMax=99;
    tRectangle.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&gfx_p->context, &tRectangle);
}

void select_cream3(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=125;
    tRectangle.xMin=114;
    tRectangle.yMax=99;
    tRectangle.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&gfx_p->context, &tRectangle);
}

void delete_cream1(Application* app_p, GFX* gfx_p){//////////////delet select ingredients
    Graphics_Rectangle tRectangle;

    tRectangle.xMax=98;
    tRectangle.xMin=88;
    tRectangle.yMax=99;
    tRectangle.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
    Graphics_fillRectangle(&gfx_p->context, &tRectangle);
}

void delete_cream2(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;

    tRectangle.xMax=112;
    tRectangle.xMin=100;
    tRectangle.yMax=99;
    tRectangle.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
    Graphics_fillRectangle(&gfx_p->context, &tRectangle);
}

void delete_cream3(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;

    tRectangle.xMax=125;
    tRectangle.xMin=114;
    tRectangle.yMax=99;
    tRectangle.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
    Graphics_fillRectangle(&gfx_p->context, &tRectangle);
}

/////////////////////////////////////////////////////////////////////////////////////////

void select_crust1(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=57;
    tRectangle.xMin=43;
    tRectangle.yMax=99;
    tRectangle.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&gfx_p->context, &tRectangle);
}

void select_crust2(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=71;
    tRectangle.xMin=59;
    tRectangle.yMax=99;
    tRectangle.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&gfx_p->context, &tRectangle);
}

void select_crust3(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=85;
    tRectangle.xMin=73;
    tRectangle.yMax=99;
    tRectangle.yMin=92;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&gfx_p->context, &tRectangle);

}


void delete_crust1(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
        tRectangle.xMax=57;
        tRectangle.xMin=44;
        tRectangle.yMax=99;
        tRectangle.yMin=92;
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &tRectangle);
}

void delete_crust2(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
        tRectangle.xMax=71;
        tRectangle.xMin=59;
        tRectangle.yMax=99;
        tRectangle.yMin=92;
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &tRectangle);

}

void delete_crust3(Application* app_p, GFX* gfx_p){
    Graphics_Rectangle tRectangle;
        tRectangle.xMax=85;
        tRectangle.xMin=73;
        tRectangle.yMax=99;
        tRectangle.yMin=92;
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &tRectangle);
}


///////////////////rectangles for other ingredients are designed in a different way and are listed in the Application.h file.




void ingredient_select1(Application* app_p, HAL* hal_p){
    //ingredient cream is selected or deleted,
    //button s1 is for selecting ingredients
    //button s2 is to delete ingredients


        if (Button_isTapped(&hal_p->boosterpackS1) && app_p->creamState == select_ing1){//there are three states which represent three rectangles for each ingredient
            select_cream1(app_p, &hal_p->gfx);
            app_p->creamState++;
        }
        else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->creamState == select_ing2){
            select_cream1(app_p, &hal_p->gfx);
            select_cream2(app_p, &hal_p->gfx);
            app_p->creamState++;
        }

        else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->creamState == select_ing3){
            select_cream1(app_p, &hal_p->gfx);
            select_cream2(app_p, &hal_p->gfx);
            select_cream3(app_p, &hal_p->gfx);
            app_p->creamState++;
        }
        else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->creamState == select_ing4){
            delete_cream3(app_p, &hal_p->gfx);
            app_p->creamState--;
        }

        else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->creamState == select_ing3){
            delete_cream3(app_p, &hal_p->gfx);
            delete_cream2(app_p, &hal_p->gfx);
            app_p->creamState--;
        }

        else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->creamState == select_ing2){
                    delete_cream3(app_p, &hal_p->gfx);
                    delete_cream2(app_p, &hal_p->gfx);
                    delete_cream1(app_p, &hal_p->gfx);
                    app_p->creamState--;
                }


}

void ingredient_select2(Application* app_p, HAL* hal_p){
    //ingredient cake crust  is selected or deleted,
        //button s1 is for selecting ingredients
        //button s2 is to delete ingredients

    if (Button_isTapped(&hal_p->boosterpackS1) && app_p->crustState == crust1){
        select_crust1(app_p, &hal_p->gfx);
        app_p->crustState++;
    }
    else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->crustState == crust2){
        select_crust1(app_p, &hal_p->gfx);
        select_crust2(app_p, &hal_p->gfx);
        app_p->crustState++;
    }

    else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->crustState == crust3){
        select_crust1(app_p, &hal_p->gfx);
        select_crust2(app_p, &hal_p->gfx);
        select_crust3(app_p, &hal_p->gfx);
        app_p->crustState++;
    }
    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->crustState == crust4){
        delete_crust3(app_p, &hal_p->gfx);
        app_p->crustState--;
    }

    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->crustState == crust3){
        delete_crust3(app_p, &hal_p->gfx);
        delete_crust2(app_p, &hal_p->gfx);
        app_p->crustState--;
    }

    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->crustState == crust2){
        delete_crust3(app_p, &hal_p->gfx);
        delete_crust2(app_p, &hal_p->gfx);
        delete_crust1(app_p, &hal_p->gfx);
        app_p->crustState--;
    }


}

void ingredient_select3(Application* app_p, HAL* hal_p, GFX* gfx_p){
    //ingredient strawberry is selected or deleted,
        //button s1 is for selecting ingredients
        //button s2 is to delete ingredients
    if (Button_isTapped(&hal_p->boosterpackS1) && app_p->berryState == berry1){
        Graphics_fillRectangle(&gfx_p->context, &myBerry1);
        app_p->berryState++;
    }
    else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->berryState == berry2){
        Graphics_fillRectangle(&gfx_p->context, &myBerry1);
        Graphics_fillRectangle(&gfx_p->context, &myBerry2);
        app_p->berryState++;
    }

    else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->berryState == berry3){
        Graphics_fillRectangle(&gfx_p->context, &myBerry1);
        Graphics_fillRectangle(&gfx_p->context, &myBerry2);
        Graphics_fillRectangle(&gfx_p->context, &myBerry3);

        app_p->berryState++;
    }
    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->berryState == berry4){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &eBerry3);

        app_p->berryState--;
    }

    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->berryState == berry3){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &eBerry3);
        Graphics_fillRectangle(&gfx_p->context, &eBerry2);
        app_p->berryState--;
    }

    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->berryState == berry2){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &eBerry3);
        Graphics_fillRectangle(&gfx_p->context, &eBerry2);
        Graphics_fillRectangle(&gfx_p->context, &eBerry1);
        app_p->berryState--;

    }

}

void ingredient_select4(Application* app_p, HAL* hal_p, GFX* gfx_p){
    //ingredient kiwi is selected or deleted,
        //button s1 is for selecting ingredients
        //button s2 is to delete ingredients
    if (Button_isTapped(&hal_p->boosterpackS1) && app_p->kiwiState == kiwi1){
        Graphics_fillRectangle(&gfx_p->context, &mykiwi1);
        app_p->kiwiState++;
    }
    else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->kiwiState == kiwi2){
        Graphics_fillRectangle(&gfx_p->context, &mykiwi1);
        Graphics_fillRectangle(&gfx_p->context, &mykiwi2);
        app_p->kiwiState++;
    }

    else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->kiwiState == kiwi3){
        Graphics_fillRectangle(&gfx_p->context, &mykiwi1);
        Graphics_fillRectangle(&gfx_p->context, &mykiwi2);
        Graphics_fillRectangle(&gfx_p->context, &mykiwi3);

        app_p->kiwiState++;
    }
    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->kiwiState == kiwi4){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &ekiwi3);

        app_p->kiwiState--;
    }

    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->kiwiState == kiwi3){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &ekiwi3);
        Graphics_fillRectangle(&gfx_p->context, &ekiwi2);
        app_p->kiwiState--;
    }

    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->kiwiState == kiwi2){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &ekiwi3);
        Graphics_fillRectangle(&gfx_p->context, &ekiwi2);
        Graphics_fillRectangle(&gfx_p->context, &ekiwi1);
        app_p->kiwiState--;

    }

}

void ingredient_select5(Application* app_p, HAL* hal_p, GFX* gfx_p){
    //ingredient cream is selected or deleted,
        //button s1 is for selecting ingredients
        //button s2 is to delete ingredients
    if (Button_isTapped(&hal_p->boosterpackS1) && app_p->mangoState == mango1){
        Graphics_fillRectangle(&gfx_p->context, &mymango1);
        app_p->mangoState++;
    }
    else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->mangoState == mango2){
        Graphics_fillRectangle(&gfx_p->context, &mymango1);
        Graphics_fillRectangle(&gfx_p->context, &mymango2);
        app_p->mangoState++;
    }

    else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->mangoState == mango3){
        Graphics_fillRectangle(&gfx_p->context, &mymango1);
        Graphics_fillRectangle(&gfx_p->context, &mymango2);
        Graphics_fillRectangle(&gfx_p->context, &mymango3);

        app_p->mangoState++;
    }
    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->mangoState == mango4){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &emango3);

        app_p->mangoState--;
    }

    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->mangoState == mango3){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &emango3);
        Graphics_fillRectangle(&gfx_p->context, &emango2);
        app_p->mangoState--;
    }

    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->mangoState == mango2){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &emango3);
        Graphics_fillRectangle(&gfx_p->context, &emango2);
        Graphics_fillRectangle(&gfx_p->context, &emango1);
        app_p->mangoState--;

    }

}


void ingredient_select6(Application* app_p, HAL* hal_p, GFX* gfx_p){
    //ingredient chocolate is selected or deleted,
        //button s1 is for selecting ingredients
        //button s2 is to delete ingredients
    if (Button_isTapped(&hal_p->boosterpackS1) && app_p->chocoState == choco1){
        Graphics_fillRectangle(&gfx_p->context, &mychoco1);
        app_p->chocoState++;
    }
    else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->chocoState == choco2){
        Graphics_fillRectangle(&gfx_p->context, &mychoco1);
        Graphics_fillRectangle(&gfx_p->context, &mychoco2);
        app_p->chocoState++;
    }

    else if (Button_isTapped(&hal_p->boosterpackS1) && app_p->chocoState == choco3){
        Graphics_fillRectangle(&gfx_p->context, &mychoco1);
        Graphics_fillRectangle(&gfx_p->context, &mychoco2);
        Graphics_fillRectangle(&gfx_p->context, &mychoco3);

        app_p->chocoState++;
    }
    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->chocoState == choco4){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &echoco3);

        app_p->chocoState--;
    }

    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->chocoState == choco3){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &echoco3);
        Graphics_fillRectangle(&gfx_p->context, &echoco2);
        app_p->chocoState--;
    }

    else if (Button_isTapped(&hal_p->boosterpackS2) && app_p->chocoState == choco2){
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&gfx_p->context, &echoco3);
        Graphics_fillRectangle(&gfx_p->context, &echoco2);
        Graphics_fillRectangle(&gfx_p->context, &echoco1);
        app_p->chocoState--;

    }

}

void reset_ingredient(Application* app_p, HAL* hal_p, GFX* gfx_p){
    //this function is to refresh everyhting in the game screen back to its initial status.
    //all rectangle colors must be reset back to its original color.
    delete_crust1(app_p, &hal_p->gfx);
    delete_crust2(app_p, &hal_p->gfx);
    delete_crust3(app_p, &hal_p->gfx);

    delete_cream3(app_p, &hal_p->gfx);
    delete_cream2(app_p, &hal_p->gfx);
    delete_cream1(app_p, &hal_p->gfx);

    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);

    Graphics_fillRectangle(&gfx_p->context, &mychoco1);
    Graphics_fillRectangle(&gfx_p->context, &mychoco2);
    Graphics_fillRectangle(&gfx_p->context, &mychoco3);

    Graphics_fillRectangle(&gfx_p->context, &eBerry3);
    Graphics_fillRectangle(&gfx_p->context, &eBerry2);
    Graphics_fillRectangle(&gfx_p->context, &eBerry1);

    Graphics_fillRectangle(&gfx_p->context, &ekiwi3);
    Graphics_fillRectangle(&gfx_p->context, &ekiwi2);
    Graphics_fillRectangle(&gfx_p->context, &ekiwi1);

    Graphics_fillRectangle(&gfx_p->context, &mymango1);
    Graphics_fillRectangle(&gfx_p->context, &mymango2);
    Graphics_fillRectangle(&gfx_p->context, &mymango3);

}


void menu_dish(Application* app_p, HAL* hal_p, GFX* gfx_p){

    //in this function, i prepared sic differetn resipes.
    switch (app_p-> dishState) {
    case dish1:
        //GFX_clear(&hal_p->gfx);
        GFX_print(&hal_p->gfx, "Chocolate Berry Cake!", 13, 0);
        GFX_print(&hal_p->gfx, "crust(1)  cream(2)", 14, 0);
        GFX_print(&hal_p->gfx, "choco(2)  berry(1)", 15, 0);
        app_p->matchState = match1;
        break;

    case dish2:
        //GFX_clear(&hal_p->gfx);
        GFX_print(&hal_p->gfx, "Berry Kiwi Cake!", 13, 0);
        GFX_print(&hal_p->gfx, "crust(1)  cream(3)", 14, 0);
        GFX_print(&hal_p->gfx, "berry(2)  kiwi(2)", 15, 0);
        app_p->matchState = match2;
        break;

    case dish3:
        //GFX_clear(&hal_p->gfx);
        GFX_print(&hal_p->gfx, "Chocolate Kiwi Cake!", 13, 0);
        GFX_print(&hal_p->gfx, "crust(2)  cream(2)", 14, 0);
        GFX_print(&hal_p->gfx, "choco(1)  kiwi(3)", 15, 0);
        app_p->matchState = match3;
        break;


    case dish4:
        //GFX_clear(&hal_p->gfx);
        GFX_print(&hal_p->gfx, "Mango Berry Cake!", 13, 0);
        GFX_print(&hal_p->gfx, "crust(1)  cream(2)", 14, 0);
        GFX_print(&hal_p->gfx, "mango(2)  berry(3)", 15, 0);
        app_p->matchState = match4;
        break;

    case dish5:
            //GFX_clear(&hal_p->gfx);
            GFX_print(&hal_p->gfx, "Chocolate Mango Cake!", 13, 0);
            GFX_print(&hal_p->gfx, "crust(1)  choco(2)", 14, 0);
            GFX_print(&hal_p->gfx, "mango(2)  berry(2)", 15, 0);
            app_p->matchState = match5;
            break;

    case dish6:
                //GFX_clear(&hal_p->gfx);
                GFX_print(&hal_p->gfx, "Cream Berry Cake!", 13, 0);
                GFX_print(&hal_p->gfx, "crust(1)  cream(3)", 14, 0);
                GFX_print(&hal_p->gfx, "choco(2)  berry(2)", 15, 0);
                app_p->matchState = match6;
                break;

    default:
        break;


    }

    match_dish(app_p, hal_p);

}


void match_dish(Application* app_p, HAL* hal_p){

    //in this function, i listed all recipes ingredients selection to match the recipes listed above.
    switch (app_p-> matchState){

    case match1:

        match_one(app_p, hal_p);//i made a void function in outside, the maincontents can be found below, the similar contents can be viewd in case three
        //some case contents are listed outside in order to decrease the line numbers in each function.

        break;


    case match2:
        match_two(app_p, hal_p);

        break;

    case match3:
        //in this case, the following are the right selections of this recipe. in this case, all codes are look like this in each case
        if ((app_p->crust = 2) == (int)app_p->crustState  && (app_p->cream = 2) == (int)app_p->creamState &&
                (app_p->kiwi = 3) == (int)app_p->kiwiState && (app_p->choco = 1) == (int)app_p->chocoState &&
                (app_p->mango = 0) == (int)app_p->mangoState && (app_p->berry = 0) == (int)app_p->berryState){
            if (Button_isTapped(&hal_p->boosterpackJS)){//////////////////////

                app_p->income = app_p->income + INCOME_GAIN;//if ingredients are selected correctly, the money will increase by 5
                app_p->time = app_p->time + INCOME_GAIN;//the ingredeints are right, then the time will increase by 5
                //after each recipe, the game screen must be reset
                reset_ingredient(app_p, hal_p, &hal_p->gfx);
                reset_uing(app_p, hal_p);

                random(app_p, hal_p);//after each recipe, the following recipe will be selected ramdomly, this function content will be showed later

            }

        }
        else if (Button_isTapped(&hal_p->boosterpackJS)){
            app_p->error++;
            reset_ingredient(app_p, hal_p, &hal_p->gfx);
            reset_uing(app_p, hal_p);

            random(app_p, hal_p);

        }

        break;


    case match4:
        match_four(app_p, hal_p);

        break;

    case match5:
        match_five(app_p, hal_p);

        break;



    case match6:
        match_six(app_p, hal_p);

        break;

    default:
        break;

    }

    error_print(app_p, hal_p, &hal_p->gfx);


    char buffer9[15];
                snprintf(buffer9, 15, "%02d", app_p->income);//print out the income as well as money value
                GFX_print(&hal_p->gfx, buffer9, 0, 2);

                add_timer(app_p, hal_p);//this function will be implemented later


}

void reset_uing(Application* app_p, HAL* hal_p){//reset all ingredients states
    app_p->crustState = crust1;
    app_p->creamState = select_ing1;
    app_p->chocoState = choco1;
    app_p->berryState = berry1;
    app_p->kiwiState = kiwi1;
    app_p->mangoState = mango1;
}

void error_print(Application* app_p, HAL* hal_p, GFX* gfx_p){

 ///print out strikes on game screen due to the number of incorrectly selected recipes.
    if (app_p->error == STRIKE_GAINA){

        GFX_print(&hal_p->gfx, "x", 0, 14);

    }

    if (app_p->error == STRIKE_GAINB){
        GFX_print(&hal_p->gfx, "xx", 0, 14);
    }

    if (app_p->error == STRIKE_GAINC){
        GFX_print(&hal_p->gfx, "xxx", 0, 14);
    }




}




void random(Application* app_p, HAL* hal_p){
    //this is the randome number function

    app_p->randomnumber= (int)hal_p->joystick.x + (int)hal_p->joystick.y;//use the joystick to generate a number
    app_p->remainder = app_p->randomnumber % divisor;//set a function for the number, and set the remainder as the randome number.
    //there will be 6 teams of remainders at last, corresponding to six recipes.
    /////////////////////////////////////////for menu1
    if (app_p->remainder == remaina){
        app_p->dishState = dish1;
    }

    if (app_p->remainder == remainb){
        app_p->dishState = dish2;
    }

    if (app_p->remainder == remainc){
        app_p->dishState = dish3;
    }

    if (app_p->remainder == remaind){
        app_p->dishState = dish4;
    }

    if (app_p->remainder == remaine){
        app_p->dishState = dish5;
    }

    if (app_p->remainder == remainf){
            app_p->dishState = dish6;
        }

}



void highscore(Application* app_p, HAL* hal_p){//recods three hightest scores
    if (app_p->income>0  && app_p->result == 1){
        app_p->sc1 = app_p->income;
    }
    ////////////////////////
    if (app_p->income > 0 && app_p->result == 2){
        if (app_p->income < app_p->sc1){
            app_p->sc2 = app_p->income;
        }
        else if (app_p->income >= app_p->sc1){
            app_p->sc2 = app_p->sc1;
            app_p->sc1 = app_p->income;
        }
    }

    //////////////////////////////////////////////
    if (app_p->income > 0 && app_p->result >= 3){
        if (app_p->income >= app_p->sc1){
            app_p->sc3 = app_p->sc2;
            app_p->sc2 = app_p->sc1;
            app_p->sc1 = app_p->income;

        }
        else if (app_p->income < app_p->sc1 && app_p->income >= app_p->sc2){
            app_p->sc3 = app_p->sc2;
            app_p->sc2 = app_p->income;
        }
        else if (app_p->income < app_p->sc2 && app_p->income >= app_p->sc3){
            app_p->sc3 = app_p->income;
        }

        else if (app_p->income < app_p->sc3){
            app_p->sc4 = app_p->income;
        }
    }





}


void add_timer(Application* app_p, HAL* hal_p){
    if ((app_p->time == 0 && app_p->income > 0) || (app_p->income > 0 && app_p->error == 3)){//if there are income, the game win

            app_p->result++;
            app_p->state = WIN_SCREEN;
            Application_winScreen(app_p, hal_p, &hal_p->gfx);
        }
        else if ((app_p->income == 0 && app_p->time == 0) || (app_p->income == 0 && app_p->error == 3)){// no income or time runs out or all strikes, the game lose,

            app_p->result++;
            app_p->state = LOSE_SCREEN;
            Application_loseScreen(app_p, &hal_p->gfx);
        }
}

void match_one(Application* app_p, HAL* hal_p){
    if ((app_p->crust = 1) == (int)app_p->crustState  && (app_p->cream = 2) == (int)app_p->creamState &&
                   (app_p->choco = 2) == (int)app_p->chocoState && (app_p->berry = 1) == (int)app_p->berryState &&
                   (app_p->kiwi = 0) == (int)app_p->kiwiState && (app_p->mango = 0) == (int)app_p->mangoState){
               if (Button_isTapped(&hal_p->boosterpackJS)){
                   app_p->income = app_p->income + INCOME_GAIN;
                   app_p->time = app_p->time + INCOME_GAIN;
                   reset_ingredient(app_p, hal_p, &hal_p->gfx);
                   reset_uing(app_p, hal_p);
                   random(app_p, hal_p);

               }

           }
           else if (Button_isTapped(&hal_p->boosterpackJS)){
               app_p->error = app_p->error+1;
               reset_ingredient(app_p, hal_p, &hal_p->gfx);
               reset_uing(app_p, hal_p);
               random(app_p, hal_p);

           }
}



void match_two(Application* app_p, HAL* hal_p){
    if ((app_p->crust = 1) == (int)app_p->crustState  && (app_p->cream = 3) == (int)app_p->creamState &&
                   (app_p->kiwi = 2) == (int)app_p->kiwiState && (app_p->berry = 2) == (int)app_p->berryState &&
                   (app_p->choco = 0) == (int)app_p->chocoState && (app_p->mango = 0) == (int)app_p->mangoState){
               if (Button_isTapped(&hal_p->boosterpackJS)){

                   app_p->income = app_p->income + INCOME_GAIN;
                   app_p->time = app_p->time + INCOME_GAIN;
                   reset_ingredient(app_p, hal_p, &hal_p->gfx);
                   reset_uing(app_p, hal_p);

                   random(app_p, hal_p);

               }

           }
           else if (Button_isTapped(&hal_p->boosterpackJS)){
               app_p->error++;
               reset_ingredient(app_p, hal_p, &hal_p->gfx);
               reset_uing(app_p, hal_p);

               random(app_p, hal_p);

           }
}




void match_four(Application* app_p, HAL* hal_p){
    if ((app_p->crust = 1) == (int)app_p->crustState  && (app_p->cream = 2) == (int)app_p->creamState &&
                   (app_p->mango = 2) == (int)app_p->mangoState && (app_p->berry = 3) == (int)app_p->berryState &&
                   (app_p->choco = 0) == (int)app_p->chocoState && (app_p->kiwi = 0) == (int)app_p->kiwiState){
               if (Button_isTapped(&hal_p->boosterpackJS)){

                   app_p->income = app_p->income + INCOME_GAIN;
                   app_p->time = app_p->time + INCOME_GAIN;
                   reset_ingredient(app_p, hal_p, &hal_p->gfx);
                   reset_uing(app_p, hal_p);
                   random(app_p, hal_p);

               }

           }
           else if (Button_isTapped(&hal_p->boosterpackJS)){

               app_p->error++;
               reset_ingredient(app_p, hal_p, &hal_p->gfx);
               reset_uing(app_p, hal_p);
               random(app_p, hal_p);

           }

}


void match_five(Application* app_p, HAL* hal_p){
    if ((app_p->crust = 1) == (int)app_p->crustState  && (app_p->cream = 0) == (int)app_p->creamState &&
                   (app_p->mango = 2) == (int)app_p->mangoState && (app_p->choco = 2) == (int)app_p->chocoState &&
                   (app_p->berry = 2) == (int)app_p->berryState && (app_p->kiwi = 0) == (int)app_p->kiwiState){
               if (Button_isTapped(&hal_p->boosterpackJS)){

                   app_p->income = app_p->income + INCOME_GAIN;
                   app_p->time = app_p->time + INCOME_GAIN;
                   reset_ingredient(app_p, hal_p, &hal_p->gfx);
                   reset_uing(app_p, hal_p);
                   random(app_p, hal_p);

               }

           }
           else if (Button_isTapped(&hal_p->boosterpackJS)){

               app_p->error++;
               reset_ingredient(app_p, hal_p, &hal_p->gfx);
               reset_uing(app_p, hal_p);
               random(app_p, hal_p);

           }

}


void match_six(Application* app_p, HAL* hal_p){
    if ((app_p->crust = 1) == (int)app_p->crustState  && (app_p->cream = 3) == (int)app_p->creamState &&
                   (app_p->mango = 0) == (int)app_p->mangoState && (app_p->choco = 2) == (int)app_p->chocoState &&
                   (app_p->berry = 2) == (int)app_p->berryState && (app_p->kiwi = 0) == (int)app_p->kiwiState){
               if (Button_isTapped(&hal_p->boosterpackJS)){

                   app_p->income = app_p->income + INCOME_GAIN;
                   app_p->time = app_p->time + INCOME_GAIN;
                   reset_ingredient(app_p, hal_p, &hal_p->gfx);
                   reset_uing(app_p, hal_p);
                   random(app_p, hal_p);

               }

           }
           else if (Button_isTapped(&hal_p->boosterpackJS)){

               app_p->error++;
               reset_ingredient(app_p, hal_p, &hal_p->gfx);
               reset_uing(app_p, hal_p);
               random(app_p, hal_p);

           }

}

