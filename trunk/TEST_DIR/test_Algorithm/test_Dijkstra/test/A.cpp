// A*�㷨ʵ��
//
//���ʵ����һ��A*�㷨���о����õģ��������޸ĵ�ͼȻ�󿴵�����ȷѰ·������ֳɾ͸У��е���Сʱ����ڵ��ϣ�����һ�����ϰ�ң�Ȼ����������ǵ�·�������ϰ��Ȼ�����ϲ�ͣ��̽����Ȼ�������ҵ��µ�·�ߵĸо������Ǻ�����˼��
//�ã��Ѵ����¼�ڴˣ������Ժ�ο���
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

/************************************************************************/
/* A*�㷨ʵ�֣�����A*�㷨��ͼ      
40X10�ĵ�ͼ��B������ʼ�㣬E�����յ㣻                                              
�ո������ͨ����|������ǽ������ͨ����
xx0123456789012345678901234567890123456789xx
xx����������������������������������������xx
0|                                        |0
1|                                        |1
2|                 |                      |2
3|                   |                      |3
4|                 |        E             |4
5|    B            |                      |5
6|                 |                      |6
7|                                        |7
8|                                        |8
9|                                        |9
xx����������������������������������������xx
xx0123456789012345678901234567890123456789xx
/************************************************************************/

const static int X = 40;
const static int Y = 10;
//��ͼ�ϵ����
enum E_Map
{
    E_River=-2,            //�к������޷�ͨ�����ڵ�ͼ���� ~ ���
    E_Wall=-1,            //��ǽ���޷�ͨ�����ڵ�ͼ���� | ���    
    E_Road = 0,            //û���ϰ��������˳��ͨ�У�����Ϊ0���ڵ�ͼ���ÿո���    
    E_Sand=1,            //��ɳ�أ���ͨ�������������һЩ������Ϊ1���ڵ�ͼ���� * ���
};

//const static int GameMap[Y][X] = {
//    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//    0,0,-2,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//    0,0,0,-2,-2,-2,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
//    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,-2,-2,-2,0,0,0,0,0,0,
//    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,-2,0,0,0,0,0,
//    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,0,0,0,0,0,
//    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
//};

const static int GameMap[Y][X] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,-2,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,-2,-2,-2,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,-1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,0,0,-1,-1,0,0,0,0,0,-2,-2,-2,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,-2,-2,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,-2,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
struct Point
{
    int x;
    int y;
    Point(int i=0,int j=0):x(i),y(j){}
    bool operator==(const Point & r)
    {
        return (x==r.x)&&(y==r.y);
    }
};
bool operator==(const Point& l,const Point & r)
{
    return (l.x==r.x)&&(l.y==r.y);
}
bool operator<(const Point& l,const Point & r)
{
    if(l.y<r.y)    return true;
    else if(l.y>r.y)    return false;
    else    return (l.x < r.x);
}


//��ӡ��ͼ
void PrintMap(const Point &B,const Point &E,const vector<Point>& path=vector<Point>());


void PrintMap(const Point &B,const Point &E,const vector<Point>& path)
{
    int LastMap[Y][X] = {0};
    for (int y=0;y<Y;++y)
    {
        for (int x=0;x<X;++x)
        {
            LastMap[y][x] = GameMap[y][x];
        }
    }
    //·��
    vector<Point>::const_iterator itr = path.begin();
    for (;itr != path.end();++itr)
    {
        LastMap[(*itr).y][(*itr).x] = '&';
    }
    //��ʼ��Ŀ��
    LastMap[B.y][B.x] = 'B';
    LastMap[E.y][E.x] = 'E';

    cout<<"A*Ѱ··��Ϊ��"<<endl;
    cout<<"xx0123456789012345678901234567890123456789xx"<<endl;
    cout<<"xx����������������������������������������xx"<<endl;
    for (int y=0;y<Y;++y)
    {
        cout<<y<<"[";
        for (int x=0;x<X;++x)
        {
            if (LastMap[y][x] == E_Road)
            {
                cout<<" ";
            }
            else if (LastMap[y][x] == E_Wall)
            {
                cout<<"|";
            }
            else if (LastMap[y][x] == E_River)
            {
                cout<<"~";
            }
            else if (LastMap[y][x] == E_Sand)
            {
                cout<<"*";
            }
            else if (LastMap[y][x] == 'B')
            {
                cout<<"B";
            }
            else if (LastMap[y][x] == 'E')
            {
                cout<<"E";
            }
            else if (LastMap[y][x] == '&')
            {
                cout<<"&";
            }
        }
        cout<<"]"<<y<<endl;
    }
    cout<<"xx����������������������������������������xx"<<endl;
    cout<<"xx0123456789012345678901234567890123456789xx"<<endl;
}

//���㵱ǰλ�����յ�λ�õ�Hn
double Hn(const Point & E,const Point&p)
{
    return abs(E.y-p.y) + abs(E.x-p.x);
}

