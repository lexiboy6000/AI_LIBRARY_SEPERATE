/*
void* model_pool(conv_layer* cur,
			   int A_rows,int A_cols){



//****FOLLOWING IS A --> out / out --> outline--->******
//*********************************************************
//*********************************************************

 	//*******find size of output & put into input of next********
	int extra_row = 0; int extra_col = 0;

	if(A_rows % cur->p_rows) extra_row++;
	if(A_cols % cur->p_cols) extra_col++;

	int o_rows  = A_rows / cur->p_rows + extra_row;
	int o_cols = A_cols / cur->p_cols + extra_col;

	double* cur->next_layer->input = malloc(total_channels*o_rows*o_cols*sizeof(double));
		cur->next_layer->i_rows = o_rows;
		cur->next_layer->i_cols = o_cols;

	//*********************************************************

	//first things first, create a moving ptr to A
	//make the movingA 2d to simplify the otherwise nessesary abhorant indexing

	//double* chan_A[A_rows][A_cols] = cur->A; //moves from channel to channel
	//double* pool_A[cur->p_rows][cur->p_cols];//moves within each channel (pooling)

	//special case loop sizes 
		
		//	(i) special row special col
		//			____ by _____

		//	(ii) special row
		//			____ by _____
		//	(iii) special col
		//			____ by _____


	//	-----> & each 'stuff' will loop over these

		
	for(int chan=0;chan<cur->total_channels;cur++){

		double* chan_A = cur->A + chan * A_rows*A_cols; //begining of A for this channel;

		int o_size = o_rows*o_cols;

		int o_row = 0; // incrimented during/before evertime (o_index % o_cols = 0)
		int o_col = 0; // = o_index % o_cols

		for(int o_index=0;o_index<o_size;o_index++) { //so output is a 1d flat thing

			o_col = o_index % o_cols;


			if(o_row == o_rows-1){  //special case, last row

				if(o_col == o_cols-1){ //special-special case

					for(int i=0;i<A_rows%cur->p_rows;i++){
						for(int j=0;j<A_cols%cur->p_cols;j++){
							//stuff
							//chan_A + i*A_cols + j = chan_A[i][j]
						}
					}

				}
				else{ //special case
					
					for(int i=0;i<A_rows % cur->p_rows;i++){
						for(int j=0;j<cur->p_cols;j++){
							//stuff
						}
					}
					chan_A += cur->p_cols;
				}


			}
			else{

				if(o_col == o_cols-1){ //special case
					//stuff
					for(int i=0;i<cur->p_rows;i++){
						for(int j=0;j<A_cols%cur->p_cols;j++){
							//stuff
						}
					}
					o_row++; //incriment after every special case
					chan_A += A_cols % cur->p_cols; //get to next row of A
					chan_A += (cur->p_rows-1)*A_cols; //jump p_row-1 number of rows
				}
				else{
					
					for(int i=0;i<cur->p_rows;i++){
						for(int j=0;j<cur->p_cols;j++){
							//stuff

						}
					}
					chan_A += cur->p_cols;
				}

			}





		}


	}
}

*/


//ave_pool --->


void* ave_pool(conv_layer* cur, int A_rows,int A_cols){

 	//*******find size of output & put into input of next********//
	int extra_row = 0; int extra_col = 0;

	if(A_rows % cur->p_rows) extra_row++;
	if(A_cols % cur->p_cols) extra_col++;

	int o_rows  = A_rows / cur->p_rows + extra_row;
	int o_cols = A_cols / cur->p_cols + extra_col;

	double* cur->next_layer->input = malloc(total_channels*o_rows*o_cols*sizeof(double));
		cur->next_layer->i_rows = o_rows;
		cur->next_layer->i_cols = o_cols;

	//*********************************************************

	//first things first, create a moving ptr to A
	//make the movingA 2d to simplify the otherwise nessesary abhorant indexing

	//double* chan_A[A_rows][A_cols] = cur->A; //moves from channel to channel
	//double* pool_A[cur->p_rows][cur->p_cols];//moves within each channel (pooling)

	//special case loop sizes 
		/*
			(i) special row special col
					____ by _____

			(ii) special row
					____ by _____
			(iii) special col
					____ by _____


		-----> & each 'stuff' will loop over these

		*/
	for(int chan=0;chan<cur->total_channels;cur++){

		double* chan_A = cur->A + chan * A_rows*A_cols; //begining of A for this channel;

		//int index_row = 0;
		//int index_col = 0;

		int o_size = o_rows*o_cols;

		int o_row = 0; // incrimented during/before evertime (o_index % o_cols = 0)
		int o_col = 0; // = o_index % o_cols

		for(int o_index=0;o_index<o_size;o_index++) { //so output is a 1d flat thing

			o_col = o_index % o_cols;


			if(o_row == o_rows-1){  //special case, last row

				if(o_col == o_cols-1){ //special-special case

					double ave = 0;
					for(int i=0;i<A_rows%cur->p_rows;i++){
						for(int j=0;j<A_cols%cur->p_cols;j++){
							//stuff
							ave += *(chan_A + i*A_cols + j);
							//chan_A + i*A_cols + j = chan_A[i][j]
						}
					}
					//set output
					*(cur->next_layer->input + o_index) = ave;

				}
				else{ //special case
					
					double ave = 0;
					for(int i=0;i<A_rows % cur->p_rows;i++){
						for(int j=0;j<cur->p_cols;j++){
							//stuff
							ave+= *(chan_A + i*A_cols + j);
						}
					}
					//set output
					*(cur->next_layer->input + o_index) = ave;

					//index update
					chan_A += cur->p_cols;
				}


			}
			else{

				if(o_col == o_cols-1){ //special case
					
					double ave = 0;
					for(int i=0;i<cur->p_rows;i++){
						for(int j=0;j<A_cols%cur->p_cols;j++){
							ave += *(chan_A + i*A_cols + j);
						}
					}
					//set output
					*(cur->next_layer->input + o_index) = ave;


					//index update
					o_row++; //incriment after every special case
					chan_A += A_cols % cur->p_cols; //get to next row of A
					chan_A += (cur->p_rows-1)*A_cols; //jump p_row-1 number of rows
				}
				else{
					
					double ave = 0;
					for(int i=0;i<cur->p_rows;i++){
						for(int j=0;j<cur->p_cols;j++){
							//stuff
							ave += *(chan_A + i*A_cols +j);

						}
					}
					//set output
					*(cur->next_layer->input + o_index) = ave;

					//indedx update
					chan_A += cur->p_cols;
				}

			}





		}


	}


//**************************

}



