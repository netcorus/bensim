
int getdir ();
void FindFirstFile_dir(char strFile[10]);

#ifdef _WIN32
#else
int file_select(const struct dirent *entry);// select file
#endif

int fcount;//file counter
//char file [20]="def.prg";
char file [20];
FILE * my_file;
char files [15][30];// array files name
int count=0;// count something

int getdir ()
{
	int a=0;
	fcount=0;
	#ifdef  _WIN32
     
    char strPath[10] = "*.*";
    FindFirstFile_dir(strPath);
    for (int x =0 ;x<fcount ;x++ )
        {
           strcpy(menulist[x],files[x]);
        }
    a = menu(7,1,fcount);
    strcpy(file,files[a]);
    
 #else
		//buffern=0;
        count=0;
  struct dirent **eps;
  int n;
  n = scandir ("./", &eps, file_select, alphasort);
  if (n >= 0)
    {
      int cnt;
      for (cnt = 0; cnt < n; ++cnt)
      {
        sprintf (files[cnt] ,"%s",eps[cnt]->d_name);
        //sprintf (buffer[buffern],"%d %s ",buffern,eps[cnt]->d_name);
        //buffern++;
        count++;
        fcount = count ; // fcount is number of menu entry
	  }	
	    for (int x =0 ;x<n ;x++ )
        {
           strcpy(menulist[x],files[x]);
        }
    a = menu(7,1,fcount);
    strcpy(file,files[a]);
    }
  else
  {
	      perror ("Couldn't open the directory");
  }
 #endif   
return 0;
}

#ifdef _WIN32
#else
int file_select(const struct dirent *entry)
{
	
	
	char *ptr;
	
   if ((strcmp(entry->d_name, ".")==0 ||
   (strcmp(entry->d_name, ".."))==0))
   {
	   return (0);
	}
   ptr =strrchr (entry->d_name, '.');
   if ((ptr != NULL)&&
   ((strcmp(ptr,".prg")==0)))
   return (1);
   else
   return 0;
  
   
}
 #endif

void FindFirstFile_dir(char strFile[10])
{
	
	#ifdef _WIN32
    WIN32_FIND_DATAA    find;
    HANDLE    hFile;

    //printf("%s \n",strFile);
    int    nCount = 0;
    char filesl [15][30];// array files name
    char *ptr;
    fcount=0;

    if((hFile=FindFirstFile(strFile,&find)) != INVALID_HANDLE_VALUE)
    {
        //printf("L %d %s \n",nCount,find.cFileName);
        sprintf (filesl[nCount] ,"%s", find.cFileName);
        //printf("S %d %s \n",nCount,filesl[nCount]);
        nCount++;

        while(FindNextFile(hFile,&find))
        {
            //printf("L %d %s \n",nCount,find.cFileName);
            sprintf (filesl[nCount] ,"%s",find.cFileName);
            //printf("S %d %s \n",nCount,filesl[nCount]);
            nCount++;
        }
        FindClose(hFile);        // must close  the handle
        //for (int x=0;x <= nCount-1;x++){printf("filesL %d  %s \n",x,filesl[x]);}
    }
    else {printf("%s: file(s) not found\n",strFile);}

    for (int x=0;x <= nCount;x++){
            ptr =strrchr (filesl[x], '.');
           if ((strcmp(filesl[x], ".")==0 ||
            (strcmp(filesl[x], ".."))==0) ){
       //printf("deleted %d %s \n",x,filesl[x]);
     }
     else {

        if ((ptr != NULL)&&
        ((strcmp(ptr,".prg")==0))){
        sprintf (files[fcount] ,"%s",filesl[x]);
        //printf("loc %d %s \n",fcount,files[fcount]);
        fcount++;
        }
     }

    }
     #else
     //fcount--; //return 0;
     #endif
}

