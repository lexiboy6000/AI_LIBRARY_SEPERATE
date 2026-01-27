

double* initialize_weights(int size){
	//allocation-->
	double* result = malloc(size*sizeof(double));
	//some initialization function-->
	//should make a library of funcs out of this...
	for(int i=0;i<size;i++){
		result[i] = 0.1 + (rand()%20)*0.1;
	}
	return result;
}

//******input structure**//

typedef struct input{

	double* data;
		int i_rows;
		int i_cols;
		int num_channels;

	//other stuff like dimensions, etc.

} input;

//******conv_layer******//

typedef struct conv_layer{

	//filter
	int f_rows;
	int f_cols;

	//num-channels
	int num_channels;
	int total_channels;

	//activation-func
	char* activation;

	//pooling func
	char* pooling;
		int p_rows;
		int p_cols;


	//ptrs to weights & input

	double* input; //output put into the input of next layer...
	//input = double* output;  //of size num_channels * (input_rows-f_rows+1)/p_rows *(input_cols-f_cols+1)/p_cols
		int i_rows;
		int i_cols;

	double* weights; unsigned int num_weights; //of size num_channels*f_rows*f_cols
	double* biases;  unsigned int num_biases; //of size num_channels

	//next & prev layers...
	struct conv_layer* prev_layer; //if = NULL, is initial layer
	struct conv_layer* next_layer; //if = NULL, is last layer

	//intermediate calculations for next layer
	double* inter; //add sizes of inter in here --> WILL MAKE MORE EFFICIENT
		int inter_rows;
		int inter_cols;
	double* A;     //activation(inter)


	//layer derivatives
	double* Dw;
	double* Db;


} conv_layer;


conv_layer* initialize_conv(Master* conv_master,
					int f_rows,int f_cols,
					int num_channels,
					char* activation,
					char* pooling, int p_rows,int p_cols){
	conv_layer* cur = (conv_layer*) conv_master->linked_list;

		//initialize input---> do in passes rt?
			//cur->input = input.input;
			//cur->i_rows = input.i_rows;
			//cur->i_cols = input.i_cols;

		//prev = malloc(sizeof(conv_layer)); //sus.... not sure this is doing wut I want....
			cur->f_rows         = f_rows; //fine cuz changing what's INSIDE a ptr
			cur->f_cols         = f_cols;
			cur-> num_channels  = num_channels;
			cur->activation     = activation;
			cur->pooling        = pooling;
			cur->p_rows         = p_rows;
			cur->p_cols         = p_cols;
		//total_channels -->
			cur->total_channels = input.num_channels * cur->num_channels;
		//inter rows,cols -->
			cur->inter_rows = cur->i_rows - cur->f_rows + 1;
			cur->inter_cols = cur->i_cols - cur->f_cols + 1;

		//initialize weights,biases,Dw,Db
			cur->num_weights = cur->f_rows*cur->f_cols*cur->num_channels;
			cur->num_biases = cur->num_channels;

			cur->weights = initialize_weights(num_weights);
			cur->biases  = initialize_biases(num_biases);
			cur->Dw      = malloc(num_weights);
			cur->Db      = malloc(num_biases);


		cur->prev_layer = NULL;
		cur->next_layer = NULL;

		return cur;
}

conv_layer* add_conv(conv_layer* prev,
					int f_rows,int f_cols,
					int num_channels,
					char* activation,
					char* pooling, int p_rows,int p_cols){


		conv_layer* cur = malloc(sizeof(conv_layer));
			cur->f_rows = f_rows;
			cur->f_cols = f_cols;
			cur->num_channels = num_channels;
			cur->activation = activation;
			cur->pooling = pooling;
			cur-> p_rows = p_rows;
			cur->p_cols = p_cols;

		//inter rows,cols
			cur->inter_rows = cur->i_rows - cur->f_rows + 1;
			cur->inter_cols = cur->i_cols - cur->f_cols + 1;

			//*****************LINKING STRUCTS***************//
		cur->prev_layer = prev;
		cur->next_layer = NULL;

		cur->prev_layer->next_layer = cur;

		//find total_channels
		cur->total_channels = cur->prev_layer->total_channels * cur->num_channels;


		//initialize weights,biases,Dw,Db
			cur->num_weights = cur->f_rows*cur->f_cols*cur->num_channels;
			cur->num_biases = cur->num_channels;

			cur->weights = initialize_weights(num_weights);
			cur->biases  = initialize_biases(num_biases);
			cur->Dw      = malloc(num_weights);
			cur->Db      = malloc(num_biases);

		return cur;
}

