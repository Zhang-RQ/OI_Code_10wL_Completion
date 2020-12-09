#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXM=1E6+10;
const int MAXN=5E6+10;

struct edge_t{
	int v,nxt;
}Edge[MAXN*4];

char str[MAXM];
int stk[MAXN],top,ins[MAXN],SCC,bl[MAXN],dft,Dfn[MAXN],Dft,Siz[MAXN],ppid[MAXN][2];
int cnt_e,head[MAXN],son[MAXM][2],cnt=1,rt=1,tot,n,ed[MAXM][2],id[MAXN][2],sid[MAXM][2],dfn[MAXN],low[MAXN];//0 up,1 down
vector<int> pre[MAXM][2],suf[MAXM][2];
vector<pair<int,int> > pid[MAXM];

void add(int u,int v)
{
	Edge[++cnt_e]=(edge_t){v,head[u]};
	head[u]=cnt_e;
}

void insert(int id,int tp)
{
	int x=rt;
	for(int i=1;str[i];i++)
	{
		if(!son[x][str[i]-'0']) son[x][str[i]-'0']=++cnt;
		x=son[x][str[i]-'0'];
	}
	pid[x].emplace_back(id,tp);
	ed[id][tp]=x;
	ppid[id][tp]=pid[x].size()-1;
}

void build()
{
	for(int i=1;i<=n;i++)
		sid[i][0]=++tot,sid[i][1]=++tot;
	for(int i=1;i<=cnt;i++)
		id[i][0]=++tot,id[i][1]=++tot;
	for(int i=1;i<=cnt;i++)
	{
		if(son[i][0]) add(id[son[i][0]][0],id[i][0]),add(id[i][1],id[son[i][0]][1]);
		if(son[i][1]) add(id[son[i][1]][0],id[i][0]),add(id[i][1],id[son[i][1]][1]);
	}
	for(int i=1;i<=cnt;i++)
		if(pid[i].size())
		{
			int k=pid[i].size();
			pre[i][0].resize(k);pre[i][1].resize(k);
			suf[i][0].resize(k);suf[i][1].resize(k);
			for(int j=0;j<k;j++)
			{
				pre[i][0][j]=++tot;pre[i][1][j]=++tot;
				if(j) add(pre[i][0][j-1],pre[i][0][j]),add(pre[i][1][j],pre[i][1][j-1]);
				add(sid[pid[i][j].first][pid[i][j].second],pre[i][0][j]),add(pre[i][1][j],sid[pid[i][j].first][pid[i][j].second^1]);
			}
			for(int j=k-1;~j;j--)
			{
				suf[i][0][j]=++tot;suf[i][1][j]=++tot;
				if(j!=k-1) add(suf[i][0][j+1],suf[i][0][j]),add(suf[i][1][j],suf[i][1][j+1]);
				add(sid[pid[i][j].first][pid[i][j].second],suf[i][0][j]),add(suf[i][1][j],sid[pid[i][j].first][pid[i][j].second^1]);
			}
			add(suf[i][0][0],id[i][0]);add(id[i][1],suf[i][1][0]);
		}
	for(int i=1;i<=n;i++)
	{
		int x=ed[i][0],y=ed[i][1],x0=son[ed[i][0]][0],y0=son[ed[i][0]][1],x1=son[ed[i][1]][0],y1=son[ed[i][1]][1];
		if(x0) add(sid[i][0],id[x0][1]),add(id[x0][0],sid[i][1]);
		if(y0) add(sid[i][0],id[y0][1]),add(id[y0][0],sid[i][1]);
		if(x1) add(sid[i][1],id[x1][1]),add(id[x1][0],sid[i][0]);
		if(y1) add(sid[i][1],id[y1][1]),add(id[y1][0],sid[i][0]);
		if(ppid[i][0]) add(sid[i][0],pre[x][1][ppid[i][0]-1]),add(pre[x][0][ppid[i][0]-1],sid[i][1]);
		if(ppid[i][0]!=pid[x].size()-1) add(sid[i][0],suf[x][1][ppid[i][0]+1]),add(suf[x][0][ppid[i][0]+1],sid[i][1]);
		if(ppid[i][1]) add(sid[i][1],pre[y][1][ppid[i][1]-1]),add(pre[y][0][ppid[i][1]-1],sid[i][0]);
		if(ppid[i][1]!=pid[y].size()-1) add(sid[i][1],suf[y][1][ppid[i][1]+1]),add(suf[y][0][ppid[i][1]+1],sid[i][0]);
	}
	
}

void tarjan(int x)
{
	dfn[x]=low[x]=++dft;
	ins[x]=1;stk[++top]=x;
	for(int _=head[x];_;_=Edge[_].nxt)
	{
		int v=Edge[_].v;
		if(!dfn[v]) tarjan(v),low[x]=min(low[x],low[v]);
		else if(ins[v]) low[x]=min(low[x],dfn[v]);
	}
	if(low[x]==dfn[x])
	{
		++SCC;
		while(stk[top]!=x)
		{
			bl[stk[top]]=SCC;
			ins[stk[top]]=0;
			--top;
		}
		--top;
		bl[x]=SCC;
		ins[x]=0;
	}
}

int main()
{
	scanf("%d",&n);
	for(int _=1;_<=n;_++)
	{
		int pos=0,len;
		scanf("%s",str+1);
		len=strlen(str+1);
		for(int i=1;i<=len;i++)
			if(str[i]=='?') pos=i;
		str[pos]='0';insert(_,0);
		str[pos]='1';insert(_,1);
	}
	build();
	for(int i=1;i<=tot;i++) if(!dfn[i]) tarjan(i);
	for(int i=1;i<=n;i++) if(bl[sid[i][0]]==bl[sid[i][1]]) return puts("NO"),0;
	puts("YES");
}
