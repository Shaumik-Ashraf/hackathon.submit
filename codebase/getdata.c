//getdata.c
//defines getdata function
//processes form GET request and cgi data to double data matrix

//returns EXIT_SUCCESS if works,
//else returns EXIT_FAILURE and sets error_msg

//NOTE: error_msg max: 256, assumes it is allocated

#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<errno.h>
#include<ctype.h>
//#include<math.h>


int getdata(double*** matrixp, int** rowsizesp, int* rownump, char* error_msg) {
	
	char* request_method = getenv("REQUEST_METHOD");
	char* query;
	
	char* datavalue;  //decoded string containing csv data
	int datavalue_length;
	
	char* ptr1;
	char* ptr2;
	
	int comma_ctr, newline_ctr;
	int i, j, k;
	
	
	/*=================================================================
	  ==EXTRACT DATAVALUE FROM QUERY===================================
	  ===============================================================*/
	if( strcmp(request_method, "GET")!=0 ) {
		strcpy(error_msg, "Incorrect form request\n");
		return(EXIT_FAILURE);
	}
	
	query = getenv("QUERY_STRING"); //Check QUERY_STRING max!!!
	//printf("query:%s\n", query); //DEBUG@@@@@@@@@@@@@@@
	
	ptr1 = strstr(query, "datakey=");
	if( ptr1==NULL ) {
		strcpy(error_msg, "Incorrect form request\n");
		return(EXIT_FAILURE);
	}
	ptr1 = strchr(ptr1,'=');
	ptr1++;
	
	for(datavalue_length=0, i=0; ptr1[i]!='&' && ptr1[i]!='\0'; datavalue_length++, i++) {
		if( ptr1[i]=='%' ) datavalue_length -= 2;
	}
	//INSERT INT OVERFLOW CHECK!!
	datavalue_length++;
	
	datavalue = (char*)malloc(datavalue_length*sizeof(char));
	if( datavalue==NULL ) {
		strcpy(error_msg, "Sorry, not enough memory\n");
		return(EXIT_FAILURE);
	}
	
	//decode section of query and cpy it to datavalue
	//i=query iterator, j=datavalue iterator
	for(i=0, j=0; ptr1[i]!='&' && ptr1[i]!='\0' && j<datavalue_length-1; i++, j++) {
		if( ptr1[i]=='+' ) {
			//datavalue[j] = ' ';
			j--;  //remove spaces from datavalue
		}
		else if( ptr1[i]=='%' ) {
			
			unsigned int hexchar;
			
			if( sscanf(ptr1+i+1, "%2x", &hexchar)!=1 ) {
				strcpy(error_msg, "Error reading data\n");
				return(EXIT_FAILURE);
			}
			
			if( hexchar==0x0d ) { //characters to ignore: CR
				j--;
				i+=2;
			}
			else if( hexchar!='\n' && hexchar!=',' ) {
				//printf("Invalid symbol: _%c_ _%x_ \n", hexchar, hexchar); //DEBUG@@@@@@@@@@@@@@@@@@@@@@@@@
				strcpy(error_msg, "Data contained invalid symbol\n");
				return(EXIT_FAILURE);
			}
			else {
				datavalue[j] = (char)hexchar;
				i+=2;
			}
			
		}
		else if( isalnum(ptr1[i]) && !isalpha(ptr1[i]) ) {
			datavalue[j] = ptr1[i];
		}
		else if( ptr1[i]=='.' || ptr1[i]=='-' ) {  //these chars are NOT converted to hex
			datavalue[j] = ptr1[i];
		}
		else {
			strcpy(error_msg, "Data contained invalid character\n");
			//printf("Invalid character: _%c_ _%x_; pos: %i\n", ptr1[i], ptr1[i], i); //DEBUG@@@@@@@@@@@@
			return(EXIT_FAILURE);
		}
		
	}
	
	datavalue[j+1] = '\0';
	//printf("decoding done!!\n");  //DEBUG@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
	//free(query);
	//printf("datavalue:%s\n", datavalue);  //DEBUG@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
	/*==============================================================
	  ==ALLOC MEM AND SCAN DATA FROM DATAVALUE======================
	  ============================================================*/
	
	for(newline_ctr=0, i=0; datavalue[i]!='\0'; i++) {
		if( datavalue[i]=='\n' ) newline_ctr++;
	}
	*rownump = newline_ctr;
	
	if( (*rowsizesp=(int*)malloc(newline_ctr*sizeof(int)))==NULL ) {
		strcpy(error_msg, "Sorry, not enough memory\n");
		return(EXIT_FAILURE);
	}
	if( (*matrixp=(double**)malloc(newline_ctr*sizeof(double*)))==NULL ) {
		strcpy(error_msg, "Sorry, not enough memory\n");
		return(EXIT_FAILURE);
	}
	
	for(comma_ctr=0, i=0, j=0; datavalue[i]!='\0'; i++) {
		if( datavalue[i]==',' ) comma_ctr++;
		else if( datavalue[i]=='\n' ) {
			(*rowsizesp)[j] = comma_ctr+1;
			comma_ctr = 0;
			j++;
		}
		else;
	}
	
	for(i=0; i<*rownump; i++) {
		(*matrixp)[i] = (double*)malloc( (*rowsizesp)[i] * sizeof(double) );
		if( (*matrixp)[i]==NULL ) {
			strcpy(error_msg, "Sorry, not enough memory\n");
			return(EXIT_FAILURE);
		}
	}
	
	for(i=0, ptr1=datavalue; i<*rownump; i++) {
		for(j=0; j<(*rowsizesp)[i]; j++) {
			//if( sscanf( ptr1, "lf", &((*matrixp)[i][j]) )!=1 ) {
			int r;
			r = sscanf(ptr1, "%lf", &((*matrixp)[i][j]) );
			if( r!=1 ) {
				printf("Scanning data: _%5s_; pos:(%i,%i)\n", ptr1, i, j);
				strcpy(error_msg, "Unable to scan data\n");
				return(EXIT_FAILURE);
			}
			
			if( j!=(*rowsizesp)[i]-1 ) { //if not last element in row
				ptr1 = strchr(ptr1, ',');
				ptr1++;
			}   
		}
		
		if( i != *rownump-1 ) { //if not last row
			ptr1 = strchr(ptr1,'\n');
			ptr1++;
		}
	}
	
	
	return(EXIT_SUCCESS);
	
}