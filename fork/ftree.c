#include <stdio.h>
// Add your system includes here.

#include "ftree.h"
#include "hash.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <libgen.h>
/*
 * Returns the FTree rooted at the path fname.
 */

int copy_ftree(const char *src, const char *dest){
	
	int count = 1;
	struct stat buf;
  	int error = lstat(src, &buf);
  	struct stat buf2;
  	int error2 = lstat(dest, &buf2);
  	
  	const char s[2] = "/";
   char *curr;
  	char *src_name;
  	char *src_cpy1 = malloc(200);
  	strcpy(src_cpy1, src);
	curr = strtok(src_cpy1, s);
	
   while( curr != NULL ) 
   {
   	src_name = curr;
     	curr = strtok(NULL, s);
   }
   char* directory_diff = malloc(200);
   strcpy(directory_diff, dest);
   strcat(directory_diff, "/");
   strcat(directory_diff, src_name);
   struct stat buf3;
  	int error3 = lstat(directory_diff, &buf3);
   
   if(error3 == 0){
   	if (((S_ISREG((buf3).st_mode)) && (S_ISDIR((buf).st_mode))) || 
   	((S_ISDIR((buf3).st_mode)) && (S_ISREG((buf).st_mode))))
   	{
   		fprintf(stderr, "Same name but different type\n");
   		exit(-1);
   	}
   }
   
  	if(error < 0) {
		perror("lstat src\n");
		exit(-1);
	}
	
	if(error2 < 0) {
		perror("lstat dest\n");
		exit(-1);
	}
   
  	if ((S_ISREG((buf2).st_mode))){
  		fprintf(stderr, "dest is not directory\n");
		exit(-1);
  	}
  	
	if ((S_ISREG((buf).st_mode))){
		FILE* error3 = fopen(src,"r");
		if(error3 == NULL) {
			perror("fopen");
			exit(-1);
	 	}


		opendir(dest);
		const char s[2] = "/";
   	char *current;
  		char *real_src_name;
  		char *src_cpy = malloc(200);
  		strcpy(src_cpy, src);
		current = strtok(src_cpy, s);
   	while( current != NULL ) 
   	{
   		real_src_name = current;
      	current = strtok(NULL, s);
   	}
   
   	char* directory_copy = malloc(200);
   	strcpy(directory_copy, dest);
   	strcat(directory_copy, "/");
   	strcat(directory_copy, real_src_name);
   	FILE *fp_dest = fopen(directory_copy, "r");
   	FILE *fp_src = fopen(src, "r");
   	
		if (fp_dest == NULL){
			struct stat buf_src;
			int eor2 = lstat(src, &buf_src);
			if ((eor2 == -1)) {
				perror("lstat");
				exit(-1);
			}
			FILE *fp = fopen(directory_copy, "w");
			char copy_value[2];
			int check_point;
			while ((check_point = fread(copy_value, 1, 1, fp_src)) != 0){
				if (check_point == -1){
						perror("read\n");
						exit(-1);
				}
				fwrite(copy_value, 1, 1, fp);
			}
			chmod(directory_copy, buf_src.st_mode &0777);
			fclose(fp);
			fclose(fp_src);
		}
		
		else if (fp_dest != NULL){
			struct stat buf_dest;
			struct stat buf_src;
			int eor1 = lstat(directory_copy, &buf_dest);
			int eor2 = lstat(src, &buf_src);
			if ((eor1 == -1) || (eor2 == -1)) {
				perror("lstat");
				exit(-1);
			}
			char* dest_hash = hash(fp_dest);
			char* src_hash = hash(fp_src);
			int check_hash = strcmp(dest_hash, src_hash);
			if (((buf_dest).st_size != (buf_src).st_size) || (check_hash != 0)){
				FILE *fp_dest = fopen(directory_copy, "w");
   			FILE *fp_src = fopen(src, "r");	
				char copy_value[2];
				int check_point;
				while ((check_point = fread(copy_value, 1, 1, fp_src)) != 0){
					if (check_point == -1){
							perror("read\n");
							exit(-1);
					}
					fwrite(copy_value, 1, 1, fp_dest);
				}
				chmod(directory_copy, buf_src.st_mode &0777);
				fclose(fp_dest);
				fclose(fp_src);
			}
		}
	free(directory_copy);
	free(src_cpy);
	return 0;
	}
	
	// If the given source is directory
	if (S_ISDIR((buf).st_mode)){
		
		const char s[2] = "/";
   	char *current;
  		char *real_src_name;
  		char *src_cpy = malloc(200);
  		strcpy(src_cpy, src);
		current = strtok(src_cpy, s);
		
   	while(current != NULL) {
   		real_src_name = current;
      	current = strtok(NULL, s);
   	}
   	
		char* directory_copy = malloc(200);
   	strcpy(directory_copy, dest);
   	strcat(directory_copy, "/");
   	strcat(directory_copy, real_src_name);
   	
   	struct stat buf_src;
		int eor2 = lstat(src, &buf_src);
		if ((eor2 == -1)) {
				perror("lstat");
				exit(-1);
		}
		
		DIR *dir_check = opendir(directory_copy);
		
		if (dir_check == NULL){
			mkdir(directory_copy, 0777);//mkdir(directory_copy, buf_src.st_mode&0777);
		}

		DIR *dir = opendir(src);
		struct dirent *dp;
		while ((dp = readdir(dir)) != NULL){
			if ((*dp).d_name[0] != '.'){
				char *s = "/";
    			char* src_path = malloc(200);
    			strcpy(src_path, (char *)src);
    			strcat(src_path, s);
    			strcat(src_path, (*dp).d_name);
    			strcat(src_path, "\0");
    			struct stat buffer;
    			lstat(src_path, &buffer);
    			printf("%s\n",src_path);

    			if (S_ISDIR(buffer.st_mode)){
    				int result;
    				result = fork();
    				//sleep(2);
    				if (result > 0){
						int time;
						int status = wait(&time);
    					if (status == -1) {
    						perror("wait");
    						exit(-1);
    					}else{
    						if(WIFEXITED(time)){
    							if(WEXITSTATUS(time) >= 0){
									count += WEXITSTATUS(time);						
    							}
    						}
    					}
    				}
    				else if(result == 0){
    					int something = copy_ftree(src_path, directory_copy);
    					exit(something);
    				}
    				else if (result == -1){
    					perror("fork");
    					exit(-1);
    				}
    			}
    			
    			if (S_ISREG(buffer.st_mode)){
    				char *dest_path = malloc(200);
					strcpy(dest_path, directory_copy);
   				strcat(dest_path, "/");
   				strcat(dest_path, (*dp).d_name);
    				FILE *fp_dest = fopen(dest_path, "r");
    				FILE *fp_src = fopen(src_path, "r");
    				if (fp_src == NULL){
    					perror("FILE");
    					exit(-1);
    				}
    				if (fp_dest == NULL){
    					struct stat buf_src;
						int eor2 = lstat(src_path, &buf_src);
						if ((eor2 == -1)) {
							perror("lstat");
							exit(-1);
						}
						FILE *fp = fopen(dest_path, "w");
						char copy_value[2];
						int check_point;
						while ((check_point = fread(copy_value, 1, 1, fp_src)) != 0){
							if (check_point == -1){
								perror("read\n");
								exit(-1);
							}
							fwrite(copy_value, 1, 1, fp);
						}
						chmod(dest_path, buf_src.st_mode & 0777);
						fclose(fp);
						fclose(fp_src);
					}
					else{
						struct stat buf_dest;
						struct stat buf_src;
						int eor1 = lstat(dest_path, &buf_dest);
						int eor2 = lstat(src_path, &buf_src);
						if ((eor1 == -1) || (eor2 == -1)) {
							perror("lstat4");
							exit(-1);
						}
						char* dest_hash = hash(fp_dest);
						char* src_hash = hash(fp_src);
						int check_hash = strcmp(dest_hash, src_hash);
						int dest_size = (buf_dest).st_size;
						int src_size = (buf_src).st_size;
						fclose(fp_dest);
						if ((dest_size != src_size) || (check_hash != 0)){
							FILE *fp_dest = fopen(dest_path, "w");
   						FILE *fp_src = fopen(src_path, "r");
							char copy_value[2];
							int check_point;
							while ((check_point = fread(copy_value, 1, 1, fp_src)) != 0){
								if (check_point == -1){
									perror("read\n");
									exit(-1);
								}
								fwrite(copy_value, 1, 1, fp_dest);
							}
						chmod(dest_path, buf_src.st_mode&0777);
						fclose(fp_dest);
						fclose(fp_src);
						}
					}
				free(dest_path);
    			}
    		free(src_path);
    		}
    	}
    chmod(directory_copy, buf_src.st_mode&0777);
    free(directory_copy);
    closedir(dir);
	}
	free(src_cpy1);
   free(directory_diff);
	return count;
}







