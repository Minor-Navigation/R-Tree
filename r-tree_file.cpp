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
	//struct node *child[MAX_CHILD+1];
	long long child[MAX_CHILD+1];
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
			child[i]=-1;
	}
	node(double lo, double la)
	{
		isLeaf=true;
		isParentLeaf=false;
		lon1=lon2=lo;
		lat1=lat2=la;
		no_child=0;
		for (int i=0;i<=MAX_CHILD;i++)
			child[i]=-1;
	}
} node;

class rtree{

public:
	ll root;
	fstream file;
	rtree()
	{
		root=-1;
		file.open("test.txt",ios::in | ios::out | ios::trunc);
	}
	rtree(ll r, string filename)
	{
		root=r;
		if(r!=-1)
			file.open(filename,ios::in | ios::out);
		else
			file.open(filename,ios::in | ios::out | ios::trunc);

	}
	rtree(string filename)
	{
		root=-1;
		file.open(filename,ios::in | ios::out | ios::trunc);
	}
	void insert(double lon, double lat)
	{
		if(root==-1)
		{
			node k(lon,lat,lon,lat);
			file.seekp(0);
			file.seekg(0);
			file.write((char*)&k , sizeof(node));
			root=0;

			file.seekg(0,ios::end);
			file.seekp(0,ios::end);
			k.child[k.no_child++] = file.tellp();
			k.isParentLeaf=true;

			node child(lon,lat);

			file.write((char*)&child , sizeof(node));
			file.seekp(root);file.seekg(root);
			file.write((char*)&k , sizeof(node));
		}
		else{

			ll p_ptr=insert_rec(lon,lat,root);
			if(p_ptr==-1)
				return;
			node p,r;
			file.seekp(p_ptr);file.seekg(p_ptr);
			file.read((char*)&p , sizeof(node));

			file.seekp(root);file.seekg(root);
			file.read((char*)&r , sizeof(node));
			node r1(min(p.lon1,r.lon1) , min(p.lat1,r.lat1) , max(p.lon2,r.lon2) , max(p.lat2,r.lat2));


			r1.child[r1.no_child++]=root;
			r1.child[r1.no_child++]=p_ptr;

			if(r.isLeaf)
				r1.isParentLeaf=true;
			else
				r1.isParentLeaf=false;

			file.seekp(0,ios::end);file.seekg(0,ios::end);
			root = file.tellp();
			file.write((char*)&r1 , sizeof(node));
			

		}
	}
	ll insert_rec(double lon, double lat, ll n_ptr)
	{
		if(n_ptr==-1)
		{
			node n(lon,lat);
			file.seekp(0,ios::end);file.seekg(0,ios::end);
			n_ptr = file.tellp();
			file.write((char*)&n , sizeof(node));
			return n_ptr;
		}
		node n;
		file.seekp(n_ptr);file.seekg(n_ptr);
		file.read((char*)&n , sizeof(node));

		if(n.isParentLeaf)
		{
			n.child[n.no_child] = insert_rec(lon,lat,n.child[n.no_child]);
			n.no_child++;
			n.lon1 = min(n.lon1,lon);
			n.lat1=min(n.lat1,lat);
			n.lon2 = max(n.lon2,lon);
			n.lat2=max(n.lat2,lat);
			file.seekp(n_ptr);file.seekg(n_ptr);
			file.write((char*)&n , sizeof(node));
		}
		else
		{

			node k;
			ll min1=-1,k_ptr;
			double area_inc=INT_MAX ,lon1,lon2,lat1,lat2;
			for(int i=0;i<n.no_child;i++)
			{
				k_ptr=n.child[i];
				file.seekp(k_ptr);file.seekg(k_ptr);
				file.read((char*)&k , sizeof(node));
				lon1 = min(k.lon1,lon);
				lat1=min(k.lat1,lat);
				lon2 = max(k.lon2,lon);
				lat2=max(k.lat2,lat);

				double area = (lon2-lon1)*(lat2-lat1) - (k.lon2 - k.lon1)*(k.lat2 - k.lat1);
				if(area<=area_inc)
				{
					if(area==area_inc)
					{
						if(min1 == -1)
							min1=k_ptr;
						else {
							node m;
							file.seekp(min1);file.seekg(min1);
							file.read((char*)&m , sizeof(node));
							if(k.no_child < m.no_child)
								min1=k_ptr;
						}
					}
					else
					{
						min1=k_ptr;
						area_inc=area;
					}
				}
			}
			n.lon1 = min(n.lon1,lon);
			n.lat1=min(n.lat1,lat);
			n.lon2 = max(n.lon2,lon);
			n.lat2=max(n.lat2,lat);

			k_ptr=insert_rec(lon,lat,min1);
			if(k_ptr!=-1)
				n.child[n.no_child++] = k_ptr;
			file.seekp(n_ptr);file.seekg(n_ptr);
			file.write((char*)&n , sizeof(node));


		}

		if(n.no_child <= MAX_CHILD)
			return -1;

		return split(n_ptr);


	}

