//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include "Enemies.h"
#include <list>
#include "derecha.h"
#include "abajo.h"
#include "arriba.h"
#include <sstream>
#include <fstream>
#include "SDL/SDL_ttf.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <list>
#include <algorithm>
#include <vector>
#include "BOSS.h"

using namespace std;
//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;


int FRAMES_PER_SECOND =60 ;

//The surfaces
SDL_Surface *buttonSheet = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *seconds = NULL;
//The event structure
SDL_Surface *background = NULL;
SDL_Surface *hurt = NULL;
SDL_Event event;
stringstream stream;
SDL_Surface *score = NULL;


TTF_Font *font = NULL;

bool fullscreen = false;
bool shoted = false;
bool shoted2 = false;
int frameshot = 0;
int frameshot2 = 0;

//The surfaces
SDL_Surface *background1 = NULL;
SDL_Surface* shots[4];
SDL_Surface *stretch = NULL;
SDL_Surface* logos = NULL;
SDL_Surface* slides[3];
SDL_Surface* press = NULL;
SDL_Surface* play1 = NULL;
SDL_Surface* play2 = NULL;
SDL_Surface* options = NULL;
SDL_Surface* salir = NULL;
SDL_Surface* guntip = NULL;

//The event structure

//The font
TTF_Font *fuente = NULL;
TTF_Font *big = NULL;
SDL_Surface* delay = NULL;




//The color of the font
SDL_Color textColorc = { 255, 255, 255 };



int x = 0, y = 0;

//The clip regions of the sprite sheet
SDL_Color textColor = { 0, 0, 0 };
class Timer
{
private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};
Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}
//The button
class Button
{


private:
    //The attributes of the button
    SDL_Rect box;

    //The part of the button sprite sheet that will be shown
    SDL_Rect* clip;

public:
    //Initialize the variables
    int x;
    int y;
    Button( int x, int y, int w, int h);

    //Handles events and set the button's sprite region
    void handle_events();

    //Shows the button on the screen
    void show();
};

SDL_Surface *load_image( std::string filename )
{

    //Return the optimized surface
    return IMG_Load( filename.c_str() );

}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen

    // For windowed
    screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Button Test", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the button sprite sheet background1 = load_image( "background1.png" );


    shots[0] = load_image("shots0.png");
    shots[1] = load_image("shots1.png");
    shots[2] = load_image("shots2.png");
    shots[3] = load_image("shots3.png");
    stretch = load_image("strechy.png");
    delay = load_image("explode.png");
    slides[0] = load_image("slide0.png");
    slides[1] = load_image("slide1.png");
    slides[2] = load_image("slide2.png");
    guntip = load_image("tipweapon.png");
    fuente = TTF_OpenFont( "nano.ttf", 25 );;
    background1 = load_image("background1.png");
    //Open the font
    font = TTF_OpenFont( "fast_money.ttf", 19 );

    buttonSheet = load_image( "button.png" );
    background = load_image("background.png");
    hurt = load_image("hurt.png");
    //If there was a problem in loading the button sprite sheet
    if( buttonSheet == NULL )
    {
        return false;
    }
    if(font ==NULL)
        return false;

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( buttonSheet );
    SDL_FreeSurface(background);
    SDL_FreeSurface(background1);
    TTF_CloseFont( font );
    TTF_CloseFont( fuente );
    //Quit SDL_ttf
    TTF_Quit();

    Mix_CloseAudio();
    //Quit SDL
    SDL_Quit();
}



Button::Button( int x, int y,int w, int h)
{
    box.x =x;
    box.y = y;
    box.w = w;
    box.h = h;
}

void Button::handle_events()
{
    //The mouse offsets

    //If the mouse moved
    if( event.type == SDL_MOUSEMOTION )
    {
        //Get the mouse offsets
        x = event.motion.x;
        y = event.motion.y;

        box.x = x-18;
        box.y = y-18;

    }
    //If a mouse button was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;


            //If the mouse is over the button


        }
    }

    //If a mouse button was released
    if( event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( y > box.y ) )
            {
                //Set the button sprite

            }
        }
    }
}
void juego();
void pause()
{

    bool quit = false;
    bool cap = true;
    Timer fps;
    SDL_Surface* paus = load_image("pause.png");

    apply_surface(0, 0, paus, screen);
    while( quit == false )
    {
        cap = (!cap);
        fps.start();



        while( SDL_PollEvent( &event ) )
        {
            if(event.type == SDL_KEYDOWN )
            {
                if(event.key.keysym.sym == SDLK_RETURN)
                {
                    return;
                }
            }
        }

        if(SDL_Flip(screen) == -1)
        {

            return;
        }
        ;

        if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }

    }
}

