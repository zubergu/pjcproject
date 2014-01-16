/*
 * 
 * THIS WILL SORT ONLY POSITIVE INTEGERS  in range [1:99] DUE TO DEFINITION OF EOS AND EOA BOTH AS NEGATIVE INTEGERS
 * 
 * 
 */

#include <ncurses.h>// for GUI, TRUE, FALSE
#include <stdlib.h> // for rand()
#include <string.h> // for memset() and memcpy
#include <time.h>
#define MAXL 20		// max number of elements to sort
#define EOS -1	// end of series indicator
#define EOA -2 // end of array indicator




/********
 * 
 * 	SORTING RELATED FUNCTIONS
 * 
 ******/
void printarr(int *, int ); // it will just print out given array, 2nd argument it MAXL, but it's not necessary atm
void divide(int *, int *, int *, int); // divides input array to two temp arrays
int check_sorted(int *, int *);	// checks if array is sorted, after dividing into 2 parts, returns TRUE if sorted, FALSE otherwise
void merge(int *, int *, int *);	// merge two temps series by series and save merged series to input 
void randomize(int *, int );		// fill input with random values between 1 and ?
void get_values(int *, int);           // fill input array with user values between 1 and ?


/******
 * 
 * 	DISPLAY RELATED FUNCTIONS
 * 
 *****/
void initialize_windows(WINDOW **, WINDOW **, WINDOW **, WINDOW **);
void print_windows(WINDOW **, WINDOW **, WINDOW **, WINDOW **,
		    int *, int *, int *, int *,
		    int, int, int, int,
		    int
		    );


// TEMP INPUT DATA COPY
int temp_input[MAXL+1];			     // this will be a copy of input, just for display in GUI

// ARRAYS OF WINDOWS
  WINDOW * inp_windows[MAXL];
  WINDOW * temp1_windows[MAXL];
  WINDOW * temp2_windows[MAXL];
  WINDOW * out_windows[MAXL];
/********
 * 
 * MAIN PROGRAM
 * 
 * *******/

int main(void)
{
  int input[MAXL+1]; //= {19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, EOA}; // input array, simulates input file

  int temp1[MAXL+2];			      // temp array, simulates temporary file, +2 for last # and @
  int temp2[MAXL+2];			      // same as above
  int is_sorted = FALSE; // TRUE if input is sorted, FALSE otherwise
  
  
  
  

  
  
  srand(time(NULL)); // seed for random 
  
  memset(temp1,'\0', sizeof(int)*(MAXL+1)); // this will zero all bytes in an array
  memset(temp2,'\0', sizeof(int)*(MAXL+1)); // same as above
  
  
  randomize(input, MAXL); // randomize input array of integers
  //get_values(input, MAXL);  // get user input
  
  
  
  initscr(); // start graphics here
  refresh();
  curs_set(0); // no visible cursor
  noecho();    // no visible input
  
  start_color();
  
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_WHITE, COLOR_RED);
  init_pair(3, COLOR_BLUE, COLOR_WHITE);
  init_pair(4, COLOR_WHITE, COLOR_YELLOW);
  
  
  initialize_windows(inp_windows, temp1_windows, temp2_windows, out_windows);
  
  while (is_sorted == FALSE)
  {
    memcpy(temp_input, input, sizeof(input)); // copy input to temporary input in every iteration
    print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, 0, 0, 0,
		    0
		    );
 
    divide(input,temp1,temp2,MAXL);
     
    
    is_sorted = check_sorted(temp1, temp2);
    
    merge(input, temp1,temp2);
    
    
    
  }
  
  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, MAXL+2, 1, MAXL+1,
		    0
		    );
  endwin();
  return 0;
}





void randomize(int * input, int maxl)
{
  int i = 0;
  
  for (i=0; i<maxl; i++)
  {
    input[i] = rand()%99 +1;
  }
  input[i] =EOA; // put EOA at the end of array 
}


void get_values(int * input, int maxl)
{
  int i = 0;
  int user_input; //value user tries to input
  
  while (i<maxl)
  {
    printf("Podaj wartosc do posortowania: ");
    if(scanf("%d", &user_input) != 1)
    {
      scanf("%*s"); // clear input buffer if something went wrong
      printf("Nie rozpoznalem liczby, sprobuj jeszcze raz.\n");
      continue;
      
    }
    if (user_input < 0 )
    {
      printf("Przyjmuje tylko liczby calkowite wieksze od 0.\n");
      continue;
    }
    input[i] = user_input;
    i++;
  }
  input[i] = EOA; // end array with EOA indicator
}




void printarr(int * arr, int maxl)
{
  int i=0;
  do
  {
    if (arr[i] == EOS)
      printf(" # ");
    else if (arr[i] == EOA)
      printf(" @ ");
    else
      printf(" %d ", arr[i]);
  }
  while ( arr[i++] != EOA);
  putchar('\n');
}

