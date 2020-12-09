#pragma GCC optimize("O2,Ofast,inline,unroll-all-loops,-ffast-math")
#pragma GCC target("avx,sse2,sse3,sse4,popcnt")

#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;
 
using namespace std;

struct FastIO{
    static const int S=1e7;
    int wpos;
    char wbuf[S];
    FastIO():wpos(0){}

    inline int xchar()
		{
			static char buf[S];
			static int len=0,pos=0;
			if(pos==len)
				pos=0,len=fread(buf,1,S,stdin);
			return buf[pos++];
		}

    inline int operator () ()
		{
			int c=xchar(),x=0,ng=0;
			while (!isdigit(c)) ng|=(c=='-'),c=xchar();
			for(;isdigit(c);c=xchar()) x=x*10+c-'0';
			return ng?-x:x;
		}

    inline ll operator ! ()
		{
			int c=xchar(),ng=0;ll x=0;
			while(!isdigit(c)) ng|=(c=='-'),c=xchar();
			for(;isdigit(c);c=xchar()) x=x*10+c-'0';
			return ng?-x:x;
		}

    inline void wchar(int x)
		{
			if(wpos==S) fwrite(wbuf,1,S,stdout),wpos=0;
			wbuf[wpos++]=x;
		}

    inline void operator () (ll x)
		{
			if (x<0) wchar('-'),x=-x;
			char s[24];
			int n=0;
			while(x||!n) s[n++]='0'+x%10,x/=10;
			while(n--) wchar(s[n]);
			wchar('\n');
		}

    inline void space(ll x)
		{
			if (x<0) wchar('-'),x=-x;
			char s[24];
			int n=0;
			while(x||!n) s[n++]='0'+x%10,x/=10;
			while(n--) wchar(s[n]);
			wchar(' ');
		}

    inline void nextline() {wchar('\n');}

    ~FastIO() {if(wpos) fwrite(wbuf,1,wpos,stdout),wpos=0;}
}io;
 
const int MAXN=1E5+10;
 
namespace Segment_Tree{
    int t[MAXN<<3],cov[MAXN<<3];
 
    void pushup(int x,int l,int r)
    {
        if(cov[x]) t[x]=r-l+1;
        else t[x]=t[x<<1]+t[x<<1|1];
    }
 
    void change(int x,int l,int r,int cl,int cr,int dlt)
    {
        if(cl<=l&&r<=cr) return cov[x]+=dlt,pushup(x,l,r),void();
        int mid=(l+r)>>1;
        if(cl<=mid) change(x<<1,l,mid,cl,cr,dlt);
        if(cr>mid)  change(x<<1|1,mid+1,r,cl,cr,dlt);
        pushup(x,l,r);
    }
}
 
struct OPT{
    int x,l,r,f;
 
    bool operator < (const OPT &rhs) const {return x<rhs.x;}
}op[MAXN*200];

vector<pair<int,int> > q[MAXN];
vector<int> T[MAXN];
int cnt,dep[MAXN],dfn[MAXN],dfn_end[MAXN],dft,n,m;
int tAns[MAXN*30],stk[MAXN],top;
ll Ans;

bool inside(int x,int y){return dfn[x]<=dfn[y]&&dfn[y]<=dfn_end[x];}//y in x
 
void dfs(int x,int fa)
{
    dfn[x]=++dft;dep[x]=dep[fa]+1;
    for(int v:T[x])
        if(v!=fa)
            dfs(v,x);
	dfn_end[x]=dft;
}

void dfs2(int x,int fa)
{
	stk[++top]=x;
	for(auto qe:q[x])
		tAns[qe.first]=stk[top-qe.second];
	for(int v:T[x])
		if(v!=fa)
			dfs2(v,x);
	--top;
}
 
void solve()
{
    sort(op+1,op+1+cnt);
    for(int i=1;i<=cnt;i++)
    {
        Ans+=1ll*Segment_Tree::t[1]*(op[i].x-op[i-1].x);
        Segment_Tree::change(1,1,n,op[i].l,op[i].r,op[i].f);
    }
}
 
void add_rec(int x1,int y1,int x2,int y2)
{
    if(x1>x2||y1>y2) return;
    x1=max(1,x1);x1=min(x1,n);
    y1=max(1,y1);y1=min(y1,n);
    x2=max(1,x2);x2=min(x2,n);
    y2=max(1,y2);y2=min(y2,n);
    op[++cnt]=(OPT){x1,y1,y2,1};
    op[++cnt]=(OPT){x2+1,y1,y2,-1};
}
 
int main()
{
	int t=clock();
	n=io();
    for(int i=1,u,v;i<n;i++)
    {
		u=io();v=io();
        T[u].push_back(v);
        T[v].push_back(u);
    }
    dfs(1,0);
	int tot=0,tot1=0;
	for(int uu=1;uu<=n;uu++)
		for(int vv=uu;vv<=n;vv+=uu)
		{
			int u=uu,v=vv;
			if(u==v) continue;
			if(dep[u]>dep[v]) swap(u,v);
			if(inside(u,v))
				q[v].emplace_back(++tot1,dep[v]-dep[u]-1);
		}
	dfs2(1,0);
    for(int uu=1;uu<=n;uu++)
		for(int vv=uu;vv<=n;vv+=uu)
		{
			int u=uu,v=vv;
			if(u==v) continue;
			if(dep[u]>dep[v]) swap(u,v);
			if(inside(u,v))
			{
				int p=tAns[++tot];
				add_rec(dfn[v],1,dfn_end[v],dfn[p]-1);
				add_rec(1,dfn[v],dfn[p]-1,dfn_end[v]);
				add_rec(dfn[v],dfn_end[p]+1,dfn_end[v],n);
				add_rec(dfn_end[p]+1,dfn[v],n,dfn_end[v]);
			}
			else
			{
				add_rec(dfn[u],dfn[v],dfn_end[u],dfn_end[v]);
				add_rec(dfn[v],dfn[u],dfn_end[v],dfn_end[u]);
			}
		}
    for(int i=1;i<=n;i++) add_rec(i,i,i,i);
    solve();
    printf("%lld\n",(1ll*n*n-Ans)/2);
	fprintf(stderr,"time=%d\n",clock()-t);
}
