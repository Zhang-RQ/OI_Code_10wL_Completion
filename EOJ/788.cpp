#pragma GCC optimize("O2,Ofast,inline,unroll-all-loops,-ffast-math")
#pragma GCC target("avx,sse2,sse3,sse4,popcnt")

#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

namespace FastIO{
    inline char get_char()
    {
        static char buf[1000001],*p1=buf,*p2=buf;
        return p1==p2&&(p2=(p1=buf)+fread(buf,1,1000000,stdin),p1==p2)?EOF:*p1++;
    }

    template<typename T>
        inline void read(T& x)
	{
		x=0;static char ch=get_char();
		while(!isdigit(ch)) ch=get_char();
		while(isdigit(ch)) x=x*10+(ch^'0'),ch=get_char();
	}

#if __cplusplus >= 201103L
	template<typename T,typename ...Args>
		inline void read(T& x,Args& ...args)
	{
		read(x);
		read(args...);
	}
#endif
}

using namespace FastIO;

using namespace std;

#define double float

const int MAXN=150;
const int MAXM=1E5+10;
const int S=0;
const int T=MAXN-1;
const double EPS=1E-8;

int n,m,k,pnt[MAXN],d[MAXN],cnt,U[MAXM],V[MAXM],eid[MAXM],tote[MAXN];
double f[MAXN],val[MAXM];
vector<int> G[MAXN];
vector<pair<int,int> > v[MAXN];

namespace Flow{
	struct edge_t{
		int v,nxt;
		double f;
	}Edge[MAXM];

	int cnt_e=1,head[MAXN];
	int lvl[MAXN];
	
	void clear(){cnt_e=1;memset(head,0,sizeof head);}

	void add(int u,int v,double f)
	{
		Edge[++cnt_e]=(edge_t){v,head[u],f};
		head[u]=cnt_e;
	}

	void add2(int u,int v,double f){add(u,v,f);add(v,u,0);}

	bool bfs()
	{
		memset(lvl,0,sizeof lvl);
		lvl[S]=1;
		queue<int> q;
		q.push(S);
		while(!q.empty())
		{
			int x=q.front();q.pop();
			for(int i=head[x];i;i=Edge[i].nxt)
			{
				int v=Edge[i].v;
				if(abs(Edge[i].f)<EPS||lvl[v])
					continue;
				lvl[v]=lvl[x]+1;
				q.push(v);
			}
		}
		return lvl[T]!=0;
	}

	double dfs(int x,double curf)
	{
		if(x==T) return curf;
		double adf=0;
		for(int i=head[x];i;i=Edge[i].nxt)
		{
			int v=Edge[i].v;
			if(abs(Edge[i].f)<EPS||lvl[v]!=lvl[x]+1)
				continue;
			double mnf=dfs(v,min(curf-adf,Edge[i].f));
			if(abs(mnf)<EPS) lvl[v]=0;
			adf+=mnf;
			Edge[i].f-=mnf;
			Edge[i^1].f+=mnf;
		}
		return adf;
	}

	double Max_Flow()
	{
		double Ret=0;
		while(bfs())
			Ret+=dfs(S,1E9);
		return Ret;
	}
}

int main()
{
	read(n,m,k);
	for(int i=1;i<=m;i++)
	{
		read(U[i],V[i]);
		G[U[i]].push_back(V[i]),++d[V[i]];
		eid[i]=++tote[U[i]];
	}
	for(int i=1,x,y;i<=k;i++)
	{
		read(x,y);
		v[U[x]].emplace_back(x,y);
	}
	queue<int> q;
	for(int i=1;i<=n;i++)
		if(!d[i])
			q.push(i);
	while(!q.empty())
	{
		int x=q.front();q.pop();
		pnt[++cnt]=x;
		for(int v:G[x])
			if(!--d[v])
				q.push(v);
	}
	reverse(pnt+1,pnt+1+n);
	for(int i=1;i<=n;i++)
	{
		int x=pnt[i];
		if(!G[x].size())
			continue;
		double L=0,R=50;
		int Tst=8;
		while(Tst--)
		{
			Flow::clear();
			double Mid=(L+R)/2,Ans=0;
			int a=0;
			for(int v:G[x])
				val[++a]=f[v]-Mid;
			for(int o=1;o<=a;o++)
				if(val[o]>0)
					Flow::add2(S,o,val[o]),Ans+=val[o];
				else
					Flow::add2(o,T,-val[o]);
			for(auto p:v[x])
				Flow::add2(eid[p.first],eid[p.second],1E9);
			Ans-=Flow::Max_Flow();
			if(Ans<EPS) R=Mid;
			else L=Mid;
		}
		f[x]=L+1;
	}
	cout<<fixed<<setprecision(10)<<f[1]<<endl;
}
