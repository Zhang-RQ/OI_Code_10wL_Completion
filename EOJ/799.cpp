#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int P=998244353;
const int MAXN=120;
const int MAXK=1010;
const int inv2=(P+1)>>1;

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

template<typename T1,typename T2>
    void Inc(T1 &a,T2 b){a+=b;if(a>=P) a-=P;}

template<typename T1,typename T2>
    void Dec(T1 &a,T2 b){a-=b;if(a<0) a+=P;}

template<typename T1,typename T2>
    T1 Add(T1 a,T2 b){a+=b;return a>=P?a-P:a;}

template<typename T1,typename T2>
    T1 Sub(T1 a,T2 b){a-=b;return a<0?a+P:a;}

#define ll __int128

ll cnt[MAXN][70][MAXK],A[MAXN],B[MAXN],C[MAXN],n,k,tmp[MAXK],tmp2[MAXK];
ll f[MAXK],pwk[70];

void Max_Mul(ll *f,ll *g,ll *Res,int len)
{
	static ll pref[MAXK],preg[MAXK];
	pref[0]=f[0];preg[0]=g[0];
	for(int i=1;i<len;i++)
		pref[i]=Add(pref[i-1],f[i]);
	for(int i=1;i<len;i++)
		preg[i]=Add(preg[i-1],g[i]);
	memset(Res,0,sizeof tmp);
	for(int i=1;i<len;i++)
		Inc(Res[i],1ll*f[i]*preg[i-1]%P);
	for(int i=1;i<len;i++)
		Inc(Res[i],1ll*g[i]*pref[i-1]%P);
	for(int i=0;i<len;i++)
		Inc(Res[i],1ll*f[i]*g[i]%P);
}

ll S1(ll n){n%=P;return n*(n+1)%P*inv2%P;}

ll calc(ll n,ll a,ll b,ll c)
{
	if(n<0) return 0;
	if(!n) return b/c;
	if(!a) return 1ll*(n+1)%P*(b/c)%P;
	if(a>=c||b>=c)
		return Add(calc(n,a%c,b%c,c),Add(a/c*S1(n),b/c*(n+1)%P));
	ll m=(a*n+b)/c;
	return Sub(1ll*n*m%P,calc(m-1,c,c-b-1,a));
}

ll g(int i,int b,int v)
{
	if(v<0) return 0;
	return Add(Sub(calc(A[i],B[i],C[i],pwk[b+1]),calc(A[i],B[i],C[i]+(k-1)*pwk[b]*(v+1),pwk[b+1])),1ll*(A[i]+1)*(v+1)%P);
}

#undef ll

int main()
{
//	freopen("pressure.in","r",stdin);
//	freopen("pressure.out","w",stdout);
	read(n,k);
	for(int i=0;i<k;i++)
		read(f[i]);
	for(int i=1;i<=n;i++)
		read(A[i],B[i],C[i]);
	__int128 Mx=0;
	for(int i=1;i<=n;i++)
		Mx=max(Mx,1ll*A[i]*B[i]+C[i]);
	int Mxb=0;
	pwk[Mxb]=1;
	while(pwk[Mxb]<Mx) pwk[Mxb+1]=pwk[Mxb]*k,Mxb++;
	pwk[Mxb+1]=pwk[Mxb]*k;
	for(int i=1;i<=n;i++)
		for(int bit=0;bit<=Mxb;bit++)
			for(int v=0;v<k;v++)
				cnt[i][bit][v]=Sub(g(i,bit,v),g(i,bit,v-1));
	ll Ans=0;
	for(int i=0;i<=Mxb;i++)
	{
		memset(tmp,0,sizeof tmp);
		tmp[0]=1;
		for(int j=1;j<=n;j++)
		{
			Max_Mul(tmp,cnt[j][i],tmp2,k);
			memcpy(tmp,tmp2,sizeof tmp);
		}
		for(int j=0;j<k;j++)
			Inc(Ans,1ll*f[j]*tmp[j]%P);
	}
	printf("%lld\n",Ans);
}
