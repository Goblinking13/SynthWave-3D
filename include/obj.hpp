#pragma once
#include<vector>
#define PI 3.1415926

extern class vec3d;

struct polygon
{
  vec3d p1,p2,p3;
  
  polygon(vec3d p1, vec3d p2, vec3d p3) : p1(p1) , p2(p2), p3(p3) {};
};

struct rotation
{
  double x=0, y =0, z=0;
};

class object
{
  

  
public:
  
  
  
  object(){
    obj2Wld = 0;
    obj2Wld[0][0] = 1;
    obj2Wld[1][1] = 1;
    obj2Wld[2][2] = 1;
    obj2Wld[3][3] = 1;
    
    ScaleMtrx[0][0] = 1;
    ScaleMtrx[1][1] = 1;
    ScaleMtrx[2][2] = 1;
    ScaleMtrx[3][3] = 1;
    
  };
  
  object(const object& other) : obj2Wld(other.obj2Wld)
  {
    this->objRotation.x = other.objRotation.x;
    this->objRotation.y = other.objRotation.y;
    this->objRotation.z = other.objRotation.z;
    
    this->mesh = other.mesh;
    this->ScaleMtrx = other.ScaleMtrx;
    
    
  }
  
  std::vector<polygon> mesh;
  rotation objRotation;
  Mtx4x4 obj2Wld;
  Mtx4x4 ScaleMtrx;
  
  void addPoly(polygon &p)
  {
    mesh.emplace_back(p.p1,p.p2,p.p3);
  }
  
  void addPoly(vec3d &p1 ,vec3d &p2,vec3d &p3 )
  {
    mesh.emplace_back(p1,p2,p3);
  }
  
  void moveX(double n)
  {
    obj2Wld[0][3] += n;
//    for(polygon& poly : mesh)
//    {
//      poly.p1.x += n;
//      poly.p2.x += n;
//      poly.p3.x += n;
//    }
  }
  
  void moveY(double n)
  {
    obj2Wld[1][3] += n;
//    for(polygon& poly : mesh)
//    {
//      poly.p1.y += n;
//      poly.p2.y += n;
//      poly.p3.y += n;
//    }
  }
  
  void moveZ(double n)
  {
    
    obj2Wld[2][3] += n;
    
//    for(polygon& poly : mesh)
//    {
//      poly.p1.z += n;
//      poly.p2.z += n;
//      poly.p3.z += n;
//    }
  }
  
  double degToRad(double deg)
  {
    return (PI/180.0f)*deg;
  }
  
  vec3d rotateX(vec3d &v, double deg)
  {
    vec3d a;
    a.x = v.x;
    a.y = v.y*cos(deg) - v.z*sin(deg);
    a.z = v.y*sin(deg) + v.z*cos(deg);
    return a;
  }
  
  vec3d rotateY(vec3d &v, double deg)
  {
    vec3d a;
    a.x = v.x*cos(deg) + v.z*sin(deg);
    a.y = v.y;
    a.z = -v.x*sin(deg) + v.z*cos(deg);
    return a;
  }
  
  vec3d rotateZ(vec3d &v, double deg)
  {
    vec3d a;
    a.x = v.x*cos(deg) - v.y*sin(deg);
    a.y = v.x*sin(deg) + v.y*cos(deg);
    a.z = v.z;
    return a;
  }
  

  
  
  void rotate(double x, double y, double z)
  {
    
    double xd = degToRad(x);
    double yd = degToRad(y);
    double zd = degToRad(z);
    
    for(polygon& poly : mesh)
    {
      if(xd != 0)
      {
        poly.p1 = rotateX(poly.p1, xd);
        poly.p2 = rotateX(poly.p2, xd);
        poly.p3 = rotateX(poly.p3, xd);
      }
      
      if(yd != 0)
      {
        poly.p1 = rotateY(poly.p1, yd);
        poly.p2 = rotateY(poly.p2, yd);
        poly.p3 = rotateY(poly.p3, yd);
      }
      
      if(zd != 0)
      {
        poly.p1 = rotateZ(poly.p1, zd);
        poly.p2 = rotateZ(poly.p2, zd);
        poly.p3 = rotateZ(poly.p3, zd);
      }
      
    }
    
  }
 
  void objToWorld()
  {
    for(polygon& pol : mesh)
    {
      pol.p1.x = obj2Wld[0][0]* pol.p1.x + obj2Wld[0][1]*pol.p1.y + obj2Wld[0][2]*pol.p1.z + obj2Wld[0][3]*1;
      pol.p1.y = obj2Wld[1][0]* pol.p1.x + obj2Wld[1][1]*pol.p1.y + obj2Wld[1][2]*pol.p1.z + obj2Wld[1][3]*1;
      pol.p1.z = obj2Wld[2][0]* pol.p1.x + obj2Wld[2][1]*pol.p1.y + obj2Wld[2][2]*pol.p1.z + obj2Wld[2][3]*1;
      
      pol.p2.x = obj2Wld[0][0]* pol.p2.x + obj2Wld[0][1]*pol.p2.y + obj2Wld[0][2]*pol.p2.z + obj2Wld[0][3]*1;
      pol.p2.y = obj2Wld[1][0]* pol.p2.x + obj2Wld[1][1]*pol.p2.y + obj2Wld[1][2]*pol.p2.z + obj2Wld[1][3]*1;
      pol.p2.z = obj2Wld[2][0]* pol.p2.x + obj2Wld[2][1]*pol.p2.y + obj2Wld[2][2]*pol.p2.z + obj2Wld[2][3]*1;
      
      pol.p3.x = obj2Wld[0][0]* pol.p3.x + obj2Wld[0][1]*pol.p3.y + obj2Wld[0][2]*pol.p3.z + obj2Wld[0][3]*1;
      pol.p3.y = obj2Wld[1][0]* pol.p3.x + obj2Wld[1][1]*pol.p3.y + obj2Wld[1][2]*pol.p3.z + obj2Wld[1][3]*1;
      pol.p3.z = obj2Wld[2][0]* pol.p3.x + obj2Wld[2][1]*pol.p3.y + obj2Wld[2][2]*pol.p3.z + obj2Wld[2][3]*1;
    }
  }
  
  
  void formMatrixO2W()
  {
    double rX = degToRad(objRotation.x);
    double rY = degToRad(objRotation.y);
    double rZ = degToRad(objRotation.z);
    
    
    obj2Wld[0][0] = ScaleMtrx[0][0] *cos(rY)*cos(rZ);
    obj2Wld[0][1] = ScaleMtrx[1][1]* (sin(rX)*sin(rY)*cos(rZ) - sin(rY) * cos(rX));
    obj2Wld[0][2] = ScaleMtrx[2][2] * (cos(rX) * sin(rY) * cos(rZ) + sin(rX) * sin(rZ));
    
    obj2Wld[1][0] = ScaleMtrx[0][0] * (cos(rY) * sin(rZ));
    obj2Wld[1][1] = ScaleMtrx[1][1] * (sin(rX)*sin(rY)*sin(rZ) - cos(rZ) * cos(rX));
    obj2Wld[1][2] = ScaleMtrx[2][2] * (cos(rX) * sin(rY) * sin(rZ) + sin(rX) * cos(rZ));
    
    obj2Wld[2][0] = ScaleMtrx[0][0] * (-sin(rY));
    obj2Wld[2][1] = ScaleMtrx[1][1] * (sin(rX) * cos(rY));
    obj2Wld[2][2] = ScaleMtrx[2][2] * (cos(rX) * cos(rY));
    
 
  }
  
};


