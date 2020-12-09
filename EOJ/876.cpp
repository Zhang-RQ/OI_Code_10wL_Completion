#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXL=1048576;
const int P=998244353;
const int g[]={3,(P+1)/3};

template<typename T1,typename T2>
	void Inc(T1 &a,T2 b){a+=b;if(a>=P) a-=P;}

template<typename T1,typename T2>
	void Dec(T1 &a,T2 b){a-=b;if(a<0) a+=P;}

template<typename T1,typename T2>
	T1 Add(T1 a,T2 b){a+=b;return a>=P?a-P:a;}

template<typename T1,typename T2>
	T1 Sub(T1 a,T2 b){a-=b;return a<0?a+P:a;}

int rev[MAXL],n;
ll b[MAXL],c[MAXL];
ll G[2][MAXL],res[MAXL],fac[MAXL],ifac[MAXL];

ll ksm(ll a,ll b)
{
	ll ret=1;
	for(;b;b>>=1,(a*=a)%=P)
		if(b&1)
			(ret*=a)%=P;
	return ret;
}

int init(int n)
{
	int tot=1,lg2=0;
	while(tot<n)
		tot<<=1,lg2++;
	for(int i=0;i<tot;i++)
		rev[i]=(rev[i>>1]>>1)|((i&1)<<(lg2-1));
	return tot;
}

void NTT(ll *a,int n,int f)
{
	ll *ed=a+n;
	for(int i=0;i<n;i++)
		if(i<rev[i])
			swap(a[i],a[rev[i]]);
	for(int i=2;i<=n;i<<=1)
		for(ll *x=a,*y=a+(i>>1);x!=ed;x+=i,y+=i)
			for(int k=0;k<(i>>1);k++)
			{
				ll u=x[k],t=y[k]*G[f][MAXL/i*k]%P;
				x[k]=Add(u,t);y[k]=Sub(u,t);
			}
	if(f)
		for(int i=0,inv=ksm(n,P-2);i<n;i++)
			a[i]=a[i]*inv%P;
}

void solve(int n)
{
	if(n==1)
		return res[1]=1,void();
	int lim=n>>1;
	solve(lim);
	int tot=init((lim+1)<<1);
	memset(b,0,sizeof(ll)*tot);
	memset(c,0,sizeof(ll)*tot);
	for(int i=0;i<=lim;i++)
		c[i]=ksm(lim,i)*ifac[i]%P;
	for(int i=0;i<=lim;i++)
		b[i]=res[lim-i]*fac[lim-i]%P;
	NTT(b,tot,0);NTT(c,tot,0);
	for(int i=0;i<tot;i++)
		b[i]=b[i]*c[i]%P;
	NTT(b,tot,1);
	reverse(b,b+lim+1);
	for(int i=0;i<=lim;i++)
		b[i]=b[i]*ifac[i]%P;
	memset(c,0,sizeof(ll)*tot);
	for(int i=lim+1;i<=tot;i++)
		b[i]=c[i]=0;
	for(int i=0;i<=lim;i++)
		c[i]=res[i];
	NTT(b,tot,0);NTT(c,tot,0);
	for(int i=0;i<tot;i++)
		b[i]=b[i]*c[i]%P;
	NTT(b,tot,1);
	for(int i=0;i<=n;i++)
		res[i]=b[i];
	if(n&1)
	{
		for(int i=n;~i;i--)
			res[i]=Add(res[i]*(n-1)%P,i?res[i-1]:0);
	}
}

int main()
{
//	int t=clock();
	int a,b;
	G[0][0]=G[1][0]=1;
	G[0][1]=ksm(g[0],(P-1)/MAXL);
	G[1][1]=ksm(g[1],(P-1)/MAXL);
	for(int i=2;i<MAXL;i++)
		G[0][i]=G[0][i-1]*G[0][1]%P,
		G[1][i]=G[1][i-1]*G[1][1]%P;
	fac[0]=ifac[0]=1;
	for(int i=1;i<MAXL;i++)
		fac[i]=fac[i-1]*i%P;
	ifac[MAXL-1]=ksm(fac[MAXL-1],P-2);
	for(int i=MAXL-2;i;i--)
		ifac[i]=ifac[i+1]*(i+1)%P;
	scanf("%d",&n);
	scanf("%d%d",&a,&b);
	if(a==0||b==0)
		return puts("0"),0;
	if(n==1)
		return puts("1"),0;
	solve(n-1);
	printf("%lld\n",res[a+b-2]*fac[a+b-2]%P*ifac[a-1]%P*ifac[b-1]%P);
//	fprintf(stderr,"time=%d\n",clock()-t);
}
