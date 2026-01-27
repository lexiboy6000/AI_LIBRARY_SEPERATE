

/* vocabulary --> 

	1. batch      --> a single set of input data that goes through forward & backpass
	2. collection --> a collection of batches that derivs are summed over
	3. epoch      --> the complete set of input data (set of all collections)

*/


/* collection algorithm --> 

(0) prior --> 
compute & create M_initial
create Dw,Db 
--------

(1) for each batch in collection --> 
	
	+= Dw
	+= Db

--------

(2) then learn, ex: 

	w -= learning_rate * Dw
	b -= learning_rate * Db



*/


////////////////////////////////////////////////
////////////////////////////////////////////////

void pass_batch(Network network,double* this_batch,int label){ //for dense output

	//change means of network input passing

	Master* cur_master = network->master_zero;

	input layer_input = network->network_input;
		layer_input.data = this_batch; //this fine??

	//*****************forward pass -->****************************************// 
	while(cur_master->next != 0){
		if(!strcmp(cur_master->type,"conv")){

			//prepare input
			conv_layer* first_layer = (conv_layer*)cur_master->linked_list;
			first_layer->input = layer_input.data;
			first_layer->i_rows = layer_input.i_rows;
			first_layer->i_cols = layer_input.i_cols;

			//run through linked list & set output to first_layer
			layer_input = forward_conv_list(cur_master); //cur_master->last_layer set in here
		}
		else if(!strcmp(cur_master->type,"dense")){

			//prepare input
				dense_layer* first_layer = (dense_layer*)cur_master->linked_list;
				first_layer->input = layer_input.data;
				first_layer->layer_size = layer_input.num_channels;
				//not nessesary to initialize i_rows,i_cols --> all treated as flattened array

			//run through linked list & set output to first_layer
				layer_input = forward_dense_list(cur_master); //cur_master->last_layer set in here

		}
		else if (!strcmp(cur_master->type,"prep")){
			//do some function
			//update layer_input accordingly (dep on function)

		}

		//go to next master...
		cur_master = cur_master->next;

	}

	//**********************M_initial -->**************************************//

	int o_size = layer_input->i_rows*layer_input->i_cols*layer_input->num_channels;
	double* M_initial = malloc(o_size*sizeof(double));

	D_loss(network->loss,M_initial,layer_input->data,o_size,label);

	//**********************backprop-->****************************************//
	//output = layer_input
	//void* ptr to last_layer of each master already set. 



	while(cur_master->prev != NULL){
		if(!strcmp(cur_master->type,"dense")){
			//M_initial = backprop dense
			M_initial = backprop_conv_list(cur_master,M_initial);
		}
		else if(!strcmp(cur_master->type,"conv")){
			//M_initial = backprop conv
			M_initial = backprop_dense_list(cur_master,M_initial);

		}
		else if(!strcmp(cur_master->type,"prep")){
			//M_initial = backprop prep
		}

		cur_master = cur_master->prev;
	}

	free(M_initial);

}

////////////////////////////////////////////////
////////////////////////////////////////////////




//givens -->

	//	my_network.num_epochs = num_epochs;
	//  my_Network.collection_size = collection_size;
	//	my_network.learning_params = learning_params;

	//batch_size = my_network.network_input.i_rows*i_cols*num_channels
	//*************************


	//given batch_size == GLOBAL??? found in learn (= size of each input)

void memset_Derivs(Network network){

	Master cur_master = network->master_zero;

	while(cur_master->next != 0){
		if(!strcmp(cur_master->type,"conv")){
			conv_layer* layer = (conv_layer*) cur_master->linked_list;
			while(layer->next_layer != NULL){
				memset(layer->Dw);
				memset(layer->Db);
				layer = layer->next_layer;
			}

		}
		else if(!strcmp(cur_master->type,"dense")){
			dense_layer* layer = (dense_layer*) cur_master->linked_list;
			while(layer->next_layer != NULL){
				memset(layer->Dw);
				memset(layer->Db);
				layer = layer->next_layer;
			}

		}
		else if (!strcmp(cur_master->type,"prep")){

			//memset the Dw,Db associated with this prep-layer
		}

		//go to next master...
		cur_master = cur_master->next;

	}

}


