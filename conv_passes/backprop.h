
/*
The following is an outline for the backprop of a convolutional conv_layer
Every instance of / STUFF / is to be replaced with the code to update Dw,Db, M

void* backprop_conv_example_pool(conv_layer* cur,double* Dw,double* Db,double* M){

	double* out = cur->next_layer->input;
		int o_rows = out->i_rows;
		int o_cols = out->i_cols;
		int o_size = o_rows*o_cols;


	//moving ptrs
	double* moving_A     = cur->A;
	double* moving_input = cur->input;


	for(int chan=0;chan<cur->total_channels;chan++){


		int o_row = 0;
		int o_col = 0;

		for(int o_index=0; o_index<o_size;o_index++){

			o_col = o_index % o_cols;

			if(o_row == o_rows-1){

				if(o_col == o_cols-1){
					//stuff in A
					// moving_A[cur->inter_rows % cur->p_rows][cur->inter_cols % cur->p_cols]

					for(int A_row=0;A_row < cur->inter_rows%cur->p_rows;A_row++){

						int index_moving_input = 0;
						for(int A_col=0;A_col<cur->inter_cols%cur->p_cols;A_col++){

								/STUFF/

							//update index_moving_input (next col)
							index_moving_input++;
						}
						//update moving_input to next row
						moving_input += cur->i_cols;
					}



					//update moving_A
					moving_A += cur->inter_cols % cur->p_cols;
					moving_A += (cur->inter_rows-1)*cur->inter_cols;
					//moving_A now points to start of next channel A

					//update moving_input to the next channel
					moving_input += (cur->inter_cols%cur->p_cols) + cur->f_cols; //to end of col
					moving_input += (cur->f_rows-1)*cur->i_cols; 				 //to next channel

				}
				else{
					//stuff in A
					// moving_A[cur->inter_rows % cur->p_rows][cur->p_cols]

					double* next_moving_input = moving_input + cur->p_cols;

					for(int A_row=0;A_row<cur->inter_rows%cur->p_rows){

						int index_moving_input = 0;
						for(int A_col=0;A_col<cur->p_cols;A_col++){

								/STUFF/ 

							//update index_moving_input (next col)
							index_moving_input++;
						}

						//update moving_input to new row
						moving_input += cur->i_cols;
					}


					//update moving_A
					moving_A += cur->p_cols;

					//update moving_input
					moving_input = next_moving_input;
				}

			}
			else{
				if(o_col == o_cols-1){
					//stuff in A
					//moving_A[cur->p_rows][cur->inter_cols % cur->p_cols]

					for(int A_row=0;A_row<cur->p_rows;A_row++){

						int index_moving_input = 0;
						for(int A_col=0;A_col<cur->inter_cols%cur->p_cols){

								/STUFF/

							//update index_moving_input (next col)
							index_moving_input++;
						}

						moving_input += cur->i_cols;
					}

					//now increment to next o_row
					o_row++;

					//update moving_A
					moving_A += cur->inter_cols % cur->p_cols;
					moving_A += (cur->p_rows-1)*cur->inter_cols;

					//update moving_input CORRECT????
					moving_input += cur->inter_cols%cur->p_cols + cur->f_cols; //pass through end of row

				}
				else{
					//stuff in A
					//moving_A[cur->p_rows][cur->p_cols]

					double* next_moving_input = moving_input + cur->p_cols;

					for(int A_row=0;A_row<cur->p_rows;A_row++){

						int index_moving_input = 0;
						for(A_col=0;A_col<cur->p_cols;A_col++){

								/STUFF/

							//update index_moving_input (next col)
							index_moving_input++;
						}
						//update moving_input (next row)
						moving_input += cur->i_cols;
					}


					//update moving_A
					moving_A += cur->p_cols;

					//update moving_input
					moving_input = next_moving_input;

				}


			}
		}
	}
}

*/
//this_A = moving_A[cur->inter_cols*A_row+A_col];
//this_Db = Db+chan;
//this_Dw = Dw + chan*w_size;
//this_M_new = M_new + chan*i_size+i_index+index_moving_input;
//this_input = moving_input + index_moving_input
//update(cur,moving_input+index_moving_input,moving_A[cur->inter_cols*A_row+A_col],
//		 Dw+chan*w_size,Db+chan,M_new+chan*i_size+i_index+index_moving_input);