void juego()
{

    int gastar = 0;
    bool nueva_carga = false;
    int reload = 33;
    SDL_Surface *bullet[32];
    int framegastar = 0;
    bullet[0] = load_image("bullet0.png");
    bullet[1] = load_image("bullet1.png");
    bullet[2] = load_image("bullet2.png");
    bullet[3] = load_image("bullet3.png");
    bullet[4] = load_image("bullet4.png");
    bullet[5] = load_image("bullet5.png");
    bullet[6] = load_image("bullet6.png");
    bullet[7] = load_image("bullet7.png");
    bullet[8] = load_image("bullet8.png");
    bullet[9] = load_image("bullet9.png");
    bullet[10] = load_image("bullet10.png");
    bullet[11] = load_image("bullet11.png");
    bullet[12] = load_image("bullet12.png");
    bullet[13] = load_image("bullet13.png");
    bullet[14] = load_image("bullet14.png");
    bullet[15] = load_image("bullet15.png");
    bullet[16] = load_image("bullet16.png");
    bullet[17] = load_image("bullet17.png");
    bullet[18] = load_image("bullet18.png");
    bullet[19] = load_image("bullet19.png");
    bullet[20] = load_image("bullet20.png");
    bullet[21] = load_image("bullet21.png");
    bullet[22] = load_image("bullet22.png");
    bullet[23] = load_image("bullet23.png");
    bullet[24] = load_image("bullet24.png");
    bullet[25] = load_image("bullet25.png");
    bullet[26] = load_image("bullet26.png");
    bullet[27] = load_image("bullet27.png");
    bullet[28] = load_image("bullet28.png");
    bullet[29] = load_image("bullet29.png");
    bullet[30] = load_image("bullet30.png");
    bullet[31] = load_image("bullet31.png");

    int animgastar1 = 0;
    int animgastar2 = 3;
    int animgastar3 = 6;
    int animgastar4 = 9;
    int animgastar5 = 12;
    int animgastar6 = 15;
    int animgastar7 = 18;
    int animgastar8 = 21;
    int animgastar9 = 25;
    int animgastar10 = 28;
    int animgastar11 = 31;

    SDL_ShowCursor(0);
    SDL_Surface*enemy[5];
    enemy[0]= load_image("running0.png");
    enemy[1]= load_image("running1.png");
    enemy[2]=load_image("enemyhit0.png");
    enemy[3]=load_image("enemyhit1.png");
    enemy[4]=load_image("dead0.png");


    SDL_Surface *en[2];
    en[0]= load_image("running2.png");
    en[1]= load_image("running3.png");

    SDL_Surface *shoot[4];
    shoot[0]= load_image("enemies0.png");
    shoot[1]= load_image("enemies1.png");
    shoot[2]= load_image("enemies2.png");
    shoot[3]= load_image("enemies3.png");

    SDL_Surface * Bt[8];
    Bt[0] = load_image("transport0.png");
    Bt[1] = load_image("transport1.png");
    Bt[2] = load_image("transport2.png");
    Bt[3] = load_image("transport3.png");
    Bt[4] = load_image("transport4.png");
    Bt[5] = load_image("transport5.png");
    Bt[6] = load_image("transport6.png");
    Bt[7] = load_image("transport7.png");

    SDL_Surface * Bsh[7];
    Bsh[0] = load_image("bosshot0.png");
    Bsh[1] = load_image("bosshot1.png");
    Bsh[2] = load_image("bosshot2.png");
    Bsh[3] = load_image("bosshot3.png");
    Bsh[4] = load_image("bosshot4.png");
    Bsh[5] = load_image("bosshot5.png");
    Bsh[6] = load_image("bosshot6.png");
    //Quit flag
    bool quit = false;
    srand(time(NULL));
    //Make the button
    Button myButton(120,120,277,229);
    int frames=0;
    list<Enemies*>enem;
    bool izq= true;
    bool der = true;
    bool paused = false;
    Uint32 startin =0;
    Timer fps;
    int shotanim = 0;
    int boss = false;
    int m=0;
    int sh =0;
    int n=0;
    int d=0;
    int iz=0;
    int transport =0;
    int bs =0;
    bool bossy= false;
    Timer myTimer;
    myTimer.start();
    bool flagizq = false;
    bool flagder = false;
    int s=0;

    enem.push_back((new BOSS(SCREEN_WIDTH/2,240,screen,Bt[0])));
    while( quit == false )
    {
        paused = true;
        fps.start();

        if(frames%377 == 0 && izq)
        {
            if(!flagizq)
                enem.push_back(new Enemies(0,360,screen,enemy[0]));
            else
                enem.push_back(new Enemies(0,350,screen,enemy[0]));
            flagizq=!flagizq;
            iz++;
            if(iz==2)
            {

                izq=false;

            }

        }
        if(frames%677==0 && der)
        {
            if(!flagder)
                enem.push_back(new derecha(560,340,screen,en[0]));
            else
                enem.push_back(new derecha(560,330,screen,en[0]));
            flagder =!flagder;
            d++;
            if(!boss)
            {
                if(d==2 )
                    der=false;
            }
            else if(boss)
            {
                if(d==1 )
                    der=false;
            }
        }


        int clickx =-1;
        int clicky= -1;
        while( SDL_PollEvent( &event ) )
        {
            //Handle button events
            myButton.handle_events();

            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                    if (gastar <= 10){
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    clickx = event.button.x;
                    clicky = event.button.y;
                    shoted = true;
                    frameshot = 0;
                    gastar++;
                        animgastar1 = 0;
                        animgastar2 = 3;
                        animgastar3 = 6;
                        animgastar4 = 9;
                        animgastar5 = 12;
                        animgastar6 = 15;
                        animgastar7 = 18;
                        animgastar8 = 21;
                        animgastar9 = 24;
                        animgastar10 = 27;
                        animgastar11 = 30;
                        framegastar = 0;
                        if (gastar > 32)
                            gastar = 32;

                }
                }
                if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    nueva_carga = true;
                }


            }

            if(event.type == SDL_KEYDOWN )
            {
                if(event.key.keysym.sym == SDLK_RETURN)
                {
                    myTimer.pause();
                    pause();
                    myTimer.unpause();
                }
            }


            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }

        }
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
        apply_surface(0,0,background,screen);
        list<Enemies*>::iterator i=enem.begin();
        while(i!=enem.end())
        {

            if((*i)->borrar)
            {
                shoted2 = true;
                s+=100;
                if((*i)->tipo=="n")
                {
                    iz--;
                    if(iz==0)
                        izq= true;

                }
                if((*i)->tipo=="d")
                {
                    d--;
                    if(d==0)
                        der= true;


                }

                (*i)->dib(enemy[4]);
                (*i)->where= true;
                Enemies*temp = (*i);
                i = enem.erase(i);
                delete temp;

            }
            else
            {

                if((*i)->tipo == "b" && bossy )
                {
                    (*i)->dib(Bt[transport]);
                }
                else
                {
                    if((*i)->tipo == "b" && boss)
                    {
                        (*i)->dib(Bsh[bs]);
                        if((*i)->morir==15)
                        {
                            (*i)->dib(Bt[7]);
                            (*i)->dib(Bt[6]);
                            (*i)->dib(Bt[5]);
                            (*i)->dib(Bt[4]);
                            (*i)->dib(Bt[3]);
                            (*i)->dib(Bt[2]);
                            (*i)->dib(Bt[1]);
                            (*i)->dib(Bt[0]);
                            (*i)->mover(true);
                            bossy = true;

                        }



                    }
                }


                if((*i)->tipo=="n")
                {
                    if((*i)->where && (*i)->x<=560 )
                    {
                        if((*i)->x==560)
                            (*i)->where = false;
                    }
                    if((*i)->x==0 )
                    {
                        (*i)->where = true;

                    }
                    if(!(*i)->shooting)
                        (*i)->mover((*i)->where);
                    if((*i)->shooting)
                    {
                        (*i)->dib(shoot[sh]);
                        if(sh==3)
                            //apply_surface(0,0,hurt,screen);
                            if((*i)->morir ==1 && (*i)->parp)
                            {
                                (*i)->dib(load_image("enemyhit1.png"));
                                (*i)-> parp =false;
                            }
                    }
                    else if((*i)->where)
                    {
                        (*i)->dib(enemy[m]);
                        if((*i)->morir ==1 && (*i)->parp)
                        {
                            (*i)->dib(load_image("enemyhit0.png"));
                            (*i)-> parp =false;
                        }
                    }
                    else
                    {
                        (*i)->dib(en[m]);
                        if((*i)->morir ==1 && (*i)->parp)
                        {
                            (*i)->dib(load_image("enemyhit3.png"));
                            (*i)-> parp =false;
                        }
                    }

                }
                else if((*i)->tipo == "d")
                {
                    if((*i)->where && (*i)->x>=0 )
                    {
                        if((*i)->x==0)
                            (*i)->where = false;
                    }
                    if((*i)->x>560 )
                    {
                        (*i)->where = true;
                    }
                    if(!(*i)->shooting)
                        (*i)->mover((*i)->where);
                    if((*i)->shooting)
                    {
                        (*i)->dib(shoot[sh]);
                        if(sh==3)
                            // apply_surface(0,0,hurt,screen);

                            if((*i)->morir ==1 && (*i)->parp)
                            {
                                (*i)->dib(load_image("enemyhit2.png"));
                                (*i)-> parp =false;
                            }


                    }
                    else if((*i)->where)
                    {
                        (*i)->dib(en[n]);
                        if((*i)->morir ==1 && (*i)->parp)
                        {
                            (*i)->dib(load_image("enemyhit3.png"));
                            (*i)-> parp =false;
                        }
                    }
                    else
                    {
                        (*i)->dib(enemy[n]);
                        if((*i)->morir ==1 && (*i)->parp)
                        {
                            (*i)->dib(load_image("enemyhit0.png"));
                            (*i)-> parp =false;
                        }
                    }



                }

                if(frames>5)
                {
                    if(frames%255==0 && (*i)->tipo =="n")
                        (*i)->shooting =true;
                    else if(frames%355==0 && (*i)->tipo =="d")
                        (*i)->shooting =true;
                    (*i)->logica(clickx,clicky);

                }

            }

            i++;
        }



        //Show the button
        myButton.show();
        if(frames%15==0)
        {
            m++;
            sh++;
            n++;
            bs++;
            if(bossy)
                transport++;
            if(m>1)
                m=0;
            if(sh>3)
                sh=0;
            if(n>1)
                n=0;

            if(transport >7 )
            {
                bossy =false;
                transport=0;
            }
            if(bs>6)
                bs=0;
        }

        if(frames%30)
        {
            if(shotanim < 3)
            {

                shotanim++;
            }

        }
        //Update the screen


        frames++;
        std::stringstream puntaje;
        puntaje<<"score : "<<s;
        score= TTF_RenderText_Solid( fuente,puntaje.str().c_str(), textColor );
        apply_surface(  0, 0, score, screen );
        if(!boss)
        {
            std::stringstream time;
            time << "Timer: " <<60 - ((myTimer.get_ticks()-startin)/1000);
            seconds = TTF_RenderText_Solid( fuente, time.str().c_str(), textColor );
            apply_surface( ( SCREEN_WIDTH - seconds->w ) / 2, 0, seconds, screen );

        }
         if (nueva_carga)
        {
            animgastar1 = 0;
            animgastar2 = 3;
            animgastar3 = 6;
            animgastar4 = 9;
            animgastar5 = 12;
            animgastar6 = 15;
            animgastar7 = 18;
            animgastar8 = 21;
            animgastar9 = 24;
            animgastar10 = 27;
            animgastar11 = 30;
            framegastar = 0;
            gastar = 0;
            apply_surface(530,0, bullet[0], screen);
            nueva_carga=false;
        }


        if (gastar == 0)
        {
            apply_surface(530,0, bullet[0], screen);
        }

        if (gastar == 1 )
        {

            apply_surface(530,0,bullet[animgastar1],screen);
            if (framegastar % 5 == 0)
                animgastar1++;
            framegastar++;

            if(animgastar1 > 3)
            {
                animgastar1 = 3;
            }

        }
        if (gastar == 2)
        {
            apply_surface(530,0,bullet[animgastar2],screen);
            if (framegastar % 5 == 0)
                animgastar2++;
            framegastar++;

            if(animgastar2 > 6)
            {
                animgastar2 = 6;
            }

        }
        if (gastar == 3)
        {
            apply_surface(530,0,bullet[animgastar3],screen);
            if (framegastar % 5 == 0)
                animgastar3++;
            framegastar++;

            if(animgastar3 > 9)
            {
                animgastar3 = 9;
            }

        }
        if (gastar == 4)
        {
            apply_surface(530,0,bullet[animgastar4],screen);
            if (framegastar % 5 == 0)
                animgastar4++;
            framegastar++;

            if(animgastar4 > 12)
            {
                animgastar4 = 12;
            }

        }
        if (gastar == 5)
        {
            apply_surface(530,0,bullet[animgastar5],screen);
            if (framegastar % 5 == 0)
                animgastar5++;
            framegastar++;

            if(animgastar5 > 15)
            {
                animgastar5 = 15;
            }

        }
        if (gastar == 6)
        {
            apply_surface(530,0,bullet[animgastar6],screen);
            if (framegastar % 5 == 0)
                animgastar6++;
            framegastar++;

            if(animgastar6 > 18)
            {
                animgastar6 = 18;
            }

        }
        if (gastar == 7)
        {
            apply_surface(530,0,bullet[animgastar7],screen);
            if (framegastar % 5 == 0)
                animgastar7++;
            framegastar++;

            if(animgastar7 > 21)
            {
                animgastar7 = 21;
            }

        }
        if (gastar == 8)
        {
            apply_surface(530,0,bullet[animgastar8],screen);
            if (framegastar % 5 == 0)
                animgastar8++;
            framegastar++;

            if(animgastar8 > 24)
            {
                animgastar8 = 24;
            }

        }
        if (gastar == 9)
        {
            apply_surface(530,0,bullet[animgastar9],screen);
            if (framegastar % 5 == 0)
                animgastar9++;
            framegastar++;

            if(animgastar9 > 27)
            {
                animgastar9 = 27;
            }

        }
        if (gastar == 10)
        {
            apply_surface(530,0,bullet[animgastar10],screen);
            if (framegastar % 5 == 0)
                animgastar10++;
            framegastar++;

            if(animgastar10 > 30)
            {
                animgastar10 = 30;
            }

        }
        if (gastar == 11)
        {
            apply_surface(530,0,bullet[animgastar11],screen);
            if (framegastar % 5 == 0)
                animgastar11++;
            framegastar++;

            if(animgastar11 > 32)
            {
                animgastar11 = 32;
            }

        }


        if(((myTimer.get_ticks()-startin)/1000)==60)
        {
            boss = true;
            bossy =true;
            //quit = true;
        }





        if( SDL_Flip( screen ) == -1 )
        {
            return ;
        }
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }

        //Fill the screen white




    }



}
void MadnessMenu()
{
    int frame = 0;
    int i = 0;
    bool cap = true;
    Timer fps;
    int menux = 0;
    int menuy = 0;
    int groundx = 0;
    int groundy = 0;
    int movex = 0;
    int movey = -518;
    bool quit = false;
    int selecting = 1;
    bool fullscreen = false;
    SDL_Surface* snow[9];
    SDL_Surface* snows[5];
    SDL_Surface* logo[4];

    bool pressed =false;

    snow[0] = load_image("snow0.png");
    snow[1] = load_image("snow1.png");
    snow[2] = load_image("snow2.png");
    snow[3] = load_image("snow3.png");
    snow[4] = load_image("snow4.png");
    snow[5] = load_image("snow5.png");
    snow[6] = load_image("snow6.png");
    snow[7] = load_image("snow7.png");
    snow[8] = load_image("snow8.png");
    logo[0] = load_image("logo0.png");
    logo[1] = load_image("logo1.png");
    logo[2] = load_image("logo2.png");
    logo[3] = load_image("logo3.png");
    snows[0] = load_image("snow9.png");
    snows[1] = load_image("snow10.png");
    snows[2] = load_image("snow11.png");
    snows[3] = load_image("snow12.png");
    snows[4] = load_image("snow13.png");








    while(quit == false)
    {
        fps.start();
        cap = ( !cap );
        press = TTF_RenderText_Solid(font, "PRESS ENTER TO START", textColorc);
        play1 = TTF_RenderText_Solid(fuente, "Single Player", textColorc);
        play2 = TTF_RenderText_Solid(fuente ,"Multiplayer", textColorc);
        options = TTF_RenderText_Solid(fuente, "Options", textColorc);
        salir = TTF_RenderText_Solid(fuente, "Exit", textColorc);
        while(SDL_PollEvent(&event))
        {

            if(event.type == SDL_QUIT)
            {


                exit(0);
            }



            if(event.type == SDL_KEYDOWN )
            {




                if(!pressed)
                {
                    if(event.key.keysym.sym == SDLK_RETURN)
                    {
                        pressed = true;

                        menux = 1;

                        if(menux<2)
                        {
                            menux=2;
                            pressed = false;

                        }


                    }

                }

            }

        }


        if(SDL_Flip(screen) == -1)
        {

            return;
        }
        apply_surface(0,0,background1, screen);
        apply_surface(0,0,snow[frame], screen);
        apply_surface(0,0,snows[movex], screen);
        apply_surface(0,0,logo[groundx], screen);

        if(menux!= 0)
        {
            apply_surface(0,0,slides[menuy], screen);
            if(i%6 == 0)
            {
                if(menuy<2)
                {
                    menuy++;
                }
            }

        }
        if(menuy == 2)
        {
            apply_surface(240,280,play1,screen);
            apply_surface(240,310,play2,screen);
            apply_surface(240,340,options,screen);
            apply_surface(240,370,salir,screen);

            if(selecting == 1)
            {

                apply_surface(205, 280, guntip, screen);

            }

            if(selecting == 2)
            {
                apply_surface(205, 310, guntip, screen);


            }

            if(selecting == 3)
            {

                apply_surface(205, 340, guntip, screen);


            }

            if(selecting == 4)
            {
                apply_surface(205, 370, guntip, screen);


            }

            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_KEYDOWN )
                {


                    if(event.key.keysym.sym == SDLK_F11)
                    {
                        screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE | 		SDL_FULLSCREEN );
                    }
                    if(event.key.keysym.sym == SDLK_F10)
                    {
                        screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );
                    }
                    if(event.key.keysym.sym == SDLK_RETURN)
                    {
                        //                    Mix_PlayChannel( -1, enter, 0 );
                        //                    Mix_VolumeChunk(enter, 50);


                        if(selecting == 1)
                        {
                            juego();

                        }
                        if(selecting == 2)
                        {

                        }
                        if(selecting == 3)
                        {


                        }

                        if(selecting == 4)
                        {
                            screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );
                            exit(0);
                        }
                    }
                    if(event.key.keysym.sym == SDLK_UP)
                    {
//                    Mix_PlayChannel( -1, navig, 0 );
//                    Mix_VolumeChunk(navig, 50);

                        if(selecting>1)
                        {
                            selecting--;
                            break;
                        }
                    }

                    if(event.key.keysym.sym == SDLK_DOWN)
                    {
//                    Mix_PlayChannel( -1, navig, 0 );
//                    Mix_VolumeChunk(navig, 50);
                        if(selecting<4)
                        {
                            selecting++;
                            break;
                        }
                    }
                }
            }

        }

        if(menux == 0)
        {
            apply_surface(125,300,press,screen);
        }





        if(i%10 == 0)
        {
            frame++;
            movex++;

            if(frame >9)
            {
                frame = 8;

            }
            if(movex>4)
            {
                movex = 0;
            }



        }

        if(i%20 == 0)
        {

            if(groundx<3)
            {
                apply_surface(0,0,delay, screen);
                groundx++;

            }



        }





        i++;


        if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }



}

