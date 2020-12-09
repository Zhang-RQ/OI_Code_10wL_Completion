#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXN=1E5+10;

struct edge_t{
	int v,w,nxt;
};

struct Graph{
	edge_t Edge[MAXN];
	int head[MAXN],cnt_e;

	int& operator () (int x){return head[x];}
	edge_t& operator [] (int x){return Edge[x];}

	void add(int u,int v,int w)
	{
		Edge[++cnt_e]=(edge_t){v,w,head[u]};
		head[u]=cnt_e;
	}	
}G,Gr;

map<pair<int,int>,int> M;
int n,vis[MAXN],inR[MAXN],ins[MAXN],stk[MAXN],top,a[MAXN];
ll f[MAXN][2],g[MAXN][20],jmp[MAXN][20],Ans;

void find_ring(int x)
{
	stk[++top]=x;ins[x]=1;
	for(int _=G(x);_;_=G[_].nxt)
	{
		int v=G[_].v;
		if(ins[v]) throw v;
		find_ring(v);
	}
	--top;ins[x]=0;
}

void dfs(int x)
{
	vis[x]=1;
	for(int _=Gr(x);_;_=Gr[_].nxt)
	{
		int v=Gr[_].v;
		if(inR[v]) continue;
		dfs(v);
		f[x][0]+=max(f[v][0],f[v][1]);
	}
	for(int _=Gr(x);_;_=Gr[_].nxt)
	{
		int v=Gr[_].v;
		if(!inR[v])
			f[x][1]=max(f[x][1],f[x][0]+Gr[_].w);
	}
}

void solve(int x)
{
	int cnt=0,tot=0;
	ll tAns=0;
	try{
		top=0;
		find_ring(x);
	}catch(int beg){
		bool f=0;
		for(int i=1;i<=top;i++)
		{
			f|=stk[i]==beg;
			if(f) a[++cnt]=stk[i],inR[stk[i]]=1;
		}
	}
	if(cnt==n) puts("0"),exit(0);
	tot=2*cnt;
	for(int i=1;i<=cnt;i++)
		a[i+cnt]=a[i];
	for(int i=1;i<=cnt;i++)
		dfs(a[i]);
	for(int i=1;i<=tot;i++)
	{
		g[i][0]=max(f[a[i+1]][0]+M[{a[i],a[i+1]}],max(f[a[i+1]][1],f[a[i+1]][0])),
		jmp[i][0]=i+1;
	}
	for(int j=1;j<=19;j++)
		for(int i=1;i<=tot;i++)
			jmp[i][j]=jmp[jmp[i][j-1]][j-1],
			g[i][j]=g[i][j-1]+g[jmp[i][j-1]][j-1];
	int stp=cnt-1;
	for(int i=1;i<=cnt;i++)
	{
		int cur=i;
		ll tmp=max(f[a[i]][0],f[a[i]][1]);
		for(int j=19;~j;j--)
			if((stp>>j)&1)
				tmp+=g[cur][j],cur=jmp[cur][j];
		tAns=max(tAns,tmp);
	}
	Ans+=tAns;
}

int main()
{
	ll sum=0;
	scanf("%d",&n);
	for(int i=1,a,w;i<=n;i++)
	{
		scanf("%d%d",&a,&w);
		G.add(i,a,w);
		Gr.add(a,i,w);
		M[{i,a}]=w;
		sum+=w;
	}
	for(int i=1;i<=n;i++)
		if(!vis[i])
			solve(i);
	printf("%lld\n",sum-Ans);
}
