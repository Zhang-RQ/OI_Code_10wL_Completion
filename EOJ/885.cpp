#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXN=510;
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

int n,k,p[MAXN],f[MAXN][MAXN];
int S1[MAXN][MAXN],S2[MAXN][MAXN],S3[MAXN][MAXN],S4[MAXN][MAXN];
int S5[MAXN][MAXN],S6[MAXN][MAXN],S7[MAXN][MAXN],S8[MAXN][MAXN];

void init()
{
	for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
		{
			S1[i][j]=Add(S1[i-1][j],f[i][j]);
			S2[i][j]=Add(S2[i-1][j],S1[i][j]);
			S3[i][j]=Add(S3[i][j-1],f[i][j]);
			S4[i][j]=Add(S4[i][j-1],S3[i][j]);
			S5[i][j]=Add(S5[i-1][j],i>=j?f[i][i-j]:0);
			S6[i][j]=Add(S6[i-1][j],i>=j?1ll*f[i][i-j]*i%P:0);
			S7[i][j]=Add(S7[i-1][j],i>=j?f[i-j][i]:0);
			S8[i][j]=Add(S8[i-1][j],i>=j?1ll*f[i-j][i]*i%P:0);
		}
}

ll C2(int n){return 1ll*n*(n+1)/2%P;}

int main()
{
	scanf("%d%d",&n,&k);
	for(int i=1;i<=n;i++)
		scanf("%d",&p[i]);
	for(int i=1;i<=n;i++)
		for(int j=i+1;j<=n;j++)
			if(p[i]<p[j])
				f[i][j]=1;
			else
				f[j][i]=1;
	init();
	for(int _=1;_<=k;_++)
	{
		for(int i=1;i<=n;i++)
			for(int j=1;j<=n;j++)
				if(i!=j)
				{
					int l=i,r=j;
					if(l>r)
					{
						swap(l,r);
						f[i][j]=f[i][j]*(C2(l-1)+C2(r-l-1)+C2(n-r))%P;
						Inc(f[i][j],Sub(Sub(S4[r][r-1],S4[r][r-l-1]),S4[r][l-1]));
						Inc(f[i][j],Sub(Add(S2[n][l],S2[l-1][l]),Add(S2[n+l-r][l],S2[r-1][l])));
						Inc(f[i][j],Sub(Add(S8[n-l][r-l],S8[r][r-l]),Add(S8[r-l][r-l],S8[n][r-l])));
						Inc(f[i][j],Sub(Add(1ll*(n+1)*S7[n][r-l]%P,1ll*(r-l)*S7[r-l][r-l]%P),Add(1ll*(n+1-l)*S7[n-l][r-l]%P,1ll*r*S7[r][r-l]%P)));
					}
					else
					{
						f[i][j]=f[i][j]*(C2(l-1)+C2(r-l-1)+C2(n-r))%P;
						Inc(f[i][j],Sub(Sub(S2[r-1][r],S2[r-l-1][r]),S2[l-1][r]));
						Inc(f[i][j],Sub(Add(S4[l][n],S4[l][l-1]),Add(S4[l][n+l-r],S4[l][r-1])));
						Inc(f[i][j],Sub(Add(S6[n-l][r-l],S6[r][r-l]),Add(S6[r-l][r-l],S6[n][r-l])));
						Inc(f[i][j],Sub(Add(1ll*(n+1)*S5[n][r-l]%P,1ll*(r-l)*S5[r-l][r-l]%P),Add(1ll*(n+1-l)*S5[n-l][r-l]%P,1ll*r*S5[r][r-l]%P)));
						
					}
				}
		init();
	}
	ll Ans=0;
	for(int i=1;i<=n;i++)
		for(int j=1;j<i;j++)
			Inc(Ans,f[i][j]);
	printf("%lld\n",Ans*ksm(ksm(n*(n+1)/2,P-2),k)%P);
}