void display()
{
    int frame = 0;
    bool cap = true;
    Timer fps;
    SDL_Surface* msj = NULL;

    int movex = 0;
    int movey = 0;
    bool quit = false;
    int selecting = 1;

    while(quit == false)
    {
        msj = TTF_RenderText_Solid(fuente, "ASJKDAS PRESENTS:", {204,255,229});
        fps.start();
        cap = ( !cap );

        apply_surface(0,movey,stretch, screen);
        apply_surface(180,200,msj, screen);

        frame++;
        movey-=2;
        if(movey == -520)
        {
            MadnessMenu();

        }
        if(SDL_Flip(screen) == -1)
        {

            return;
        }
        ;

        if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }

    }

}

int frameaux = 0;
void Button::show()
{
    //Show the button
    if(frameshot>=4)
    {
        shoted = false;


    }
    if(frameshot2>=1)
    {

        shoted2 = false;
    }


    if(!shoted)
        apply_surface( box.x, box.y, buttonSheet, screen,NULL );
    else
        apply_surface((event.button.x)-50,(event.button.y)-50, shots[frameshot], screen);

    if(frameaux%4 == 0)
    {
        frameshot++;
        frameshot2++;
    }

    frameaux++;
}
int main( int argc, char* args[] )
{
    bool cap = true;
    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }


    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Clean up
    display();

    clean_up();

    return 0;
}