double* max_pool(conv_layer* cur,int* index_largest_output,int A_rows,int A_cols){

	unsigned long smallest_double = 0xFFEFFFFFFFFFFFFF;

 	//*******find size of output & put into input of next********//
	int extra_row = 0; int extra_col = 0;

	if(A_rows % cur->p_rows) extra_row++;
	if(A_cols % cur->p_cols) extra_col++;

	int o_rows  = A_rows / cur->p_rows + extra_row;
	int o_cols = A_cols / cur->p_cols + extra_col;

	int o_size = cur->total_channels*o_rows*o_cols;

	double* cur->next_layer->input = malloc(o_size*sizeof(double));
		cur->next_layer->i_rows = o_rows;
		cur->next_layer->i_cols = o_cols;

	index_largest_output = malloc(o_size*sizeof(int));
	//*********************************************************

	//first things first, create a moving ptr to A
	//make the movingA 2d to simplify the otherwise nessesary abhorant indexing

	//double* chan_A[A_rows][A_cols] = cur->A; //moves from channel to channel
	//double* pool_A[cur->p_rows][cur->p_cols];//moves within each channel (pooling)

	//special case loop sizes 
		/*
			(i) special row special col
					____ by _____

			(ii) special row
					____ by _____
			(iii) special col
					____ by _____


		-----> & each 'stuff' will loop over these

		*/
	for(int chan=0;chan<cur->total_channels;cur++){

		double* chan_A = cur->A + chan * A_rows*A_cols; //begining of A for this channel;

		//int index_row = 0;
		//int index_col = 0;

		int o_size = o_rows*o_cols;

		int o_row = 0; // incrimented during/before evertime (o_index % o_cols = 0)
		int o_col = 0; // = o_index % o_cols

		for(int o_index=0;o_index<o_size;o_index++) { //so output is a 1d flat thing

			o_col = o_index % o_cols;


			if(o_row == o_rows-1){  //special case, last row

				if(o_col == o_cols-1){ //special-special case

					double pool = *(double*) smallest_double;
					int index_largest = 0;
					for(int i=0;i<A_rows%cur->p_rows;i++){
						for(int j=0;j<A_cols%cur->p_cols;j++){
							//stuff
							if(chan_A[i*A_cols+j] > pool) {
								pool = chan_A[i*A_cols+j];
								index_largest = i*A_cols + j;
							}
							//chan_A + i*A_cols + j = chan_A[i][j]
						}
					}
					//set output
					*(cur->next_layer->input + chan*o_size + o_index) = pool;
					*(index_largest_output   + chan*o_size + o_index) = i*A_cols+j;


				}
				else{ //special case
					
					double pool = *(double*) smallest_double;
					double index_largest = 0;
					for(int i=0;i<A_rows % cur->p_rows;i++){
						for(int j=0;j<cur->p_cols;j++){
							//stuff
							if(chan_A[i*A_cols+j] > pool){ 
								pool = chan_A[i*A_cols+j];
								index_largest = i*A_cols+j;
							}
						}
					}
					//set output
					*(cur->next_layer->input + chan*o_size + o_index) = pool;
					*(index_largest_output + chan*o_size + o_index) = i*A_cols+j;

					//index update
					chan_A += cur->p_cols;
				}


			}
			else{

				if(o_col == o_cols-1){ //special case
					
					double pool = *(double*) smallest_double;
					int index_largest = 0;
					for(int i=0;i<cur->p_rows;i++){
						for(int j=0;j<A_cols%cur->p_cols;j++){
							if(chan_A[i*A_cols+j] > pool){ 
								pool = chan_A[i*A_cols+j];
								index_largest = i*A_cols + j;

							}
						}
					}
					//set output
					*(cur->next_layer->input + chan*o_size + o_index) = pool;
					*(index_largest_output + chan*o_size + o_index) = index_largest;


					//index update
					o_row++; //incriment after every special case
					chan_A += A_cols % cur->p_cols; //get to next row of A
					chan_A += (cur->p_rows-1)*A_cols; //jump p_row-1 number of rows
				}
				else{
					
					double pool = *(double*) smallest_double;
					int index_largest = 0;
					for(int i=0;i<cur->p_rows;i++){
						for(int j=0;j<cur->p_cols;j++){
							//stuff
							if(chan_A[i*A_cols +j] > pool){ 
								pool = chan_A[i*A_cols+j];
								index_largest = i*A_cols+j;
							}


						}
					}
					//set output
					*(cur->next_layer->input + chan*o_size + o_index) = pool;
					*(index_largest_output + chan*o_size + o_index) = index_largest;

					//indedx update
					chan_A += cur->p_cols;
				}

			}





		}


	}


//**************************

}