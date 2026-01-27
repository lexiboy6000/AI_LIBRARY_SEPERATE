void forward_conv_ave_pool(conv_layer* cur){

	conv_layer* prev = cur->prev_layer;
	conv_layer* next = cur->next_layer;

	//**********compute convolution (cur->inter)****************************************//
	//**********compute convolution****************************************//
	//**********compute convolution****************************************//

	int cur_inter_size = cur->total_channels * cur->inter_rows * cur->inter_cols;
	int weights_size = cur->f_rows*cur_f_cols;

	cur->inter = malloc(cur_inter_size * sizeof(double)); ///when freed ??********* calloc??

	int inter_index   = 0;
	int input_index   = 0;
	int weights_index = 0;
	int biases_index  = 0;

	for(int prev_chan=0;prev_chan<prev->total_channels;prev_chan++){
		for(int cur_chan=0;cur_chan<cur->num_channels;cur_chan++){
			//stuff

			for(int inter_row=0;inter_row<cur->inter_rows;inter_row++){
				for(int inter_col=0;inter_col<cur->inter_cols;inter_col++){
					//stuff

					cur->inter[inter_index] = 0;
					for(int f_row=0;f_row<cur->f_rows;f_row++){
						for(int f_col=0;f_col<cur->f_cols;f_col++){
							cur->inter[inter_index] += cur->input[input_index + f_row*cur->i_cols + f_col]
													  *cur->weights[weights_index + f_row*cur->f_cols + f_col];
						}
					}
					cur->inter[inter_index] += cur->biases[biases_index];

					inter_index++;
				}
			}

			//update weights&biases indecies
			weights_index += weights_size;
			biases_index++;

		}

		//update input index
		input_index += i_rows*i_cols;
	}

	//**********compute activation (A) ***************************************//
	//**********compute activation (A)****************************************//
	//**********compute activation (A)****************************************//

	cur->A = malloc(cur_inter_size * sizeof(double));  //when freed??**********


	for(int i=0;i<cur_inter_size;i++){ //could put this whole thing in prev. conv loop actually
		*(cur->A + i) = activation(cur->activation,*(cur->inter + i));
	}

	//*******compute pooling ************************************************//
	//*******compute pooling ************************************************//
	//*******compute pooling ************************************************//

	ave_pool(cur,A_rows,A_cols);
}


void forward_conv_max_pool(conv_layer* cur,int* index_largest_output){

	conv_layer* prev = cur->prev_layer;
	conv_layer* next = cur->next_layer;

	//**********compute convolution (cur->inter)****************************************//
	//**********compute convolution****************************************//
	//**********compute convolution****************************************//

	int cur_inter_size = cur->total_channels * cur->inter_rows * cur->inter_cols;
	int weights_size = cur->f_rows*cur_f_cols;

	cur->inter = malloc(cur_inter_size * sizeof(double)); ///when freed ??********* calloc??

	int inter_index   = 0;
	int input_index   = 0;
	int weights_index = 0;
	int biases_index  = 0;

	for(int prev_chan=0;prev_chan<prev->total_channels;prev_chan++){
		for(int cur_chan=0;cur_chan<cur->num_channels;cur_chan++){
			//stuff

			for(int inter_row=0;inter_row<cur->inter_rows;inter_row++){
				for(int inter_col=0;inter_col<cur->inter_cols;inter_col++){
					//stuff

					cur->inter[inter_index] = 0;
					for(int f_row=0;f_row<cur->f_rows;f_row++){
						for(int f_col=0;f_col<cur->f_cols;f_col++){
							cur->inter[inter_index] += cur->input[input_index + f_row*cur->i_cols + f_col]
													  *cur->weights[weights_index + f_row*cur->f_cols + f_col];
						}
					}
					cur->inter[inter_index] += cur->biases[biases_index];

					inter_index++;
				}
			}

			//update weights&biases indecies
			weights_index += weights_size;
			biases_index++;

		}

		//update input index
		input_index += i_rows*i_cols;
	}

	//**********compute activation (A) ***************************************//
	//**********compute activation (A)****************************************//
	//**********compute activation (A)****************************************//

	cur->A = malloc(cur_inter_size * sizeof(double));  //when freed??**********

	for(int i=0;i<cur_inter_size;i++){ //could put this whole thing in prev. conv loop actually
		*(cur->A + i) = activation(cur->activation,*(cur->inter + i));
	}

	//*******compute pooling ************************************************//
	//*******compute pooling ************************************************//
	//*******compute pooling ************************************************//

	max_pool(cur,index_largest_output,A_rows,A_cols);

}