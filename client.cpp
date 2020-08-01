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
	GEN p = gp_read_str("984900564946096700490874221432698721065852649803936961789215583215532533741972967436277027081557425733992821873631940817584280130927057090038885234521");
	GEN q = gp_read_str("740254187565987948958102873509716416589947705017628095218693058329222342064069077267414257536015202269233297216017410636540147640945352437735678341301");
	//GEN p = gp_read_str("604329747228966517797276002244205200743037264356775873508274030077307582965089472056214542887933062485070698353064392531");
	//GEN q = gp_read_str("507077020262454331269031749656740084893790234052618426056716546869172277403737400042361642255195005825505672282999958271");
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
    	string ss = argv[1];
    	int id = stoi(ss);
    	//********************************************************//
    	freopen("test.txt", "r", stdin);   // Read from file
		//printf("Hello I am Client\n");
		// We will choose one of the test_images from this file for authentication process.
		int dim, size;
		scanf("%d",&size);
		scanf("%d",&dim);
		vector<vector<int>> arr(size+1 , vector<int> (dim+1));   
		GEN encrypted(cgetg(dim+2,t_VEC));
		for(int i=1;i<=size;i++)
		{
		    for(int j=1;j<=dim;j++)
		        scanf("%d",&arr[i][j]);
		}
		
		//************************************************************//
		// i denote which test_image we choose for authentication
		freopen ("/dev/tty", "r", stdin);  
		int temp[dim+1];
		for(int j=1;j<=dim;j++)
		    temp[j] = arr[id][j];
		    
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
		//cout << dim << endl;
		freopen("clientTOserver.txt","w",stdout);   // Write to the file
		
		pari_printf("%Ps\n%Ps\n",g,n);
		cout << dim << endl;
		for(int i=1;i<=dim;i++)
			pari_printf("%Ps ",gel(encrypted,i));
		
		freopen ("/dev/tty", "w", stdout);    // Change back to stdout from file to standard out
	
		cout << "User with ID : " << id/2 + (id%2==1) << " wants to enter into System." << endl;
		//**************************************************************//
		//We have to send our template to server
		//we put it into file called "clientTOserver.txt"
		if(fork()==0)
		{
		    //printf("I am child of client program and I am sending query template to server\n");
		    char *args[]={"./server",NULL}; 
		    execv(args[0],args);
		}
		wait(NULL);
		//printf("I am parent of client program and I am ready to procced in Client program\n");
		
		// We have got answer in File called "serverTOclient.txt"
		freopen("serverTOclient.txt","r",stdin);
		
		int result;
		scanf("%d",&result);
		double time_ser;
		scanf("%lf",&time_ser);
		freopen ("/dev/tty", "r", stdin);    // Change back to stdout from file to standard out
		//printf("%lf\n",time_ser);
		//printf("Client FINISH\n");
		clock_t end;
		end = clock();
		time_ser +=  ((double)(end-start)) / CLOCKS_PER_SEC;
		if(result==1)
			cout << "User is valid and User can enter into system" << endl;
		else
			cout << "User is not valid, You can try again" << endl;
		cout << "Time Taken by Authentication Process: ";
		cout << time_ser << endl;
		cout << endl;
		
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
		   	
		   	clock_t end = clock();
		    freopen ("/dev/tty", "r", stdin);    
		    temp = Fp_pow(g,out,nsquare);
		    temp = Fp_mul(temp, Fp_pow( r , n , nsquare) , nsquare);
		    freopen("clientTOserver.txt","w",stdout);  
			pari_printf("%Ps\n",temp);
			printf("%f\n",(double)(end - start) / CLOCKS_PER_SEC);
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
			
			clock_t end = clock();
			GEN nby2 = gdivent(n,stoi((long)2));
			freopen("clientTOserver.txt","w",stdout);
			
			if(gcmp(temp,nby2)==(long)1)
				cout << 0 << endl;
			else
				cout << 1 << endl;
		    printf("%f\n",(double)(end - start) / CLOCKS_PER_SEC);
		    freopen ("/dev/tty", "r", stdout);    // Change back to stdout from file to standard out
		}
	}
}