void update(Network network){

	Master cur_master = network->master_zero;

	while(cur_master->next != 0){
		if(!strcmp(cur_master->type,"conv")){
			conv_layer* layer = (conv_layer*) cur_master->linked_list;
			while(layer->next_layer != NULL){
				for(unsigned int i=0;i<layer->num_weights;i++) layer->weights[i] -= network->learning_params[0]* layer->Dw[i];
				for(unsigned int i=0;i<layer->num_channels;i++)layer->biases[i]  -= network->learning_params[0]* layer->Db[i];
				layer = layer->next_layer;
			}

		}
		else if(!strcmp(cur_master->type,"dense")){
			dense_layer* layer = (dense_layer*) cur_master->linked_list;
			while(layer->next_layer != NULL){
				unsigned int num_weights = layer->layer_size * layer->next_layer->layer_size;
				unsigned int num_biases  = layer->next_layer->layer_size;
				for(unsigned int i=0;i<num_weights;i++) layer->weights[i] -= network->learning_params[0]* layer->Dw[i];
				for(unsigned int i=0;i<num_biases;i++)  layer->biases[i]  -= network->learning_params[0]* layer->Db[i];
					layer = layer->next_layer;
			}

		}
		else if (!strcmp(cur_master->type,"prep")){

			//nothing to do here unless prep-layer has weights
		}

		//go to next master...
		cur_master = cur_master->next;

	}

}

void learn_collection(Network network,int* labels,int collection_val,int num_batches){ //Dw, Db initialized in here (not allocated tho)

	//spesific batch i in collection given by (collection_val*collection_size+i)%num_batches

	memset_Derivs(Network network);

	for(int i=0;i<network->collection_size;i++){

		int label_index = ((collection_val*network->collection_size+i)%num_batches);

		double* this_batch = network.network_input.data 
							 + batch_size* label_index;
		int* this_label    = labels + label_index;

		pass_batch(network,this_batch,*this_label);

	}



}


//NOT THE LEARN EPOCH, CHANGE TO JUST LEARN or something --> OR IS IT??? --> fix
void learn_epoch(Network network,int* labels){ //& pass thru the exp-output == ???? --> could make char* labels

	//shuffle -->

		for(int i=0;i<num_batches;i++){ //num_batches = num_images (i.e. num-input-datas)

			int j = rand() % num_batches;

			int temp = index_batches[i];
			index_batches[i] = index_batches[j];
			index_batches[j] = temp;

			int label_temp = labels[i];
			labels[i] = labels[j];
			labels[j] = label_temp;


		}


	//learn each collection in epoch -->

	for(int collection_val=0;collection_val<num_collections;collection_val++){
		//pass for each batch in collection --> call to learn_collection
		//update weights,biases

		learn_collection(network,labels,collection_val,num_batches);

		update(network);
	}

}

//given collection size = number of batches in a collection
//given batch_size = size of each batch

void learn(Network network,int* labels){


	//find batch_size & num_collections

	int batch_size = network.network_input.i_rows * network.network_input.i_cols * network.network_input.num_channels;
	int num_batches = sizeof(network.network_input.data)/batch_size;  //given or calculate as done here???

	int num_collections = network.collection_size / batch_size;
	if(network.collection_size % batch_size) num_collections++;

	//initialize indecies for shuffling tracking -->
	int index_batches[num_batches];// num_batches == num_labels
	for(int i=0;i<num_batches;i++) index_batches[i] = i;


	//allocate & initialize weights & biases |||&&&& allocate Dw & Db
	//--> actually allocations should all be done upon initialize/add layer funcs
	//should only need to memset stuff
	//already done in network.h

	//...//

	for(int epoch=0;epoch<network->num_epochs;epoch++){
		learn_epoch(network,labels);
	}



}


