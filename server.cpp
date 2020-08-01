#include<bits/stdc++.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <unistd.h>
#include<omp.h>
#include<pari/pari.h>
#include <chrono>
#include <thread>

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

int main()
{
	
	double tot_time = 0.0;
    clock_t ini_start;
    ini_start = clock();
	pari_init(2500001792,2);
    //printf("Hello I am server\n");
    
    //**********************************************************//
    //Public Parameter of Pailier Cryptosystem
    
    freopen("clientTOserver.txt","r",stdin);
    int query_dim,client_store;
    GEN g,n;
    g = input_gen();
    n = input_gen();
    GEN nsquare = gsqr(n);
    cin >> query_dim;
    GEN en_query(cgetg(query_dim+2,t_VEC));
    // Input Encrypted Image
    for(int i=1;i<=query_dim;i++)
        gel(en_query,i) = input_gen();

    freopen ("/dev/tty", "r", stdin);    
 
    //****************************************************//
    //Eigen_Faces (Extracted Using Principle Component Analysis)
    
    freopen("eigen_face.txt","r",stdin);
    int basis_number, basis_Dim;
    scanf("%d %d",&basis_Dim, &basis_number);
    basis_number = 12;
    int norm = 1000;
    // basis_number -> number of basis vector
    // basis_dim -> Dimension of basis vector
    vector<vector<double> > basis( basis_number+1 , vector<double> (basis_Dim+1));  
    //cout << "Basis Input Start" << endl;
    for(int i=1;i<=basis_Dim;i++)
    {
        for(int j=1;j<=basis_number;j++)
            cin >> basis[j][i];
    }
    //cout << "Basis Input End" << endl;
    freopen ("/dev/tty", "r", stdin);  

	//****************************************************************//
	// Stored Images(At server side)
    /*freopen("New_test.txt","r",stdin);
    int store_size,store_dim, norm = 1000;
    scanf("%d %d",&store_size,&store_dim);
    vector<vector<ll> > store( store_size+1, vector<ll> (store_dim+1));  

    //cout << "Store Input Start" << endl;
    for(int i=1;i<=store_size;i++)
    {
        for(int j=1;j<=store_dim;j++)
            scanf("%lld",&store[i][j]);
    }
    
    //cout << "Store Input End" << endl;
    freopen ("/dev/tty", "r", stdin); */
    
    //***************************************************************
   	// This section was specially for Debugging Purpose (This should not be here.)
    /*freopen("test.txt","r",stdin);
    
    int test_size,test_dim;
    cin >> test_size >> test_dim;
    vector<vector<ll> > test(test_size+1 , vector<ll> (test_dim+1));  
    //cout << "Test Input Start" << endl;

    for(int i=1;i<=test_size;i++)
    {
        for(int j=1;j<=test_dim;j++)
		{
			cin >> test[i][j];
		}            
    }
    //cout << "Test Input End" << endl;
    freopen ("/dev/tty", "r", stdin); 
	*/
	//***********************************************************//
	// Mean of Each Pixel of Images. And Images have 92*112 pixels.
    freopen("mean_Of_EachPixel.txt","r",stdin);
    int mean_sz;
    cin >> mean_sz;
	vector<ll> mean(mean_sz+1);
    //cout << "Mean Start" << endl;
    for(int i=1;i<=mean_sz;i++)
    {
        double x;
        scanf("%lf",&x);
        mean[i] = llround(x);
    } 
    //cout << "Mean End" << endl;
	freopen ("/dev/tty", "r", stdin);
	

    //***************************************************************//
    freopen("store_feature.txt","r",stdin);
    int store_size;
    cin >> store_size >> basis_number;
	// In this section, we are generating feature vector of stored images using basis_Vectors.
    vector<vector<ll> > store_feature(store_size+1 , vector<ll> (basis_number+1));  
    //cout << "Store Feature Start" << endl;
    
    for(int i=1;i<=store_size;i++)
    {
        for(int j=1;j<=basis_number;j++)
        {
            cin >> store_feature[i][j];
        }
    }
	
	freopen("/dev/tty","r",stdin);
    //cout << "Store Feature End" << endl;
    
    //****************************************************************
    // Again this section was here for debugging purpose.
    /*vector<vector<ll> > test_feature(test_size+1 , vector<ll> (basis_number+1));  
    
    //cout << "Test Feature Start" << endl;

    for(int i=1;i<=test_size;i++)
    {
        for(int j=1;j<=basis_number;j++)
        {
            long long int temp = 0.0;
            for(int k=1;k<=store_dim;k++)
                temp += (test[i][k]-mean[k])*llround(norm*basis[j][k]);
            test_feature[i][j] = llround(temp);
        }
    }
	
	// This is for finding distance in original form. (For debugging purpose.)
	
    //cout << "Test Feature End" << endl;
	//freopen("dist.txt","w",stdout);
	int iidd = 1;
	ll mn = 1e18;
	int ans = -1;
	for(int i=1;i<=store_size;i++)
	{
		ll diss = 0;
		for(int j=1;j<=basis_number;j++)
		{
			diss += (store_feature[i][j]-test_feature[iidd][j])*(store_feature[i][j]-test_feature[iidd][j]);
		}
		//cout << "i : " << diss << endl;
		if(mn > diss)
		{	
			ans = i;
			mn = diss;
		}
	}
	*/
	// From now onwards we will only deal with Encrypted numbers.
	
	//*********************************************************
	// In this section, we are calculating mean subtracted images 
	//cout << "Mean Subtracted Start" << endl;   
	//cout << query_dim << endl; 
	GEN mean_sub_query(cgetg(query_dim+2,t_VEC));
	GEN temp;
	GEN r = gp_read_str("46919");
	GEN pre = Fp_pow(r,n,nsquare); 
    for(int i=1;i<=query_dim;i++)
    {
        long te = abs(mean[i]);
        temp = stoi(te);
        GEN tt;
        if(mean[i]>0)
	        tt = Fp_inv(g,nsquare);
	    else
	    	tt = g;
	    
        temp = Fp_pow(tt,temp,nsquare);
        temp = Fp_mul(temp,gel(en_query,i),nsquare);
        gel(mean_sub_query,i) = temp;
    }   
    //cout << basis_number << endl;
    //cout << "Mean Subtracted End" << endl;

	//************************************************************//
	// Now we are converting Input encrypted mean subtracted image into feature vector.
    GEN en_query_feature(cgetg(basis_number+2,t_VEC));
    //cout << "En_Feature(Query) Start" << endl;
    int i;
    GEN final1;
    clock_t start1,end1;
    start1 = clock();
    GEN base,expo;
    {
        for(i=1;i<=basis_number;i++)    
        {
        	//cout << i << endl;
            final1 = stoi((long)1); 
            clock_t start,end;
            start = clock();
            int pos = 0, neg = 0;
            int pos_z = 0, neg_z = 0;
            for(int j=1;j<=basis_Dim;j++)
            {
                ll uij = llround(norm*basis[i][j]);
               	base = gel(mean_sub_query,j);
               	expo = stoi((long)abs(uij));
 				//cout << j << endl;
 				//cout << uij << endl;
                if(uij<0)
                {
                    temp = Fp_inv(base,nsquare);
                    temp = Fp_pow(temp,expo,nsquare);
                }
                else
                	temp = Fp_pow(base,expo,nsquare);
                //cout << j << endl;
				final1 = Fp_mul(final1,temp,nsquare);
            }
            end = clock();
            gel(en_query_feature,i) = final1;
        }
    }   
    end1 = clock();
    //printf("%f\n ",double(end1 - ini_start) / CLOCKS_PER_SEC);
    //cout << "En_Feature(Query) End" << endl;
    
    //*******************************************************//
    // In this section, we are finding minimum distance from Encrypted Test feature vector with stored feature vector.. 
    GEN min_Dist = gadd(nsquare,nsquare);
    //cout << "Distance-start" << endl;
    int id;
    GEN precomp;
    precomp = Fp_pow(r,n,nsquare);
    //GEN original1 = stoi((long)0);
    int cnt = 0;
    clock_t start,end;
    start = clock();
    for(i=1;i<=store_size;i++)
    {
        //cout << i << endl;
        // Calculation of S1
        GEN Encrypt_S1 = gp_read_str("0");
        for(int j=1;j<=basis_number;j++)
        {
        	temp = stoi((long)(store_feature[i][j]*store_feature[i][j]));
			Encrypt_S1 = gadd(Encrypt_S1,temp);
        }
        
        temp = Fp_pow(g,Encrypt_S1,nsquare);
        temp = Fp_mul(temp,precomp,nsquare);
    	Encrypt_S1 = temp;
    	//cout << "End S1" << endl;
    	// Calculation of S2
        GEN Encrypt_S2 =  gp_read_str("1");
        for(int j=1;j<=basis_number;j++)        
        {
        	temp = stoi((long)store_feature[i][j]);
        	temp = gmul(temp, stoi((long)2));
        	Encrypt_S2 = Fp_mul(Encrypt_S2, Fp_pow( Fp_inv(gel(en_query_feature,j), nsquare) , temp , nsquare)  , nsquare);
		}
    	//cout << "End S2" << endl;
		// Calculation of S3
		temp = stoi((long)1);
		GEN rnd;
		//printf("%d",basis_number);
        freopen("serverTOclient.txt","w",stdout);
        printf("1\n");
        printf("%d\n",basis_number);
        
        //original1 = stoi((long)0);
        for(int j=1;j<=basis_number;j++)
        {
        	while(1)
        	{
        		rnd = randomi(gp_read_str("2303"));
        		if(gcmp(rnd,stoi((long)0)) == (long)0)
        			continue;
				if(gcmp(ggcd(rnd,n),stoi((long)1)) == (long)0 )
					break;
        	}
			
			temp = Fp_mul( temp , Fp_pow( Fp_inv( gel(en_query_feature,j) , nsquare) , gmul(stoi((long)2),rnd) , nsquare ) , nsquare);
            
            GEN ppp = gmul(rnd,stoi((long)(-1)));
            GEN rnd_s = Fp_mul(ppp,rnd,n);
            rnd_s = Fp_add(rnd_s,n,n);
            GEN En_rnd_s = Fp_pow(g,rnd_s,nsquare);
            En_rnd_s = Fp_mul(En_rnd_s, precomp, nsquare);
            
            temp = Fp_mul(temp, En_rnd_s, nsquare);
            GEN En_rnd = Fp_pow(g,rnd,nsquare);
            En_rnd = Fp_mul(En_rnd, precomp,nsquare);
			
			GEN out = Fp_mul(gel(en_query_feature,j),En_rnd,nsquare);
			
			//original1 = gadd(original1, gsqr( gadd(stoi((long)test_feature[1][j]),rnd) ) );
            pari_printf("%Ps ",out);
        }
        
        freopen ("/dev/tty", "w", stdout);
        if(fork()==0)
        {   
            char *args[]={"./client",NULL}; 
            execv(args[0],args);
        }
        
        wait(NULL);
	
        freopen("clientTOserver.txt","r",stdin);
        
		GEN En_S3_dash;
		En_S3_dash = input_gen();
		double time_cli;
		scanf("%lf",&time_cli);
		
        freopen ("/dev/tty", "r", stdin);   
        //cout << "comm-1 " << time_cli << endl;
        //cout << i << endl;
        //cout << time_cli << endl;
		//pari_printf("server --> %Ps\n",En_S3_dash);
		GEN Encrypt_S3 = Fp_mul(temp, En_S3_dash, nsquare);
		tot_time += time_cli;
		// Calculating Distance 
		GEN distance = Fp_mul(Encrypt_S1, Encrypt_S2, nsquare);
		distance = Fp_mul(distance, Encrypt_S3,nsquare);	
		
		if(i==1)
		{
			id = 1;
			min_Dist = distance;
			end = clock();
			continue;			
		}
		// Comparision of current distance with min_D to keep tack of minimum distance. (Need communication)
		GEN comp1;
		comp1 = Fp_inv(distance,nsquare);
	    comp1 = Fp_mul(comp1,min_Dist,nsquare);	
        comp1 = Fp_pow(comp1,stoi((long)3),nsquare);
		
        freopen("serverTOclient.txt","w",stdout);
        printf("0\n");
		pari_printf("%Ps\n",comp1);
		
        freopen ("/dev/tty", "w", stdout);    
        if(fork()==0)
        {
            char *args[]={"./client",NULL}; 
            execv(args[0],args);
        }
        
        wait(NULL);
        freopen("clientTOserver.txt","r",stdin);
        
        int val;
        scanf("%d",&val);
		scanf("%lf",&time_cli);
		//cout << time_cli << endl;
        freopen ("/dev/tty", "r", stdin);  
        //cout << "comm-2 " << time_cli << endl;  
        if(val)
		{
			id = i;
			min_Dist = distance;
		}
		tot_time += time_cli;
        //cout << i << endl;
    }
    end = clock();
    //printf("%f\n",double(end-start)/CLOCKS_PER_SEC);
    
	//cout << "Distance-End" << endl;
	//printf("%lf\n",tot_time);
	//cout << "Original Id of min : " << ans << endl;
	//cout << "Id of min : " << id <<  endl; 
	GEN threshold = gp_read_str("2147483647");
	GEN en_thre = Fp_pow(g,threshold,nsquare);
	en_thre = Fp_mul(en_thre, precomp, nsquare);
	GEN comp1;
	comp1 = Fp_inv(min_Dist,nsquare);
    comp1 = Fp_mul(comp1,en_thre,nsquare);	
    comp1 = Fp_pow(comp1,stoi((long)3),nsquare);
	
    freopen("serverTOclient.txt","w",stdout);
    
    printf("0\n");
	pari_printf("%Ps\n",comp1);
	
    freopen ("/dev/tty", "w", stdout);    
    if(fork()==0)
    {
        char *args[]={"./client",NULL}; 
        execv(args[0],args);
    }
    wait(NULL);
    
    freopen("clientTOserver.txt","r",stdin);
		
	int val;
	double time_cli;
	scanf("%d",&val);
	scanf("%lf",&time_cli);

	freopen ("/dev/tty", "r", stdin);    
	tot_time += time_cli;
	
	clock_t ini_end;
   	ini_end = clock();
	tot_time += ((double)(ini_end-ini_start))/CLOCKS_PER_SEC;
   	//printf("\nTook %f seconds\n", tot_time);
   	freopen("serverTOclient.txt","w",stdout); 
	printf("%d\n",val);
	printf("%f\n", tot_time);
   	freopen ("/dev/tty", "w", stdout);   
    //printf("Server FINISH\n");

   
    return 0;
}

