#pragma once
#include<vector>
#include<iostream>

class vec3d
{
public:
  double x , y, z;
  
  vec3d( double x =0.0f  , double y = 0.0f, double z= 0.0f) : x(x) , y(y) , z(z) {};
  
  
  
};

typedef  std::vector<std::vector<double>> mtx4;
class Mtx4x4
{
 
  mtx4 mtrx;
  public:
  
  Mtx4x4()
  {
    mtrx = mtx4(4,std::vector<double>(4,0));
  }
  
  
  
  std::vector<double>& operator [](int i)
  {
    return mtrx[i];
  }
  
  void operator = (double n)
  {
    for(std::vector<double> &v : mtrx)
      for(double& p : v)
        p = n;
  }
  
  mtx4 operator * (mtx4& oth)
  {
    mtx4 res(4, std::vector<double>(4, 0));
  
    for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    res[i][j] += mtrx[i][k] * oth[k][j];
                }
            }
        }
  
    return res;
  }
  
};



