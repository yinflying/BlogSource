#include <cstdio>
#include <cstring>
#include <ctime>
#include <chrono>
using namespace std;

class CVect{
public:
    double *data;
    int m_Size;
    int m_Max;
    void Init();
    void Free();
    CVect();
    CVect(int size, double value=0.0);
    CVect(int size, const double *pdata);
    CVect(const CVect& arr);
    CVect operator +(const CVect &v);
    void print();
    ~CVect();
};

void CVect::Init()
{
    this->m_Size = 0;
    this->m_Max = 1;
    this->data = new double[m_Max];
}
void CVect::Free()
{
    delete []this->data;
}
CVect::CVect()
{
    this->Init();
}
CVect::CVect(int size, double value)
{
    if(0 == size)
        this->Init();
    else
    {
        this->m_Size = size;
        this->m_Max = size;
        data = new double[m_Max];
        for(int i = 0; i < this->m_Size; ++i)
            data[i] = value;
    }
}
CVect::CVect(const CVect& arr)
{
    this->m_Size = arr.m_Size;
    this->m_Max = arr.m_Max;
    this->data = new double[this->m_Max];
    memcpy(data,arr.data,arr.m_Size*sizeof(double));
}
CVect::CVect(int size, const double *pdata)
{
    printf("CVect::CVect Init\n");
    this->m_Size = size;
    this->m_Max = size;
    this->data = new double[this->m_Max];
    memcpy(data,pdata,size*sizeof(double));
}
void CVect::print()
{
    if(0 == this->m_Size)
        printf("Error: Empty array\n");
    else
    {
        for(int i = 0; i < this->m_Size; ++i)
            printf("%.3f   ",this->data[i]);
        printf("\n");
    }
}
CVect CVect::operator +(const CVect &v)
{
    CVect tmp = v;
    auto start1 = std::chrono::system_clock::now();
    for(int j = 0; j < 10000; j++)
    {
        for(int i = 0; i < this->m_Size; ++i)
            tmp.data[i] = this->data[i] + v.data[i];
    }
    auto end1 = std::chrono::system_clock::now();
    auto elapsed1 = end1 - start1;
    printf("Using Index: %li ticks\n",elapsed1.count());

    auto start = std::chrono::system_clock::now();
    for(int j = 0; j < 10000; j++)
    {
        double *pout = tmp.data;
        double *pthis = this->data;
        double *pv = v.data;
        for(double *PEnd = &tmp.data[tmp.m_Size]; pout < PEnd ; )
            *pout++ = *pthis++ + *pv++;
    }
    auto end = std::chrono::system_clock::now();
    auto elapsed = end - start;
    printf("Using Pointer: %li ticks\n",elapsed.count());
    printf("Index/Pointer: %.3f\n",double(elapsed1.count())/double(elapsed.count()));
    return tmp;
}

CVect::~CVect()
{
    this->Free();
}

int main()
{
    CVect a1(100,1);
    CVect a2 = a1;
    CVect a3 = a1 + a2;
    return 0;
}
