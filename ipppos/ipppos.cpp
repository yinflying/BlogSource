#include <iostream>
#include <math.h>
using namespace std;

const double PI = 3.1415926535897932384; ///< PI
const double D2R = 0.017453292519943; ///< Coefficient of convert Degree to Radian
const double R2D = 57.29577951308232; ///< Coefficient of convert Radian to Degree

/**
 * @brief Calculate ionospheric pierce point position(H=450km)
 * @param azi (I) Azimuth angle of beacon to satellite(unit in radian)
 * @param ele (I) Elevation of beacon to satellite(unit in radian)
 * @param r_BL[2] (I) Beacon's position in gedetic coordianate
 *      r_BL[0] : Latitude (unit in radian);
 *      r_BL[1] : Longitude (unit in radian);
 * @param &IPP_BL[2] (O) Ionospheric pierce point position in gedetic coordinate
 *      IPP_BL[0] : Latitude (unit in radian);
 *      IPP_BL[1] : Longitude (unit in radian);
 * @return 0: all right
 */
static int getIPP(const double& azi,const double& ele,
        const double r_BL[2],double (&IPP_BL)[2])
{
    double RE=6371000.0; double H=450000.0;
    double zr=PI/2-ele;
    double z=asin(RE*sin(zr)/(RE+H));
    IPP_BL[0]=asin(cos(zr-z)*sin(r_BL[0])+sin(zr-z)*cos(r_BL[0])*cos(azi));
    double D = tan(zr-z)*cos(azi) - tan(PI/2-r_BL[0]);
    double D1 = -tan(zr-z)*cos(azi) - tan(PI/2+r_BL[0]);
    if ( (IPP_BL[0] > 70*D2R && D > 0) || (IPP_BL[0] < -70*D2R && D1>0))
        IPP_BL[1]=r_BL[1]+PI-asin(sin(zr-z)*sin(azi)/cos(IPP_BL[0]));
    else
        IPP_BL[1]=r_BL[1]+asin(sin(zr-z)*sin(azi)/cos(IPP_BL[0]));
    if(IPP_BL[1]>PI)
        IPP_BL[1]=IPP_BL[1]-2*PI;
    else if(IPP_BL[1]<-PI)
        IPP_BL[1]=IPP_BL[1]+2*PI;
    return 0;
}

int main()
{
    double azi = 0.8727;
    double ele = 1.0472;
    double r_BL[2] = {0.8727, 0.5236};
    double IPP_BL[2] = {0,0};
    getIPP(azi,ele,r_BL,IPP_BL);
    cout<<IPP_BL[0]<<" "<<IPP_BL[1]<<endl;
    //correct result:
    //  (0.8964 0.5698)
}
