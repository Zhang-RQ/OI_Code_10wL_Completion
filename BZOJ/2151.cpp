#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cctype>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<stack>
#include<cassert>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXN=2E5+10;
const ll INF=1ll<<60;

struct dat{
	ll x,y;
	dat(ll _x=0,ll _y=0){x=_x;y=_y;}
	dat operator + (const dat &rhs) const {return dat(x+rhs.x,y+rhs.y);}
	bool operator < (const dat &rhs) const {return x<rhs.x||(x==rhs.x&&y>rhs.y);}
}f[MAXN][2];

int n,m,a[MAXN];

dat dp(ll Mid)
{
	f[1][0]=dat(-INF,0);f[1][1]=dat(a[1]-Mid,1);
	for(int i=2;i<=n;i++)
		f[i][0]=max(f[i-1][0],f[i-1][1]),
		f[i][1]=f[i-1][0]+dat(a[i]-Mid,1);
	dat Ans=f[n][0];
	f[1][0]=dat(0,0);f[1][1]=dat(-INF,0);
	for(int i=2;i<=n;i++)
		f[i][0]=max(f[i-1][0],f[i-1][1]),
		f[i][1]=f[i-1][0]+dat(a[i]-Mid,1);
	Ans=max(Ans,max(f[n][0],f[n][1]));
	return Ans;
}

int main()
{
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++)
		scanf("%d",&a[i]);
	if((n>>1)<m) return puts("Error!"),0;
	int L=-1E9,R=1E9;ll Ans=0;
	while(L<=R)
	{
		int mid=(L+R)>>1;
		dat t=dp(mid);
		if(t.y<=m) Ans=t.x+1ll*m*mid,R=mid-1;
		else L=mid+1;
	}
	printf("%lld\n",Ans);
}