//������������λ��(���������������ڵ���Χ8������)�����Gn
double Gg(const Point & p1,const Point& p2)
{
    double d = GameMap[p2.y][p2.x];
    return ((p1.x-p2.x)!=0&&(p1.y-p2.y)!=0)?(1.5+d):(1.0+d);
}

//̽��λ��p����һ��(p.x + addx,p.y + addy)��Gn,Hn
void testNext(const Point & E,const Point&p,const set<Point>& closeTbl,
    map<Point,double> &mapGn,map<Point,double> &mapGnTemp,
    multimap<double,Point> &HnPoint,int addx,int addy)
{
    int x = p.x + addx;
    int y = p.y + addy;    
    if (x>=0 && y>=0 && x<X && y<Y && GameMap[y][x]>=0)
    {
        Point t = Point(x,y);
        if (closeTbl.find(t) != closeTbl.end())
        {
            return;
        }
        //�õ���Ӧ���α�����Gn
        double dgn = mapGn[p] + Gg(p,t);
        mapGnTemp[t] = dgn;

        map<Point,double>::iterator itr = mapGn.find(t);
        if (itr == mapGn.end() || itr->second > dgn)
        {
            mapGn[t] = dgn;
        }
        HnPoint.insert(make_pair(Hn(E,t),t));
    }
}

void APath(const Point & B,const Point & E,vector<Point>&path)
{    
    //A*�㷨��Fn = Gn + Hn
    path.clear();
    vector<Point> openTbl;
    openTbl.push_back(B);
    set<Point>    closeTbl;
    closeTbl.insert(B);
    map<Point,double> mapGn;
    mapGn[B] = 0;
    while(!openTbl.empty())
    {
        Point p = openTbl.back();
        openTbl.pop_back();
        if (p == E)
        {
            path.push_back(E);
            break;
        }
        multimap<double,Point> FnPoint;
        multimap<double,Point> HnPoint;    //��ǰλ����Χ���п�ѡ���λ�õ��յ��Hn
        map<Point,double> mapGnTemp;    //��ǰλ����Χ���п�ѡ���λ�õ�����Gn

        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,-1,-1);        //����λ��
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,-1,0);        //���λ��
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,-1,1);        //����λ��
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,0,-1);        //����λ��
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,0,1);            //����λ��
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,1,-1);        //����λ��
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,1,0);            //�ұ�λ��
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,1,1);            //����λ��
        //HnPoint�ĵ�һά���Ǵ�p�㿪ʼ�����յ㣨Hn�����Ч����Χ�����pt
        double hn = HnPoint.begin()->first;
        Point pt = HnPoint.begin()->second;

        //�ж�pt�Ƿ�̽���
        map<Point,double>::iterator itrFind = mapGn.find(pt);
        if (itrFind != mapGn.end() && (itrFind->second) < mapGnTemp[pt])
        {
            /**************************************************************************************
            pt�Ѿ���̽���������֮ǰ��Gn��С��˵�������ø�С�Ĵ��۵���pt��
            ����˵������֮ǰѡ���p�㲻�����ѡ������Ӧ�ñ��p��Ч��Ȼ����˵�֮ǰ����������ѡ��
            ****************************************************************************************/

            //��Ǹõ��Ѿ���̽�⣬ʹ���´β��ٱ�ѡ��
            closeTbl.insert(p);
            //����һ��������̽��֮ǰ�߹���һ����
            p = path.back();
            path.pop_back();
            closeTbl.erase(p);
            openTbl.push_back(p);
            continue;
        }    

        //ptû�б�̽���������������ѡ����Խ�p����·����Ȼ����һ��̽��pt 
        openTbl.push_back(pt);

        closeTbl.insert(p);
        path.push_back(p);

    }    
}


int main3(int argc, char * argv[])
{
    const Point B(4,5),E(36,4);

    vector<Point> path;
    APath(B,E,path);
    PrintMap(B,E,path);

    return 0;
}

//������ע�����滭�ĵ�ͼ��Ϊ�˼���ʾ��ͼ���յ���ʾЧ������ʵ�����ά��������ĵ�ͼ��Ȥ����


//ԭ����
//A*��A-Star)�㷨��һ�־�̬·����������·����Ч�ķ�����
// ��ʽ��ʾΪ�� f(n)=g(n)+h(n),
// ���� f(n) �Ǵӳ�ʼ�㾭�ɽڵ�n��Ŀ���Ĺ��ۺ�����
// g(n) ����״̬�ռ��дӳ�ʼ�ڵ㵽n�ڵ��ʵ�ʴ��ۣ�
// h(n) �Ǵ�n��Ŀ��ڵ����·���Ĺ��ƴ��ۡ�
// ��֤�ҵ����·�������Ž�ģ��������ؼ����ڹ��ۺ���h(n)��ѡȡ��
// ����ֵh(n)<= n��Ŀ��ڵ�ľ���ʵ��ֵ����������£������ĵ����࣬������Χ��Ч�ʵ͡����ܵõ����Ž⡣
// ��� ����ֵ>ʵ��ֵ, �����ĵ����٣�������ΧС��Ч�ʸߣ������ܱ�֤�õ����Ž⡣
// http://baike.baidu.com/view/7850.htm