/*BigInt encryption(BigInt m , BigInt r)
{
    BigInt c = pow1(g,m.to_long_long(),n*n);
    c = (c * pow1(r,n.to_long_long(),n*n))%(n*n);
    return c;
}*/

/*void keyGeneration(int bitLength)
{
	srand(time(NULL));

	BigInt p = findLargePrime(bitLength);
    BigInt q;

	do

	{
		q = findLargePrime(bitLength);
	}
	while (q==p);

	g = BigInt("2");

   lambda = ((p-1)*(q-1)) / (gcd(p-1, q-1));
   n = p*q;
   nsquare = n*n;

   cout<<"p prime : "<<p<<endl;
   cout<<"q prime : "<<q<<endl;
   cout<<"g : "<<g<<endl;
   cout<<"lambda : "<<lambda<<endl;
   cout<<"n : "<<n<<endl;
   cout<<"nsquare : "<<nsquare<<endl;

    if(gcd(((pow2(g,lambda, nsquare)-1) / n), n) != 1)
    {
    	cout << "Find another g.";
       return;
    }
}*/

/*if(i==1)
{	
	GEN temp = stoi((long)test[i][j]);
	GEN nume = Fp_pow(gel(en_query,j),lamda,nsquare);
	nume = gsub(nume,stoi((long)1));
	nume = gdivent(nume,n);
	GEN deno = Fp_pow(g,lamda,nsquare);
	deno = gsub(deno,stoi((long)1));
	deno = gdivent(deno,n);
	GEN ddcc = Fp_mul(nume, Fp_inv(deno,n) , n );
	
	pari_printf("DECRYPT : %Ps \n",ddcc);
	pari_printf("ORIGINAL : %Ps \n",temp);
}
GEN p = gp_read_str("456211210019");
GEN q = gp_read_str("456211221913");
GEN lamda = glcm(gsub(p,stoi((long)1)),gsub(q,stoi((long)1)));
*/