void divide(int * input, int * temp1, int * temp2, int maxl)
{
    int index1;	// index for temp1,indicating where next element will be written
    int index2;	// index for temp2, same as above
    int input_index;	// index for reading from input array
    int to_temp1=TRUE; // TRUE if last element went to temp1, FALSE otherwise
    
    
    index1=0;	// we start writing to temps, starting from 0
    index2=0;
    to_temp1 = TRUE;  // and first element will go to temp1
    for(input_index=0;input_index<MAXL;input_index++)
    {
      if (input_index == 0) // if we read first element
      {
	temp1[index1]=input[input_index];
	index1++;
	print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, 0,
		    0
		    );
      }
      else if (to_temp1 == TRUE) // else if last element was put in temp1
      {
	if (input[input_index] >= temp1[index1-1]) // if input el. is >= last element put in temp1
	{
	  temp1[index1] = input[input_index];
	  index1++;
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, 0,
		    0
		    );
	}
	else // if input element is < last element in temp1
	{
	  temp1[index1]=EOS;
	  temp2[index2]=input[input_index];
	  index1++;
	  index2++;
	  to_temp1=FALSE;
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, 0,
		    0
		    );
	}
      }
      else if(to_temp1 == FALSE)
      {
	if (input[input_index] >= temp2[index2-1]) // if input el. is >= last element put in temp2
	{
	  temp2[index2] = input[input_index];
	  index2++;
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, 0,
		    0
		    );
	}
	else // if input element is < last element in temp2
	{
	  temp2[index2]=EOS;
	  temp1[index1]=input[input_index];
	  index1++;
	  index2++;
	  to_temp1=TRUE;
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, 0,
		    0
		    );
	}
      }
    }
    // after reading all input elements put END of SERIES in last array and  END of ARRAY in temp1 and temp2 
    if (to_temp1 == TRUE)
    {
      temp1[index1] = EOS;
      index1++;
      print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, 0,
		    0
		    );
    }
    else
    {
      temp2[index2] = EOS;
      index2++;
      print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, 0,
		    0
		    );
    }
    temp1[index1] = EOA;
    temp2[index2] = EOA;
    print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, 0,
		    0
		    );
}

int check_sorted(int * temp1, int * temp2)
{
  return (temp1[0] == EOA || temp2[0] == EOA) ? TRUE:FALSE;
}

void merge(int * input, int * temp1, int * temp2)
{
  int index1 = 0;
  int index2 = 0;
  int input_index = 0;
  
  while(temp1[index1]!= EOA || temp2[index2] != EOA) // if temp1 or temp2 has any elements left
  {
    if (temp1[index1] == EOA) // if temp1 is empty copy all elements of temp2 to input
    {
      
      while(temp2[index2]!=EOA)
      {
	
	if(temp2[index2] != EOS)
	{
	  input[input_index] = temp2[index2];
	  input_index++;
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, input_index,
		    1
		    );
	}
	index2++;
      }
    }
    else if (temp2[index2] == EOA) //else if temp2 is empty copy all elements of temp1 to input
    {
      
      while(temp1[index1]!=EOA)
      {
	
	if(temp1[index1] != EOS)
	{
	  input[input_index] = temp1[index1];
	  input_index++;
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, input_index,
		    1
		    );
	}
	index1++;
      }
    }
    
    else		// else if none is at EOA then merge series
    {
      
      if (temp1[index1]==EOS && temp2[index2] == EOS ) // if both are at end of series then skip to next elements
      {
	
	index1++;
	index2++;
      }
      else
      {
	
	while(temp1[index1]!= EOS || temp2[index2] != EOS) // until both temp1 and temp2 elements are EOS
	{
	  
	  if (temp1[index1] == EOS)
	  {
	    
	    while(temp2[index2] != EOS)
	    {
	      
	      input[input_index] = temp2[index2];
	      input_index++;
	      index2++;
	      print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, input_index,
		    1
		    );
	    }
	  }
	  else if (temp2[index2] == EOS )
	  {
	    
	    while(temp1[index1] != EOS )
	    {
	      
	      input[input_index] = temp1[index1];
	      input_index++;
	      index1++;
	      print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, input_index,
		    1
		    );
	    }
	  }
	  else if (temp1[index1] <= temp2[index2])
	  {
	    
	    input[input_index] = temp1[index1];
	    input_index++;
	    index1++;
	    print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, input_index,
		    1
		    );
	  }
	  else if (temp2[index2] < temp1[index1])
	  {
	    
	    input[input_index] = temp2[index2];
	    input_index++;
	    index2++;
	    print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL+1, index1, index2, input_index,
		    1
		    );
	  }
	}
      }
    }
  }
}


/*
 * 
 * GRAPHICS
 * 
 */
