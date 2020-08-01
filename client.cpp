#include<bits/stdc++.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <unistd.h>
#include<pari/pari.h>

using namespace std;

typedef long long int ll;

GEN input_gen()
{
	string s;
	cin >> s;
	char pp[s.length()+1];
	strcpy(pp,s.c_str());
	GEN temp = gp_read_str(pp);
	return temp;
}


int main(int argc, char** argv) 
{   
    clock_t start;
    start = clock();
    pari_init(2500001792,2);
    // Generating Private parameter of Paillier Cryptosystem
    // Encryption Start
	GEN p = gp_read_str("6218021483076269348132291041647045291708771276517");
	GEN q = gp_read_str("88877553569593810325200721320646244933078402661102507");

	GEN n = gmul(p,q);
	GEN r = stoi((long)46919);
	GEN g = gadd(n,stoi((long)1));
	GEN nsquare = gsqr(n);
	GEN lamda = glcm(gsub(p,stoi((long)1)),gsub(q,stoi((long)1)));
    GEN precomp = Fp_pow(r,n,nsquare);
    GEN m;
    GEN c;
	
    if(argc==2)
    {	
    	//********************************************************//
    	freopen("test.txt", "r", stdin);   // Read from file
		printf("Hello I am Client\n");
		// We will choose one of the test_images from this file for authentication process.
		int dim, size;
		cin >> size >> dim;
		vector<vector<int>> arr(size+1 , vector<int> (dim+1));   
		GEN encrypted(cgetg(dim+2,t_VEC));
		for(int i=1;i<=size;i++)
		{
		    for(int j=1;j<=dim;j++)
		        cin >> arr[i][j];
		}

		//************************************************************//
		// i denote which test_image we choose for authentication
		freopen ("/dev/tty", "r", stdin);  
		int temp[dim+1];
		int i = 4;
		for(int j=1;j<=dim;j++)
		    temp[j] = arr[i][j];
		    
		// Here, we are encrypting test_image for keeping privacy of users' biometric data from outsider(Including serve).
		int count = 0;
		for(int i=1;i<=dim;i++)
		{
		    m = stoi((long)temp[i]);
		    c = Fp_pow(g,m,nsquare);
		    c = Fp_mul(c,precomp,nsquare);
		    gel(encrypted,i) = c;	
		}
		
		//*************************************************************//
		// Sending encrypted image to the server
		freopen("clientTOserver.txt","w",stdout);   // Write to the file
		
		pari_printf("%Ps %Ps ",g,n);
		printf("%d\n",dim);
		for(int i=1;i<=dim;i++)
			pari_printf("%Ps ",gel(encrypted,i));
		
		freopen ("/dev/tty", "w", stdout);    // Change back to stdout from file to standard out
		
		//**************************************************************//
		//We have to send our template to server
		//we put it into file called "clientTOserver.txt"
		if(fork()==0)
		{
		    printf("I am child of client program and I am sending query template to server\n");
		    char *args[]={"./server",NULL}; 
		    execv(args[0],args);
		}
		wait(NULL);
		printf("I am parent of client program and I am ready to procced in Client program\n");
		
		// We have got answer in File called "serverTOclient.txt"
		freopen("serverTOclient.txt","r",stdin);
		
		string s;
		cin >> s;
		char pp[s.length()+1];
		strcpy(pp,s.c_str());
		GEN output = gp_read_str(pp);
		// Output is Encrypted minimum distance
		cout << "Encrypted Minimum Distance : " << endl;
		// Finding Original distance. Then we will compare this with pre-define thershold(For final decision whether user is valid or not.)
		GEN nume = Fp_pow(output,lamda,nsquare);
		nume = gsub(nume,stoi((long)1));
		nume = gdivent(nume,n);
		GEN deno = Fp_pow(g,lamda,nsquare);
		deno = gsub(deno,stoi((long)1));
		deno = gdivent(deno,n);
		GEN ddcc = Fp_mul(nume, Fp_inv(deno,n) , n );
		pari_printf("%Ps\n",ddcc);
		
		freopen ("/dev/tty", "r", stdin);    // Change back to stdout from file to standard out

		printf("Client FINISH\n");
		clock_t end;
		end = clock();
		printf("\nTook %f seconds\n", (double)(end-start)/CLOCKS_PER_SEC);
		
    }
	else
	{
		freopen("serverTOclient.txt","r",stdin);
		bool f;
		cin >> f;
		if(f)
		{
			// For finding value of S3_dash we need client's help --> we have En(x)
			// We have to find x^2 at client side --> En(x^2) Sending to server 
		    int len;
		    cin >> len;
			GEN temp;
			GEN out = stoi((long)0);
		    for(int i=1;i<=len;i++)
		    {
				temp = input_gen();
				GEN nume = Fp_pow(temp,lamda,nsquare);
				nume = gsub(nume,stoi((long)1));
				nume = gdivent(nume,n);
				GEN deno = Fp_pow(g,lamda,nsquare);
				deno = gsub(deno,stoi((long)1));
				deno = gdivent(deno,n);
				temp = Fp_mul(nume, Fp_inv(deno,n) , n );
				temp = Fp_mul(temp,temp,n);
				out = Fp_add(out,temp,n);
		    }
		   	
		    freopen ("/dev/tty", "r", stdin);    
		    temp = Fp_pow(g,out,nsquare);
		    temp = Fp_mul(temp, Fp_pow( r , n , nsquare) , nsquare);
		    freopen("clientTOserver.txt","w",stdout);   
			pari_printf("%Ps\n",temp);
		    freopen ("/dev/tty", "w", stdout); 
		}
		else
		{
			//Again for comparision, we need help of client and here we are finding whether a is greater than b(1) or not(0).
			// ( * ) denote return value.
			
		    GEN comp;
			
			comp = input_gen();
			
		    freopen ("/dev/tty", "r", stdin);    // Change back to stdout from file to standard out
			
			GEN temp;
			GEN nume = Fp_pow(comp,lamda,nsquare);
			nume = gsub(nume,stoi((long)1));
			nume = gdivent(nume,n);
			GEN deno = Fp_pow(g,lamda,nsquare);
			deno = gsub(deno,stoi((long)1));
			deno = gdivent(deno,n);
			temp = Fp_mul(nume, Fp_inv(deno,n) , n );
			
			GEN nby2 = gdivent(n,stoi((long)2));
			freopen("clientTOserver.txt","w",stdout);
			
			if(gcmp(temp,nby2)==(long)1)
				cout << 0 << endl;
			else
				cout << 1 << endl;
		        
		    freopen ("/dev/tty", "r", stdout);    // Change back to stdout from file to standard out
		}
	}
}
