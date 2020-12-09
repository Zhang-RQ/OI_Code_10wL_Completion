#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int Pool_siz=1.5E7+10;
const int MAXS=1E7+15;
const int MAXK=5010;
const int P=1E9+7;

template<typename T1,typename T2>
	void Inc(T1 &a,T2 b){a+=b;if(a>=P) a-=P;}

template<typename T1,typename T2>
	void Dec(T1 &a,T2 b){a-=b;if(a<0) a+=P;}

template<typename T1,typename T2>
	T1 Add(T1 a,T2 b){a+=b;return a>=P?a-P:a;}

template<typename T1,typename T2>
	T1 Sub(T1 a,T2 b){a-=b;return a<0?a+P:a;}

ll ksm(ll a,ll b)
{
	ll ret=1;
	for(;b;b>>=1,(a*=a)%=P)
		if(b&1)
			(ret*=a)%=P;
	return ret;
}

int vis[MAXS];
ll n,fac[MAXK],ifac[MAXK],Ans,lim,pr[MAXS],pc,cntpn,m;
ll S[MAXK][MAXK],inv[MAXK],preS[MAXS],g[MAXS];

void sieve()
{
	preS[1]=1;
	for(int i=2;i<=lim;i++)
	{
		if(!vis[i])
			pr[++pc]=i,preS[i]=ksm(i,m);
		for(int j=1;j<=pc&&1ll*i*pr[j]<=lim;j++)
		{
			vis[i*pr[j]]=1;
			preS[i*pr[j]]=preS[i]*preS[pr[j]]%P;
			if(!(i%pr[j]))
				break;
		}
	}
	for(int i=1;i<=lim;i++)
		Inc(preS[i],preS[i-1]);
	S[0][0]=inv[1]=1;
	for(int i=1;i<=m;i++)
		for(int j=1;j<=i;j++)
			S[i][j]=Add(S[i-1][j]*j%P,S[i-1][j-1]);
	for(int i=2;i<=m+1;i++)
		inv[i]=Sub(P,P/i)*inv[P%i]%P;
	for(int i=1;i<=m;i++)
		for(int j=1;j<=i;j++)
			S[i][j]=S[i][j]*inv[j+1]%P;
	for(int i=1;i<=pc;i++)
		g[i]=Sub(ksm(pr[i],m),ksm(pr[i],2*m));
}

ll S_n(ll x)
{
	if(x<=lim)
		return preS[x];
	ll ret=0,tmp=(x+1)%P;
	for(int i=0;i<=m;i++)
	{
		Inc(ret,S[m][i]*tmp%P);
		tmp=(x-i)%P*tmp%P;
	}
	return ret;
}

void dfs(int x,ll now,ll nowg)
{
	if(x>pc||now>n/pr[x]/pr[x]+1)
		return;
	for(int y=x;y<=pc;y++)
	{
		if(now>n/pr[y]/pr[y]+1)
			break;
		ll nxt=now*pr[y]*pr[y];
		for(int k=2;k<=60&&nxt<=n;nxt*=pr[y])
		{
			Inc(Ans,nowg*g[y]%P*S_n(n/nxt)%P);
			dfs(y+1,nxt,nowg*g[y]%P);
			if(nxt>n/pr[y]+1)
				break;
		}
	}
}

int main()
{
	scanf("%lld%lld",&n,&m);
	lim=sqrt(n)+10;sieve();
	Ans=S_n(n);//1 is powerful number
	dfs(1,1,1);
	printf("%lld\n",Ans);
}
