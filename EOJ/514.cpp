#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXN=4E5+10;
const int P=998244353;

template<typename T1,typename T2>
	void Inc(T1 &a,T2 b){a+=b;if(a>=P) a-=P;}

template<typename T1,typename T2>
	void Dec(T1 &a,T2 b){a-=b;if(a<0) a+=P;}

template<typename T1,typename T2>
	T1 Add(T1 a,T2 b){a+=b;return a>=P?a-P:a;}

template<typename T1,typename T2>
	T1 Sub(T1 a,T2 b){a-=b;return a<0?a+P:a;}

int n;
ll tAns,Ans;

namespace T1{int dis[MAXN];}
namespace T2{int dis[MAXN];}

namespace T2{
	struct node_t{
		int l,r,siz;
		ll val;
	}t[MAXN*100];

	vector<pair<int,int>> nT[MAXN];
	vector<int> T[MAXN];
	int siz[MAXN],Fa[MAXN],vis[MAXN],rt,mx_rt,cnt,ref[MAXN],fa_pnt[MAXN],son_pnt[MAXN];
	int st[20][MAXN<<1],lca[20][MAXN<<1],lg2[MAXN<<1],dft,pos[MAXN],anc[MAXN],val[MAXN];
	int DCT_rt,tot,f[MAXN];

	void dfs_pre(int x,int fa)
	{
		int lst=x;val[x]=dis[x];
		for(auto e:nT[x])
			if(e.first!=fa)
			{
				dis[e.first]=Add(dis[x],e.second);
				dfs_pre(e.first,x);
				++tot;
				T[tot].push_back(e.first);
				T[e.first].push_back(tot);
				Fa[e.first]=tot;
				T[tot].push_back(lst);
				T[lst].push_back(tot);
				Fa[tot]=lst;
				val[tot]=dis[x];
				lst=tot;
			}
	}

	void dfs_dp(int x,int fa)
	{
		f[x]=T1::dis[x];
		siz[x]=1;
		for(auto e:nT[x])
			if(e.first!=fa)
			{
				dfs_dp(e.first,x);
				Dec(Ans,2ll*siz[e.first]*f[x]%P*val[x]%P);
				Dec(Ans,2ll*siz[x]*f[e.first]%P*val[x]%P);
				siz[x]+=siz[e.first];
				Inc(f[x],f[e.first]);
			}
	}

	void dfs(int x,int fa,int dep)
	{
		lca[0][pos[x]=++dft]=x;
		st[0][pos[x]]=dep;
		for(int v:T[x])
			if(v!=fa)
			{
				dfs(v,x,dep+1);
				lca[0][++dft]=x;
				st[0][dft]=dep;
			}
	}

	int LCA(int x,int y)
	{
		x=pos[x];y=pos[y];
		if(x>y)
			swap(x,y);
		int len=lg2[y-x+1];
		return st[len][x]<st[len][y-(1<<len)+1]?lca[len][x]:lca[len][y-(1<<len)+1];
	}
	
	bool chk(int u,int v){return Fa[v]==u?vis[v]:vis[u];}

	void get_rt(int x,int fa,int sz)
	{
		siz[x]=1;
		for(int v:T[x])
			if(v!=fa&&!chk(x,v))
				get_rt(v,x,sz),siz[x]+=siz[v];
		int mx=max(siz[x],sz-siz[x]);
		if(!vis[x]&&Fa[x]&&mx<mx_rt)
			mx_rt=mx,rt=x;
	}

	int construct(int x)
	{
		vis[x]=1;
		int now=++cnt;
		ref[x]=ref[Fa[x]]=now;
		mx_rt=1<<30,rt=-1;
		fa_pnt[now]=Fa[x];
		get_rt(x,0,siz[x]);
		if(~rt)
			t[now].r=construct(rt),anc[t[now].r]=now;
		mx_rt=1<<30,rt=-1;
		get_rt(Fa[x],0,siz[Fa[x]]);
		if(~rt)
			t[now].l=construct(rt),anc[t[now].l]=now;
		return now;
	}

	int insert(int p)
	{
		int x=ref[p],ret=++cnt;
		if(p==fa_pnt[x])
			t[ret].val=val[p];
		else
			t[ret].siz=1;
		while(anc[x])
		{
			int tmp=++cnt;
			if(x==t[anc[x]].l)
			{
				t[tmp].l=ret;
				t[tmp].val+=val[LCA(p,fa_pnt[anc[x]])];
				ret=tmp;
			}
			else
			{
				t[tmp].r=ret;
				t[tmp].siz++;
				ret=tmp;
			}
			x=anc[x];
		}
		return ret;
	}

