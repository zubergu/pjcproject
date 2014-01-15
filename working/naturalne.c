/*
 * 
 * THIS WILL SORT ONLY POSITIVE INTEGERS DUE TO DEFINITION OF EOS AND EOA BOTH AS NEGATIVE INTEGERS
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

void print_screen(int *input, int *temp_input, int *temp1, int *temp2, int fill_output); /* this function 
								    will fill windows with current values in arrays,
								    fill_output will indicate if output windows
								    should be empty(before merge) or filled
								    (after merge)
								    */
void fill_array(WINDOW ** arr_of_windows, int * arr_of_ints); /*
								this function will fill windows with elements
								of arrays of ints
								*/
								
void initialize_windows(int maxl);

WINDOW * input_windows[MAXL]; 	// each array will hold windows* for elements of arrays
WINDOW * tempinput_windows[MAXL]; // I made them global so I don't have to pass them to print_screen every time
WINDOW * temp1_windows[MAXL];
WINDOW * temp2_windows[MAXL];


int main(void)
{
  int input[MAXL+1]; //= {19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, EOA}; // input array, simulates input file
  //int input[MAXL+1] = {1,2,3,5,7,8,11,13,6,9,EOA}; // input array, simulates input file

  int temp1[MAXL+2];			      // temp array, simulates temporary file, +2 for last # and @
  int temp2[MAXL+2];			      // same as above
  int temp_input[MAXL+1];			     // this will be a copy of input, just for display in GUI
  int is_sorted = FALSE; // TRUE if input is sorted, FALSE otherwise
  
  
  srand(time(NULL)); // seed for random 
  
  memset(temp1,'\0', sizeof(int)*(MAXL+1)); // this will zero all bytes in an array
  memset(temp2,'\0', sizeof(int)*(MAXL+1)); // same as above
  
  
  //randomize(input, MAXL); // randomize input array of integers
  get_values(input, MAXL);  // get user input
  /*
  initscr();
  refresh();
  initialize_windows(MAXL);
  getchar();
  */
  while (is_sorted == FALSE)
  {
    
    
    //print_screen(input, temp_input, temp1, temp2,1);
    //getchar();
    
    divide(input,temp1,temp2,MAXL);
    
    //print_screen(input, temp_input, temp1, temp2,1);
    //getchar();
    is_sorted = check_sorted(temp1, temp2);
    
    merge(input, temp1,temp2);
    //print_screen(input, temp_input, temp1, temp2,1);
    //getchar();
    
    
  }
  /*
  refresh();
  getchar();
  endwin();
  */
  printarr(input, MAXL);
  return 0;
}





void randomize(int * input, int maxl)
{
  int i = 0;
  
  for (i=0; i<maxl; i++)
  {
    input[i] = rand()%100 +1;
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
      printf("Nie rozpoznalem liczby, sproboj jeszcze raz.\n");
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
      }
      else if (to_temp1 == TRUE) // else if last element was put in temp1
      {
	if (input[input_index] >= temp1[index1-1]) // if input el. is >= last element put in temp1
	{
	  temp1[index1] = input[input_index];
	  index1++;
	}
	else // if input element is < last element in temp1
	{
	  temp1[index1]=EOS;
	  temp2[index2]=input[input_index];
	  index1++;
	  index2++;
	  to_temp1=FALSE;
	}
      }
      else if(to_temp1 == FALSE)
      {
	if (input[input_index] >= temp2[index2-1]) // if input el. is >= last element put in temp2
	{
	  temp2[index2] = input[input_index];
	  index2++;
	}
	else // if input element is < last element in temp2
	{
	  temp2[index2]=EOS;
	  temp1[index1]=input[input_index];
	  index1++;
	  index2++;
	  to_temp1=TRUE;
	}
      }
    }
    // after reading all input elements put END of SERIES in last array and  END of ARRAY in temp1 and temp2 
    if (to_temp1 == TRUE)
    {
      temp1[index1] = EOS;
      index1++;
    }
    else
    {
      temp2[index2] = EOS;
      index2++;
    }
    temp1[index1] = EOA;
    temp2[index2] = EOA;
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
	    }
	  }
	  else if (temp2[index2] == EOS )
	  {
	    
	    while(temp1[index1] != EOS )
	    {
	      
	      input[input_index] = temp1[index1];
	      input_index++;
	      index1++;
	    }
	  }
	  else if (temp1[index1] <= temp2[index2])
	  {
	    
	    input[input_index] = temp1[index1];
	    input_index++;
	    index1++;
	  }
	  else if (temp2[index2] < temp1[index1])
	  {
	    
	    input[input_index] = temp2[index2];
	    input_index++;
	    index2++;
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
void initialize_windows( int maxl)
{
  int i = 0;
  // probably here the window size should be calculated
  
  for (i=0; i < maxl; i++)
  {
    tempinput_windows[i] = newwin(5,5,1, i*5 + 10);
    box(tempinput_windows[i], 0,0);
    wrefresh(tempinput_windows[i]);
  }
  for (i=0; i < maxl; i++)
  {
    temp1_windows[i] = newwin(5,5,6, i*5);
    box(temp1_windows[i], 0,0);
    wrefresh(temp1_windows[i]);
  }
  for (i=0; i < maxl; i++)
  {
    temp2_windows[i] = newwin(5,5,11, i*5);
    box(temp2_windows[i], 0,0);
    wrefresh(temp2_windows[i]);
  }
  for (i=0; i < maxl; i++)
  {
    input_windows[i] = newwin(5,5,16, i*5 + 10);
    box(input_windows[i], 0,0);
    wrefresh(input_windows[i]);
  }
}


void fill_array(WINDOW ** arr_of_windows, int * arr_of_ints)
{
  int i = 0;
  int win_index = 0;
  
  while(arr_of_ints[i] != EOA)
  {
    if (arr_of_ints[i] != EOS)
    {
      mvwprintw(arr_of_windows[win_index],1,1, "%d", arr_of_ints[i]);
      wrefresh(arr_of_windows[win_index]);
      win_index++;
    }
    i++;
  }
}
      
    
  
void print_screen(int * input, int * temp_input, int * temp1, int * temp2, int fill_output)
{
  fill_array(tempinput_windows, temp_input);
  fill_array(input_windows, input);
  fill_array(temp1_windows, temp1);
  fill_array(temp2_windows, temp2);
  refresh();
}