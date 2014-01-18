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
#include <ctype.h> // isdigit
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
		    int,int, int
		    );


// TEMP INPUT DATA COPY
int temp_input[MAXL+1];			     // this will be a copy of input, just for display in GUI


attr_t attribs; // only for checking window attributes, never used really (except wattr_get)
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
  
  
  //randomize(input, MAXL); // randomize input array of integers
  
  
  
  
  initscr(); // start graphics here
  refresh();
  curs_set(0); // no visible cursor
  noecho();    // no visible input
  start_color();
  
  
  
  
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_RED);
  init_pair(3, COLOR_BLUE, COLOR_WHITE);
  init_pair(4, COLOR_WHITE, COLOR_YELLOW);
  init_pair(5, COLOR_WHITE, COLOR_BLUE);
  
  get_values(input, MAXL);  // print menu and get input
  wbkgd(stdscr, COLOR_PAIR(5));
  wrefresh(stdscr);
  while (is_sorted == FALSE)
  {
    memcpy(temp_input, input, sizeof(input)); // copy input to temporary input in every iteration
    initialize_windows(inp_windows, temp1_windows, temp2_windows, out_windows);
    print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL, -1, -1, -1,
		    0,0,0
		    );
 
    divide(input,temp1,temp2,MAXL);
     
    
    is_sorted = check_sorted(temp1, temp2);
    
    merge(input, temp1,temp2);
    
    
    
  }
  
  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL, MAXL+2, -1, MAXL+1,
		    0,0,0
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
	print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    input_index, index1, index2-1, -1,
		    0,0,1
		    );
	index1++;
	
      }
      else if (to_temp1 == TRUE) // else if last element was put in temp1
      {
	if (input[input_index] >= temp1[index1-1]) // if input el. is >= last element put in temp1
	{
	  temp1[index1] = input[input_index];
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    input_index, index1, index2-1, -1,
		    0,0,1
		    );
	  index1++;
	  
	}
	else // if input element is < last element in temp1
	{
	  temp1[index1]=EOS;
	  temp2[index2]=input[input_index];
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    input_index, index1, index2, -1,
		    0,0,1
		    );
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
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    input_index, index1-1, index2, -1,
		    0,0,1
		    );
	  index2++;
	  
	}
	else // if input element is < last element in temp2
	{
	  temp2[index2]=EOS;
	  temp1[index1]=input[input_index];
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    input_index, index1, index2, -1,
		    0,1,1
		    );
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
    print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    input_index, index1, index2, -1,
		    0,0,0
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
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL, index1, index2, input_index,
		    1,0,1
		    );
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
	  print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL, index1, index2, input_index,
		    1,0,1
		    );
	  input_index++;
	  
	}
	index1++;
      }
    }
    
    else		// else if none is at EOA then merge series
    {
      
      if (temp1[index1]==EOS && temp2[index2] == EOS ) // if both are at end of series then skip to next elements
      {
	print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL, index1, index2, input_index-1,
		    1,1,1
		    );
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
	      print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL, index1, index2, input_index,
		    1,0,1
		    );
	      input_index++;
	      index2++;
	      
	    }
	  }
	  else if (temp2[index2] == EOS )
	  {
	    
	    while(temp1[index1] != EOS )
	    {
	      
	      input[input_index] = temp1[index1];
	      print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL, index1, index2, input_index,
		    1,0,1
		    );
	      input_index++;
	      index1++;
	      
	    }
	  }
	  else if (temp1[index1] <= temp2[index2])
	  {
	    
	    input[input_index] = temp1[index1];
	    print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL, index1, index2, input_index,
		    1,0,1
		    );
	    input_index++;
	    index1++;
	    
	  }
	  else if (temp2[index2] < temp1[index1])
	  {
	    
	    input[input_index] = temp2[index2];
	    print_windows(inp_windows, temp1_windows, temp2_windows, out_windows,
		    temp_input, temp1, temp2,input,
		    MAXL, index1, index2, input_index,
		    1,0,1
		    );
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

void get_values(int * input, int maxl)
{
  int i = 0;
  char user_input[3]; //value user tries to input
  int inlen;
  echo();
  curs_set(1);
  WINDOW * mainscreen;
  mainscreen = newwin(LINES, COLS, 0,0);
  box(mainscreen, 'X','X');
  wbkgd(mainscreen, COLOR_PAIR(5));
  keypad(mainscreen,TRUE);

  mvwaddstr(mainscreen, 1,1, "SORTOWANIE PRZEZ LACZENIE NATURALNE");
  wrefresh(mainscreen);
  
  wgetch(mainscreen);
  user_input[0]=0;
  
  while (user_input[0] != 'Q' && i<maxl)
  {
    mvwprintw(mainscreen,3,1,"Podaj wartosci do posortowania (1-99):         ");
    mvwprintw(mainscreen,4,1,"Wpisz Q aby przerwac.                              ");
    mvwprintw(mainscreen,5,1,"Wartosc:                                       ");
    mvwprintw(mainscreen,10,1,"                                              ");
    wmove(mainscreen,5, 10);
    wrefresh(mainscreen);
    wgetnstr(mainscreen, user_input,2);
    
    inlen=strlen(user_input);
    
    if( inlen == 0 || (inlen==1  && !isdigit(user_input[0])) || (inlen==2 && (!isdigit(user_input[0]) || !(isdigit(user_input[1])))))
    {
      if (user_input[0]=='Q')
      {
	break;
      }
      mvwprintw(mainscreen,10,1,"Przyjmuje tylko liczby calkowite 0-99.\n");
      wrefresh(mainscreen);
      napms(1000);
      continue;
    }
    else
    {
      input[i] = atoi(user_input);
      
      mvwprintw(mainscreen, 8,3+i*3,"%3d", input[i]);
      
      i++;
      wrefresh(mainscreen);
    }
  }
  input[i] = EOA; // end array with EOA indicator
  
  if(i != maxl)
  {
    mvwprintw(mainscreen, 10, 1, "Przerwano dzialanie programu.                    ");
    wgetch(mainscreen);
    endwin();
    exit(1);
  }
  noecho();
  curs_set(0);
  delwin(mainscreen);
  clear();
  refresh();
}


















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
    wbkgd(input[win_index], COLOR_PAIR(1)); //BLACK AND WHITE AT THE BEGINNING
    wrefresh(input[win_index]);
  }
  
  for (win_index = 0; win_index < MAXL; win_index++)
  {
    temp1[win_index] = newwin(win_size, win_size, 1+win_size, 10+win_size * win_index);
    box(temp1[win_index],0,0);
    wbkgd(temp1[win_index], COLOR_PAIR(1)); //BLACK AND WHITE AT THE BEGINNING
    wrefresh(temp1[win_index]);

  }
  
  for (win_index = 0; win_index < MAXL; win_index++)
  {
    temp2[win_index] = newwin(win_size, win_size, 2+2*win_size, 10+win_size * win_index);
    box(temp2[win_index],0,0);
    wbkgd(temp2[win_index], COLOR_PAIR(1)); //BLACK AND WHITE AT THE BEGINNING
    wrefresh(temp2[win_index]);

    
  }
  
  for (win_index = 0; win_index < MAXL; win_index++)
  {
    output[win_index] = newwin(win_size, win_size, 3+3*win_size, win_size * win_index);
    box(output[win_index],0,0);
    wbkgd(output[win_index], COLOR_PAIR(1)); //BLACK AND WHITE AT THE BEGINNING
    wrefresh(output[win_index]);

  }

}


