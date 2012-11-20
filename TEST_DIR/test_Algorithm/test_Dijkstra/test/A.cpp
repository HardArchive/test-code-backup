// A*算法实现
//
//最近实现了一下A*算法，感觉蛮好的，尤其是修改地图然后看电脑正确寻路后的那种成就感，有点像小时候蹲在地上，看着一堆蚂蚁搬家，然后故意在他们的路上设置障碍物，然后看蚂蚁不停的探索，然后重新找到新的路线的感觉，真是很有意思。
//好！把代码记录在此，便于以后参考。
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

/************************************************************************/
/* A*算法实现，测试A*算法地图      
40X10的地图，B代表起始点，E代表终点；                                              
空格代表能通过；|代表是墙，不能通过；
xx0123456789012345678901234567890123456789xx
xx————————————————————xx
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
xx————————————————————xx
xx0123456789012345678901234567890123456789xx
/************************************************************************/

const static int X = 40;
const static int Y = 10;
//地图上的情况
enum E_Map
{
    E_River=-2,            //有河流，无法通过，在地图上用 ~ 标出
    E_Wall=-1,            //有墙，无法通过，在地图上用 | 标出    
    E_Road = 0,            //没有障碍物，能最快的顺利通行，代价为0，在地图上用空格标出    
    E_Sand=1,            //是沙地，能通过，但是相对难一些，代价为1，在地图上用 * 标出
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


//打印地图
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
    //路径
    vector<Point>::const_iterator itr = path.begin();
    for (;itr != path.end();++itr)
    {
        LastMap[(*itr).y][(*itr).x] = '&';
    }
    //起始和目标
    LastMap[B.y][B.x] = 'B';
    LastMap[E.y][E.x] = 'E';

    cout<<"A*寻路路径为："<<endl;
    cout<<"xx0123456789012345678901234567890123456789xx"<<endl;
    cout<<"xx————————————————————xx"<<endl;
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
    cout<<"xx————————————————————xx"<<endl;
    cout<<"xx0123456789012345678901234567890123456789xx"<<endl;
}

//计算当前位置与终点位置的Hn
double Hn(const Point & E,const Point&p)
{
    return abs(E.y-p.y) + abs(E.x-p.x);
}

//计算相邻两个位置(即包括对象线在内的周围8个坐标)的相对Gn
double Gg(const Point & p1,const Point& p2)
{
    double d = GameMap[p2.y][p2.x];
    return ((p1.x-p2.x)!=0&&(p1.y-p2.y)!=0)?(1.5+d):(1.0+d);
}

//探测位置p的下一步(p.x + addx,p.y + addy)的Gn,Hn
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
        //得到对应本次遍历的Gn
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
    //A*算法：Fn = Gn + Hn
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
        multimap<double,Point> HnPoint;    //当前位置周围所有可选择的位置到终点的Hn
        map<Point,double> mapGnTemp;    //当前位置周围所有可选择的位置到起点的Gn

        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,-1,-1);        //左上位置
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,-1,0);        //左边位置
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,-1,1);        //左下位置
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,0,-1);        //上面位置
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,0,1);            //下面位置
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,1,-1);        //右上位置
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,1,0);            //右边位置
        testNext(E,p,closeTbl,mapGn,mapGnTemp,HnPoint,1,1);            //右下位置
        //HnPoint的第一维就是从p点开始到达终点（Hn）最高效的周围坐标点pt
        double hn = HnPoint.begin()->first;
        Point pt = HnPoint.begin()->second;

        //判断pt是否被探测过
        map<Point,double>::iterator itrFind = mapGn.find(pt);
        if (itrFind != mapGn.end() && (itrFind->second) < mapGnTemp[pt])
        {
            /**************************************************************************************
            pt已经被探测过，并且之前的Gn更小，说明可以用更小的代价到达pt。
            所以说明我们之前选择的p点不是最佳选择，首先应该标记p无效，然后回退到之前的坐标重新选择！
            ****************************************************************************************/

            //标记该点已经被探测，使得下次不再被选择
            closeTbl.insert(p);
            //回退一步，重新探测之前走过的一个点
            p = path.back();
            path.pop_back();
            closeTbl.erase(p);
            openTbl.push_back(p);
            continue;
        }    

        //pt没有被探测过，或者是最优选项，所以将p加入路径，然后下一步探测pt 
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

//最上面注释里面画的地图是为了简单演示地图最终的显示效果，其实代码二维数组给出的地图有趣多了


//原理简介
//A*（A-Star)算法是一种静态路网中求解最短路最有效的方法。
// 公式表示为： f(n)=g(n)+h(n),
// 其中 f(n) 是从初始点经由节点n到目标点的估价函数，
// g(n) 是在状态空间中从初始节点到n节点的实际代价，
// h(n) 是从n到目标节点最佳路径的估计代价。
// 保证找到最短路径（最优解的）条件，关键在于估价函数h(n)的选取：
// 估价值h(n)<= n到目标节点的距离实际值，这种情况下，搜索的点数多，搜索范围大，效率低。但能得到最优解。
// 如果 估价值>实际值, 搜索的点数少，搜索范围小，效率高，但不能保证得到最优解。
// http://baike.baidu.com/view/7850.htm