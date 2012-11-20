//�����ȶ��У��ѣ�ʵ�ֵ�Dijkstra�㷨�����·�����е���Ȩͼ���ã����ڳ���ͼ����Ƚ����㡣
//Ϊ�˷���������Ұ�pair<int, int>����һ��pii���ͣ����Ҷ�����һ����Ԫ�����ȶ��У�first������ʼ�ڵ㵽�˽ڵ�ľ��룬second��ʾ�ýڵ�ı�š��洢ͼ�ķ�ʽ�Ҳ�����ָ��ʵ�ֵ��ڽӱ�
//��ݴ���ĺ��Ĵ��벻�ֻ࣬�ж��У�STL��Ȼ����vector��priority_queue��pair����������û��-o2���Ż���Ч�ʻ����б�֤�ģ�����pair����΢��һ�㡣
//�������ҵĴ��룺
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


//����
//����Dijkstra(�Ͻ�˹����)�㷨�ǵ��͵ĵ�Դ���·���㷨�����ڼ���һ���ڵ㵽�������нڵ�����·������Ҫ�ص�������ʼ��Ϊ������������չ��ֱ����չ���յ�Ϊֹ��Dijkstra�㷨�Ǻ��д����Ե����·���㷨���ںܶ�רҵ�γ��ж���Ϊ������������ϸ�Ľ��ܣ������ݽṹ��ͼ�ۣ��˳�ѧ�ȵȡ�Dijkstraһ��ı���ͨ�������ַ�ʽ��һ�������ú���ʱ��ŷ�ʽ��һ������OPEN, CLOSE��ķ�ʽ��������������ú���ʱ��ŵķ�ʽ��ע����㷨Ҫ��ͼ�в����ڸ�Ȩ�ߡ�
//��������
//  ��������ͼ G=(V,E) �У�����ÿ���� E[i] �ĳ���Ϊ w[i]���ҵ��ɶ��� V0 �������������·��������Դ���·����

//�Ͻ�˹�����㷨
//�Ͻ�˹����(Dijkstra)�㷨˼�롡��
//��·�����ȵ�������������·���㷨��
//������V�ֳ����飺
//������1��S����������·���Ķ���ļ���
//������2��V-S=T����δȷ�����·���Ķ��㼯��
//������T�ж��㰴���·�������Ĵ�����뵽S�У�
//������֤����1����Դ��V0��S�и���������·�����ȶ�������
//������V0��T���κζ�������·������
//������2��ÿ�������Ӧһ������ֵ
//����S�ж��㣺��V0���˶�������·������
//����T�ж��㣺��V0���˶����ֻ����S�ж������м�
//������������·������
//�������ݣ�����֤��V0��T�ж���Vk�����·�������Ǵ�V0��Vk��
//����ֱ��·����Ȩֵ�����Ǵ�V0��S�ж��㵽Vk��·��Ȩֵ֮��
//��������֤����֤��
//���������·������
//�����㷨�������£�
//����1. ��ʹʱ�� S={V0},T={���ඥ��}��T�ж����Ӧ�ľ���ֵ
//����������<V0,Vi>��d(V0,Vi)Ϊ<V0,Vi>���ϵ�Ȩֵ
//������������<V0,Vi>��d(V0,Vi)Ϊ��
//����2. ��T��ѡȡһ�������ֵΪ��С�Ķ���W�Ҳ���S�У�����S
//����3. ������T�ж���ľ���ֵ�����޸ģ����ӽ�W���м䶥�㣬��V0��Vi��
//��������ֵ���̣����޸Ĵ˾���ֵ
//�����ظ���������2��3��ֱ��S�а������ж��㣬��W=ViΪֹ
//http://baike.baidu.com/view/7839.htm