/* update part --> 

//adding to Db
double mini_Db = holder*d_activation(moving_A[cur->inter_cols*A_row + A_col]);
Db[chan] += mini_Db;
//adding to Dw
{
	int w_index = 0;
	for(int w_row=0;w_row<cur->f_rows;w_row++){
		for(int w_col=0;w_col<cur->f_cols;w_col++){

		//update Dw
		Dw[chan*w_size+w_index] += mini_Db*
							moving_input[index_moving_input + 
							w_row*cur->i_cols+w_col]; //w_row,col comp of moving_input+index

		w_index++;


		//update M_new ... 
		int i = chan*i_size+i_index+index_moving_input + w_row*cur->i_cols+w_col;
		M_new[i] += mini_Db*cur->weights[w_row*cur->f_cols+w_col];

		}
	}
}

*/


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

void* update(conv_layer* cur,double* this_input, double this_A,double holder,
			 double* this_Dw,double* this_Db,double* this_M_new){

	//int i_size = cur->i_rows*cur->i_cols;
	double mini_Db = holder*d_activation(this_A);
	*this_Db += mini_Db;
	
	int w_index = 0;
	for(int w_row=0;w_row<cur->f_rows;w_row++){
		for(int w_col===0;w_col<cur->f_cols;w_col++){

			//update Dw
			this_Dw[w_index] += mini_Db*this_input[w_row*cur->i_cols+w_col];
			w_index++;
			//update M_new
			this_M_new[w_row*cur->i_cols+w_col] += mini_Db*cur->weights[w_row*cur->f_cols+w_col];

		}
	}


}


