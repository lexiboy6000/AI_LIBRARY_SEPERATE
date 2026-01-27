
void forward_dense(dense_layer* cur){ //will gcc know what dense_layer is here??
 
	dense_layer* next = cur->next_layer;

	/* sizes of stuff --> 
		double* input   | cur->layer_size
		double* weights | cur->layer_size * next->layer_size
		double* baises  | next->layer_size
		double* output  | next->layer_size
	*/

	next->input = calloc(next->layer_size,sizeof(double)); //need to extern??
	int w_index=0;
	for(int i_next=0;i_next<next->layer_size;i_next++){
		for(int i_cur=0;i_cur<cur->layer_size;i_cur++){
			cur->f[i_next] += cur->input[i_cur]*cur->weights[w_index];
			w_index++;
		}
		cur->f[i_next] += cur->biases[i_next];
		next->input[i_next] = activation(cur->activation,cur->f[i_next]);
	}



}


double* backprop_dense(dense_layer* cur,double* M,double* Dw,double* Db){

	dense_layer* next = cur->next_layer;
	/*sizes of stuff -->
	
	double* input   | cur->layer_size
	double* weights | cur->layer_size * next->layer_size
	double* biases  | next->layer_size
	double* output  | next->layer_size


	*/

	double* M_new = calloc(cur->layer_size,sizeof(double));

	double holder;
	for(int a=0;a<next->layer_size;a++){
		holder = M[a]*d_activation(cur->activation,cur->f[a]);

		Db[a] += holder;
		for(int b=0;b<cur->layer_size;b++){
			Dw[a*cur->layer_size + b] += holder*input[b];
			M_new[b] += holder*cur->weights[a*cur->layer_size + b];
		}

	}

	//free(M);
	//M = M_new; //even nessesary?? probably could/should delete
	return M_new;
}



input forward_dense_list(Master* master){

	input output;

	dense_layer* layer = (dense_layer*) master->linked_list;
	//layer->input = input.input_data;
		//input.i_rows
		//input.i_cols
		//input.num_channels

	//foward pass -->

	while(layer->next_layer != NULL){

		forward_dense(layer);

		layer = layer->next_layer;
	}

	//set last layer
	master->last_layer = (void*)layer;

	output.input_data = layer->input;
	output.i_rows     = 1;
	output.i_cols     = 1;
	output.num_channels = layer->layer_size;

	return output;

}


double* backprop_dense_list(Master cur_master,double* M_initial){

	dense_layer* layer = (dense_layer*) cur_master->last_layer;

	/*dense_layer* layer = master->linked_list;

	//foward pass -->

	while(layer->next_layer != NULL){

		forward_dense(layer);

		layer = layer->next_layer;
	}*/

	//backprop -->

	double* M = M_initial;

	while(layer->prev_layer != NULL){

		double* M_new = backprop_dense(layer,M,layer->Dw,layer->Db);
		free(M);
		M = M_new;

		//free this f & the output that's now no longer needed
		free(layer->f);
		if(layer->next_layer != NULL) free(layer->next_layer->input); //fine to free NULL??

		layer = layer->prev_layer;
	}

	free(layer->input); // no longer needed rt?
	return M;
}