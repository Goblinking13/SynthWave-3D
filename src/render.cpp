#include "render.hpp"
#include"vec3.hpp"
#include"obj.hpp"
#include<SDL2/SDL.h>


vec3d screenSpacePoint(const vec3d &p, int width , int height)
{
  
    double tmpX = p.x/(-p.z);
    double tmpY = p.y/(p.z);
  
    tmpX =(1.0f + tmpX) /2.0f;
    tmpY = (1.0f + tmpY)/2.0f;
    
    tmpX = tmpX * width;
    tmpY = tmpY * height;
    
    
  return {tmpX,tmpY,0};
}


object screenSpaceObject(const object &v, int width , int height)
{
  object objectInScreen;
  
  for(const polygon &pol : v.mesh)
  {
    vec3d p1 =screenSpacePoint(pol.p1, width, height);
    vec3d p2 =screenSpacePoint(pol.p2, width, height);
    vec3d p3 =screenSpacePoint(pol.p3, width, height);
    objectInScreen.addPoly( p1,p2,p3);
  }
  
  return objectInScreen;
}

vec3d mtrx4x4Vec3(vec3d& v, Mtx4x4 &mtrx)
{
  vec3d ans;
  
  ans.x = v.x * mtrx[0][0] + v.y*mtrx[0][1] + v.z * mtrx[0][2] + mtrx[0][3];
  ans.y = v.x * mtrx[1][0] + v.y*mtrx[1][1] + v.z * mtrx[1][2] + mtrx[1][3];
  ans.z = v.x * mtrx[2][0] + v.y*mtrx[2][1] + v.z * mtrx[2][2] + mtrx[2][3];
  
  return ans;
}

vec3d countNormal(vec3d &p1, vec3d &p2, vec3d &p3)
{
  vec3d v1, v2 ,n;
  
  v1.x = p2.x - p1.x;
  v1.y = p2.y - p1.y;
  v1.z = p2.z - p1.z;
  
  v2.x = p3.x - p1.x;
  v2.y = p3.y - p1.y;
  v2.z = p3.z - p1.z;
  
  n.x = v1.y*v2.z - v2.y*v1.z;
  n.y = v1.z*v2.x - v2.z * v1.x;
  n.z = v1.x*v2.y - v1.y*v2.x;
  return n;
}

bool isPolyVisible(vec3d &camera, vec3d &n, vec3d &p1)
{
 
  double tmp =(p1.x - camera.x) * n.x + (p1.y - camera.y) * n.y + (p1.z - camera.z ) *n.z ;
  if(tmp<0.0f)
    return true;
  
  return false;
}

void drawWireFrame(SDL_Renderer *renderer, object &object,int width, int height, SDL_Color color)
{
  int count = 0;
  object.formMatrixO2W();
  vec3d cam;
  vec3d camDir(0,-1,-1);
  vec3d camDir1(0,1,1);
  vec3d camDir2(-1,1,1);
  
  std::vector<SDL_Vertex> vrtx;

  for(polygon& pol : object.mesh)
  {
    
    
    
    
    
    SDL_SetRenderDrawColor( renderer, 30, 79, 201, 255 );
    count ++;
    vec3d point1 = mtrx4x4Vec3(pol.p1, object.obj2Wld);
    vec3d point2 = mtrx4x4Vec3(pol.p2, object.obj2Wld);
    vec3d point3 = mtrx4x4Vec3(pol.p3, object.obj2Wld);
    
    
    vec3d n = countNormal(point1, point2, point3);
    
    if(!isPolyVisible(cam,n, point1))
      continue;
    
    
    point1 = screenSpacePoint(point1, width, height);
    point2 = screenSpacePoint(point2, width, height);
    point3 = screenSpacePoint(point3, width, height);
   
    if(!(abs(pol.p1.x - pol.p2.x) == 1 && abs(pol.p1.z - pol.p2.z) == 1) ){
      if((pol.p1.y + pol.p2.y)/2.0f <= 5)
        SDL_SetRenderDrawColor( renderer,133, 18, 153, 255 );
        SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
    }
      
  
    
    if(!(abs(pol.p1.x - pol.p3.x) == 1 && abs(pol.p1.z - pol.p3.z) == 1))
    {
      if((pol.p1.y + pol.p3.y)/2.0f <= 5)
        SDL_SetRenderDrawColor( renderer, 133, 18, 153, 255 );
     SDL_RenderDrawLine(renderer, point1.x, point1.y, point3.x, point3.y);
    }

    
    if(!(abs(pol.p3.x - pol.p2.x) == 1 && abs(pol.p3.z - pol.p2.z) == 1))
    {
      if((pol.p2.y + pol.p3.y)/2.0f <= 6)
        SDL_SetRenderDrawColor( renderer, 133, 18, 153, 255 );
     SDL_RenderDrawLine(renderer, point2.x, point2.y, point3.x, point3.y);
    }
  }
  

}


void drawSun(SDL_Renderer *renderer, int x ,int y, int r,int segments)
{


  float cx = x;
  float cy = y;

  float angleStep = 2 * M_PI / segments;
  float angle = 0;

  std::vector<SDL_Vertex> vrtx;

  int initPos= cy - r;

  for (int i = 0; i < segments; ++i) {
    float x1 = cx + r * cos(angle);
    float y1 = cy + r * sin(angle);

    angle += angleStep;

    float x2 = cx + r * cos(angle);
    float y2 = cy + r * sin(angle);

    vrtx.push_back({{cx, cy},{237, 201, 21}, {0, 0} });
    vrtx.push_back({{x1, y1},{237, 201, 21}, {0, 0}});
    vrtx.push_back({{x2, y2},{237, 201, 21}, {0, 0}});

  }

  vrtx.push_back({{cx-r, cy},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+10},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+10},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+10},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy},{36, 36, 43}, {0, 0}});
  
  vrtx.push_back({{cx-r, cy+15},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+23},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+15},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+23},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+23},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+15},{36, 36, 43}, {0, 0}});
  
  vrtx.push_back({{cx-r, cy+26},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+31},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+26},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+31},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+31},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+26},{36, 36, 43}, {0, 0}});
  
  vrtx.push_back({{cx-r, cy+33},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+36},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+33},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+36},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+36},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+33},{36, 36, 43}, {0, 0}});
  
  vrtx.push_back({{cx-r, cy+38},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+41},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+38},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+41},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+41},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+38},{36, 36, 43}, {0, 0}});
  
  vrtx.push_back({{cx-r, cy+43},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+45},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+43},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+45},{36, 36, 43}, {0, 0} });
  vrtx.push_back({{cx-r, cy+45},{36, 36, 43}, {0, 0}});
  vrtx.push_back({{cx+r, cy+43},{36, 36, 43}, {0, 0}});
  
  SDL_RenderGeometry(renderer, NULL, vrtx.data(), vrtx.size(), NULL, 0);
}
