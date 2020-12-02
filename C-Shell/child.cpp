#include <bits/stdc++.h>
#include<iostream>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<pwd.h>
#include<ctype.h>
#include<dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<termios.h>
#include<ctype.h>
 
using namespace std;
 
#define ll         long long int
#define ld          double
#define vi          vector <int>
#define vl          vector<ll>
#define vp          vector <pair<int,int>>
#define vvi         vector <vector <int>>
#define vpl         vector<pair<ll,ll>>
#define vvl         vector<vector<ll>>
#define pb          push_back
#define mp          make_pair
#define pi          pair<int,int>
#define all(a)      a.begin(),a.end()
#define tillN(a,n)  a.begin(),a.begin() + n
#define ff          first
#define ss          second
#define minel(a)	*min_element(all(a))
#define maxel(a)	*max_element(all(a))
#define minelN(a,n) *min_element(tillN(a,n))
#define maxelN(a,n) *max_element(tillN(a,n))
#define mod         1000000007
 
ll inf = 1e18;
 
ll expo(ll x,ll y)
{
        ll ans = 1;
        while(y!=0)
        {
                if(y%2)
                {
                        ans = ans*x;
                }
 
                y = y>>1;
                x = x*x;
        }
 
        return ans;
}
 
ll gcd(ll a,ll b)
{
	if(a==0)
	  return b;
 
	 return gcd(b%a , a);
}
 
ll lcm(ll a, ll b)
{
	return (a*b) / gcd(a,b);
}
 
ll max(ll x,ll y)
{
    if(x > y)
        return x;
    return y;
 
}
 
ll min(ll x,ll y)
{
    if(x < y)
        return x;
    return y;
 
}
 
vl SieveOfEratosthenes(ll n) 
{
    
    vl primes; 
     
    bool prime[n+1]; 
    memset(prime, true, sizeof(prime)); 
  
    for (ll p=2; p*p<=n; p++) 
    { 
         
        if (prime[p] == true) 
        {   
            for (ll i=p*p; i<=n; i += p) 
                prime[i] = false; 
        } 
    } 
  
    
    for (ll p=2; p<=n; p++) 
       if (prime[p]) 
          primes.pb(p); 
 
    return primes;
}
 
vl getDivisors(ll num)
{
    vl divisors;
    for(ll i=1;i*i<=num;i++)
    {
        if(!(num%i))
        {
            if(i*i == num)
                divisors.pb(i);
            else
            {
                divisors.pb(i);
                divisors.pb(num/i);
            }
        }
    }
 
    sort(all(divisors));
    return divisors;
}
 
void displayVector(vl vect)
{
    for(ll i=0;i<vect.size();i++)
        cout << vect[i] << " ";
    cout << endl;
}
 
int visited[300005] = {0};
int recStack[300005] = {0};
vl gr[300005];
vl topSort;
 
bool hasCycle(ll node)
{
	if(!visited[node])
	{
		visited[node] = 1;
		recStack[node] = 1;
		ll len = gr[node].size();
		ll nbr;
		for(ll i=0;i<len;i++)
		{
			nbr = gr[node][i];
			if(!visited[nbr] && hasCycle(nbr))
				return true;
			else
			{
				if(recStack[nbr])
					return true;
			}
		}
	}
 
	recStack[node] = 0;
	return false;
}
 
void topoSort(ll node)
{
	visited[node] = 1;
	ll len = gr[node].size();
	ll nbr;
	for(ll i=0;i<len;i++)
	{
		nbr = gr[node][i];
		if(!visited[nbr])
		{
			topoSort(nbr);
		}
	}
 
	topSort.pb(node);
}
 
 
int main()
{
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    sleep(2);
    printf("Hello Hi\n");

}
