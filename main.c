#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for string comp -- int strcmp(s1,s2)
#include <math.h>   //for exp,log,square
#include "network.h"

//need to have an initial function to put input into first linked_list -->

int main(){ ////////////!!!!!!!-------need to update acc to network.h-------------!!!!!!!!///////


	//first some kind of initialization of input -->
	input network_input;
		//input.input_data
		//input.i_rows
		//input.i_cols


	double learning_params[] = {0.05,0.9,1};
	Network my_network = create("SGD","cross_entropy",32,16,learning_params);

	Master* master_one = intitialize_Master(my_network,"conv");  //is *linked_list actually pointing to conv_one?
		conv_layer* conv_one = initialize_conv(network_input,master_one,3,2,32,"RELU","max_pooling",2,2);
		conv_layer* conv_two = add_conv(conv_one,3,2,64,"RELU","max_pooling",3,3);
		conv_layer* conv_three = add_conv(conv_two,2,2,2,"RELU","ave_pooling",2,2);


	Master* master_two = add_Master(master_one,"dense");
		dense_layer* dense_one = initialize_dense(???,master_two,30,"sigmoid");
		dense_layer* dense_two = add_dense(dense_one,10,"softmax");


	/*Network my_network = create("SGD","cross_entropy",32,16,learning_params);

	Node* node_one = add_conv(my_network.node_zero,3,2,32,"RELU","max_pooling",2,2);
		//struct conv_layer data = *(struct conv_layer*) node_one.layer_data;
		//printf("%d \n",data.f_rows);

	Node* node_two = add_conv(node_one,3,2,32,"RELU","max_pooling",2,4);

		printf("%d \n",((*(struct conv_layer*) (*node_two).layer_data).p_cols)); //using dot
		printf("%d \n",(((struct conv_layer*) node_two->layer_data)->p_cols)); //using ->


	//*** possibly add a flatten() that lets cpu know when ending the conv & starting the dense.

	Node* node_three = add_dense(node_two,10,"softmax"); //is last layer so node_three->next_layer = NULL

		printf("%d \n", (((struct dense_layer*) node_three->layer_data)->layer_size)); //testing


	*/
	//IDEA --> maybe instead we have something like this: 
		//a master linked list containing...
			// 0. 0th layer (intput) -->
			// 1. conv linked list
			// 2. dense linked list

	//user example like this then--->
		//create network() --> creates master linked list
		//add conv()       --> adds conv layer
		//add conv()       --> adds conv layer
		//flatten()        --> prepares/flattens layer for a dense layer
		//add dense()      --> adds dense layer
		//add dense()      --> adds dense layer


/*
	free((*node_one).layer_data);
	free((*node_two).layer_data);
	free((*node_three).layer_data);

	free(my_network.node_zero);
	free(node_one);
	free(node_two);
	free(node_three);

*/
	return 0;
}