void initialize_windows(WINDOW ** input, WINDOW ** temp1, WINDOW ** temp2, WINDOW ** output)
{
  int x,y;
  int win_size=4;
  int win_index=0;
  // input windows
  
  // calculate win_size here later
  for (win_index=0; win_index<MAXL; win_index++)
  {
    input[win_index] = newwin(win_size, win_size, 0 , win_size * win_index);
    box(input[win_index],0,0);
  }
  
  for (win_index = 0; win_index < MAXL; win_index++)
  {
    temp1[win_index] = newwin(win_size, win_size, 1+win_size, 10+win_size * win_index);
    box(temp1[win_index],0,0);
  }
  
  for (win_index = 0; win_index < MAXL; win_index++)
  {
    temp2[win_index] = newwin(win_size, win_size, 2+2*win_size, 10+win_size * win_index);
    box(temp2[win_index],0,0);
  }
  
  for (win_index = 0; win_index < MAXL; win_index++)
  {
    output[win_index] = newwin(win_size, win_size, 3+3*win_size, win_size * win_index);
    box(output[win_index],0,0);
  }
}


void print_windows(WINDOW ** in_wind, WINDOW ** temp1_wind, WINDOW ** temp2_wind, WINDOW ** out_wind,
		   int * input, int * temp1, int * temp2, int * output,
		   int in_ind, int temp1_ind, int temp2_ind, int out_ind,
		   int merging
		  ) // merging indicates if previous temp1 and temp2 should print to the index or leave following windows intact
{
  
  int win_index;
  int arr_index;
  /*
   * 
   * Print input array
   * 
   */
  win_index=0;
  arr_index=0;
  
  while(win_index < MAXL)
  {
    if (arr_index < in_ind )
    {
      if (input[arr_index] != EOA  && input[arr_index]!= EOS)
      {
	if (!merging)
	{
	  mvwprintw(in_wind[win_index],1,1,"  "); // clear previous number to be sure
	}
	mvwprintw(in_wind[win_index],1,1,"%d", input[arr_index]);
	arr_index++;
	win_index++;
	
      }
      else
      {
	arr_index++;
      }
    }
    else
    {
      if (!merging)
      {
	mvwprintw(in_wind[win_index],1,1,"  "); // clear previous number
      }
      win_index++;
    }
    wrefresh(in_wind[win_index-1]);
  }

  /*
   * 
   * Print temp1
   * 
   */
  win_index=0;
  arr_index=0;
  
  while(win_index < MAXL)
  {
    if (arr_index < temp1_ind )
    {
      if (temp1[arr_index] != EOA  && temp1[arr_index]!= EOS)
      {
	if (!merging)
	{
	  mvwprintw(temp1_wind[win_index],1,1,"  "); // clear previous number
	}
	
	mvwprintw(temp1_wind[win_index],1,1,"%d", temp1[arr_index]);
	
	arr_index++;
	win_index++;
	
      }
      else
      {
	arr_index++;
      }
    }
    else
    {
      if (!merging)
	{
	  mvwprintw(temp1_wind[win_index],1,1,"  "); // clear previous number
	}
      win_index++;
    }
    wrefresh(temp1_wind[win_index-1]);
  }
  
  
  
  /*
   * 
   * Print temp2
   * 
   */
  win_index=0;
  arr_index=0;
  
  while(win_index < MAXL)
  {
    if (arr_index < temp2_ind )
    {
      if (temp2[arr_index] != EOA  && temp2[arr_index]!= EOS)
      {
	if (!merging)
	{
	  mvwprintw(temp2_wind[win_index],1,1,"  "); // clear previous number
	}
	mvwprintw(temp2_wind[win_index],1,1,"%d", temp2[arr_index]);
	arr_index++;
	win_index++;
	
      }
      else
      {
	arr_index++;
      }
    }
    else
    {
      if (!merging)
	{
	mvwprintw(temp2_wind[win_index],1,1,"  "); // clear previous number
	}
      win_index++;
    }
    wrefresh(temp2_wind[win_index-1]);
  }
  
  
  /*
   * 
   * Print output
   * 
   */
  win_index=0;
  arr_index=0;
  
  while(win_index < MAXL)
  {
    if (arr_index < out_ind )
    {
      if (output[arr_index] != EOA  && output[arr_index]!= EOS)
      {
	mvwprintw(out_wind[win_index],1,1,"  "); // clear previous number
	mvwprintw(out_wind[win_index],1,1,"%d", output[arr_index]);
	arr_index++;
	win_index++;
	
      }
      else
      {
	arr_index++;
      }
    }
    else
    {
      mvwprintw(out_wind[win_index],1,1,"  "); // clear previous number
      win_index++;
    }
    wrefresh(out_wind[win_index-1]);
  }
  getch();
}

