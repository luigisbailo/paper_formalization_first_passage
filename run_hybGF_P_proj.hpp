#pragma once

void run_hybGF_P_proj ( int N_A, int N_B, int R_A, int R_B, double D_A, double D_B, double tau_bm, double alpha, double Tsim, int nProj, double L, int *stat, double diffStat[][10] ) {

	const gsl_rng_type *Type;
	gsl_rng *r;
	gsl_rng_env_setup ();
	Type = gsl_rng_default;
	r = gsl_rng_alloc (Type);
	FILE *devurandom = fopen("/dev/urandom","r");
	unsigned long int seed;
	fread(&seed, sizeof(seed), 1, devurandom);
	fclose(devurandom);
	gsl_rng_set(r, seed);

	double tProj = Tsim / nProj;
	int countProj = 1; 

	const int N = N_A + N_B;
	
	double R;
	int nBurst = 0;
	int nGF = 0;

	const double sqrt2TAU_BM = sqrt(2*tau_bm);

	double Teq = tProj;

	double distRow [N], maxSh;

	particle particles [N]; 
	double shells [N];	
	int partList [N];

    initPos_hybGF ( particles, r, N_A, N_B, R_A, R_B, D_A, D_B, tau_bm, alpha, L); 

    initShell_GF_proj ( particles, r, N, tau_bm, sqrt2TAU_BM, L, &stat[1]);

    //sort() is a prebuild c++ funct. It sorts particles for increasing exit times
    std::sort ( particles, particles+N, compareTime );
    for (int n=0; n<N; n++) partList[n]=n;


    while ( particles[partList[0]].tau_exit < Tsim ) {

//        std::cout << particles[partList[0]].tau_exit << std::endl;
    	if ( particles[partList[0]].burst ) stat[0]++;

//		 std::cout << std::setprecision(6);
//		 printPos_per ( particles, partList, N );
//		 // printDist_per (particles, partList, N, L);
//		 std::cout << "\n";

		updatePart_GF_P_proj ( &particles[partList[0]], r, tau_bm, L );    
		//differently from aGF, updatePart() here samples also the exit position from the shell

		// check_GF ( particles, partList,  N, L );
		// check_times ( particles, partList, N);

//		std::cout << "check 1 " << std::endl;
		getDist ( particles, partList, distRow, &maxSh, N, L );
//		std::cout << "check 2 " << std::endl;

//		burst_P_GF ( particles, partList, distRow, r, N, partList[0], L);

		R = getR_GF ( particles, partList, shells, distRow, N, L );
		//it returns zero in case the determined shell is smaller than the smallest possible

		particles[partList[0]].burst = false;

		if ( R > 0 ) {
//std::cout << "GF" << std::endl;
			stat [1] ++;
			if (R>L/8) R=L/8;
			GFstep_GF_proj ( &particles[partList[0]], r, R , tau_bm);
			particles[partList[0]].gf = true;

		}
		else{ 
			
			stat [2] ++;
			BMstep ( particles, partList, distRow, r, tau_bm,  sqrt2TAU_BM, N, L );
			particles[partList[0]].gf = false;
		}

		sortBurst ( particles, partList, N);


		sortPart ( particles,partList,N);

//
//            std::cout << "not synch" << std::endl;
//            std::cout << std::setprecision(6);
//            printPos_per(particles, partList, N);
//            // printDist_per (particles, partList, N, L);
//            std::cout << "\n";


		if ( particles[partList[0]].tau_exit > tProj | particles[partList[0]].tau_exit == tProj ) {
//            std::cout << "synch" << std::endl;
//            std::cout << particles[partList[0]].tau_exit << std::endl;
//             std::cout << std::setprecision(6);
//             printPos_per ( particles, partList, N );

		    synchPart_P_GF ( particles, partList, r, N, tProj, L );
//
//            std::cout << std::setprecision(6);
//            printPos_per ( particles, partList, N );
//            // printDist_per (particles, partList, N, L);
//            std::cout << "\n";

		    for ( int n=0; n<N; n++ ){

		    	diffStat[countProj-1][n] += pow(particles[n].pos[0]-particles[n].pos_init[0] + particles[n].pos_period[0]*L, 2);
		    	diffStat[countProj-1][n] += pow(particles[n].pos[1]-particles[n].pos_init[1] + particles[n].pos_period[1]*L, 2);
		    	diffStat[countProj-1][n] += pow(particles[n].pos[2]-particles[n].pos_init[2] + particles[n].pos_period[2]*L, 2);

		    }

		    initShell_GF ( particles, r, N, tau_bm, sqrt2TAU_BM, L, &stat[1]);

		    std::sort ( particles, particles+N, compareTime );
		    for (int n=0; n<N; n++) partList[n]=n;

			countProj ++;
			tProj = countProj * Tsim / nProj;

		}



    } ;

    gsl_rng_free (r);

}