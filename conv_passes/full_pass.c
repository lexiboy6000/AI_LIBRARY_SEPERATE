//extern functions -->

#include "forward_pass.h"
#include "pooling.h"
#include "backprop.h"


//****************************pass conv-list************************************//
//****************************pass conv-list************************************//
//****************************pass conv-list************************************//
//****************************pass conv-list************************************//


int** index_largest_output = malloc(0);
int largest_order = 0;

input forward_conv_list(Master* master){

	input output; 

	conv_layer* layer = (conv_layer*) master->linked_list;
	//layer->input = input.input_data; -->already done in master / pass_batch()
		//layer->i_rows = input.i_rows;
		//layer->i_cols = input.i_cols;

//foward-->

	while(layer->next_layer != NULL){

		if(strcmp(layer->pooling,"ave_pooling"))
			forward_conv_ave_pool(layer);
		else if(strcmp(layer->pooling,"max_pooling"))
		{
			forward_conv_max_pool(layer,index_largest_output[largest_order]);
			largest_order++;
		}

		layer = layer->next_layer;
	}

	//set last layer to be layer before null
	master->last_layer = (void*)layer;

	//set output from last layer
	output.input_data   = layer->input;
	output.i_rows       = layer->i_rows;
	output.i_cols       = layer->i_cols;
	output.num_channels = layer->total_channels;

	return output;

}


double* backprop_conv_list(Master cur_master,double* M_initial){

	conv_layer* layer = (conv_layer*) cur_master->last_layer;

	double* M = M_initial; //note that M_initial gets freed aswell 
	while(layer->prev_layer != NULL){

		if(strcmp(layer->pooling,"ave_pooling"))
		{
			double* M_new = backprop_conv_ave_pool(layer,M,layer->Dw,layer->Db);
			free(M);
			M = M_new;

			//free layer's outputs,inters,As
			if(layer->next_layer != NULL) free(layer->next_layer->input);
			free(layer->A);
			free(layer->inter);

		}
		if(strcmp(layer->pooling,"max_pooling")) 
		{
			double* M_new = backprop_conv_max_pool(layer,M,layer->Dw,layer->Db,index_largest_output[largest_order]);

			free(index_largest_output[largest_order]);
			largest_order--;

			free(M);
			M=M_new;

			//free layer's outputs,inters,As
			if(layer->next_layer != NULL) free(layer->next_layer->input);
			free(layer->A);
			free(layer->inter);
		}

		layer = layer->prev_layer;
	}

	free(layer->input); //no longer need cuz return M rt?

	return M;
	//free initial input?? --> depends on if & type of layers before this, not nessesarily


}


/*

void pass_conv_list(Master* master,double* M_initial,double* Dw,double* Db){

	conv_layer* layer = (conv_layer*) master->linked_list;

	int** index_largest_output = malloc(0);
	int largest_order = 0;

//foward-->

	while(layer->next_layer != NULL){

		if(strcmp(layer->pooling,"ave_pooling"))
			forward_conv_ave_pool(layer);
		if(strcmp(layer->pooling,"max_pooling"))
		{
			forward_conv_max_pool(layer,index_largest_output[largest_order]);
			largest_order++;
		}

		layer = layer->next_layer;
	}

//backprop-->

	double* M = M_initial; //note that M_initial gets freed aswell 
	while(layer->prev_layer != NULL){

		if(strcmp(layer->pooling,"ave_pooling"))
		{
			double* M_new = backprop_conv_ave_pool(layer,M,Dw,Db);
			free(M);
			M = M_new;

			//free layer's outputs,inters,As
			if(layer->next_layer != NULL) free(layer->next_layer->input);
			free(layer->A);
			free(layer->inter);

		}
		if(strcmp(layer->pooling,"max_pooling")) 
		{
			double* M_new = backprop_conv_max_pool(layer,M,Dw,Db,index_largest_output[largest_order]);

			free(index_largest_output[largest_order]);
			largest_order--;

			free(M);
			M=M_new;

			//free layer's outputs,inters,As
			if(layer->next_layer != NULL) free(layer->next_layer->input);
			free(layer->A);
			free(layer->inter);
		}

		layer = layer->prev_layer;
	}


	//free initial input?? --> depends on if & type of layers before this, not nessesarily

}