//returns M_new
double* backprop_conv_ave_pool(conv_layer* cur,double* M,double* Dw,double* Db){ //finna pass thru a ptr-func

	//double* out = cur->next_layer->input; //never actually used
		int o_rows = out->i_rows;
		int o_cols = out->i_cols;
		int o_size = o_rows*o_cols;

	//moving ptrs
	double* moving_A     = cur->A;
	double* moving_input = cur->input;

	//w_size & i_size
	int w_size = cur->f_rows*cur->f_cols;
	int i_size = cur->i_rows*cur->i_cols;

	//size of derivative stuff
		//M  --- | total_channels * o_size
		//Dw --- | total_channels * f_rows * f_cols
		//Db --- | total_channels

	int sizeof_M_new = cur->prev_layer->total_channels*cur->i_rows*cur->i_cols; //put into calloc??
	double* M_new = calloc(sizeof_M_new,sizeof(double));

	for(int chan=0;chan<cur->total_channels;chan++){

		int o_row = 0;
		int o_col = 0;
		int i_index = 0;
		for(int o_index=0; o_index<o_size;o_index++){

			double holder = M[chan*o_size + o_index]*(1 / ( A_row_SIZE*A_col_SIZE ));

			o_col = o_index % o_cols;
			if(o_row == o_rows-1){

				if(o_col == o_cols-1){
					//stuff in A
					// moving_A[cur->inter_rows % cur->p_rows][cur->inter_cols % cur->p_cols]

					int A_row_SIZE = cur->inter_rows%cur->p_rows;
					int A_col_SIZE = cur->inter_cols%cur->p_cols;

					for(int A_row=0;A_row < A_row_SIZE;A_row++){

						int index_moving_input = 0;
						for(int A_col=0;A_col<A_col_SIZE;A_col++){
							update(cur,moving_input+index_moving_input,moving_A[cur->inter_cols*A_row+A_col],holder,
						  		   Dw+chan*w_size,Db+chan,M_new+chan*i_size+i_index+index_moving_input);
							//update index_moving_input (next col)
							index_moving_input++;
						}
						//update moving_input to next row
						moving_input += cur->i_cols;
						i_index      += cur->i_cols;
					}



					//update moving_A
					moving_A += A_col_SIZE;
					moving_A += (cur->inter_rows-1)*cur->inter_cols;
					//moving_A now points to start of next channel A

					//update moving_input to the next channel*************************//

						//don't need to worry about i_index cuz resets to zero on start of next chan

						moving_input += A_col_SIZE + cur->f_cols; //to end of col
						moving_input += (cur->f_rows-1)*cur->i_cols-1; //to one before next channel
						//BUT!! only want to go to next channel if chan % prev->total == 0
						//else, back to begining.
						if((chan+1) % cur->prev_layer->total_channels){ //only go to next if next prev channel
							moving_input -= cur->i_rows * cur->i_cols-1; //else, back to begining
						}
						else if(chan != cur->total_channels-1){  //we do this to prevent seg fault & this only runs if incrimenting channel of input
							moving_input++;
						}

				}
				else{
					//stuff in A
					// moving_A[cur->inter_rows % cur->p_rows][cur->p_cols]

					double* next_moving_input = moving_input + cur->p_cols;
					int next_i_index          = i_index      + cur->p_cols;

					int A_row_SIZE = cur->inter_rows%cur->p_rows;

					for(int A_row=0;A_row<A_row_SIZE;A_row++){

						int index_moving_input = 0;
						for(int A_col=0;A_col<cur->p_cols;A_col++){
							update(cur,moving_input+index_moving_input,moving_A[cur->inter_cols*A_row+A_col],holder,
						  		   Dw+chan*w_size,Db+chan,M_new+chan*i_size+i_index+index_moving_input);
							//update index_moving_input (next col)
							index_moving_input++;
						}

						//update moving_input to new row
						moving_input += cur->i_cols;
						i_index      += cur->i_cols;
					}


					//update moving_A
					moving_A += cur->p_cols;

					//update moving_input
					moving_input = next_moving_input;
					i_index      = next_i_index;
				}

			}
			else{
				if(o_col == o_cols-1){
					//stuff in A
					//moving_A[cur->p_rows][cur->inter_cols % cur->p_cols]

					int A_col_SIZE = cur->inter_cols%cur->p_cols;

					for(int A_row=0;A_row<cur->p_rows;A_row++){

						int index_moving_input = 0;
						for(int A_col=0;A_col<A_col_SIZE;A_col++){
							update(cur,moving_input+index_moving_input,moving_A[cur->inter_cols*A_row+A_col],holder,
						  		   Dw+chan*w_size,Db+chan,M_new+chan*i_size+i_index+index_moving_input);
							//update index_moving_input (next col)
							index_moving_input++;
						}

						moving_input += cur->i_cols;
						i_index      += cur->i_cols;
					}

					//now increment to next o_row
					o_row++;

					//update moving_A
					moving_A += cur->inter_cols % cur->p_cols;
					moving_A += (cur->p_rows-1)*cur->inter_cols;

					//update moving_input CORRECT????
					moving_input += A_col_SIZE + cur->f_cols; //pass through end of row
					i_index      += A_col_SIZE + cur->f_cols;

				}
				else{
					//stuff in A
					//moving_A[cur->p_rows][cur->p_cols]

					double* next_moving_input = moving_input + cur->p_cols;
					int next_i_index          = i_index      + cur->p_cols;

					for(int A_row=0;A_row<cur->p_rows;A_row++){

						int index_moving_input = 0;
						for(A_col=0;A_col<cur->p_cols;A_col++){
							update(cur,moving_input+index_moving_input,moving_A[cur->inter_cols*A_row+A_col],holder,
						  		   Dw+chan*w_size,Db+chan,M_new+chan*i_size+i_index+index_moving_input);
							//update index_moving_input (next col)
							index_moving_input++;
						}
						//update moving_input (next row)
						moving_input += cur->i_cols;
						i_index      += cur->i_cols;
					}


					//update moving_A
					moving_A += cur->p_cols;

					//update moving_input
					moving_input = next_moving_input;
					i_index      = next_i_index;

				}


			}
			//inside o_index, outside everything else-->
		}
		//outside of o_index, inside of chan -->
	}
	//outside of chan -->

	//free(M);
	//M = M_new;
	return M_new;
}


