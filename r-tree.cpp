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
typedef struct node{
public:
	bool isLeaf,isParentLeaf;

	double lon1,lat1,lon2,lat2;
	struct node *child[MAX_CHILD+1];
	int no_child;
	node()
	{

	}
	node(double lo1,double la1, double lo2,double la2)
	{
		isLeaf=false;
		lon1=lo1;
		lat1=la1;
		lon2=lo2;
		lat2=la2;
		no_child=0;
		for (int i=0;i<=MAX_CHILD;i++)
			child[i]=null;
	}
	node(double lo, double la)
	{
		isLeaf=true;
		lon1=lon2=lo;
		lat1=lat2=la;
		no_child=0;
		for (int i=0;i<=MAX_CHILD;i++)
			child[i]=null;
	}
} node;

class rtree{

public:
	node *root;
	rtree()
	{
		root=null;
	}
	void insert(double lon, double lat)
	{
		if(root==null)
		{
			root= new node(lon,lat,lon,lat);
			root->child[root->no_child++] = new node(lon,lat);
			root->isParentLeaf=true;
		}
		else{

			node *p=insert_rec(lon,lat,root);
			if(p==null)
				return;
			node *r1 = new node(min(p->lon1,root->lon1) , min(p->lat1,root->lat1) , max(p->lon2,root->lon2) , max(p->lat2,root->lat2));
			r1->child[r1->no_child++]=root;
			r1->child[r1->no_child++]=p;
			if(root->isLeaf)
				r1->isParentLeaf=true;
			else
				r1->isParentLeaf=false;
			root=r1;

		}
	}
	node* insert_rec(double lon, double lat, node* n)
	{
		if(n==null)
		{
			n=new node(lon,lat);
			return n;
		}
		else if(n->isParentLeaf)
		{
			n->child[n->no_child] = insert_rec(lon,lat,n->child[n->no_child]);
			n->no_child++;
			n->lon1 = min(n->lon1,lon);
			n->lat1=min(n->lat1,lat);
			n->lon2 = max(n->lon2,lon);
			n->lat2=max(n->lat2,lat);
		}
		else
		{

			node *k, *min1=null;
			double area_inc=INT_MAX ,lon1,lon2,lat1,lat2;
			for(int i=0;i<n->no_child;i++)
			{
				k=n->child[i];
				lon1 = min(k->lon1,lon);
				lat1=min(k->lat1,lat);
				lon2 = max(k->lon2,lon);
				lat2=max(k->lat2,lat);

				double area = (lon2-lon1)*(lat2-lat1) - (k->lon2 - k->lon1)*(k->lat2 - k->lat1);
				if(area<=area_inc)
				{
					if(area==area_inc)
					{
						if(k->no_child < min1->no_child)
							min1=k;
					}
					else
					{
						min1=k;
						area_inc=area;
					}
				}
			}
			n->lon1 = min(n->lon1,lon);
			n->lat1=min(n->lat1,lat);
			n->lon2 = max(n->lon2,lon);
			n->lat2=max(n->lat2,lat);

			k=insert_rec(lon,lat,min1);
			if(k!=null)
				n->child[n->no_child++] = k;
		}

		if(n->no_child <= MAX_CHILD)
			return null;

		return split(n);


	}

	node* split( node* n)
	{
		node* child[MAX_CHILD+1],child1[MAX_CHILD],child2[MAX_CHILD];
		int size1=0,size2=0;
		double max_dis=0,dis,a1,a2,lo1,lo2,la1,la2;
		int it1,it2;
		double avg_lon[MAX_CHILD+1],avg_lat[MAX_CHILD+1];
		for(int i=0;i<=MAX_CHILD; i++)
		{
			child[i]=n->child[i];
			n->child[i]=null;
			avg_lon[i] = child[i]->lon1 + child[i]->lon2;
			avg_lat[i] = child[i]->lat1 + child[i]->lat2;
			avg_lon[i]/=2.0;
			avg_lat[i]/=2.0;

			for(int j=0; j<i;j++)
			{
				dis = square((avg_lon[i] - avg_lon[j])) + square((avg_lat[i] - avg_lat[j])) ;
				if(dis>max_dis)
				{
					max_dis=dis;
					it1=i;
					it2=j;
				}
			}
		}
		n->lon1 = child[it1]->lon1;
		n->lat1 = child[it1]->lat1;
		n->lon2 = child[it1]->lon2;
		n->lat2 = child[it1]->lat2;
		node* p = new node(child[it2]->lon1,child[it2]->lat1,child[it2]->lon2,child[it2]->lat2);
		p->isParentLeaf=n->isParentLeaf;
		n->no_child=0;

		n->child[n->no_child++]= child[it1];
		p->child[p->no_child++]= child[it2];
		for(int i=0;i<=MAX_CHILD;i++)
		{
			if(i!=it1 && i!=it2)
			{
				lo1=min(n->lon1 , child[i]->lon1);
				lo2=max(n->lon2 , child[i]->lon2);
				la1=min(n->lat1 , child[i]->lat1);
				la2=max(n->lat2 , child[i]->lat2);
				a1 = (lo2-lo1)*(la2-la1) - (n->lon2 - n->lon1)*(n->lat2 - n->lat1);
			
				lo1=min(p->lon1 , child[i]->lon1);
				lo2=max(p->lon2 , child[i]->lon2);
				la1=min(p->lat1 , child[i]->lat1);
				la2=max(p->lat2 , child[i]->lat2);
				a2 = (lo2-lo1)*(la2-la1) - (p->lon2 - p->lon1)*(p->lat2 - p->lat1);

				if(a1<a2)
				{
					n->child[n->no_child++]= child[i];	
					n->lon1=min(n->lon1 , child[i]->lon1);
					n->lon2=max(n->lon2 , child[i]->lon2);
					n->lat1=min(n->lat1 , child[i]->lat1);
					n->lat2=max(n->lat2 , child[i]->lat2);
				}
				else
				{
					p->child[p->no_child++]= child[i];	
					p->lon1=min(p->lon1 , child[i]->lon1);
					p->lon2=max(p->lon2 , child[i]->lon2);
					p->lat1=min(p->lat1 , child[i]->lat1);
					p->lat2=max(p->lat2 , child[i]->lat2);
				}

			}
		}


		return p;

	}

	void display(node *n , int t)
	{
		if(n==null)
			return;
		cout<<endl;
		for(int i=0;i<t;i++)
			cout<<"\t";
		cout<<n->lon1<<","<<n->lat1<<"  "<<n->lon2<<","<<n->lat2;
		//cout<<"\t"<<n->no_child;
		for(int i=0;i<n->no_child;i++)
		{
			display(n->child[i],t+1);
		}
	}
	void display()
	{
		display(root,0);
	}
};

int main()
{
    fastio
    fileio
    rtree r;
    r.insert(4,4);
    
    r.insert(3,2);
    r.insert(3,2);
    r.insert(3,2);
    r.insert(3,2);
    r.insert(3,2);
    r.display();
    cout<<endl<<endl;
    r.insert(2,3);
    r.insert(3,2);
    r.display();
    return 0;
}
