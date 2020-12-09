#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

namespace FastIO{
    inline char get_char()
    {
        static char buf[1000001],*p1=buf,*p2=buf;
        return p1==p2&&(p2=(p1=buf)+fread(buf,1,1000000,stdin),p1==p2)?EOF:*p1++;
    }

    template<typename T>
        inline void read(T& x)
	{
		static char ch=get_char();int f=x=0;
		while(!isdigit(ch)) f|=(ch=='-'),ch=get_char();
		while(isdigit(ch)) x=x*10+(ch^'0'),ch=get_char();
		x=f?-x:x;
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

const int MAXN=1E6+10;
const double EPS=1E-8;
const ll INF=1ll<<60;

int Pool[MAXN*100],hd[MAXN],tl[MAXN],*q[MAXN],*cur=Pool;
int n,x[MAXN],val[MAXN],totv,siz[MAXN];
ll f[MAXN];

ll X(int i){return i;}

ll Y(int i){return f[i]-1ll*i*(i+1)/2;}

double slope(int i,int j){return (Y(j)-Y(i))/(X(j)-X(i)+EPS);}

ll Query(int x,int k)
{
	if(hd[x]>tl[x]) return -INF;
	while(hd[x]<tl[x]&&slope(q[x][hd[x]],q[x][hd[x]+1])>k)
		++hd[x];
	return Y(q[x][hd[x]])-1ll*k*X(q[x][hd[x]]);
}

void Add(int x)
{
	for(;x<=totv;x+=x&(-x))
		siz[x]++;
}

void Push(int x,int id)
{
	while(hd[x]<tl[x]&&slope(q[x][tl[x]-1],id)>slope(q[x][tl[x]-1],q[x][tl[x]]))
		--tl[x];
	q[x][++tl[x]]=id;
}

void Push_t(int x,int id)
{
	for(;x<=totv;x+=x&(-x))
		Push(x,id);

}

ll Query_t(int x,int k)
{
	ll Ret=-(1ll<<60);
	for(;x;x-=x&(-x))
		Ret=max(Ret,Query(x,k));
	return Ret;
}

int main()
{
	int T=clock();
	read(n);
	for(int i=1;i<=n;i++)
		read(x[i]);
	x[0]=-(1<<30);
	for(int i=0;i<=n;i++)
		val[++totv]=x[i];
	sort(val+1,val+1+totv);
	totv=unique(val+1,val+1+totv)-val-1;
	for(int i=0;i<=n;i++)
		x[i]=lower_bound(val+1,val+1+totv,x[i])-val,Add(x[i]);
	for(int i=1;i<=totv;i++)
		q[i]=cur,cur+=2*siz[i]+2;
	Push_t(x[0],0);
	for(int i=1;i<=n;i++)
	{
		f[i]=Query_t(x[i],-i)+val[x[i]]-(1ll*i*i-i)/2;
		Push_t(x[i],i);
	}
	ll Ans=-1ll*n*(n+1)/2;
	for(int i=1;i<=n;i++)
		Ans=max(Ans,f[i]-1ll*(n-i)*(n-i+1)/2);
	printf("%lld\n",Ans);
	fprintf(stderr,"time=%d\n",clock()-T);
}