void print_windows(WINDOW ** in_wind, WINDOW ** temp1_wind, WINDOW ** temp2_wind, WINDOW ** out_wind,
		   int * input, int * temp1, int * temp2, int * output,
		   int in_ind, int temp1_ind, int temp2_ind, int out_ind,
		   int merging, int color_change, int rwmode
		  ) // merging indicates if previous temp1 and temp2 should print to the index or leave following windows intact
{
  
  int win_index;
  int arr_index;
  static int color;
  static int outcolor; // output color
  short setcolor;

  /*
   * 
   * Print input array
   * 
   */
  win_index=0;
  arr_index=0;
  
  
  while(win_index < MAXL)
  {
    wbkgd(in_wind[win_index], COLOR_PAIR(1));
    box(in_wind[win_index], 0,0);
    if ( input[arr_index] == EOA || input[arr_index]==EOS)
    {
      arr_index++;
      continue;
    }
    if (arr_index < in_ind )
    {
      mvwprintw(in_wind[win_index],1,1,"  "); // clear previous number to be sure
      mvwprintw(in_wind[win_index],1,1,"%d", input[arr_index]);
      arr_index++;
      win_index++;
      wrefresh(in_wind[win_index-1]);
	  
    }
    else if (arr_index == in_ind)
    {
      if (merging == 0 && rwmode == 1)
      {
	box(in_wind[win_index], '#','#');
	
      }
      mvwprintw(in_wind[win_index],1,1,"  "); // clear previous number
      mvwprintw(in_wind[win_index],1,1,"%d", input[arr_index]);
      wrefresh(in_wind[win_index]);
      wgetch(in_wind[win_index]);
      arr_index++;
      win_index++;
    }
    else 
    {
      mvwprintw(in_wind[win_index],1,1,"  "); // clear previous number
      mvwprintw(in_wind[win_index],1,1,"%d", input[arr_index]);
      wrefresh(in_wind[win_index]);

      arr_index++;
      win_index++;
    }
  }

  /*
   * 
   * Print temp1
   * 
   */
#if 1
  win_index=0;
  arr_index=0;
  
  wattr_get(temp1_wind[win_index], &attribs, &setcolor, NULL);
  if (setcolor == 1)
  {
    color=2;
  }
  while(win_index < MAXL)
  {
    if (temp1[arr_index] == EOA || temp1[arr_index] == EOS) // skip every EOA or EOS
    {
      arr_index++;
      continue;
    }
    if (arr_index < temp1_ind )
    {
	mvwprintw(temp1_wind[win_index],1,1,"  "); // clear previous number to be sure	
	mvwprintw(temp1_wind[win_index],1,1,"%d", temp1[arr_index]);
	wrefresh(temp1_wind[win_index]);
	arr_index++;
	win_index++;
    }
    else if (arr_index == temp1_ind)   // here's the last element wanted
    {
      if (merging == 0  && rwmode == 1)
	{
	  if (color_change == 1)
	  {
	    
	    color++;
	    if (color > 5 ) 
	    {
	      color=2; // leave black & white, start with pair 2
	    }
	  }
	}
      else if (merging == 1 && rwmode == 1)
      {
	wattr_get(temp1_wind[win_index], &attribs, &setcolor, NULL);
	color=setcolor; // leave the same color when merging
	box(temp1_wind[win_index], '#','#');
      }
      wbkgd(temp1_wind[win_index], COLOR_PAIR(color)); // paint window with currently used color
      mvwprintw(temp1_wind[win_index],1,1,"  "); // clear previous number
      mvwprintw(temp1_wind[win_index],1,1,"%d", temp1[arr_index]);
      wrefresh(temp1_wind[win_index]);
      wgetch(temp1_wind[win_index]);
      box(temp1_wind[win_index], 0,0);
      win_index++;
      arr_index++;
    }
    
    else // elements after last wanted index
    {
      if (merge == 0)
      {
	mvwprintw(temp1_wind[win_index],1,1,"  "); // clear previous number
	wbkgd(temp1_wind[win_index], COLOR_PAIR(1)); // BLACK & WHITE FOR EMPTY WINDOWS
	
      }
      else //merge is on, print window as previously was
      {
	mvwprintw(temp1_wind[win_index],1,1,"  "); // clear previous number
	mvwprintw(temp1_wind[win_index],1,1,"%d", temp1[arr_index]);
	
      }  
      win_index++;
      arr_index++;
    }
    
  }
  
#endif
  
  /*
   * 
   * Print temp2
   * 
   */
  win_index=0;
  arr_index=0;
  
  while(win_index < MAXL)
  {
    box(temp2_wind[win_index],0,0);
    if (temp2[arr_index] == EOA || temp2[arr_index] == EOS) // skip every EOA or EOS
    {
      arr_index++;
      continue;
    }
    if (arr_index < temp2_ind )
    {
	mvwprintw(temp2_wind[win_index],1,1,"  "); // clear previous number to be sure	
	mvwprintw(temp2_wind[win_index],1,1,"%d", temp2[arr_index]);
	box(temp2_wind[win_index],0,0);
	wrefresh(temp2_wind[win_index]);
	arr_index++;
	win_index++;
    }
    else if (arr_index == temp2_ind)   // here's the last element wanted
    {
      if (merging == 0  && rwmode == 1)
	{
	  wbkgd(temp2_wind[win_index], COLOR_PAIR(color)); // paint window with currently used color
	  mvwprintw(temp2_wind[win_index],1,1,"  "); // clear previous number
	  mvwprintw(temp2_wind[win_index],1,1,"%d", temp2[arr_index]);
	  wrefresh(temp2_wind[win_index]);
	  wgetch(temp2_wind[win_index]);
	  box(temp2_wind[win_index],0,0);
	}
      else if (merging == 1 && rwmode == 1)
      {
	wattr_get(temp2_wind[win_index], &attribs, &setcolor, NULL);
	color=setcolor; // leave the same color when merging
	box(temp2_wind[win_index], '#','#');
	 wbkgd(temp2_wind[win_index], COLOR_PAIR(color)); // paint window with currently used color
	  mvwprintw(temp2_wind[win_index],1,1,"  "); // clear previous number
	  mvwprintw(temp2_wind[win_index],1,1,"%d", temp2[arr_index]);
	  wrefresh(temp2_wind[win_index]);
	  wgetch(temp2_wind[win_index]);
	  box(temp2_wind[win_index],0,0);
      }
      win_index++;
      arr_index++;
    }
    
    else // elements after last wanted index
    {
      if (merge == 0)
      {
	mvwprintw(temp2_wind[win_index],1,1,"  "); // clear previous number
	wbkgd(temp2_wind[win_index], COLOR_PAIR(1)); // BLACK & WHITE FOR EMPTY WINDOWS
      }
      else //merge is on, print window as previously was
      {
	mvwprintw(temp2_wind[win_index],1,1,"  "); // clear previous number
	mvwprintw(temp2_wind[win_index],1,1,"%d", temp2[arr_index]);
      }
      win_index++;
      arr_index++;
    }
    
  }
  
  
  /*
   * 
   * Print output
   * 
   */
  win_index=0;
  arr_index=0;
  wattr_get(out_wind[win_index], &attribs, &setcolor, NULL);
  if (setcolor == 1)
  {
    outcolor=2;
  }
  while(win_index < MAXL)
  {
    if (merging == 1 && rwmode == 1 && color_change==1)
    {
      outcolor++;
      if (outcolor > 5 ) 
      {
	outcolor=2; // leave black & white, start with pair 2
      }
      break;
    }
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
    else if (arr_index == out_ind)
    {
      if (merging == 1  && rwmode == 1)
	{
	  if (color_change == 1)
	  {
	    
	    outcolor++;
	    if (outcolor > 5 ) 
	    {
	      outcolor=2; // leave black & white, start with pair 2
	    }
	  }
	}
      mvwprintw(out_wind[win_index],1,1,"  "); // clear previous number
      mvwprintw(out_wind[win_index],1,1,"%d", output[arr_index]);
      wbkgd(out_wind[win_index], COLOR_PAIR(outcolor));
      win_index++;
      arr_index++;
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