	ll split( ll n_ptr)
	{
		node n,child[MAX_CHILD+1];
		ll child_ptr[MAX_CHILD+1],child1[MAX_CHILD],child2[MAX_CHILD];
		
		file.seekp(n_ptr);file.seekg(n_ptr);
		file.read((char*)&n , sizeof(node));

		int size1=0,size2=0;
		double max_dis=0,dis,a1,a2,lo1,lo2,la1,la2;
		int it1=0,it2=1;
		double avg_lon[MAX_CHILD+1],avg_lat[MAX_CHILD+1];
		for(int i=0;i<=MAX_CHILD; i++)
		{
			child_ptr[i]=n.child[i];
			n.child[i]=-1;
			file.seekp(child_ptr[i]);file.seekg(child_ptr[i]);
			file.read((char*)&child[i] , sizeof(node));

			avg_lon[i] = child[i].lon1 + child[i].lon2;
			avg_lat[i] = child[i].lat1 + child[i].lat2;
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
		n.lon1 = child[it1].lon1;
		n.lat1 = child[it1].lat1;
		n.lon2 = child[it1].lon2;
		n.lat2 = child[it1].lat2;
		node p(child[it2].lon1,child[it2].lat1,child[it2].lon2,child[it2].lat2);
		p.isParentLeaf=n.isParentLeaf;
		n.no_child=0;

		n.child[n.no_child++]= child_ptr[it1];
		p.child[p.no_child++]= child_ptr[it2];
		for(int i=0;i<=MAX_CHILD;i++)
		{
			if(i!=it1 && i!=it2)
			{
				lo1=min(n.lon1 , child[i].lon1);
				lo2=max(n.lon2 , child[i].lon2);
				la1=min(n.lat1 , child[i].lat1);
				la2=max(n.lat2 , child[i].lat2);
				a1 = (lo2-lo1)*(la2-la1) - (n.lon2 - n.lon1)*(n.lat2 - n.lat1);
			
				lo1=min(p.lon1 , child[i].lon1);
				lo2=max(p.lon2 , child[i].lon2);
				la1=min(p.lat1 , child[i].lat1);
				la2=max(p.lat2 , child[i].lat2);
				a2 = (lo2-lo1)*(la2-la1) - (p.lon2 - p.lon1)*(p.lat2 - p.lat1);

				if(a1<a2)
				{
					n.child[n.no_child++]= child_ptr[i];	
					n.lon1=min(n.lon1 , child[i].lon1);
					n.lon2=max(n.lon2 , child[i].lon2);
					n.lat1=min(n.lat1 , child[i].lat1);
					n.lat2=max(n.lat2 , child[i].lat2);
				}
				else
				{
					p.child[p.no_child++]= child_ptr[i];	
					p.lon1=min(p.lon1 , child[i].lon1);
					p.lon2=max(p.lon2 , child[i].lon2);
					p.lat1=min(p.lat1 , child[i].lat1);
					p.lat2=max(p.lat2 , child[i].lat2);
				}

			}
		}

		file.seekp(n_ptr);file.seekg(n_ptr);
		file.write((char*)&n , sizeof(node));

		file.seekp(0,ios::end);file.seekg(0,ios::end);
		ll p_ptr=file.tellp();
		file.write((char*)&p , sizeof(node));
		return p_ptr;

	}

	void display(ll n_ptr , int t)
	{
		if(n_ptr==-1)
			return;
		cout<<endl;
		for(int i=0;i<t;i++)
			cout<<"\t";

		node n;
		file.seekp(n_ptr);file.seekg(n_ptr);
		file.read((char*)&n , sizeof(node));
		
		cout<<n.lon1<<","<<n.lat1<<"  "<<n.lon2<<","<<n.lat2<<" "<<n.isParentLeaf<<" "<<n.isLeaf;
		//cout<<"\t"<<n->no_child;
		for(int i=0;i<n.no_child;i++)
		{
			display(n.child[i],t+1);
		}
	}
	void display()
	{
		display(root,0);
	}
};


int main()
{
    //fastio
    fileio
    rtree r(768,"test.txt");
    //rtree r(-1,"test.txt");
    r.insert(4,4);
    // r.display();
    // cout<<endl<<endl;
    r.insert(3,2);
    r.insert(3,2);
    r.insert(3,2);
    r.insert(3,2);
    r.insert(3,2);
     r.insert(2,3);
     r.insert(2,3);
     r.insert(2,3);
     r.insert(2,3);
     r.insert(2,3);
    r.insert(3,2);
    //r.insert(4,4);
    r.display();
    cout<<endl<<endl;
   
    cout<<sizeof(node)<< " "<<r.root;
    return 0;
}
