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

const int P=998244353;

int B[32],siz,n,q;

ll ksm(ll a,ll b)
{
	ll ret=1;
	for(;b;b>>=1,(a*=a)%=P)
		if(b&1)
			(ret*=a)%=P;
	return ret;
}

void insert(int x)
{
	for(int i=29;~i;i--)
		if((x>>i)&1)
		{
			if(B[i]) x^=B[i];
			else return B[i]=x,++siz,void();
		}
}

bool query(int x)
{
	for(int i=29;~i;i--)
		if((x>>i)&1)
		{
			if(B[i]) x^=B[i];
			else return false;
		}
	return true;
}

int main()
{
	n=io();q=io();
	for(int i=1,x;i<=n;i++)
		x=io(),insert(x);
	int Ans=ksm(2,n-siz);
	for(int i=1,x;i<=q;i++)
	{
		x=io();
		io(query(x)?Ans:0);
	}
}