typedef struct dense_layer{

	

	int layer_size;
	char* activation;

	double* input; //of size layer_size

	double* weights; //of size layer_size*next-layer_size
	double* biases;  //of size next-layer_size

	int total_weights; //wtf why did u put this in here??
	int total_biases;

	double* f; //of size next-layer_size (intermediate layer)

	//layer derivatives
	double* Dw;
	double* Db;

	/***********/

	struct dense_layer* prev_layer;
	struct dense_layer* next_layer;

} dense_layer;

dense_layer* initialize_dense(Master* dense_master,int layer_size,char* activation){

	dense_layer* cur = (dense_layer*) dense_master->linked_list;

		//prev = malloc(sizeof(dense_layer));
		//int input_size = input.i_rows*input.i_cols;
		//cur->layer_size = input_size;
		cur->layer_size = layer_size;
		cur->activation = activation;

		//*******linking structs******//

		cur->prev_layer = NULL;
		cur->next_layer = NULL;

		return cur;

}


dense_layer* add_dense(dense_layer* prev,int layer_size,char* activation){

	dense_layer* cur = malloc(sizeof(dense_layer));
		cur->layer_size = layer_size;
		cur->activation = activation;

	//*****linking structs********//

	cur->prev_layer = prev;
	cur->next_layer = NULL;

	cur->prev_layer->next_layer = cur;

	//for prev layer, allocate weights,baises,Dw,Db & initialize weights,biases-->

	int num_weights = cur->prev_layer->layer_size*cur->layer_size;

	cur->prev_layer->weights= initialize_weights(num_weights);
	cur->prev_layer->biases = initialize_biases(cur->layer_size);
	cur->prev_layer->Dw     = malloc(num_weights);
	cur->prev_layer->Db     = malloc(cur->layer_size);


	return cur;
	
}


typedef struct Master{

	char* type;
	void* linked_list;
		void* last_layer;

	struct Master* prev;
	struct Master* next;

} Master;

Master* initialize_Master(Network my_network,char* type){

	Master* cur = my_network.master_zero;
		cur->type = type;
		cur->prev = prev;
		cur->next = NULL;

		if(strcmp(type,"conv") == 0)
			cur->linked_list = malloc(sizeof(conv_layer)); //conv zero rt?
		if(strcmp(type,"dense")== 0)
			cur->linked_list = malloc(sizeof(dense_layer));//dense zero rt?
		

	return cur;

}

Master* add_Master(Master* prev,char* type){

	//first set static first to zero
	//first = 0;

	Master* cur = malloc(sizeof(Master));
		cur->type = type;
		cur->prev = prev;
		cur->next = NULL;

		if(strcmp(type,"conv") == 0)
			cur->linked_list = malloc(sizeof(conv_layer)); //conv zero rt?
		if(strcmp(type,"dense")== 0)
			cur->linked_list = malloc(sizeof(dense_layer));//dense zero rt?
		

	return cur;
}



typedef struct Network {

	Master* master_zero;

	//data
	//double* input_data;
	input network_input;

	//functions
	char* learn; //SGD, adaGRAD, adam, etc.
	char* loss;

	//all learning funcs use these
	int num_epochs;
	int batch_size; //a.k.a. number of "mini-batches"

	//learning params -- learning_rate, decay, etc. 
	//which params likely to be dependent on learn function
	double* learning_params;

} Network;


Network create(char* learn,char* loss,input input,// --> put in master instead
			   int num_epochs,int collection_size,
			   double* learning_params){ //0th learning_param = LEARNING RATE

	//first = 0;

	Network my_network;
		my_network.learn = learn;
		my_network.loss = loss;
		my_network.num_epochs = num_epochs;
		//my_network.batch_size = batch_size; --> contained in input
		my_network.collection_size = collection_size;
		my_network.learning_params = learning_params;

	//initialize input
	my_network.network_input.data = input.data;
	my_network.network_input.i_rows = input.i_rows;
	my_network.network_input.i_cols = input.i_cols;
	my_network.network_input.num_channels = input.num_channels;

	//setting up master_zero... NEEDED NOW THAT INPUT PART OF Network ???

	//nessesary? -- already have input in a struct input

	my_network.master_zero = malloc(sizeof(Master));
	/*
		my_network.master_zero = malloc(sizeof(Master));
		my_network.master_zero->type = "input";
		my_network.master_zero->linked_list = malloc(sizeof(input));
		my_network.master_zero->prev = NULL;
		my_network.master_zero->next = NULL;
	*/


	return my_network;
}