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
		int f=0;
		while(!isdigit(ch)) f|=ch=='-',ch=get_char();
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

const int MAXN=2E6+10;
const int P=1E9+7;
const int inv2=(P+1)>>1;

template<typename T1,typename T2>
	void Inc(T1 &a,T2 b){a+=b;if(a>=P) a-=P;}

template<typename T1,typename T2>
	void Dec(T1 &a,T2 b){a-=b;if(a<0) a+=P;}

template<typename T1,typename T2>
	T1 Add(T1 a,T2 b){a+=b;return a>=P?a-P:a;}

template<typename T1,typename T2>
	T1 Sub(T1 a,T2 b){a-=b;return a<0?a+P:a;}

int n,X[MAXN],Y[MAXN],nxt[MAXN],pre[MAXN];
ll C[MAXN],S[MAXN],Sp[MAXN],Ss[MAXN],Sx[MAXN],Sy[MAXN],Sr[MAXN];

ll Calc1(int p,int l,int r)//sum of i in [L,R] S(pnt[p]..pnt[i]) clockwise
{
	if(l>r)
		return Add(Calc1(p,l,n),Calc1(p,1,r));
	ll Ans=0;
	if(p<=l)//[p+1,l]
		Ans=Sub(S[l],S[p])*(r-l+1)%P;
	else
		Ans=Add(S[l],Sub(S[n],S[p]))*(r-l+1)%P;
	Ans=((Ans+Ss[l+1]-Ss[r+1]-(S[r]-S[l])*(n-r))%P+P)%P;
	Ans=((Ans+X[p]*(Sy[r]-Sy[l-1])-Y[p]*(Sx[r]-Sx[l-1]))%P+P)%P;
	return Ans;
}

ll Calc2(int p,int l,int r)//sum of i in [L,R] S(pnt[p]..pnt[i]) counterclockwise
{
	if(l>r)
		return Add(Calc2(p,l,n),Calc2(p,1,r));
	ll Ans=0;
	if(r<=p)//[r+1,p] p=r is 0
		Ans=Sub(S[p],S[r])*(r-l+1)%P;
	else
		Ans=Add(S[p],Sub(S[n],S[r]))*(r-l+1)%P;
	Ans=((Ans+Sp[r]-Sp[l]-(S[r]-S[l])*l)%P+P)%P;
	Ans=((Ans+Y[p]*(Sx[r]-Sx[l-1])-X[p]*(Sy[r]-Sy[l-1]))%P+P)%P;
	return Ans;
}

ll Calc_raw(int L,int R)
{
	if(L<=R)
		return Sr[R]-Sr[L]+1ll*X[L]*Y[R]-1ll*Y[L]*X[R];
	else
		return Sr[n]-Sr[L]+Sr[R]+1ll*X[L]*Y[R]-1ll*Y[L]*X[R];
}

int main()
{
	read(n);
	for(int i=1;i<=n;i++)
		read(X[i],Y[i]),nxt[i]=i==n?1:i+1,pre[i]=i==1?n:i-1;
	Sp[1]=S[1]=C[1]=((1ll*X[1]*Y[n]-1ll*Y[1]*X[n])%P+P)%P;
	Sx[1]=X[1];Sy[1]=Y[1];
	Sr[1]=1ll*X[1]*Y[n]-1ll*Y[1]*X[n];
	for(int i=2;i<=n;i++)
	{
		C[i]=((1ll*X[i]*Y[i-1]-1ll*Y[i]*X[i-1])%P+P)%P;
		S[i]=Add(S[i-1],C[i]);
		Sr[i]=Sr[i-1]+1ll*X[i]*Y[i-1]-1ll*Y[i]*X[i-1];
		Sp[i]=(Sp[i-1]+1ll*i*C[i])%P;
		Sx[i]=(Sx[i-1]+X[i]+P)%P;
		Sy[i]=(Sy[i-1]+Y[i]+P)%P;
	}
	for(int i=n;i;i--)
		Ss[i]=(Ss[i+1]+1ll*(n-i+1)*C[i])%P;
	int ptr=1;
	ll Ans=0;
	for(int i=1;i<=n;i++)
	{
		while(Calc_raw(i,nxt[ptr])<=Sr[n]/2)
			ptr=nxt[ptr];
		if(nxt[i]!=ptr)
			Dec(Ans,Calc1(i,nxt[nxt[i]],ptr));
		if(nxt[i]!=ptr)
			Inc(Ans,Calc2(i,nxt[nxt[i]],ptr));
		if(pre[i]!=nxt[ptr])
			Inc(Ans,Calc1(i,nxt[ptr],pre[pre[i]]));
		if(pre[i]!=nxt[ptr])
			Dec(Ans,Calc2(i,nxt[ptr],pre[pre[i]]));
	}
	printf("%lld\n",Ans*inv2%P);
}
