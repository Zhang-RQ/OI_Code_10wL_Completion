#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

struct FastIO{
    static const int S=1e7;
    int wpos;
    char wbuf[S];
    FastIO():wpos(0){}

    inline int xchar()
		{
			static char buf[S];
			static int len=0,pos=0;
			if(pos==len)
				pos=0,len=fread(buf,1,S,stdin);
			return buf[pos++];
		}

    inline int operator () ()
		{
			int c=xchar(),x=0,ng=0;
			while (!isdigit(c)) ng|=(c=='-'),c=xchar();
			for(;isdigit(c);c=xchar()) x=x*10+c-'0';
			return ng?-x:x;
		}

    inline ll operator ! ()
		{
			int c=xchar(),ng=0;ll x=0;
			while(!isdigit(c)) ng|=(c=='-'),c=xchar();
			for(;isdigit(c);c=xchar()) x=x*10+c-'0';
			return ng?-x:x;
		}

    inline void wchar(int x)
		{
			if(wpos==S) fwrite(wbuf,1,S,stdout),wpos=0;
			wbuf[wpos++]=x;
		}

    inline void operator () (ll x)
		{
			if (x<0) wchar('-'),x=-x;
			char s[24];
			int n=0;
			while(x||!n) s[n++]='0'+x%10,x/=10;
			while(n--) wchar(s[n]);
			wchar('\n');
		}

    inline void space(ll x)
		{
			if (x<0) wchar('-'),x=-x;
			char s[24];
			int n=0;
			while(x||!n) s[n++]='0'+x%10,x/=10;
			while(n--) wchar(s[n]);
			wchar(' ');
		}

    inline void nextline() {wchar('\n');}

    ~FastIO() {if(wpos) fwrite(wbuf,1,wpos,stdout),wpos=0;}
}io;

const int MAXN=2E5+10;

int gcd(int x,int y){return y?gcd(y,x%y):x;}

int n,m,q,fa[MAXN],fa2[MAXN],d[MAXN];
int U[MAXN],V[MAXN],W[MAXN];

int find(int x){return x==fa[x]?x:fa[x]=find(fa[x]);}
int find2(int x){return x==fa2[x]?x:fa2[x]=find2(fa2[x]);}

void merge(int x,int y)
{
	x=find(x);y=find(y);
	if(x!=y)
	{
		fa[x]=y;
		d[y]=gcd(d[x],d[y]);
	}
}

void merge2(int x,int y)
{
	x=find2(x);y=find2(y);
	if(x!=y) fa2[x]=y;
}

int main()
{
	n=io();m=io();q=io();
	for(int i=1;i<=n;i++)
		fa[i]=fa2[i]=i,
		fa2[i+n]=i+n;
	for(int i=1;i<=m;i++)
	{
		U[i]=io();V[i]=io();W[i]=io();
		d[find(U[i])]=gcd(d[find(U[i])],W[i]);
		merge(U[i],V[i]);
	}
	for(int i=1;i<=m;i++)
	{
		W[i]/=d[find(U[i])];
		if(W[i]&1) merge2(U[i],V[i]+n),merge2(U[i]+n,V[i]);
		else merge2(U[i],V[i]),merge2(U[i]+n,V[i]+n);
	}
	for(int i=1,u,v,k;i<=q;i++)
	{
		u=io();v=io();k=io();
		if(find(u)!=find(v)) io.wchar('N'),io.wchar('I'),io.wchar('E'),io.nextline();
		else
		{
			if(k&1) io(0);
			else
			{
				if(find2(u)==find2(v)||find2(u+n)==find2(v+n))
					io(0);
				else
					io(gcd(k,d[find(u)]));
			}
		}
	}
}
