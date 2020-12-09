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

const int MAXN=1E5+10;
const int MAXQ=5E5+10;

int n,k,q,diff[40],a[MAXN],tmp[40];
vector<pair<int,int> > Qr[MAXN];
ll t[MAXN<<2],tg[MAXN<<2],Ans[MAXQ];

void pushup(int x){t[x]=t[x<<1]+t[x<<1|1];}

void apply_tag(int x,int dlt,int len)
{
	t[x]+=1ll*dlt*len;
	tg[x]+=dlt;
}

void pushdown(int x,int l,int r)
{
	if(tg[x])
	{
		int mid=(l+r)>>1;
		apply_tag(x<<1,tg[x],mid-l+1);
		apply_tag(x<<1|1,tg[x],r-mid);
		tg[x]=0;
	}
}

void change(int x,int l,int r,int cl,int cr,int dlt)
{
	if(cl<=l&&r<=cr)
		return apply_tag(x,1,r-l+1),void();
	int mid=(l+r)>>1;pushdown(x,l,r);
	if(cl<=mid)
		change(x<<1,l,mid,cl,cr,dlt);
	if(cr>mid)
		change(x<<1|1,mid+1,r,cl,cr,dlt);
	pushup(x);
}

ll query(int x,int l,int r,int ql,int qr)
{
	if(ql<=l&&r<=qr)
		return t[x];
	int mid=(l+r)>>1;pushdown(x,l,r);
	if(qr<=mid)
		return query(x<<1,l,mid,ql,qr);
	else if(ql>mid)
		return query(x<<1|1,mid+1,r,ql,qr);
	else
		return query(x<<1,l,mid,ql,qr)+query(x<<1|1,mid+1,r,ql,qr);
}

int main()
{
	n=io();q=io();k=io();
	for(int i=1;i<=n;i++)
		a[i]=io();
	for(int i=1,l,r;i<=q;i++)
	{
		l=io();r=io();
		Qr[l].emplace_back(r,i);
	}
	for(int i=0;i<=30;i++)
		diff[i]=n+1;
	for(int i=n,tot;i;i--)
	{
		tot=0;
		for(int b=0;b<=30;b++)
			if(!((a[i]>>b)&1))
				diff[b]=i;
			else
				tmp[++tot]=diff[b];
		sort(tmp+1,tmp+1+tot);
		tot=unique(tmp+1,tmp+1+tot)-tmp-1;
		int cur=a[i],lst=i;
		for(int j=1;j<=tot;j++)
		{
			//[lst,tmp[j]-1]
			if(cur%k==0)
				change(1,1,n,lst,tmp[j]-1,1);
			cur&=a[tmp[j]];lst=tmp[j];
		}
		if(lst!=n+1)
		{
			if(cur%k==0)
				change(1,1,n,lst,n,1);
		}
		for(auto x:Qr[i])
			Ans[x.second]=query(1,1,n,i,x.first);
	}
	for(int i=1;i<=q;i++)
		io(Ans[i]);
}
