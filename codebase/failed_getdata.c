//REMOVED BECAUSE POST FORM REQUEST FAILED TO WORK

//getdata.c
//defines getdata function
//processes form POST request and cgi data to double data matrix

//returns EXIT_SUCCESS if works,
//else returns EXIT_FAILURE and sets error_msg

//NOTE: error_msg max: 256, assumes it is allocated

#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<errno.h>
//#include<math.h>


int getdata(double*** matrixp, int** rowsizesp, int* rownump, char* error_msg) {
	
	//vars-------------------------------
	char* request_method = getenv("REQUEST_METHOD");
	int content_length, comma_ctr, newline_ctr, i, j, k;
	FILE* tmp1 = tmpfile();
	FILE* tmp2 = tmpfile();
	
	//-----------------------------------
	
	
	//Get CGI data-----------------------
	//read post from stdin and write onto tempfile1 with decoding and counting newlines
	if( strcmp(request_method, "POST")!=0 || strcmp(request_method, "post")!=0 ) {
		strcpy(error_msg, "Incorrect form request");
		return(EXIT_FAILURE);
	}
	
	sscanf(getenv("CONTENT_LENGTH"), "%i", &content_length);
	
	for(i=0; i<content_length; i++) {
		char ch;
		char buffer[9]; //will hold "datakey="
		static int cntl = 0; //will be 1 if currently reading "datakey" value, otherwise 0
		
		ch = fgetc(stdin);
		
		//unencode ch and write it to temp1
		if( ch=='+' ) {
			fputc(' ', tmp1);
		}
		else if( ch=='%' ) {
			if( fscanf(stdin, "%2x", &ch)!=1 ) {
				strcpy(error_msg, "Invalid character in form");
				return(EXIT_FAILURE);
			}
			i += 2;
			fputc(ch, tmp1);
		}
		else {
			fputc(ch, tmp1);
		}
		
		//update buffer
		if( i<8 ) {
			buffer[i] = ch;
			buffer[8]='\0'; //last element of buffer will always be null-term
		}
		else {
			for(j=0; j<7; j++) buffer[j] = buffer[j+1];
			buffer[7] = ch;
		}
		
		//check buffer for "datakey=", update cntl
		if( strcmp(buffer, "datakey=")==0 ) {
			cntl = 1;
		}
		if( cntl==1 && (ch=='&' || ch=='\0' || ch==EOF) ) {
			cntl = 0;
		}
		
		//if cntl is true (reading datakey's value), count newlines
		if( cntl==1 && ch=='\n' ) {
			newline_ctr++;
		}
		
	}
	
	//allocate rowsizes and matrix accordingly
	*rownump = newline_ctr;
	if( (*rowsizesp = (int*)malloc(newline_ctr*sizeof(int)))==NULL ) {
		sprintf("error_msg", "%s%s", "Error allocating memory; ", strerror(errno));
		return(EXIT_FAILURE);
	}
	if( (*matrixp = (double**)malloc(newline_ctr*sizeof(double*)))==NULL ) {
		sprintf("error_msg", "%s%s", "Error allocating memory; ", strerror(errno));
		return(EXIT_FAILURE);
	}
	//------------------------------------
	
	
	//read tmp1 onto tmp2 and count commas, alloc mem-----------------------------
	//first, update content_length and rewind
	content_length = (int)ftell(tmp1);
	fseek(tmp1, 0, SEEK_SET); //could add check
	
	//start reading tmp1 onto tmp2 and count
	//comma_ctr=comma counter, i=content iterator, j=rowsizes iterator
	for(comma_ctr=0, i=0, j=0; i<content_length && j<(*rownump); i++) {

		char ch;
		char buffer[9]; //will hold "datakey="
		static int cntl = 0; //will be 1 if currently reading "datakey" value, otherwise 0
		
		ch = fgetc(tmp1);
		if( ch!=' ' ) {  //rm spaces
			fputc(ch, tmp2);
		}
		
		//update buffer
		if( i<8 ) {
			buffer[i] = ch;
			buffer[8]='\0'; //last element of buffer will always be null-term
		}
		else {
			for(j=0; j<7; j++) buffer[j] = buffer[j+1];
			buffer[7] = ch;
		}
		
		//check buffer for "datakey=", update cntl
		if( strcmp(buffer, "datakey=")==0 ) {
			cntl = 1;
		}
		if( cntl==1 && (ch=='&' || ch=='\0' || ch==EOF) ) {
			cntl = 0;
		}
		
		//if cntl count commas
		if( cntl ) {
			if( ch==',' ) {
				comma_ctr++;
			}
			if( ch=='\n' ) {  //reset comma_ctr
				(*rowsizesp)[j] = comma_ctr;
				comma_ctr = 0;
				j++;
			}
		}
		
	}
	
	//close tmp1, should automatically be removed
	fclose(tmp1);
	
	//alloc matrix
	for(i=0; i<(*rownump); i++) {
		if( ((*matrixp)[i] = (double*)malloc( (*rowsizesp)[i] * sizeof(double) ))==NULL ) {
			sprintf("error_msg", "%s%s", "Error allocating memory; ", strerror(errno));
			return(EXIT_FAILURE);
		}
	}
	
	//---------------------------------------------------------------
	
	
	//now read thru tmp2 and get data--------------------------------
	//rewind tmp2 & update content_length
	content_length = (int)ftell(tmp2);
	fseek(tmp2, 0, SEEK_SET);
	
	//i=file content iterator, k=row/newline iterator, j=column/cell/comma iterator
	for(i=0, j=0, k=0; i<content_length ; i++) {
		
		char ch;
		char buffer[9]; //will hold "datakey="
		static int cntl = 0; //will be 1 if currently reading "datakey" value, otherwise 0
		
		ch = fgetc(tmp2);
		
		//update buffer
		if( i<8 ) {
			buffer[i] = ch;
			buffer[8]='\0'; //last element of buffer will always be null-term
		}
		else {
			for(j=0; j<7; j++) buffer[j] = buffer[j+1];
			buffer[7] = ch;
		}
		
		//check buffer for "datakey=", update cntl
		if( strcmp(buffer, "datakey=")==0 ) {
			cntl = 1;
		}
		if( cntl==1 && (ch=='&' || ch=='\0' || ch==EOF) ) {
			cntl = 0;
		}
		
		//if cntl, scan for data, which should now be in format: ddd,dd.d,-ddd\nd.dd,-d.dd,dd.d\n
		if( cntl ) {
			
			//an unnecessary extra outofbounds check
			if( k > *rownump || j > (*rowsizesp)[k] ) {
				strcpy(error_msg, "Index out of bounds error\n");
				return(EXIT_FAILURE);
			}
			
			//scanning first item
			if( j==0 && k==0 ) {
				fscanf(tmp2, "%lf", &((*matrixp)[k][j]) ); //add check!
				i=ftell(tmp2);
				i--; //for the for-loop inc
				j++;
			}
			
			//scanning every item after comma
			if( ch==',' ) {
				fscanf(tmp2, "%lf", &((*matrixp)[k][j]) ); //add check!
				i=ftell(tmp2);
				i--;
				j++;
			}
			
			//newline, reset cell iterator, inc row iterator
			if( ch=='\n' ) {
				j=0;
				k++;
			}
			
		} //close if cntl
	
	} //close for-loop
	
	//done
	fclose(tmp2);
		
	//----------------------------------------
	
	return(EXIT_SUCCESS);
	
}


