#pragma once
#include<vector>
#include<SDL2/SDL.h>

//#include"vec3.hpp"
extern class object;
extern class vec3d;

vec3d screenSpacePoint(const vec3d &p, int width , int height);
object screenSpaceObject(const object &v, int width , int height);
void drawWireFrame(SDL_Renderer *renderer, object &object,int width, int height, SDL_Color color);
void drawSun(SDL_Renderer *renderer, int x ,int y, int r,int segments);
