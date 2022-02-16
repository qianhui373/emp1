
#ifndef APPLICATION_H_
#define APPLICATION_H_



#include <HAL/HAL.h>
//////
#include <HAL/Graphics.h>
#include <HAL/Timer.h>
///////

///////////////////////////////////////////////////////////////////////////////
#define TITLE_SCREEN_WAIT   3000  //3 seconds
#define RESULT_SCREEN_WAIT  3000  // 3 seconds
#define BACK_TO_BEGIN 1000//1 second
#define TIME_START 30//start time as 30


//define the following remainder's value
#define remaina 1
#define remainb 2
#define remainc 3
#define remaind 4
#define remaine 5
#define remainf 6

//define divisor
#define divisor 12



#define NUM_TEST_OPTIONS    3
#define NUM_RANDOM_NUMBERS  4

#define INCOME_GAIN 5

//define the number of strikes
#define STRIKE_GAINA 1
#define STRIKE_GAINB 2
#define STRIKE_GAINC 3




enum _GameState
{
    TITLE_SCREEN, MENU_SCREEN, HOWTOPLAY_SCREEN, HIGHSCORES_SCREEN, GAME_SCREEN, WIN_SCREEN, LOSE_SCREEN
};
typedef enum _GameState GameState;

enum _Cursor//the three cursors in menu screen
{
    CURSOR_0 = 0, CURSOR_1 = 1, CURSOR_2 = 2
};
typedef enum _Cursor Cursor;

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////
enum _creamState//ingredient cream states
{
    select_ing1 = 0, select_ing2 = 1, select_ing3 = 2, select_ing4 = 3
};
typedef enum _creamState creamState;

enum _crustState//ingredeint crust states
{
    crust1 = 0, crust2 = 1, crust3 = 2, crust4 = 3
};
typedef enum _crustState crustState;

enum _berryState//strawberry states
{
    berry1 = 0, berry2 = 1, berry3 = 2, berry4 = 3
};
typedef enum _berryState berryState;

enum _kiwiState//kiwi states
{
    kiwi1 = 0, kiwi2 = 1, kiwi3 = 2, kiwi4 = 3
};
typedef enum _kiwiState kiwiState;

enum _mangoState//mango states
{
    mango1 = 0, mango2 = 1, mango3 = 2, mango4 = 3
};
typedef enum _mangoState mangoState;

enum _chocoState//chocolate states
{
    choco1 = 0, choco2 = 1, choco3 = 2, choco4 = 3
};
typedef enum _chocoState chocoState;




enum _dishState//the six recipes.
{
    dish1, dish2, dish3, dish4, dish5, dish6
};
typedef enum _dishState dishState;

enum _matchState//select ingredients match each recipes.
{
    match1, match2, match3, match4, match5, match6
};
typedef enum _matchState matchState;

//enum _errorState
//{
//    error1 = 0, error2 = 1, error3 = 2, error4 = 3
//};
//typedef enum _errorState errorState;

struct _Application
{
 // Put your application members and FSM state variables here!
    // =========================================================================
    UART_Baudrate baudChoice;
    bool firstCall;
    bool consumeEnergy;
    bool original;


    //////////////////////////
    GameState state;  // Determines which screen is currently shown
        SWTimer timer;    // General-purpose timer for when screens must disappear
        SWTimer countTimer;

        ////////////////define all macros.
      int time;

      int error;
      int income;
      int crust;
      int cream;
      int berry;
      int kiwi;
      int mango;
      int choco;
      int randomnumber;
      int remainder;

      int sc1;
      int sc2;
      int sc3;
      int sc4;

      int result;

     creamState creamState;
     crustState crustState;
     berryState berryState;
     kiwiState kiwiState;
     mangoState mangoState;
     chocoState chocoState;

     dishState dishState;
     matchState matchState;

     //errorState errorState;
        // Booleans to determine which colors the user has picked
        bool caketime;
        bool highscores;
        bool howtocook;