	int merge(int x,int y,ll exv)
	{
		if(!x||!y)
			return x|y;
		Inc(tAns,1ll*t[x].siz*t[y].val%P*exv%P);
		Inc(tAns,1ll*t[y].siz*t[x].val%P*exv%P);
		t[x].siz+=t[y].siz;
		Inc(t[x].val,t[y].val);
		t[x].l=merge(t[x].l,t[y].l,exv);
		t[x].r=merge(t[x].r,t[y].r,exv);
		return x;
	}

	void init(int root)
	{
		dfs_pre(root,0);
		dfs(root,0,1);
		for(int i=2;i<=dft;i++)
			lg2[i]=lg2[i>>1]+1;
		for(int j=1;j<=lg2[dft];j++)
			for(int i=1;i+(1<<j)-1<=dft;i++)
				if(st[j-1][i]<st[j-1][i+(1<<(j-1))])
					st[j][i]=st[j-1][i],lca[j][i]=lca[j-1][i];
				else
					st[j][i]=st[j-1][i+(1<<(j-1))],
					lca[j][i]=lca[j-1][i+(1<<(j-1))];
		mx_rt=1<<30;rt=-1;
		get_rt(root,0,tot);
		DCT_rt=construct(rt);
	}
}

namespace T1{
	vector<pair<int,int> > T[MAXN];
	int f[MAXN],siz[MAXN],st[20][MAXN<<1],lca[20][MAXN<<1],lg2[MAXN<<1],dft,pos[MAXN],dep[MAXN];

	void dfs1(int x,int fa,int dep)
	{
		lca[0][pos[x]=++dft]=x;
		st[0][pos[x]]=dep;
		for(auto e:T[x])
			if(e.first!=fa)
			{
				dfs1(e.first,x,dep+1);
				lca[0][++dft]=x;
				st[0][dft]=dep;
			}
	}
	
	void dfs_dp(int x,int fa)
	{
		f[x]=T2::dis[x];
		siz[x]=1;
		for(auto e:T[x])
			if(e.first!=fa)
			{
				dfs_dp(e.first,x);
				Dec(Ans,2ll*siz[x]*f[e.first]%P*dis[x]%P);
				Dec(Ans,2ll*siz[e.first]*f[x]%P*dis[x]%P);
				siz[x]+=siz[e.first];
				Inc(f[x],f[e.first]);
			}
	}

	int dfs(int x,int fa)
	{
		int p=x<=n?T2::insert(x):0;
		for(auto e:T[x])
			if(e.first!=fa)
			{
				dis[e.first]=Add(dis[x],e.second);
				p=T2::merge(p,dfs(e.first,x),dis[x]);
			}
		return p;
	}

	void init(int root)
	{
		dfs1(root,0,1);
		for(int i=2;i<=dft;i++)
			lg2[i]=lg2[i>>1]+1;
		for(int j=1;j<=lg2[dft];j++)
			for(int i=1;i+(1<<j)-1<=dft;i++)
				if(st[j-1][i]<st[j-1][i+(1<<(j-1))])
					st[j][i]=st[j-1][i],lca[j][i]=lca[j-1][i];
				else
					st[j][i]=st[j-1][i+(1<<(j-1))],
					lca[j][i]=lca[j-1][i+(1<<(j-1))];
	}

	int LCA(int x,int y)
	{
		x=pos[x];y=pos[y];
		if(x>y)
			swap(x,y);
		int len=lg2[y-x+1];
		return st[len][x]<st[len][y-(1<<len)+1]?lca[len][x]:lca[len][y-(1<<len)+1];
	}
}

int main()
{
	scanf("%d",&n);T2::tot=n;
	for(int i=1,u,v,w;i<n;i++)
		scanf("%d%d%d",&u,&v,&w),
		T1::T[u].emplace_back(v,w),
		T1::T[v].emplace_back(u,w);
	for(int i=1,u,v,w;i<n;i++)
		scanf("%d%d%d",&u,&v,&w),
		T2::nT[u].emplace_back(v,w),
		T2::nT[v].emplace_back(u,w);
	T2::init(1);T1::dfs(1,0);T1::init(1);
	ll S1=0,S2=0;
	for(int i=1;i<=n;i++)
		Inc(S1,T1::dis[i]),
		Inc(S2,T2::dis[i]);
	Ans=S1*S2%P;
	for(int i=1;i<=n;i++)
		Inc(Ans,1ll*T1::dis[i]*T2::dis[i]%P*(n-2)%P);
	T1::dfs_dp(1,0);T2::dfs_dp(1,0);
	Inc(Ans,tAns*4%P);
	printf("%lld\n",Ans);
}
