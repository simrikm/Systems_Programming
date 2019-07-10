#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define STRMAX 40
#define SONGMAX 1024

struct Song{
	char title[STRMAX];
	char artist[STRMAX];
	int year_published;
};

struct Song music_library[SONGMAX];
int current_number_of_songs=0;



/**EXCEEDING 5 Lines by 3**/
int compare_toLower(char song_title1[STRMAX],char song_title2[STRMAX]){
	int i;
	char s1[STRMAX],s2[STRMAX];
	strcpy(s1,song_title1);
	strcpy(s2,song_title2);
	for(i=0;s1[i];i++){
		s1[i]=tolower(s1[i]);
	}
	for(i=0;s2[i];i++){
		s2[i]=tolower(s2[i]);
	}
		return strcmp(s1,s2);
}

int find_index_of_song_with_name(char song_name[STRMAX],int high, int low){
	while(low<=high){
		if(compare_toLower(music_library[(low+high)/2].title,song_name)==0)
			return (low+high)/2;
		return (compare_toLower(music_library[(low+high)/2].title,song_name)>0?find_index_of_song_with_name(song_name,((low+high)/2)-1,low):find_index_of_song_with_name(song_name,high,((low+high)/2)+1));
	}
	return low;
}	

void  get_title(char new_song[]){
	printf("Title: ");
	scanf(" %[^\t\n]s",new_song); 
}

void crunch_down_from(int index){
	int i;	
	for(i=current_number_of_songs;i>index;i--){
		strncpy(music_library[i].title,music_library[i-1].title,STRMAX);	
		strncpy(music_library[i].artist,music_library[i-1].artist,STRMAX);
		music_library[i].year_published=music_library[i-1].year_published;
	}
}

void crunch_up_from(int index){
	int i;
	for(i=index;i<current_number_of_songs;i++){
		strncpy(music_library[i].title,music_library[i+1].title,STRMAX);	
		strncpy(music_library[i].artist,music_library[i+1].artist,STRMAX);
		music_library[i].year_published=music_library[i+1].year_published;
	}
}                
void add_the_songfield(int index,char new_song[]){	
	strcpy(music_library[index].title,new_song);
	printf("Artist: ");
	scanf(" %[^\t\n]s",music_library[index].artist);
	printf("Year Published: ");
	scanf(" %d",&music_library[index].year_published);;

}
                        
void add_song_to_MusicLibrary(){
	char new_song[STRMAX];
	int index=0;
	get_title(new_song);
	index=find_index_of_song_with_name(new_song,current_number_of_songs-1,0);
	crunch_down_from(index);
	add_the_songfield(index,new_song);
}


void print_MusicLibrary(int start,int end,bool print_line_number){
	int line_number=1,i;
	for(i=start; i<end; i++){
		if(print_line_number)
			printf("%d Title:%s, Artist:%s, Year Published:%d\n",line_number++,music_library[i].title,music_library[i].artist,music_library[i].year_published);
		else				
			printf("Title:%s, Artist:%s, Year Published:%d\n",music_library[i].title,music_library[i].artist,music_library[i].year_published);
	}
}

bool check_if_song_exists(char song_lookup[STRMAX],int index){	
	if(compare_toLower(song_lookup,music_library[index].title)!=0){
		printf("The song you are looking for doesn't exists in the library\n");
		return false;
	}
	return true;
}

void remove_song_from_MusicLibrary_by_name(){
	char song_delete[STRMAX];
	get_title(song_delete);	
	if(check_if_song_exists(song_delete,find_index_of_song_with_name(song_delete,current_number_of_songs-1,0))){
		crunch_up_from(find_index_of_song_with_name(song_delete,current_number_of_songs-1,0));
	}

}

void lookup_song(){
	char song_lookup[STRMAX];
	printf("Title: ");
	scanf(" %[^\t\n]s",song_lookup);
	if(check_if_song_exists(song_lookup,find_index_of_song_with_name(song_lookup,current_number_of_songs-1,0))){
		print_MusicLibrary(find_index_of_song_with_name(song_lookup,current_number_of_songs-1,0),(find_index_of_song_with_name(song_lookup,current_number_of_songs-1,0))+1,false);
	}
}

void evaluate_command(int command){
	switch (command){
  		case 'i':
        case 'I':
            add_song_to_MusicLibrary();
			current_number_of_songs++;
            break;
        case 'p':
        case 'P':
            print_MusicLibrary(0,current_number_of_songs,true);
            break;
        case 'd':
        case 'D':
            remove_song_from_MusicLibrary_by_name();
			current_number_of_songs--;
            break;
        case 'l':
        case 'L':
            lookup_song();
            break;
        default:
            break;
	} 		
}

/*EXCEEDING 5 LINES BY 1*/
void read_command(char current_library[]){
	int command_input;
	printf("%s command:", current_library);
	if((command_input=getchar())!='q' && command_input!='Q'){
			evaluate_command(command_input);
			while((command_input=getchar())!='\n'&& command_input!=EOF);
				read_command(current_library);
	}
}

FILE *  open_file(char current_library[],const char *file_mode){
	FILE *file;
	file=fopen(current_library,file_mode);
	if(file==NULL){
		fprintf(stderr, "Can't open %s\n", current_library);
	}

	return file;
}

void sort_the_library(char title[],char artist[],int year,int index){
	strcpy(music_library[index].title,title);
	strcpy(music_library[index].artist,artist);
	music_library[index].year_published=year;	
	current_number_of_songs++;
}

int compare_title(const void *first_song, const void *second_song){
    struct Song *song1 =(struct Song* )first_song;
    struct Song *song2 =(struct Song* )second_song;
    int comparing_title=compare_toLower(song1->title,song2->title);
    if(comparing_title==0)
        return song1->artist-song2->artist;
    else
        return comparing_title;
}

void read_song(FILE *filename){	
	char title[STRMAX], artist[STRMAX]; 
	int year;
	while((fscanf(filename,"Title:%[^,], Artist:%[^,], Year Published:%d",title, artist, &year))==3){
	sort_the_library(title,artist,year,current_number_of_songs);
	fgetc(filename);
	}
	
}

void write_song(FILE *filename){
	int i;
	for( i=0; i<current_number_of_songs;i++)
		fprintf(filename,"Title:%s, Artist:%s, Year Published:%d\n",music_library[i].title,music_library[i].artist,music_library[i].year_published);				
}


void load_MusicLibrary(char current_library[]){
	FILE *fp;
	fp=open_file(current_library,"r");
	read_song(fp);
	qsort(music_library,current_number_of_songs,sizeof(struct Song),compare_title);
	fclose(fp);
}

void store_MusicLibrary(char current_library[]){
	FILE *fp;
	fp=open_file(current_library,"w");
	write_song(fp);
	fclose(fp);
}

void calling_the_functions(char library_name[STRMAX]){	
	load_MusicLibrary(library_name);
	read_command(library_name);
	store_MusicLibrary(library_name);
}

int main(int argc,char *argv[]){
	char default_library_name[STRMAX]="myMusic";
	char *library_name=(argc>1)?argv[1]:default_library_name;
	calling_the_functions(library_name);
	return 0;
}

