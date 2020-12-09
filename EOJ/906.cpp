#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXN=1E6+10;
const ll INF=1ll<<60;

ll gcd(ll a,ll b) {return b?gcd(b,a%b):a;}

ll exgcd(ll a,ll b,ll &x,ll &y)
{
	if(!b)
		return x=1,y=0,a;
	ll d=exgcd(b,a%b,y,x);
	y-=(a/b)*x;
	return d;
}

ll inv(ll a,ll P)
{
	ll x,y;
	ll d=exgcd(a,P,x,y);
	if(d!=1)
		throw "No Inv";
	return (x%P+P)%P;
}

int n,m,cnta,cntb;
vector<vector<pair<ll,ll> > > Va,Vb;
map<int,int> Ma,Mb;

int main()
{
	scanf("%d%d",&n,&m);
	int d=gcd(n,m),nn=n/d,mm=m/d;
	scanf("%d",&cnta);
	//printf("d=%d nn=%d mm=%d\n",d,nn,mm);
	for(int i=1,x;i<=cnta;i++)
	{
		scanf("%d",&x);
		if(!Ma.count(x%d))
			Ma[x%d]=Va.size(),Va.push_back(vector<pair<ll,ll> >());
		if(!Mb.count(x%d))
			Mb[x%d]=Vb.size(),Vb.push_back(vector<pair<ll,ll> >());
		int idxa=Ma[x%d],idxb=Mb[x%d];
		//n , r : 0 => 0+r,1 => 1m%n+r,2 => 2m%n+r,3 => 3m%n+r
		int tx=x/d;
		Va[idxa].emplace_back(tx*inv(mm,nn)%nn,0);
		Vb[idxb].emplace_back(tx*inv(nn,mm)%mm,x);//you chong de
	}
	scanf("%d",&cntb);
	for(int i=1,y;i<=cntb;i++)
	{
		scanf("%d",&y);
		if(!Ma.count(y%d))
			Ma[y%d]=Va.size(),Va.push_back(vector<pair<ll,ll> >());
		if(!Mb.count(y%d))
			Mb[y%d]=Vb.size(),Vb.push_back(vector<pair<ll,ll> >());
		int idxa=Ma[y%d],idxb=Mb[y%d];
		//n , r : 0 => 0+r,1 => 1m%n+r,2 => 2m%n+r,3 => 3m%n+r
		int ty=y/d;
		Va[idxa].emplace_back(ty*inv(mm,nn)%nn,y);
		Vb[idxb].emplace_back(ty*inv(nn,mm)%mm,0);
	}
	if(Ma.size()<d||Mb.size()<d)
		return puts("-1"),0;
	ll Ans=0;
	for(auto Od:Ma)
	{
		auto &V=Va[Od.second];
		sort(V.begin(),V.end());
		for(int i=((int)V.size())-1;~i;i--)
		{
			int pre=V[i].first?V[i].first-1:nn-1,prev=i?i-1:V.size()-1;
			if(V[prev].first!=pre)
				V.emplace_back(pre,INF);
		}
		sort(V.begin(),V.end());
		for(int i=0;i<V.size();i++)
		{
			int pre=i?i-1:V.size()-1;
			ll t=V[pre].first*mm%nn*d+Od.first;
			V[i].second=min(V[i].second,max(V[pre].second,t)+(V[i].first-V[pre].first+nn)%nn*m);
		}
		for(int i=0;i<V.size();i++)
		{
			int pre=i?i-1:V.size()-1;
			ll t=V[pre].first*mm%nn*d+Od.first;
			V[i].second=min(V[i].second,max(V[pre].second,t)+(V[i].first-V[pre].first+nn)%nn*m);
		}
		for(int i=0;i<V.size();i++)
			Ans=max(Ans,V[i].second);
	}
	for(auto Od:Mb)
	{
		auto &V=Vb[Od.second];
		sort(V.begin(),V.end());
		for(int i=((int)V.size())-1;~i;i--)
		{
			int pre=V[i].first?V[i].first-1:mm-1,prev=i?i-1:V.size()-1;
			if(V[prev].first!=pre)
				V.emplace_back(pre,INF);
		}
		sort(V.begin(),V.end());
		for(int i=0;i<V.size();i++)
		{
			int pre=i?i-1:V.size()-1;
			ll t=V[pre].first*nn%mm*d+Od.first;
			V[i].second=min(V[i].second,max(t,V[pre].second)+(V[i].first-V[pre].first+mm)%mm*n);
		}
		for(int i=0;i<V.size();i++)
		{
			int pre=i?i-1:V.size()-1;
			ll t=V[pre].first*nn%mm*d+Od.first;
			V[i].second=min(V[i].second,max(t,V[pre].second)+(V[i].first-V[pre].first+mm)%mm*n);
		}
		for(int i=0;i<V.size();i++)
			Ans=max(Ans,V[i].second);
	}
	printf("%lld\n",Ans);
}
