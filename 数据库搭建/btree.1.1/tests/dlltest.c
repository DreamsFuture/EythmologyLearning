#include <stdio.h>
#include "dll.h"

main()
{
	int i, where, swtch,x, rtnstat;
	char * buff;
	DLL 	testdll;

trace();
	testdll = DLL_Create();

trace();
	if(testdll == NULL){
		fprintf(stderr, "failed to create a DLL\n");
		exit(1);
	}
trace();
	where = DLLADD;
	for(i=0;i<120;i++){
		buff = (char *)malloc(80);
		sprintf(buff, "this is test number %d",i);	
		if( !DLL_Insert( testdll, where, buff) ){
			fprintf(stderr,"Failed to insert %d\n",i);
			exit(1);
		}
		if(i>110)
			where = DLLINSERT;
	}
trace();
	fprintf(stderr,"FIRST: %s\n",(char *)DLL_First(testdll));
	for(i=0;i<119;i++){
		fprintf(stderr,"NEXT: %s\n",(char *)DLL_Next(testdll));
	}
trace();
	while( ( buff = (char *)DLL_Prev(testdll) ) != NULL){
		fprintf(stderr,"count: %d CURRENT: %s\n",DLL_Count(testdll), DLL_Current(testdll) );
		fprintf(stderr,"PREV: %s\n",buff);
	}

	swtch = 0;
	for(i=0;i<119;i++){
		if(strncmp("this is test number 4", (char *)DLL_Next(testdll), 21 ) == 0 ){
			fprintf(stderr,"found %s\n", (char *)DLL_Current(testdll) );
			swtch = 1;
		}
		if(swtch){
			DLL_Free(testdll);
			x = DLL_Delete(testdll);
			fprintf(stderr,"Delete = %d\n",x);
		}
	}
	where = DLLADD;
	for(i=0;i<12;i++){
		buff = (char *)malloc(80);
		sprintf(buff, "this is test number add after del %d",i);	
		if( !DLL_Insert( testdll, where, buff) ){
			fprintf(stderr,"Failed to insert %d\n",i);
			exit(1);
		}
	}
	fprintf(stderr,"FIRST: %s\n",(char *)DLL_First(testdll));
	for(i=0;i<119;i++){
		buff = (char *)DLL_Next(testdll);
		if (buff == NULL)
			break;
		fprintf(stderr,"NEXT: %s\n",buff);
	}
	fprintf(stderr,"FIRST: %s\n",(char *)DLL_First(testdll));
	while( ( buff = (char *)DLL_Next(testdll)) != NULL){
		fprintf(stderr,":NEXT: %s\n",buff);
	}
	fprintf(stderr,"DELETE ALL: FIRST: %s\n",(char *)DLL_First(testdll));
	for(i=0;i<119;i++){
		DLL_Free(testdll);
		rtnstat = DLL_Delete(testdll);
		if (rtnstat == DLLFAIL)
			break;
		fprintf(stderr,"GONE\n");
	}
	where = DLLADD;
	for(i=0;i<120;i++){
		buff = (char *)malloc(80);
		sprintf(buff, "this is test number %d",i);	
		if( !DLL_Insert( testdll, where, buff) ){
			fprintf(stderr,"Failed to insert %d\n",i);
			exit(1);
		}
	}
trace();
	fprintf(stderr,"FIRST: %s\n",(char *)DLL_First(testdll));
	for(i=0;i<119;i++){
		fprintf(stderr,"NEXT: %s\n",(char *)DLL_Next(testdll));
	}
	
}
