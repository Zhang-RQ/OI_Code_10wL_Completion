#pragma GCC optimize("O2,Ofast,inline,unroll-all-loops,-ffast-math")
#pragma GCC target("avx,sse2,sse3,sse4,popcnt")

#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

mt19937 RandEngine(chrono::steady_clock::now().time_since_epoch().count());

inline int RandInt(int l,int r) {return uniform_int_distribution<int>(l,r)(RandEngine);}
inline double RandDouble(double l,double r) {return uniform_real_distribution<double>(l,r)(RandEngine);}

const int MAXN=5E4+10;

int n,son[MAXN][2],siz[MAXN],tms[MAXN],fix[MAXN],fa[MAXN],Rt;
int FA[MAXN];

int find(int x){return x==FA[x]?x:FA[x]=find(FA[x]);}

int Rank(int p)
{
	int ret=siz[son[p][0]]+tms[p];
	while(fa[p])
	{
		if(son[fa[p]][1]==p)
			ret+=siz[son[fa[p]][0]]+tms[fa[p]];
		p=fa[p];
	}
	return ret;
}

struct dat{
	int l,r;

	bool operator < (const dat &rhs) const
		{
			int L1=Rank(l),L2=Rank(rhs.l);
			if(L1==L2) return Rank(r)<Rank(rhs.r);
			else return L1<L2;
		}

	bool operator == (const dat &rhs) const
		{
			if(l==rhs.l&&r==rhs.r) return true;
			return Rank(l)==Rank(rhs.l)&&Rank(r)==Rank(rhs.r);
		}
}val[MAXN];

void pushup(int x)
{
	siz[x]=tms[x];
	if(son[x][0])
	{
		siz[x]+=siz[son[x][0]];
		fa[son[x][0]]=x;
	}
	if(son[x][1])
	{
		siz[x]+=siz[son[x][1]];
		fa[son[x][1]]=x;
	}
}

int merge(int x,int y)
{
	if(!x||!y) return x|y;
	if(fix[x]>fix[y])
		return son[x][1]=merge(son[x][1],y),pushup(x),x;
	else
		return son[y][0]=merge(x,son[y][0]),pushup(y),y;
}

pair<int,int> split(int x,int k)
{
    if(!x) return make_pair(0,0);
    if(siz[son[x][0]]+tms[x]<=k)
    {
        pair<int,int> t=split(son[x][1],k-siz[son[x][0]]-tms[x]);
        son[x][1]=t.first;pushup(x);
		fa[t.second]=0;
        return make_pair(x,t.second);
    }
    else
    {
        pair<int,int> t=split(son[x][0],k);
        son[x][0]=t.second;pushup(x);
		fa[t.first]=0;
        return make_pair(t.first,x);
    }
}

int Try_Insert_Eq(int x,dat v)
{
	if(!x) return 0;
	if(val[x]==v) return tms[x]++,pushup(x),x;
	int ret=0;
	if(v<val[x])
		ret=Try_Insert_Eq(son[x][0],v);
	else
		ret=Try_Insert_Eq(son[x][1],v);
	pushup(x);
	return ret;
}

int Lower_bound(int x,dat v) //return Rank
{
	if(!x) return 0;
	assert(!(v==val[x]));
	if(v<val[x]) return Lower_bound(son[x][0],v);
	else return Lower_bound(son[x][1],v)+siz[son[x][0]]+tms[x];
}

int main()
{
	scanf("%d",&n);
	val[1]=(dat){1,1};
	val[n+2]=(dat){n+2,n+2};
	Rt=n+2;
	tms[1]=tms[n+2]=1;
	fix[1]=RandInt(1,1E9),fix[n+2]=RandInt(1,1E9);
	son[Rt][0]=1;fa[1]=Rt;
	pushup(1);pushup(n+2);
	for(int i=1;i<=n+2;i++)
		FA[i]=i;
	for(int i=2,u,v;i<=n+1;i++)
	{
		scanf("%d%d",&u,&v);
		++u;++v;
		u=find(u);v=find(v);
		dat tmp=(dat){u,v};
		int p=Try_Insert_Eq(Rt,tmp);
		if(p) {FA[i]=p;printf("%d\n",Rank(p));continue;}
		val[i]=tmp,tms[i]=1,fix[i]=RandInt(1,1E9);
		pushup(i);
		int siz=Lower_bound(Rt,tmp);
		pair<int,int> pr=split(Rt,siz);
		Rt=merge(pr.first,merge(i,pr.second));
		printf("%d\n",siz+1);
	}
}
