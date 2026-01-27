

//activation

			/////////////RELU//////////////
double RELU(double x){
	if(x>0) return x;
	else	return 0;
};
double d_RELU(double x){
	if(x>0) return 1;
	else    return 0; //x=0, derivative def = 0;
}
			///////////sigmoid///////////////
double sigmoid(double x){
	return 1/(1+exp(-x));
};
double d_sigmoid(double sigmoid_x){
	return sigmoid_x*(1-sigmoid_x);
}
			///////////softmax///////////////
double semi_softmax(double x){  //NOT NORMALIZED
	return exp(x);
};
double d_softmax(double softmax_x){ //assuming input properly normalized...
	return softmax_x * (1 - softmax_x); //yes, is redundant; same as d_sigmoid
}


		//////activation & d_activation/////////
double activation(char* type,double x){
	if(strcmp(type,"RELU") == 0)
		return RELU(x);
	if(strcmp(type,"sigmoid") == 0)
		return sigmoid(x);
	if(strcmp(type,"softmax") == 0)
		return semi_softmax(x);			//NOT NORMALIZED HERE

}
double d_activation(char* type,double activation_x){

	if(!strcmp(type,"RELU"))
		return d_RELU(activation_x); //same as inputing x
	if(!strcmp(type,"sigmoid"))
		return d_sigmoid(activation_x); //takes in sigmoid_x, not simply x
	if(!strcmp(type,"softmax"))
		return d_softmax(activation_x);
}


//loss

double square(double x){
	return x*x;
}

double cross_entropy(double* output,int label){

	return -log(output[label]);
};
double quadratic(double* output,int o_size,int label){
	double result = 0;
	for(int i=0;i<o_size;i++){
		if(i == label) result+= square(output[i] - 1);
		else           result+= square(output[i]);
	}
	return 0.5*result;
};

double loss(char* type,double* output,int o_size,int label){

	if(strcmp(type,"cross_entropy")==0)
		return cross_entropy(output,label);
	if(strcmp(type,"quadratic") == 0)
		return quadratic(output,o_size,label);
}

//M_initial = D_loss --> same for both cross_entropy & quadratic

void D_cross_entropy(double* M_initial,double* output,int o_size,int label){
	memset(M_initial,o_size);
	M_initial[label] = -(1/output[label]);
}

void D_quadratic(double* M_initial,double* output,int o_size,int label){
	for(int i=0;i<o_size;i++){
		if(i == label) M_initial[i] = output[i]-1;
		else           M_initial[i] = output[i];
	}
}

void D_loss(char* type,double* M_initial,double* output,int o_size,int label){

	if(!strcmp(type,"cross_entropy")) D_cross_entropy(M_initial,output,o_size,label);

	if(!strcmp(type,"quadratic"))     D_quadratic(M_initial,output,o_size,label);  

}