        // Cursor tracker variable
        Cursor cursor;
        //////////////////////////////////////////////////////////////////////////////
        int randomNumbers[NUM_RANDOM_NUMBERS];
            int randomNumberChoice;
};


typedef struct _Application Application;

// Called only a single time - inside of main(), where the application is constructed

// Called once per super-loop of the main application.


////////////////////////////////////////////////////////
Application Application_construct(HAL* hal_p);
void Application_loop(Application* app_p, HAL* hal_p, GFX* gfx_p);


void Application_handleTitleScreen(Application* app_p, HAL* hal_p);
void Application_handleMenuScreen(Application* app_p, HAL* hal_p);
void Application_handleGameScreen(Application* app_p, HAL* hal_p);
void Application_handlewinScreen(Application* app_p, HAL* hal_p);
void Application_handleloseScreen(Application* app_p, HAL* hal_p);

// Helper functions which clear the screen and draw the text for each state
void Application_showTitleScreen(GFX* gfx_p);
void Application_showMenuScreen(Application* app_p, GFX* gfx_p);
void Application_showGameScreen(Application* app_, GFX* gfx_p);
void Application_winScreen(Application* app_p, HAL* hal_p, GFX* gfx_p);
void Application_loseScreen(Application* app_p, GFX* gfx_p);



void Application_updateGameScreen(Application* app_p, GFX* gfx_p);

// Used to reset the internal game variables
void Application_initGameVariables(Application* app_p, HAL* hal_p);

////////////////////////////////the new functions i set
void Application_handleHowToPlayScreen(Application* app_p, HAL* hal_p);
void Application_handleHighScoresScreen(Application* app_p, HAL* hal_p);

void Application_showHowToPlayScreen(Application* app_p, GFX* gfx_p);
void Application_showHighScoresScreen(Application* app_, GFX* gfx_p);

void Application_updateMenuScreen(Application* app_p, GFX* gfx_p);

// Called whenever the UART module needs to be updated
void Application_updateCommunications(Application* app_p, HAL* hal_p);

// Interprets an incoming character and echoes back to terminal what kind of
// character was received (number, letter, or other)
void Rectangle_movel(Application* app_p, GFX* gfx_p);//joystick move left, the rectangle turn blue,
void Rectangle_movell(Application* app_p, GFX* gfx_p);//joystick come back, the rectangle turn white, the original one turn blue
void Rectangle_mover(Application* app_p, GFX* gfx_p);//joystick move left, the rectangle turn blue,
void Rectangle_moverr(Application* app_p, GFX* gfx_p);//joystick come back, the rectangle turn white, the original one turn blue
void Rectangle_moveu(Application* app_p, GFX* gfx_p);//joystick move left, the rectangle turn blue,
void Rectangle_moveuu(Application* app_p, GFX* gfx_p);//joystick come back, the rectangle turn white, the original one turn blue
void move_upleft(Application* app_p, GFX* gfx_p);//joystick move left, the rectangle turn blue,
void move_upleftt(Application* app_p, GFX* gfx_p);//joystick come back, the rectangle turn white, the original one turn blue
void move_upright(Application* app_p, GFX* gfx_p);//joystick move left, the rectangle turn blue,
void move_uprightt(Application* app_p, GFX* gfx_p);//joystick come back, the rectangle turn white, the original one turn blue


void basic_one(Application* app_p, GFX* gfx_p);
void basic_two(Application* app_p, GFX* gfx_p);
//////////////////////////////////////////////////////////////////////////// select cream
void select_cream1(Application* app_p, GFX* gfx_p);
void select_cream2(Application* app_p, GFX* gfx_p);
void select_cream3(Application* app_p, GFX* gfx_p);
void delete_cream1(Application* app_p, GFX* gfx_p);
void delete_cream2(Application* app_p, GFX* gfx_p);
void delete_cream3(Application* app_p, GFX* gfx_p);

