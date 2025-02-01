#define DB_PERLIN_IMPL
#include<iostream>
#include<chrono>
#include<vector>
#include"vec3.hpp"
#include"render.hpp"
#include"obj.hpp"
#include"db_perlin.hpp"
#include<time.h>

#ifdef __APPLE__
#include<SDL2/SDL.h>
#else
#include<SDL.h>
#endif

SDL_Window* win;
SDL_Renderer *rendrr;

const static int WIDTH = 800;
const static int HEIGHT = 600;

void kill();
bool init();

static double fooHash[50];

void countFoo()
{

  for(int i =0; i < 50;i++)
  {
    double d = i - 25;

    fooHash[i] = d*d/20.0f;

    if(i < 5)
    {
      fooHash[i] = -i*i/20.0f + 20;
    }

    if(i > 45)
    {
      fooHash[i] = -(50-i)*(50-i)/20.0f + 20.0f*20.0f/20.0f;
    }

  }
}


int main()
{
  countFoo();

  int n = 50;
  std::vector<vec3d> mesh;

  for(int z = 0; z < n;z++)
  {
    for(int j = -n/2; j < n/2;j++)
    {
      mesh.push_back({(double)j,0,(double)z});
    }
  }
  object mountaince;


  mountaince.moveZ(3);
  mountaince.moveY(12);


  for(int x = 0; x < n-1;x++)
  {
    
    for(int z = 0 ; z < n-1;z++)
    {
      mountaince.addPoly(mesh[x+ z*n],mesh[x+1+ z*n],mesh[x+1+ (z+1)*n]  );
      mountaince.addPoly(mesh[x+ z*n],mesh[x+1+ (z+1)*n],mesh[x+ (z+1)*n]  );
    }
  }


  std::vector<vec3d> screen;
  init();
  SDL_Event e;
  bool quit = false;
  double fps = 0;
  auto start = std::chrono::high_resolution_clock::now();
  auto second = std::chrono::milliseconds(1000);
  double move = 0;
  double moveX = 0;

  int moveStep = 5, step  =5;
  moveStep = step = 1;

  while( quit == false )
  {

    start = std::chrono::high_resolution_clock::now();

    SDL_SetRenderDrawColor(rendrr, 36, 36, 43, 100);
    SDL_RenderClear(rendrr);

    
    while( SDL_PollEvent( &e ) )
    { if( e.type == SDL_QUIT ) quit = true;}
    
    
    drawSun(rendrr, WIDTH/2, HEIGHT/4, 75, 50);

    SDL_SetRenderDrawColor( rendrr, 30, 79, 201, 255 );

    drawWireFrame(rendrr, mountaince, WIDTH, HEIGHT, {30, 79, 201, 255});

    SDL_Delay(50);
    mountaince.moveZ(-0.1f);
    
    if(mountaince.getCordZ() <= 0.1)
    {
    
      mountaince.setCordZ(3.0f);
      move += 3.0f;
    }else
      SDL_RenderPresent(rendrr);


    for(polygon& pol : mountaince.mesh)
    {

      pol.p1.y  =0;
      pol.p2.y  =0;
      pol.p3.y  =0;

      double f = 1;
      double a = 10;


      for(int i = 1; i <= 10;i+=1)
      {
        pol.p1.y  += a*db::perlin(f*(pol.p1.x+moveX)/(n) , f*(pol.p1.z+move)/n);
        pol.p2.y  += a*db::perlin(f*(pol.p2.x+moveX)/n , f*(pol.p2.z+move)/n);
        pol.p3.y  += a*db::perlin(f*(pol.p3.x+moveX)/n , f*(pol.p3.z+move)/n);

        f +=1.5;
        a -=1.5;

      }

      pol.p1.y += fooHash[(int)pol.p1.x+25];
      pol.p2.y += fooHash[(int)pol.p2.x+25];
      pol.p3.y += fooHash[(int)pol.p3.x+25];

      int layer = 5;

      if(pol.p1.y < layer )
        pol.p1.y = layer;
      if(pol.p2.y < layer )
        pol.p2.y = layer;
      if(pol.p3.y < layer )
        pol.p3.y = layer;

    }

    auto delta = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(delta - start);

    if(diff.count() != 0)
    {
      fps = second/diff;
    }

    std::string str =  "waves FPS: " + std::to_string(fps);
    SDL_SetWindowTitle(win, str.c_str());
  }
//42
  kill();
  return 0;
}


void kill()
{
  SDL_DestroyRenderer(rendrr);
  SDL_DestroyWindow(win);
  rendrr = NULL;
  win = NULL;
  SDL_Quit();
}


bool init()
{

  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cout << "SDL_Init Error:" << SDL_GetError()<<std::endl;
    return -1;
  }


  win = SDL_CreateWindow("window", 100, 100, WIDTH, HEIGHT,SDL_WINDOW_ALWAYS_ON_TOP);

  if(win == NULL)
  {
    std::cout << "SDL_CreateWindow Error:" << SDL_GetError()<<std::endl;
    return -1;
  }

  rendrr = SDL_CreateRenderer(win, -1 , SDL_RENDERER_ACCELERATED);

  if(rendrr == NULL)
  {
    std::cout << "SDL_CreateRender Error:"<< SDL_GetError()<<std::endl;
    return -1;
  }
  SDL_SetRenderDrawColor(rendrr, 89, 88, 107, 100);
  SDL_RenderClear(rendrr);


  return true;
}



