#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

mt19937 RandEngine(chrono::steady_clock::now().time_since_epoch().count());

inline int RandInt(int l,int r) {return uniform_int_distribution<int>(l,r)(RandEngine);}
inline double RandDouble(double l,double r) {return uniform_real_distribution<double>(l,r)(RandEngine);}

const int MAXN=2E5+10;

int n,fix[MAXN],rt,cnt;
int son[MAXN][2],siz[MAXN],tot;
ll val[MAXN],tg[MAXN],a[MAXN];

int nd(ll v)
{
	++cnt;
	fix[cnt]=RandInt(-1E9,1E9);
	val[cnt]=v;siz[cnt]=1;
	return cnt;
}

void pushup(int x){siz[x]=siz[son[x][0]]+siz[son[x][1]]+1;}

void apply_tag(int x,ll dlt){val[x]+=dlt;tg[x]+=dlt;}

void pushdown(int x)
{
	if(tg[x])
	{
		apply_tag(son[x][0],tg[x]);
		apply_tag(son[x][1],tg[x]);
		tg[x]=0;
	}
}

int find(int x,int rk,int lim)
{
	if(!x)
		return 0;
	pushdown(x);
	if(val[x]>1ll*lim*rk)
	{
		int t=find(son[x][1],rk+siz[son[son[x][1]][0]]+1,lim);
		return t?siz[son[x][0]]+1+t:0;
	}
	else
	{
		int t=find(son[x][0],rk-siz[son[x][0]]+siz[son[son[x][0]][0]],lim);
		return t?t:siz[son[x][0]]+1;
	}
}

int merge(int x,int y)
{
	if(!x||!y)
		return x|y;
	pushdown(x);pushdown(y);
	if(fix[x]<fix[y])
		return son[x][1]=merge(son[x][1],y),pushup(x),x;
	else
		return son[y][0]=merge(x,son[y][0]),pushup(y),y;
}

pair<int,int> split(int x,int k)
{
	if(!x)
		return make_pair(0,0);
	pushdown(x);
    if(siz[son[x][0]]+1<=k)
    {
        pair<int,int> t=split(son[x][1],k-siz[son[x][0]]-1);
        son[x][1]=t.first;pushup(x);
        return make_pair(x,t.second);
    }
    else
    {
        pair<int,int> t=split(son[x][0],k);
        son[x][0]=t.second;pushup(x);
        return make_pair(t.first,x);
    }
}

void dfs(int x)
{
	pushdown(x);
	if(son[x][0])
		dfs(son[x][0]);
	a[++tot]=val[x];
	if(son[x][1])
		dfs(son[x][1]);
}

int main()
{
	scanf("%d",&n);
	for(int i=1,x;i<=n;i++)
	{
		scanf("%d",&x);
		int pos=find(rt,siz[son[rt][0]]+1,x);
		if(!pos)
			rt=merge(rt,nd(1ll*i*x));
		else
		{
			pair<int,int> t=split(rt,pos-1);
			apply_tag(t.second,x);
			rt=merge(t.first,merge(nd(1ll*pos*x),t.second));
		}
	}
	dfs(rt);
	for(int i=1;i<=tot;i++)
		a[i]+=a[i-1];
	for(int i=1;i<=tot;i++)
		printf("%lld%c",a[i],"\n "[i!=tot]);
}
