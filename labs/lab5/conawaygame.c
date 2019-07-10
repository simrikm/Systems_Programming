#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define row 40
#define column 22

void print_cell(char cell[row][column]){
	for(int i=0; i <row; i++){
		for (int j=0; j <column; j++){
		printf("%c",cell[i][j]);
		}
	printf("\n");
	}

}


int main(){

	char cell[row][column];
	int i,j,answer;

	for(i=0; i <row; i++)
		for (j=0; j <column; j++)
		cell[i][j]='-';

	srand(time(NULL));

	for(j=0;j<row;j++){	
		int r=rand()%row;
		int s=rand()%column;
		cell[r][s]='o';
		cell[s][s]='o';	
		cell[j][s]='o';
	}
	
	for(int i=0; i <row; i++){
		for (int j=0; j <column; j++){
		if(i==0||j==0||i==row-1||j==column-1)
				cell[i][j]='-';
		}
	}
	
	print_cell(cell);	

		do{
			printf("Please press 1 to continue the game else press any key to quit:");
			scanf("%d",&answer);
			if(answer!=1)
				break;
			char new_cell[row][column];
	
			for(i=0; i <row; i++)
				for (j=0; j <column; j++)
				new_cell[i][j]=cell[i][j];

			for (int r = 1; r < row-1 ; r++)
    	    {
        	    for (int c = 1; c < column-1; c++)
           		 {
					int live_neighbours=0;
				    for(int i=r-1;i<=r+1;++i){
						for(int j=c-1;j<=c+1;++j){
							if(cell[i][j]=='o')
								live_neighbours++;	
						}
					}    
       				if(cell[r][c]=='o')
						if(live_neighbours < 3|| live_neighbours>4)
							new_cell[r][c]='-';	
					 if(cell[r][c]=='-' && live_neighbours==3)
						new_cell[r][c]='o';	
        
			}
			}	
			print_cell(new_cell);			
			for(i=0; i <row; i++)
				for (j=0; j <column; j++)
					cell[i][j]=new_cell[i][j];

		}while(answer==1);	

	 return 0;
}
