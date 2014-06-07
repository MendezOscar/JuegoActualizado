#include "BOSS.h"

BOSS::BOSS(int x, int y, SDL_Surface *screen,SDL_Surface * surface)
{
  this->x = x;
    this->y = y;
    this->surface = surface;
    this->screen = screen;
    this->borrar = false;
    this->tipo="b";
    this->morir =0;
    this->parp = true;
    this->where= true;
    this->shooting = false;
}

void BOSS::mover(bool m){
if(m){
    this->x+=100;
}else{
    this->x-=50;

}

}

BOSS::~BOSS()
{
    //dtor
}

void BOSS::logica(int clickx, int clicky){
if(clickx>=x && clickx<=x+surface->w
                    &&clicky >=y&& clicky <=y+surface->h){
                    morir++;
                    if(morir==130)
                    borrar = true;
}
}
