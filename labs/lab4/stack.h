int MAXSIZE = 100;
long stack[100];
int top = -1;            

long int isempty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
   
long int isfull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}

long int peek() {
   return stack[top];
}

long int pop() {
   long int data;
	
   if(!isempty()) {
      data = stack[top];
      top = top - 1;   
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
   }
}

long int push(long int data) {

   if(!isfull()) {
      top = top + 1;   
      stack[top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}

void printAll(){
	for(long int i=top; i>-1; --i)
		printf("%ld\n", stack[i]);
	printf("\n\n");
}
