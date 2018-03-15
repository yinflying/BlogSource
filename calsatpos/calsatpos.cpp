/**
 * @file calsatpos.cpp
 * @brief Calculate satellite's position  according to Navigation data& precise ephemeris
 * @author yinflying(yinflying@foxmail.com)
 * @version 1.0.0
 * @date 2017-03-15
 */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#include <rtklib.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
using namespace std;

/*
 *Transformat ITRF2014 to CRCS 2000
 *I: (x,y,z) the coordinates in ITRF2014
 *I: epoch   the epoch of the coordiantes
 *O: (xs,ys,zs) the coordinates in CRCS2000
 */
static void ITRF14toCRCS2000(const double& x, const double&y,const double& z,
        const double& epoch,double& xs,double& ys, double& zs)
{
    double P[] = {7.4, -0.5, -62.8, 3.80, 0.00, 0.00, 0.26};
    double vP[] = {0.1, -0.5, -3.3, 0.12, 0.00, 0.00, 0.02};
    double ref_ehoch = 2010.0;
    double tP[7] = {0};
    for(int i = 0; i < 7; ++i)
    {
        tP[i] = P[i] + vP[i]*(epoch - ref_ehoch);
    }
    xs = x + tP[0] + tP[3]*x - tP[6]*y + tP[5]*z;
    ys = y + tP[1] + tP[6]*x + tP[3]*y - tP[4]*z;
    zs = z + tP[2] - tP[5]*x + tP[4]*y - tP[3]*z;
    return;
}
static void string2gtime(const string& st,const int& timezone,
        gtime_t& gt)
{
    struct tm tt = {0};
    tt.tm_year = stoi(st.substr(0,4)) - 1900;
    tt.tm_mon = stoi(st.substr(4,2)) - 1;
    tt.tm_mday = stoi(st.substr(6,2));
    tt.tm_hour = stoi(st.substr(8,2));
    tt.tm_min = stoi(st.substr(10,2));
    tt.tm_sec = stoi(st.substr(12,2));
    gt.time = mktime(&tt);
    gt.time += timezone*3600;
    gt.sec = stod("0"+st.substr(14));
}
int main(int argc, char * argv[])
{
    int ch=0;
    vector<string> rinexfile;
    vector<string> sp3file;
    string begintime;
    string endtime;
    string satename;
    int sampletime;
    while ((ch = getopt(argc, argv, "hb:e:t:I:s:")) != -1)
    {
        switch (ch) {
            case 's':
                satename = optarg;
                break;
            case 'h':
                cout<<"eph2pos是通过精密星历和广播星获取指定时间段的卫星位置";
                cout<<endl;
                cout<<"使用方法："<<endl;
                cout<<"参数选择：hbetIs"<<endl;
                cout<<"        -h       获取本帮助"<<endl;
                cout<<"        -b       起始时间"<<endl;
                cout<<"        -e       结束时间"<<endl;
                cout<<"        -t       时间间隔"<<endl;
                cout<<"        -I       输入文件"<<endl;
                cout<<"        -s       卫星编号"<<endl;
                cout<<"例如："<<endl;
                cout<<"eph2pos -I ../data/sp3/igs15905.sp3 ";
                cout<<"-b 20100702001500.54365 -e 20100702011500.0000 ";
                cout<<" -t 1000 -s G02"<<endl;
                cout<<"可以使用-I选项反复添加文件,注意时间的格式为yyyymmddhhmmss.sssss"<<endl;
                exit(0);
                break;
            case 'b':
                begintime = optarg;
                cerr<<"起始时间："<<begintime<<" ";
                break;
            case 'e':
                endtime = optarg;
                cerr<<"结束时间: "<<endtime<<endl;
                break;
            case 't':
                sampletime =stoi(optarg);
                cerr<<"取样时间间隔:"<<sampletime<<"s"<<endl;
                break;
            case 'I':
                string filename = optarg;
                cerr<<"加载文件: "<<filename<<endl;
                if(filename.substr(filename.size()-3)=="sp3")
                    sp3file.push_back(filename);
                if(filename.substr(filename.size()-1)=="n")
                    rinexfile.push_back(filename);
                if(filename.substr(filename.size()-1)=="p")
                    rinexfile.push_back(filename);
                break;
        }
    }
    nav_t nav = {0};
    obs_t obs = {0};
    sta_t sta = {""};

    for(unsigned int i = 0; i < rinexfile.size(); ++i)
        readrnx(rinexfile.at(i).data(),1,"",&obs,&nav,&sta);
    for(unsigned int i = 0; i < sp3file.size(); ++i)
        readsp3(sp3file.at(i).data(),&nav,1);
    int sateno = satid2no(satename.data());

    gtime_t gbt,gnt;
    string2gtime(begintime,8,gbt);
    string2gtime(endtime,8,gnt);

    gtime_t t = gbt;

    cout.precision(12);
    while(t.time < gnt.time - 1)
    {
        if(nav.n != 0)
        {
            double brs[3];
            double bdts,bvar;
            int ni = 0;
            for (; ni< nav.n; ++ni)
            {
                if(nav.eph[ni].sat == sateno)
                {
                    int toc = nav.eph[ni].toc.time;
                    int toe = nav.eph[ni].toe.time;
                    if(t.time >= toc - 2*3600 && t.time <= toe + 2*3600)
                    {
                        break;
                    }
                }
            }
            if(ni == nav.n-1)
                cerr<<"选时超出广播星历有效预报范围"<<endl;
            cout<<ni<<sateno<<" "<<nav.n<<endl;
            eph2pos(t,&nav.eph[ni],brs,&bdts,&bvar);
            struct tm* thet;
            thet = gmtime(&(t.time));
            cout<<"B ";
            cout<<thet->tm_year+1900<<" "<<thet->tm_mon+1<<" "<<thet->tm_mday;
            cout<<" "<<thet->tm_hour<<" "<<thet->tm_min<<" ";
            cout<<" "<<(double)thet->tm_sec + t.sec<<" ";
            cout<<brs[0]<<" "<<brs[1]<<" "<<brs[2]<<endl;
        }
        if(nav.ne != 0)
        {
            double srs[6];
            double sdts,svar;
            int ret = peph2pos(t,sateno,&nav,0,srs,&sdts,&svar);
            if(!ret)
            {
                cerr<<"精密星历错误，可能是因为历元选取错误"<<endl;
            }
            struct tm* thet;
            thet = gmtime(&(t.time));
            cout<<"P ";
            cout<<thet->tm_year+1900<<" "<<thet->tm_mon+1<<" "<<thet->tm_mday;
            cout<<" "<<thet->tm_hour<<" "<<thet->tm_min<<" ";
            cout<<" "<<(double)thet->tm_sec + t.sec<<" ";
            cout<<srs[0]<<" "<<srs[1]<<" "<<srs[2]<<" ";
            cout<<srs[3]<<" "<<srs[4]<<" "<<srs[5]<<endl;
        }
        t.time += sampletime;
    }
    return 0;
}