//returns M_new
double* backprop_conv_max_pool(conv_layer* cur,double* M,double* Dw,double* Db,
							 int* index_largest_output){


	//double* out = cur->next_layer->input; //never actually used
		int o_rows = out->i_rows;
		int o_cols = out->i_cols;
		int o_size = o_rows*o_cols;

	//moving ptrs
	double* moving_A     = cur->A;
	double* moving_input = cur->input;

	//w_size & i_size
	int w_size = cur->f_rows*cur->f_cols;
	int i_size = cur->i_rows*cur->i_cols;

	//size of derivative stuff
		//M  --- | total_channels * o_size
		//Dw --- | total_channels * f_rows * f_cols
		//Db --- | total_channels

	int sizeof_M_new = cur->prev_layer->total_channels*cur->i_rows*cur->i_cols; //put into calloc??
	double* M_new = calloc(sizeof_M_new,sizeof(double));

	for(int chan=0;chan<cur->total_channels;chan++){

		int o_row = 0;
		int o_col = 0;
		int i_index = 0;
		for(int o_index=0; o_index<o_size;o_index++){

			double holder = M[chan*o_size + o_index]*(1 / ( A_row_SIZE*A_col_SIZE ));

			o_col = o_index % o_cols;
			if(o_row == o_rows-1){

				if(o_col == o_cols-1){
					//stuff in A
					// moving_A[cur->inter_rows % cur->p_rows][cur->inter_cols % cur->p_cols]

					int A_row_SIZE = cur->inter_rows%cur->p_rows;
					int A_col_SIZE = cur->inter_cols%cur->p_cols;

					/*HERE**/ // --> only do for largest given by index of index_largest_output
						int index_max = *(index_largest_output+o_index);
						int A_row = index_max / A_col_SIZE;
						int A_col = index_max % A_col_SIZE;

						int index_moving_input = A_col;
						update(cur,moving_input+index_moving_input,moving_A[cur->inter_cols*A_row+A_col],holder,
						  	   Dw+chan*w_size,Db+chan,M_new+chan*i_size+i_index+index_moving_input);

					//update moving_A
					moving_A += A_col_SIZE;
					moving_A += (cur->inter_rows-1)*cur->inter_cols;
					//moving_A now points to start of next channel A

					//update moving_input to the next channel*************************//

						//don't need to worry about i_index cuz resets to zero on start of next chan

						moving_input += A_col_SIZE + cur->f_cols; //to end of col
						moving_input += (cur->f_rows-1)*cur->i_cols-1; //to one before next channel
						//BUT!! only want to go to next channel if chan % prev->total == 0
						//else, back to begining.
						if((chan+1) % cur->prev_layer->total_channels){ //only go to next if next prev channel
							moving_input -= cur->i_rows * cur->i_cols-1; //else, back to begining
						}
						else if(chan != cur->total_channels-1){  //we do this to prevent seg fault & this only runs if incrimenting channel of input
							moving_input++;
						}

				}
				else{
					//stuff in A
					// moving_A[cur->inter_rows % cur->p_rows][cur->p_cols]

					double* next_moving_input = moving_input + cur->p_cols;
					int next_i_index          = i_index      + cur->p_cols;

					int A_row_SIZE = cur->inter_rows%cur->p_rows;
					
					/*HERE**/
						int index_max = *(index_largest_output + o_index);
						int A_row = index_max / cur->p_rows;
						int A_col = index_max % cur->p_cols;

						int index_moving_input = A_col;
						update(cur,moving_input+index_moving_input,moving_A[cur->inter_cols*A_row+A_col],holder,
						  	   Dw+chan*w_size,Db+chan,M_new+chan*i_size+i_index+index_moving_input);

					//update moving_A
					moving_A += cur->p_cols;

					//update moving_input
					moving_input = next_moving_input;
					i_index      = next_i_index;
				}

			}
			else{
				if(o_col == o_cols-1){
					//stuff in A
					//moving_A[cur->p_rows][cur->inter_cols % cur->p_cols]

					int A_col_SIZE = cur->inter_cols%cur->p_cols;
					
					/*HERE**/
						int index_max = *(index_largest_output + o_index);
						int A_row = index_max / A_col_SIZE;
						int A_col = index_max % A_col_SIZE;

						int index_moving_input = A_col;
						update(cur,moving_input+index_moving_input,moving_A[cur->inter_cols*A_row+A_col],holder,
						  	   Dw+chan*w_size,Db+chan,M_new+chan*i_size+i_index+index_moving_input);

					//now increment to next o_row
					o_row++;

					//update moving_A
					moving_A += cur->inter_cols % cur->p_cols;
					moving_A += (cur->p_rows-1)*cur->inter_cols;

					//update moving_input CORRECT????
					moving_input += A_col_SIZE + cur->f_cols; //pass through end of row
					i_index      += A_col_SIZE + cur->f_cols;

				}
				else{
					//stuff in A
					//moving_A[cur->p_rows][cur->p_cols]

					double* next_moving_input = moving_input + cur->p_cols;
					int next_i_index          = i_index      + cur->p_cols;

					/*HERE**/
						int index_max = *(index_largest_output + o_index);
						int A_row     = index_max / cur->p_cols;
						int A_col     = index_max % cur->p_cols;

						int index_moving_input = A_col;
						update(cur,moving_input+index_moving_input,moving_A[cur->inter_cols*A_row+A_col],holder,
						  	   Dw+chan*w_size,Db+chan,M_new+chan*i_size+i_index+index_moving_input);

					//update moving_A
					moving_A += cur->p_cols;

					//update moving_input
					moving_input = next_moving_input;
					i_index      = next_i_index;

				}


			}
			//inside o_index, outside everything else-->
		}
		//outside of o_index, inside of chan -->
	}
	//outside of chan -->

	//free(M);
	//M = M_new;
	return M_new;

}

