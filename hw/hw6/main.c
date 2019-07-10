#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int* data;
	int size;
	int capacity;
}vector;

void vector_init(vector *v);
void vector_add(vector *v, int value);
int  vector_capacity(vector *v);
int vector_get(vector *v,int index);
void vector_set(vector *v, int index, int value);
void vector_free(vector *v);
void vector_init(vector *v){
	v->size=0;
	v->capacity=3;
	v->data=malloc(sizeof(int)*v->capacity);
}

int vector_capacity(vector *v){
	return v->size;
}

void vector_add(vector *v, int data_to_add){	
	if(v->size==v->capacity){
		v->capacity*=2;
		v->data=realloc(v->data,sizeof(int)*v->capacity);
	}
	
	v->data[v->size++]=data_to_add;
}	

int vector_get(vector *v, int index){
	if(index>=v->size){
		exit(1);
	}
	
	return v->data[index];

}

void vector_set(vector *v, int index, int data_to_add){
	while(index >=v->size){
		vector_add(v,0);	
	}
	v->data[index]=data_to_add;
}

void vector_free(vector *v){
	free(v->data);
}

vector store_number_above_index(vector original, vector above){
	vector_init(&above);
	vector_set(&above,vector_capacity(&original)-2,1);

	for(int i=vector_capacity(&original)-3; i >= 0;--i)
		vector_set(&above,i,vector_get(&original,i+1) * vector_get(&above,i+1));

	return above;
}

vector store_number_below_index(vector original, vector below){
	vector_init(&below);	
	vector_set(&below,0,1);	
	for(int i=1; i <= vector_capacity(&original)-2;++i){
			vector_set(&below,i,vector_get(&original,i-1) * vector_get(&below,i-1));	
	}
	return below;
}

void print_the_products(vector above, vector below){	
	printf("OUTPUT\n");
	for (int i = 0; i <vector_capacity(&above); i++) {
		printf("%d\n", vector_get(&above,i)* vector_get(&below,i));
	}

}


void free_the_vectors(vector original,vector below, vector above){
	vector_free(&original);
	vector_free(&below);
	vector_free(&above);

}

void call_the_functions(vector original, vector above, vector below){	
	below=store_number_below_index(original,below);
	above=store_number_above_index(original,above);
	print_the_products(above, below);
	free_the_vectors(original,above,below);
}

int main(){
	vector original,below, above;
	int input_number;
	vector_init(&original);
	while(!feof(stdin)){
		scanf("%d",&input_number);
		vector_add(&original,input_number);
	}
	call_the_functions(original,above, below);
	return 0;
} 
