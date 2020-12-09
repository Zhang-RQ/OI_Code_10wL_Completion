#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXN=1E5+10;

int n,L[MAXN],R[MAXN],v[MAXN];
int Ans[MAXN],vis[MAXN],t[MAXN<<3],tg[MAXN<<3];
set<int> S[MAXN<<3];
vector<pair<int,int> > ve[MAXN];

void pushup(int x){t[x]=min(t[x<<1],t[x<<1|1]);}

void apply_tag(int x,int v){t[x]+=v;tg[x]+=v;}

void pushdown(int x)
{
	if(tg[x])
	{
		apply_tag(x<<1,tg[x]);
		apply_tag(x<<1|1,tg[x]);
		tg[x]=0;
	}
}

void build(int x,int l,int r)
{
	S[x].insert(n+1);
	if(l==r) return t[x]=v[l],void();
	int mid=(l+r)>>1;
	build(x<<1,l,mid);
	build(x<<1|1,mid+1,r);
	pushup(x);
}

void Insert(int x,int l,int r,int p,int v)
{
	S[x].insert(v);
	if(l==r) return;
	int mid=(l+r)>>1;
	if(p<=mid) Insert(x<<1,l,mid,p,v);
	else Insert(x<<1|1,mid+1,r,p,v);
}

void Erase(int x,int l,int r,int p,int v)
{
	S[x].erase(v);
	if(l==r) return;
	int mid=(l+r)>>1;
	if(p<=mid) Erase(x<<1,l,mid,p,v);
	else Erase(x<<1|1,mid+1,r,p,v);
}

int Query(int x,int l,int r,int ql,int qr)
{
	if(ql>qr) return n+1;
	if(ql<=l&&r<=qr) return *S[x].begin();
	int mid=(l+r)>>1;
	if(qr<=mid) return Query(x<<1,l,mid,ql,qr);
	else if(ql>mid) return Query(x<<1|1,mid+1,r,ql,qr);
	else return min(Query(x<<1,l,mid,ql,qr),Query(x<<1|1,mid+1,r,ql,qr));	
}

void change(int x,int l,int r,int cl,int cr,int v)
{
	if(cl>cr) return;
	if(cl<=l&&r<=cr) return apply_tag(x,v),void();
	int mid=(l+r)>>1;pushdown(x);
	if(cl<=mid) change(x<<1,l,mid,cl,cr,v);
	if(cr>mid)  change(x<<1|1,mid+1,r,cl,cr,v);
	pushup(x);
}

int query(int x,int l,int r,int ql,int qr)
{
	if(ql>qr) return 0;
	if(ql<=l&&r<=qr) return t[x];
	int mid=(l+r)>>1;pushdown(x);
	if(qr<=mid) return query(x<<1,l,mid,ql,qr);
	else if(ql>mid) return query(x<<1|1,mid+1,r,ql,qr);
	else return min(query(x<<1,l,mid,ql,qr),query(x<<1|1,mid+1,r,ql,qr));
}

int main()
{
	scanf("%d",&n);
	for(int i=1;i<=n;i++)
		scanf("%d%d",&L[i],&R[i]),++v[R[i]],ve[L[i]].emplace_back(i,R[i]);
	for(int i=1;i<=n;i++)
		v[i]+=v[i-1];
	for(int i=1;i<=n;i++)
		v[i]=i-v[i];
	build(1,1,n);
	for(int i=1;i<=n;i++)
	{
		for(auto val:ve[i])
			Insert(1,1,n,val.second,val.first);
		int p=0,k=0,l=i,r=n;
		while(l<=r)
		{
			int Mid=(l+r)>>1;
			if(query(1,1,n,i,Mid)>=1) p=Mid,l=Mid+1;
			else r=Mid-1;
		}
		if(!p) p=i;
		else p=min(p+1,n);
		k=Query(1,1,n,1,p);
		if(k==n+1) return puts("-1"),0;
		Ans[i]=k;
		Erase(1,1,n,R[k],k);
		change(1,1,n,i,R[k]-1,-1);
	}
	for(int i=1;i<=n;i++)
		printf("%d%c",Ans[i],"\n "[i!=n]);
}
