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

const int MAXV=1<<19;
const int P=1E9+7;

template<typename T1,typename T2>
	void Inc(T1 &a,T2 b){a+=b;if(a>=P) a-=P;}

template<typename T1,typename T2>
	void Dec(T1 &a,T2 b){a-=b;if(a<0) a+=P;}

template<typename T1,typename T2>
	T1 Add(T1 a,T2 b){a+=b;return a>=P?a-P:a;}

template<typename T1,typename T2>
	T1 Sub(T1 a,T2 b){a-=b;return a<0?a+P:a;}

int n,sum;
ll a[MAXV],b[MAXV];

void FWT(ll *a,int n)
{
	ll *ed=a+n;
	for(int i=2;i<=n;i<<=1)
		for(ll *x=a,*y=a+(i>>1);x!=ed;x+=i,y+=i)
			for(int k=0;k<(i>>1);k++)
			{
				ll u=x[k],t=y[k];
				x[k]=Add(u,t);y[k]=Sub(u,t);
			}
}

ll get_val(ll *a,int n,int pos)
{
	ll Ret=0;
	for(int s=0;s<n;s++)
		if(__builtin_popcount(s&pos)&1)
			Dec(Ret,a[s]);
		else
			Inc(Ret,a[s]);
	return Ret;
}

int main()
{
	read(n);
	for(int i=1,x;i<=n;i++)
		read(x),a[x]=1,sum^=x;
	FWT(a,MAXV);
	if(!sum)
		return printf("%d\n",n),0;
	int Ans=n-1;
	memcpy(b,a,sizeof b);
	while(!get_val(b,MAXV,sum))
	{
		--Ans;
		for(int s=0;s<MAXV;s++)
			b[s]=b[s]*a[s]%P;
	}
	printf("%d\n",Ans);
}