/////////////////////////////////////////////////////////////////////////select crust
void select_crust1(Application* app_p, GFX* gfx_p);
void select_crust2(Application* app_p, GFX* gfx_p);
void select_crust3(Application* app_p, GFX* gfx_p);
void delete_crust1(Application* app_p, GFX* gfx_p);
void delete_crust2(Application* app_p, GFX* gfx_p);
void delete_crust3(Application* app_p, GFX* gfx_p);

//////////////////////////////////////////////////////////////////////////select berry


//////////////the listed below are the small rectangles listed below each ingredients in the game screen

const Graphics_Rectangle myBerry1 = {1, 92, 14, 99};
const Graphics_Rectangle myBerry2 = {16, 92, 28, 99};
const Graphics_Rectangle myBerry3 = {30, 92, 42, 99};
const Graphics_Rectangle eBerry1 = {1, 92, 14, 99};
const Graphics_Rectangle eBerry2 = {16, 92, 28, 99};
const Graphics_Rectangle eBerry3 = {30, 92, 42, 99};


const Graphics_Rectangle mykiwi1 = {1, 46, 14, 51};
const Graphics_Rectangle mykiwi2 = {16, 46, 28, 51};
const Graphics_Rectangle mykiwi3 = {30, 46, 41, 51};
const Graphics_Rectangle ekiwi1 = {1, 46, 14, 51};
const Graphics_Rectangle ekiwi2 = {16, 46, 28, 51};
const Graphics_Rectangle ekiwi3 = {30, 46, 41, 51};


const Graphics_Rectangle mymango1 = {46, 46, 58, 51};
const Graphics_Rectangle mymango2 = {60, 46, 73, 51};
const Graphics_Rectangle mymango3 = {75, 46, 85, 51};
const Graphics_Rectangle emango1 = {46, 46, 58, 51};
const Graphics_Rectangle emango2 = {60, 46, 73, 51};
const Graphics_Rectangle emango3 = {75, 46, 85, 51};


const Graphics_Rectangle mychoco1 = {88, 46, 99, 51};
const Graphics_Rectangle mychoco2 = {101, 46, 113, 51};
const Graphics_Rectangle mychoco3 = {115, 46, 124, 51};
const Graphics_Rectangle echoco1 = {88, 46, 99, 51};
const Graphics_Rectangle echoco2 = {101, 46, 113, 51};
const Graphics_Rectangle echoco3 = {115, 46, 124, 51};



////////////////////////six ingredients selection
void ingredient_select1(Application* app_p, HAL* hal_p);
void ingredient_select2(Application* app_p, HAL* hal_p);
void ingredient_select3(Application* app_p, HAL* hal_p, GFX* gfx_p);
void ingredient_select4(Application* app_p, HAL* hal_p, GFX* gfx_p);
void ingredient_select5(Application* app_p, HAL* hal_p, GFX* gfx_p);
void ingredient_select6(Application* app_p, HAL* hal_p, GFX* gfx_p);

//////////////////////////////////////////////////////////////////////define all function implemented in the main.c

void menu_dish(Application* app_p, HAL* hal_p, GFX* gfx_p);
void match_dish(Application* app_p, HAL* hal_p);



void reset_ingredient(Application* app_p, HAL* hal_p, GFX* gfx_p);



void reset_uing(Application* app_p, HAL* hal_p);



void error_print(Application* app_p, HAL* hal_p, GFX* gfx_p);



void random(Application* app_p, HAL* hal_p);

void highscore(Application* app_p, HAL* hal_p);

void add_timer(Application* app_p, HAL* hal_p);

void match_one(Application* app_p, HAL* hal_p);

void match_two(Application* app_p, HAL* hal_p);

void match_four(Application* app_p, HAL* hal_p);

void match_five(Application* app_p, HAL* hal_p);

void match_six(Application* app_p, HAL* hal_p);

// Generic circular increment function
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);


#endif /* APPLICATION_H_ */



