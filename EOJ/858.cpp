#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

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

const int MAXN=4E5+10;

int D;

struct node_t{
	int l,r,del,id,vis;
	int d[2],x[2],y[2];
}t[MAXN<<1];

struct pnt_t{
	int d[2],id;

	bool operator < (const pnt_t &rhs) const {return d[D]<rhs.d[D];} 
}a[MAXN];

int rt1,rt2,cnt,n,dfn1[MAXN],dfn2[MAXN],dfne1[MAXN],dfne2[MAXN],dft1,dft2,fa1[MAXN],fa2[MAXN];
int dep1[MAXN],dep2[MAXN];
int res[MAXN],tot,bak[MAXN];
vector<int> T1[MAXN],T2[MAXN];

bool full_in(const node_t &x,int xl,int xr,int yl,int yr) //x in rect
{
	return xl<=x.x[0]&&x.x[1]<=xr&&yl<=x.y[0]&&x.y[1]<=yr;
}

bool intersect(const node_t &x,int xl,int xr,int yl,int yr)//have intersect
{
	return !(x.x[1]<xl||x.x[0]>xr||x.y[1]<yl||x.y[0]>yr);
}

void pushup(int x)
{
	t[x].x[0]=t[x].x[1]=t[x].d[0];
	t[x].y[0]=t[x].y[1]=t[x].d[1];
	if(!t[t[x].l].del)
		t[x].x[0]=min(t[x].x[0],t[t[x].l].x[0]),
		t[x].x[1]=max(t[x].x[1],t[t[x].l].x[1]),
		t[x].y[0]=min(t[x].y[0],t[t[x].l].y[0]),
		t[x].y[1]=max(t[x].y[1],t[t[x].l].y[1]);
	if(!t[t[x].r].del)
		t[x].x[0]=min(t[x].x[0],t[t[x].r].x[0]),
		t[x].x[1]=max(t[x].x[1],t[t[x].r].x[1]),
		t[x].y[0]=min(t[x].y[0],t[t[x].r].y[0]),
		t[x].y[1]=max(t[x].y[1],t[t[x].r].y[1]);
}

void build(int &x,int l,int r,int d)
{
	if(l>r)
		return;
	x=++cnt;
	D=d;
	int mid=(l+r)>>1;
	nth_element(a+l,a+mid,a+r+1);
	t[x].id=a[mid].id;
	t[x].d[0]=a[mid].d[0];
	t[x].d[1]=a[mid].d[1];
	build(t[x].l,l,mid-1,d^1);
	build(t[x].r,mid+1,r,d^1);
	pushup(x);
}

void dfs(int x)
{
	if(t[x].del)
		return;
	if(!t[x].vis)
		res[++tot]=t[x].id;
	t[x].del=1;
	if(!t[t[x].l].del)
		dfs(t[x].l);
	if(!t[t[x].r].del)
		dfs(t[x].r);
}

void query(int x,int xl,int xr,int yl,int yr)
{
	if(t[x].del)
		return;
	if(full_in(t[x],xl,xr,yl,yr))
		return dfs(x),void();
	if(!t[x].vis)
	{
		if(xl<=t[x].d[0]&&t[x].d[0]<=xr&&yl<=t[x].d[1]&&t[x].d[1]<=yr)
			res[++tot]=t[x].id,t[x].vis=1;
	}
	if(!t[t[x].l].del&&intersect(t[t[x].l],xl,xr,yl,yr))
		query(t[x].l,xl,xr,yl,yr);
	if(!t[t[x].r].del&&intersect(t[t[x].r],xl,xr,yl,yr))
		query(t[x].r,xl,xr,yl,yr);
	pushup(x);
}

void dfs1(int x,int fa)
{
	dep1[x]=dep1[fa]+1;
	dfn1[x]=++dft1;
	for(int v:T1[x])
		if(v!=fa)
			dfs1(v,x);
	dfne1[x]=dft1;
}

void dfs2(int x,int fa)
{
	dep2[x]=dep2[fa]+1;
	dfn2[x]=++dft2;
	for(int v:T2[x])
		if(v!=fa)
			dfs2(v,x);
	dfne2[x]=dft2;
}

int main()
{
	t[0].del=1;
	read(n);
	for(int i=2;i<=n;i++)
		read(fa1[i]),T1[i].push_back(fa1[i]),T1[fa1[i]].push_back(i);
	for(int i=2;i<=n;i++)
		read(fa2[i]),T2[i].push_back(fa2[i]),T2[fa2[i]].push_back(i);
	dfs1(1,0);dfs2(1,0);
	for(int i=1;i<n;i++)
	{
		a[i].id=i;
		a[i].d[0]=dfn2[i+1];
		a[i].d[1]=dfn2[fa1[i+1]];
		if(a[i].d[0]>a[i].d[1])
			swap(a[i].d[0],a[i].d[1]);
	}
	build(rt1,1,n-1,0);
	for(int i=1;i<n;i++)
	{
		a[i].id=i;
		a[i].d[0]=dfn1[i+1];
		a[i].d[1]=dfn1[fa2[i+1]];
		if(a[i].d[0]>a[i].d[1])
			swap(a[i].d[0],a[i].d[1]);
	}
	build(rt2,1,n-1,0);
	int cur=0,idx;
	read(idx);
	int x=dfn2[idx+1],y=dfn2[fa1[idx+1]];
	if(x>y)
		swap(x,y);
	query(rt1,x,x,y,y);
	while(tot)
	{
		puts(cur?"Red":"Blue");
		sort(res+1,res+1+tot);
		for(int i=1;i<=tot;i++)
			printf("%d%c",res[i],"\n "[i!=tot]),bak[i]=res[i];
		if(cur)
		{
			int rtot=tot;tot=0;
			for(int i=1;i<=rtot;i++)
			{
				int e=bak[i]+1;
				int u=dep2[e]>dep2[fa2[e]]?e:fa2[e];
				if(dfne2[u]>1)
					query(rt1,1,dfn2[u]-1,dfn2[u],dfne2[u]);
				if(dfne2[u]<n)
					query(rt1,dfn2[u],dfne2[u],dfne2[u]+1,n);
				
			}
		}
		else
		{
			int rtot=tot;tot=0;
			for(int i=1;i<=rtot;i++)
			{
				int e=bak[i]+1;
				int u=dep1[e]>dep1[fa1[e]]?e:fa1[e];
				if(dfne1[u]>1)
					query(rt2,1,dfn1[u]-1,dfn1[u],dfne1[u]);
				if(dfne1[u]<n)
					query(rt2,dfn1[u],dfne1[u],dfne1[u]+1,n);
				
			}
		}
		cur^=1;
	}
}
