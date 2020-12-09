#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXN=2E5+10;

int vis[MAXN],pr[MAXN],pc,lim;
int id[2*MAXN],Rt,tot;
ll pos[MAXN],n,ps[MAXN];
ull g[MAXN];

int ID(ull x){return x<MAXN?id[x]:id[2*MAXN-n/x];}

void sieve()
{
	for(int i=2;i<MAXN;i++)
	{
		if(!vis[i]) pr[++pc]=i,ps[pc]=ps[pc-1]+i;
		for(int j=1;j<=pc&&i*pr[j]<MAXN;j++)
		{
			vis[i*pr[j]]=1;
			if(!(i%(pr[j])))
			   break;
		}
	}
}

ll S(int i,int j)
{
	if(j>lim||pr[j]>pos[i]) return 0;
	ll ret=g[i]-ps[j-1];
	if(1ll*pr[j]*pr[j]>pos[i]) return ret;
	for(int k=j;1ll*pr[k]*pr[k]<=pos[i];k++)
		for(ll pw=pr[k];pw*pr[k]<=pos[i];pw*=pr[k])
			ret+=pw*S(ID(pos[i]/pw),k+1)+pw*pr[k];
	return ret;
}

ll Solve(ll n)
{
	if(n<=1) return 0;
	Rt=sqrt(n);tot=0;::n=n;
	lim=upper_bound(pr+1,pr+1+pc,Rt)-pr-1;
	for(ll p=1;p<=n;)
	{
		pos[++tot]=n/p;
		p=n/(n/p)+1;
	}
	ll Ret=0;
	for(int i=1;i<=tot;i++)
		id[pos[i]<MAXN?pos[i]:2*MAXN-n/pos[i]]=i;
	for(int i=1;i<=tot;i++)
		g[i]=pos[i]*(pos[i]+1)/2-1;
	for(int j=1;j<=lim;j++)
		for(int i=1;i<=tot&&1ll*pr[j]*pr[j]<=pos[i];i++)
			g[i]-=pr[j]*(g[ID(pos[i]/pr[j])]-ps[j-1]);
	for(int i=1;i<=lim;i++)
		Ret+=S(ID(n/pr[i]),i);
	return Ret;
}

int main()
{
	sieve();
	ll l,r;
	cin>>l>>r;
	cout<<Solve(r)-Solve(l-1)<<endl;
}
