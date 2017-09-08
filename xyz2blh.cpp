
/**
 * @file xyz2blh.cpp
 * @brief Cartesian coordinate and geodedic coordinate convert
 * @author yinflying(yinflying@foxmail.com)
 * @version 1.0.0
 * @date 2017-09-08
 */
#include <iostream>
#include <math.h>
using namespace std;

const double PI = 3.1415926535897932384; ///< PI
const double D2R = 0.017453292519943; ///< Coefficient of convert Degree to Radian
const double R2D = 57.29577951308232; ///< Coefficient of convert Radian to Degree

/**
 * @brief Convert cartesian system coodinate (x,y,z) to geodetic coordinate(B,L,H),
 * adopt WGS81 reference ellipsoid
 * @param x x component of cartesian(unit in meter)
 * @param y y component of cartesian(unit in meter)
 * @param z z component of cartesian(unit in meter)
 * @param B (O)Latitude of geodetic coordianate,East is positive and West is negative(unit in degree,decimal)
 * @param L (O)Longitude of geodetic coordiante,North is positive and South is negative(unit in degree,decimal)
 * @param H (O)Geodetic height(ellipical height) of geodetic coordinate(unit in meter)
 * @return
 */
static int xyz2BLH(const double& x, const double& y,const double& z,
        double& B, double& L, double& H)
{
    const double a = 6378137;
    const double b = 6356752.3142;
    const double e2 = 0.0066943799013;
    const double ee2 = 0.00673949674227;

    double sqrtxy = sqrt(x*x + y*y);
    double theta = atan(a*z/(b*sqrtxy));
    double tanB0 = (z+ee2*b*pow(sin(theta),3))/(sqrtxy - e2*a*pow(cos(theta),3));
    double B0 = atan(tanB0);
    double DB = 1;
    while(DB > 1e-10)
    {
        double B1 = z/sqrtxy*(1+a*e2*sin(B0)/(z*sqrt(1-e2*sin(B0)*sin(B0))));
        B1 = atan(B1);
        DB = fabs(B1 - B0);
        B0 = B1;
    }
    B = B0;

    double N = a /sqrt(1-e2*pow(sin(B),2));
    if(x > 1e-10)
    {
        L = atan(y/x);
    }
    else if(x < -1e-10)
    {
        L= atan(y/x) + PI;
    }
    else
    {
        if(y > 0)
            L = PI*0.5;
        else
            L = PI*1.5;
    }
    H = sqrtxy * cos(B) + z*sin(B) - N*(1-e2*pow(sin(B),2));

    B = B*R2D;
    L = L*R2D;
    if ( L > 180.0 )
        L = L - 360.0;
    return 0;
}

/**
 * @brief Convert geodetic coordinate(B,L,H) to cartesian system coodinate (x,y,z),
 * adopt WGS81 reference ellipsoid
 * @param B Latitude of geodetic coordianate,East is positive and West is negative(unit in degree,decimal)
 * @param L Longitude of geodetic coordiante,North is positive and South is negative(unit in degree,decimal)
 * @param H Geodetic height(ellipical height) of geodetic coordinate(unit in meter)
 * @param x (O)x component of cartesian(unit in meter)
 * @param y (O)y component of cartesian(unit in meter)
 * @param z (O)z component of cartesian(unit in meter)
 * @return
 */
static int BLH2xyz(double B,double L,double H,double& x, double& y, double& z)
{
    B = B*D2R; L = L*D2R;
    const double a = 6378137;
    const double e2 = 0.0066943799013;
    double N = a/sqrt(1-e2*pow(sin(B),2));
    x = (N+H)*cos(B)*cos(L);
    y = (N+H)*cos(B)*sin(L);
    z = (N*(1-e2)+H) * sin(B);
    return 0;
}

//test
int main()
{
    double x = -953076.900000;
    double y = -6542517.500000;
    double z =  2453130.200000;
    double B,L,H;
    xyz2BLH(x,y,z,B,L,H);
    cout<<B<<endl;
    cout<<L<<endl;
    cout<<H<<endl;
    double nx,ny,nz;
    BLH2xyz(B,L,H,nx,ny,nz);
    cout<<nx - x<<endl;
    cout<<ny - y<<endl;
    cout<<nz - z<<endl;
}
