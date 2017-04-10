#include <bits/stdc++.h>
using namespace std;

#define min(a,b) ((a<b) ? (a) : (b))
#define max(a,b) ((a>b) ? (a) : (b))
#define abs(a) ((a>0) ? (a) : ((-1)*a))
#define ll long long
#define FOR(i,n) for(int i=0;i<n;i++)
#define FORL(i,n) for(long long i=0;i<n;i++)
#define MOD 1000000007
#define PI 3.141592653589
#define pb push_back
#define mp make_pair
int dp[5009][5009]={0};
#define fastio ios_base::sync_with_stdio(false); cin.tie(0);
#define fileio freopen("input.txt", "r", stdin); freopen("output.txt", "w", stdout);
#define MAX_CHILD 5
#define null NULL
#define square(a) a*a

class hello{
public:
	int id;
	long long x;
	hello()
	{
		id=0;
		x=0;
	}
	hello(int i, ll x1)
	{
		id=i;
		x=x1;
	}
};

int main()
{
    //fastio
	//WRITING
    /*
    fileio
    fstream t;
    hello h(1,200000);
    cout<<sizeof(h)<<endl;
    t.open("test.txt",ios::in | ios::out | ios::ate);
    if(t.is_open())
    {
    t.write((char*)&h , sizeof(hello));
    cout<<t.tellp();
    t.close();
    }
    else
    	cout<<"error";
    */

	//READING

	fstream t;
	t.open("test.txt",ios::in | ios::out);
	cout<<t.tellp()<<" "<<t.tellg();
	hello h;
	t.read((char*)&h, sizeof(hello));
	cout<<t.tellp()<<" "<<t.tellg();
	cout<<h.id;

    return 0;
}
