#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXN=3E5+10;
const int MAXV=1E7+10;

int n,d,q,a[MAXN],lim=1,lst[MAXV],stkx[MAXN],stkn[MAXN],topx,topn;
ll s[MAXN<<2],hs[MAXN<<2],tg[MAXN<<2],htg[MAXN<<2];
ll Ans[MAXN];
vector<pair<int,int> > V[MAXN];

void pushup(int x){s[x]=s[x<<1]+s[x<<1|1];hs[x]=hs[x<<1]+hs[x<<1|1];}

void apply_tag(int x,ll dlt,int len)
{
	s[x]+=1ll*dlt*len;
	tg[x]+=dlt;
}

void apply_htag(int x,ll dlt,int len)
{
	hs[x]+=1ll*dlt*len;
	htg[x]+=dlt;
}

void pushdown(int x,int l,int r)
{
	if(tg[x])
	{
		int mid=(l+r)>>1;
		apply_tag(x<<1,tg[x],mid-l+1);
		apply_tag(x<<1|1,tg[x],r-mid);
		tg[x]=0;
	}
	if(htg[x])
	{
		int mid=(l+r)>>1;
		apply_htag(x<<1,htg[x],mid-l+1);
		apply_htag(x<<1|1,htg[x],r-mid);
		htg[x]=0;
	}
}

void Add(int x,int l,int r,int cl,int cr,int dlt,int id)
{
	if(cl<=l&&r<=cr)
		return apply_tag(x,dlt,r-l+1),apply_htag(x,1ll*(id-1)*dlt,r-l+1),void();
	int mid=(l+r)>>1;pushdown(x,l,r);
	if(cl<=mid)
		Add(x<<1,l,mid,cl,cr,dlt,id);
	if(cr>mid)
		Add(x<<1|1,mid+1,r,cl,cr,dlt,id);
	pushup(x);
}

void Set_Zero(int x,int l,int r,int p,int id)
{
	if(l==r)
		return hs[x]-=1ll*s[x]*(id-1),s[x]=0,void();
	int mid=(l+r)>>1;pushdown(x,l,r);
	if(p<=mid)
		Set_Zero(x<<1,l,mid,p,id);
	else
		Set_Zero(x<<1|1,mid+1,r,p,id);
	pushup(x);
}

ll Ask(int x,int l,int r,int ql,int qr,int id)
{
	if(ql<=l&&r<=qr)
		return s[x]*id-hs[x];
	int mid=(l+r)>>1;pushdown(x,l,r);
	if(qr<=mid)
		return Ask(x<<1,l,mid,ql,qr,id);
	else if(ql>mid)
		return Ask(x<<1|1,mid+1,r,ql,qr,id);
	else
		return Ask(x<<1,l,mid,ql,qr,id)+Ask(x<<1|1,mid+1,r,ql,qr,id);
}

int main()
{
	scanf("%d%d%d",&n,&d,&q);
	for(int i=1;i<=n;i++)
		scanf("%d",&a[i]);
	for(int i=1,l,r;i<=q;i++)
		scanf("%d%d",&l,&r),
		V[r].emplace_back(l,i);
	for(int i=1;i<=n;i++)
	{
		int _lim=lim;
		if(a[i-1]%d!=a[i]%d)
			lim=i;
		lim=max(lim,lst[a[i]]+1);
		lst[a[i]]=i;
		for(int p=_lim;p<lim;p++)
			Set_Zero(1,1,n,p,i);
		while(topx&&a[stkx[topx]]<=a[i])
		{
			int L=max(lim,stkx[topx-1]+1);
			if(L<=stkx[topx])
				Add(1,1,n,L,stkx[topx],a[i]/d-a[stkx[topx]]/d,i);
			--topx;
		}
		while(topn&&a[stkn[topn]]>=a[i])
		{
			int L=max(lim,stkn[topn-1]+1);
			if(L<=stkn[topn])
				Add(1,1,n,L,stkn[topn],a[stkn[topn]]/d-a[i]/d,i);
			--topn;
		}
		stkx[++topx]=stkn[++topn]=i;
		if(lim<i)
			Add(1,1,n,lim,i-1,-1,i);
		for(auto Q:V[i])
			Ans[Q.second]=Ask(1,1,n,Q.first,i,i);
	}
	for(int i=1;i<=q;i++)
		printf("%lld\n",Ans[i]);
}
