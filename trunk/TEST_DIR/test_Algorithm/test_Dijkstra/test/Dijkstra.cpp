//用优先队列（堆）实现的Dijkstra算法，最短路问题中的正权图适用，对于稠密图计算比较优秀。
//为了方便起见，我把pair<int, int>定义一个pii类型，并且定义了一个二元的优先队列，first代表起始节点到此节点的距离，second表示该节点的编号。存储图的方式我采用了指针实现的邻接表。
//这份代码的核心代码不多，只有多行，STL虽然用了vector、priority_queue、pair，但是相信没有-o2的优化下效率还是有保证的，可能pair会稍微慢一点。
//下面是我的代码：
#include <cstdio>
#include <cstring>
#include <utility>
#include <queue>
using namespace std;

#define maxn 10008
#define INF 10000008

typedef struct G_NODE
{
	int u, v, w;
	struct G_NODE *next;
} Gnode;

int n, m, d[maxn];
Gnode *a[maxn];

void add_edge(int x, int y, int c)
{
	Gnode *p = new Gnode;
	p->u = x;
	p->v = y;
	p->w = c;
	p->next = a[x]->next;
	a[x]->next = p;
}

void init_g()
{
	scanf("%d %d", &n, &m);
	for (int i = 1; i <= n; ++i)
	{
		a[i] = new Gnode;
		a[i]->next = NULL;
	}
	for (int i = 0; i < m; ++i)
	{
		int x, y, c;
		scanf("%d %d %d", &x, &y, &c);
		add_edge(x, y, c);
		add_edge(y, x, c);
	}
}

typedef pair<int, int> pii;
priority_queue<pii, vector<pii>, greater<pii> > q;
bool done[maxn];

void dijkstra(int s)
{
	Gnode *p;

	memset(done, false, sizeof(done));
	for (int i = 1; i <= n; ++i)
		d[i] = (i == s ? 0 : INF);

	q.push(make_pair(d[s], s));
	while (!q.empty())
	{
		pii u = q.top();
		q.pop();
		int x = u.second;
		if (done[x])
			continue;
		for (p = a[x]->next; p; p = p->next) if (d[p->v] > d[x] + p->w)
		{
			d[p->v] = d[x] + p->w;
			q.push(make_pair(d[p->v], p->v));
		}
	}
}

int main()
{
	freopen("g.in", "r", stdin);
	freopen("g.out", "w", stdout);

	init_g();
	dijkstra(1);
	for (int i = 1; i <= n; ++i)
		printf("%d ", d[i]);
	printf("\n");

	return 0;
}


//定义
//　　Dijkstra(迪杰斯特拉)算法是典型的单源最短路径算法，用于计算一个节点到其他所有节点的最短路径。主要特点是以起始点为中心向外层层扩展，直到扩展到终点为止。Dijkstra算法是很有代表性的最短路径算法，在很多专业课程中都作为基本内容有详细的介绍，如数据结构，图论，运筹学等等。Dijkstra一般的表述通常有两种方式，一种用永久和临时标号方式，一种是用OPEN, CLOSE表的方式，这里均采用永久和临时标号的方式。注意该算法要求图中不存在负权边。
//问题描述
//  　在无向图 G=(V,E) 中，假设每条边 E[i] 的长度为 w[i]，找到由顶点 V0 到其余各点的最短路径。（单源最短路径）

//迪杰斯特拉算法
//迪杰斯特拉(Dijkstra)算法思想　　
//按路径长度递增次序产生最短路径算法：
//　　把V分成两组：
//　　（1）S：已求出最短路径的顶点的集合
//　　（2）V-S=T：尚未确定最短路径的顶点集合
//　　将T中顶点按最短路径递增的次序加入到S中，
//　　保证：（1）从源点V0到S中各顶点的最短路径长度都不大于
//　　从V0到T中任何顶点的最短路径长度
//　　（2）每个顶点对应一个距离值
//　　S中顶点：从V0到此顶点的最短路径长度
//　　T中顶点：从V0到此顶点的只包括S中顶点作中间
//　　顶点的最短路径长度
//　　依据：可以证明V0到T中顶点Vk的最短路径，或是从V0到Vk的
//　　直接路径的权值；或是从V0经S中顶点到Vk的路径权值之和
//　　（反证法可证）
//　　求最短路径步骤
//　　算法步骤如下：
//　　1. 初使时令 S={V0},T={其余顶点}，T中顶点对应的距离值
//　　若存在<V0,Vi>，d(V0,Vi)为<V0,Vi>弧上的权值
//　　若不存在<V0,Vi>，d(V0,Vi)为∝
//　　2. 从T中选取一个其距离值为最小的顶点W且不在S中，加入S
//　　3. 对其余T中顶点的距离值进行修改：若加进W作中间顶点，从V0到Vi的
//　　距离值缩短，则修改此距离值
//　　重复上述步骤2、3，直到S中包含所有顶点，即W=Vi为止
//http://baike.baidu.com/view/7839.htm

