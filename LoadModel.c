#include "LoadModel.h"

static char* readline(FILE* f);


void LoadModel(char * filename, float ** vertices_return, int * num_v_return, 
								float ** uv_return, int * num_uv_return,
								float ** normals_return, int * num_norm_return,
								int ** faces_return, int * num_f_return)
{
	FILE * file = fopen(filename, "r");
	if(file == NULL)
	{
		printf("Couldn't read file\n");
		exit(1);
	}

	float * vertices = NULL;
	float * uv = NULL;
	float * normals = NULL;
	int * faces = NULL;
	int num_vertices = 0;
	int num_faces = 0;
	int num_uv = 0;
	int num_normal = 0;

	char * line;
	//printf("Reading data\n");

	while((line = readline(file)))
	{
		//printf("Reading line\n");
		// vertex
		if (line[0]=='v' && line[1]==' ')
		{
			readcoord(line, 3, &vertices, num_vertices*3, 3*num_vertices);
			num_vertices++;
		}
		// normal
		else if (line[0]=='v' && line[1] == 'n')
		{
			readcoord(line, 3, &normals, num_normal*3, 3*num_normal);
			num_normal++;
		}
		// texture coordinates
		else if (line[0]=='v' && line[1] == 't')
		{
			readcoord(line, 2, &uv, num_uv*2, 2*num_uv);
			num_uv++;
		}
		// face - only accepts triangles
		else if (line[0]=='f')
		{
			readIntCoord(line, 9, &faces, num_faces*9, 9*num_faces);
			num_faces++;
		}
	}

	fclose(file);

	*vertices_return = vertices;
	*num_v_return = num_vertices;
	printf("Vertices: %d\n", num_vertices);

	*uv_return = uv;
	*num_uv_return = num_uv;
	printf("UVs: %d\n",num_uv);

	*normals_return = normals;
	*num_norm_return = num_normal;
	printf("Normals: %d\n",num_normal);

	*faces_return = faces;
	*num_f_return = num_faces;
	printf("Faces: %d\n",num_faces);

	printf("Finished loading model: %s\n", filename);
}


//
//  Return true if CR or LF
//
static int CRLF(char ch)
{
   return ch == '\r' || ch == '\n';
}


static int linelen=0;    //  Length of line
static char* line=NULL;  //  Internal storage for line
static char* readline(FILE* f)
{
   char ch;  //  Character read
   int k=0;  //  Character count
   while ((ch = fgetc(f)) != EOF)
   {
      //  Allocate more memory for long strings
      if (k>=linelen)
      {
         linelen += 8192;
         line = (char*)realloc(line,linelen);
         if (!line) {
         	printf("Error allocating data\n");
         	exit(1);//Fatal("Out of memory in readline\n");
         }
      }
      //  End of Line
      if (CRLF(ch))
      {
         // Eat extra CR or LF characters (if any)
         while ((ch = fgetc(f)) != EOF)
           if (!CRLF(ch)) break;
         //  Stick back the overrun
         if (ch != EOF) ungetc(ch,f);
         //  Bail
         break;
      }
      //  Pad character to line
      else
         line[k++] = ch;
   }
   //  Terminate line if anything was read
   if (k>0) line[k] = 0;
   //  Return pointer to line or NULL on EOF
   return k>0 ? line : NULL;
}


void readcoord(char * line, int num_coord, float ** array, int num_in_array, int size)
{
	if(num_in_array + num_coord > size)
	{
		*array = (float *)realloc(*array,(size + num_coord)*sizeof(float));
	}
	readfloat(line, num_coord, *array + num_in_array);
	//printf("Read %d coordinates", num_coord);
}

void readIntCoord(char * line, int num_coord, int ** array, int num_in_array, int size)
{
	if(num_in_array + num_coord > size)
	{
		*array = (int *)realloc(*array,(size + num_coord)*sizeof(int));
	}
	readInts(line, num_coord, *array + num_in_array);
}

void readfloat(char * line, int n, float * loc)
{
	char delim[] = "\n";
	normalizeDelimiter(line);
	//printf("%s\n",line);
	char * word = strtok(line, delim);
	for(int i = 0; i < n; i++)
	{
		word = strtok(NULL, delim);
		//printf("%s:",word);
		if(sscanf(word, "%f", loc+i) != 1)
			{
				printf("Error reading float: %.f\n",*(loc+i));
				exit(1); // MAKE SURE THIS IS CORRECT
			}
	}
}

void readInts(char * line, int n, int * loc)
{
	char delim[] = "\n";
	normalizeDelimiter(line);
	char * word = strtok(line, delim);
	for(int i = 0; i < n; i++)
	{
		word = strtok(NULL, delim);
		if(sscanf(word, "%d", loc+i) != 1)
			{
				printf("Error reading int\n");
				exit(1); // MAKE SURE THIS IS CORRECT
			}
	}
}

void printFloatArray(float * f,int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		printf("%.3f, ", *(f+i));
	}
	printf("\n");
}

void printIntArray(int * f,int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		printf("%d, ", *(f+i));
	}
	printf("\n");
}

void normalizeDelimiter(char * line)
{
	int i;
	for(i = 0; line[i] != '\0'; i++)
	{
		if(line[i] == '/' || line[i] == ' ' || line[i] == '\n')
			line[i] = '\n';
	}
}
