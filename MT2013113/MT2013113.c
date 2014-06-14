#include<stdio.h>
#include <stdlib.h>
#define MAX 1000
#define size 1000
#define inf 9999

/* Structure for maintaining information about passenger request */

/* passenger request information */

struct passenger   
{
int request_id;
int source;
int destination;
int waiting_start_time;
int waiting_end_time;
};

typedef struct passenger passenger;		//Structure instantiation

/* Structure for maintaining taxi locations */

struct taxi_locations
{
int id;
int location;
};

typedef struct taxi_locations taxi_locations;
taxi_locations taxi_available_location[MAX];

/* structure for taxi information */

struct taxi_
{
int id;
int time;
int location;
};

/* structure for maintaining the taxi position at the time of leaving passenger to the destination */

struct sort				
{
int short_dist;
int id;
};

typedef struct sort sort;
sort change_pos[MAX];
typedef struct taxi_ taxi_;

/* Structure for calculating revenue and maintaining the record of passenger in a particular cab */

struct output
{
int taxi_id;
int capacity;
int request;
int revenue;
int request_served[1000];
int t;
};

typedef struct output output;
output out_taxi[MAX];

passenger passeng[MAX];
taxi_ taxi[MAX];

/* keeping track of information about the passenger source and destination information carried out by the particular taxi */

struct taxi_da			
{
int source;
int destination;
int taxi_id;
int capacity;
};

typedef struct taxi_da taxi_da;
taxi_da taxi_data[100][100];

/* Function Prototypes */

void shortest_path_calculate(int v);
int min_(int a,int b);
void floyd(int v);
int city_locations[MAX],shortest_path_city_location[MAX][MAX];
void sort_request(int n);
void sort_taxi(int n);
void update_info_taxi(int j,int request);

/* Main function starts */

int main( int argc, char *argv[])
{
if(argc!=2)						/* Check for the correct number of arguments in command line */
{
printf("Enter correct number of arguments");
}
else
{
FILE *fp;
int capacity_of_vehicle;				
int shortest_path_city_location_copy[MAX][MAX];
int v,i,z,k,j,pass_no;
int min_loc,current_loc,min=0,flag;
int a[10],b[10];
int input_size[4];
int x,y,c;
int no_of_taxi;		
fp = fopen(argv[1], "r");   
if(fp == NULL)
	{
	    puts("Cannot open file for reading");
	    exit(EXIT_FAILURE);
	  }

for(x=0;x<4;x++)
{
fscanf(fp,"%d",&c);
input_size[x]=c;
}

/* Storing the input from the file in the variables */

v=input_size[0];						/* no of request */						
no_of_taxi=input_size[1];						/*total number of taxi*/
capacity_of_vehicle=input_size[2];				/* total passenger cab can accomodate at one time */
pass_no=input_size[3];

/* Input the locations */

for(x = 0; x < v; ++x ) 
{
	for(y = 0; y < v; ++y ) 
	{
		fscanf(fp, "%d", &c );
		shortest_path_city_location[x][y]=c;
	}
}

/* Initialize output taxi */

for(i=0;i<no_of_taxi;i++)
{
out_taxi[i].capacity=0;
out_taxi[i].taxi_id=i;
out_taxi[i].request=-1;
out_taxi[i].revenue=0;
out_taxi[i].t=0;
}

/* Input the locations of taxi */

for(x=0;x<no_of_taxi;x++)	
{
fscanf(fp,"%d",&c);
taxi[x].location=c-1;
}
for(i=0;i<no_of_taxi;i++)
{
taxi[i].id=i;
}

/* Enter details of the passenger */

for(x=0;x<pass_no;x++)				
{
fscanf(fp,"%d",&c);
passeng[x].source=c-1;
fscanf(fp,"%d",&c);
passeng[x].destination=c-1;
fscanf(fp,"%d",&c);
passeng[x].waiting_start_time=c;
fscanf(fp,"%d",&c);
passeng[x].waiting_end_time=c;
passeng[x].request_id=x;
}

fclose(fp);		/* End the file operation */

/* Calling Shortest path funtion to calculate shortest path in between lall the locations */

shortest_path_calculate(v);		

printf("\n\n");

/* sorting request on the basis of start time */

sort_request(pass_no);							


/*...........................Storing shortest path locations in another array...............................*/

for(i=0;i<v;i++)
{
	for(j=0;j<v;j++)
	{
		shortest_path_city_location_copy[i][j]=shortest_path_city_location[i][j];
	}
}

/* Request  entertainment */

/* take request identify the nearest taxi allocate request to the taxi */

int t=0,p,q,r,no_of_request_proc=0;
flag=0,y=0;
int u=0;
/* For each Request check for cab which satisfy all the time constraints*/
int interm[1000];
for(i=0;i<pass_no;i++)			
	{
		for(z=0;z<v;z++)
		{	
			if(i==interm[z])
			{
			break;
			flag=1;
			}
		}
if(flag==1)
{
continue;
}	t=0;
		for(k=0;k<no_of_taxi;k++)
		{
			
			taxi_available_location[t].location=shortest_path_city_location[taxi[k].location][passeng[i].source];
			taxi_available_location[t].id=taxi[k].id;
			t++;			
		}

		sort_taxi(no_of_taxi);			//identify the nearest location to allocate taxi to the request

		for(j=0;j<no_of_taxi;j++)			
		{
			if((passeng[i].waiting_end_time>=taxi[taxi_available_location[j].id].time+taxi_available_location[j].location*2))/* check if the passenger end time constraint satisfied (Taxi reached untill the time passenger was standing at a particular location) */
			{
				if(out_taxi[taxi_available_location[j].id].capacity<capacity_of_vehicle)   /* Check for capacity constraint of cab */
				{
					for(z=0;z<out_taxi[taxi_available_location[j].id].capacity;z++)
					{
						if(taxi_data[taxi_available_location[j].id][z].destination==passeng[i].source )
						{
							t=taxi_data[taxi_available_location[j].id][z].destination;
							taxi_data[taxi_available_location[j].id][z].destination=taxi_data[taxi_available_location[j].id][out_taxi[taxi_available_location[j].id].capacity-1].destination;
							taxi_data[taxi_available_location[j].id][out_taxi[taxi_available_location[j].id].capacity-1].destination=t;
							out_taxi[taxi_available_location[j].id].capacity--;
						}
					}
					/*update taxi and output information*/
				
					no_of_request_proc++;
					update_info_taxi(j,i);		/*sending the information about the current taxi and current request*/	

					break;
				}								/*for capacity satisfaction*/
										
				else						   		/* if capacity exceeds beyond 5 */
				{
					/* Keep on moving passenger to the destination untill the cab is empty */


					if(out_taxi[taxi_available_location[j].id].capacity==capacity_of_vehicle)
					{
				
			/* Storing up information about the source and destination of the passneger in intermediate structure for further processing */
						for(k=out_taxi[taxi_available_location[j].id].capacity;k>=0;k--) 
						{
							for(z=0;z<k;z++)
							{
								change_pos[z].short_dist=shortest_path_city_location[taxi[taxi_available_location[j].id].location][taxi_data[taxi_available_location[j].id][z].destination];
								change_pos[z].id=z;
							}

			/* Sort the passenger in increasing order shortest distance of there destination from taxi current location */
							
							for(x=0;x<k;x++)
							{
								for(y=0;y<k-x-1;y++)
								{
									if(change_pos[y].short_dist>change_pos[y+1].short_dist)
									{
										p=change_pos[y].short_dist;
										change_pos[y].short_dist=change_pos[y+1].short_dist;
										change_pos[y+1].short_dist=p;
										p=change_pos[y].id;
										change_pos[y].id=change_pos[y+1].id;
										change_pos[y+1].id=p;
									}						
								}
							}

			/* Update current location of the taxi capacity of the taxi */
	
							taxi[taxi_available_location[j].id].time+=change_pos[0].short_dist*2;
							taxi[taxi_available_location[j].id].location=taxi_data[taxi_available_location[j].id][change_pos[0].id].destination;
							out_taxi[taxi_available_location[j].id].capacity--;
							t=taxi_data[taxi_available_location[j].id][change_pos[k].id].destination;
							taxi_data[taxi_available_location[j].id][change_pos[k].id].destination=taxi_data[taxi_available_location[j].id][change_pos[0].id].destination;
							taxi_data[taxi_available_location[j].id][change_pos[0].id].destination=t;
							
			/* Break the loop as the cab is empty */								
							for(u=i+1;u<pass_no;u++)
							{
								for(z=0;z<v;z++)
		{	
			if(i==interm[z])
			{
			break;
			flag=1;
			}
		}
if(flag==1)
{
continue;
}
								if(passeng[u].waiting_start_time<=taxi[taxi_available_location[j].id].time && passeng[u].waiting_end_time>=taxi[taxi_available_location[j].id].time)
								{
									/* update taxi, output structure, update the output with the request corresponding to a particular taxi*/ 

/* update revenue */
interm[v++]=u;
out_taxi[taxi_available_location[j].id].revenue+=shortest_path_city_location[passeng[u].source][passeng[u].destination];

/* request id processed strored within the taxi*/

out_taxi[taxi_available_location[j].id].request_served[out_taxi[taxi_available_location[j].id].t++]=passeng[u].request_id;

/* update time */

taxi[taxi_available_location[j].id].time=taxi[taxi_available_location[j].id].time;

/* update location to the source location of the request been served */

taxi[taxi_available_location[j].id].location=passeng[u].source;

/* store the source location of the current request */

taxi_data[taxi_available_location[j].id][out_taxi[taxi_available_location[j].id].capacity].source=passeng[u].source;

/* store the destination location of the current request */

taxi_data[taxi_available_location[j].id][out_taxi[taxi_available_location[j].id].capacity].destination=passeng[u].destination;

/* update the current capacity of the cab */

out_taxi[taxi_available_location[j].id].capacity++;

/* check if the cab has reached before the starting time of the request to be served */

no_of_request_proc++;
break;
								}
							}
							if(out_taxi[taxi_available_location[j].id].capacity==0)
							{
								break;	
							}
						}								
					}					//  capacity wala else
				}
			}						// for time constraint else condition
		}							// for taxi loop 						
	}					// for request loop
t=0;

for(i=0;i<no_of_taxi;i++)
{
	printf("Taxi %d : %d\n",i+1,out_taxi[i].revenue);
	t=t+out_taxi[i].revenue;
}

printf("\n");
printf("Total Revenue : %d\n",t);
printf("\n");


for(i=0;i<no_of_taxi;i++)
{
printf("taxi %d : ",i+1);
	for(j=0;j<out_taxi[i].t;j++)
	{
		printf("%d ",out_taxi[i].request_served[j]);
	}
printf("\n\n");
}

}							/*Else condition for the file argument*/

}

void shortest_path_calculate(int v)
{
int i,j;
floyd(v);
}

void floyd(int v)							/* Floyd warshall for the shortest path calculation */
{
int k, i,j,d[100][100];
k=0;
for(i=0;i<v;i++)
{
	for(j=0;j<v;j++)
	{
  		if(shortest_path_city_location[i][j]==-1)
		{
			shortest_path_city_location[i][j]=1000;
		}
	}
}

for(i=0;i<v;i++)
{
	for(j=0;j<v;j++)
	{
  	d[i][j]=shortest_path_city_location[i][j];
	}
}

for(k=0;k<v;k++)
{
	for(i=0;i<v;i++)
    	{
      		for(j=0;j<v;j++)
		{
       			d[i][j]=min_(d[i][j], d[i][k]+d[k][j]);
		}
      	}
}
	for(i=0;i<v;i++)
	{
		for(j=0;j<v;j++)
		{
			shortest_path_city_location[i][j]=d[i][j];
		}
	}
}

/* Used for minimum calculation done by floyd warshall algo */

int min_(int a,int b)
{
	if(a>b)
	{
		return b;
	}
	else
	{
		return a;
	}
}

/* Sort the request in ascending order of there waiting start time */

void sort_request(int n)
{
int i,j,t;
for(i=0;i<n;i++)
	{
	for(j=0;j<n-i-1;j++)
		{
		if(passeng[j].waiting_start_time>passeng[j+1].waiting_start_time)
			{
			t=passeng[j].source;
			passeng[j].source=passeng[j+1].source;
			passeng[j+1].source=t;
			t=passeng[j].destination;
			passeng[j].destination=passeng[j+1].destination;
			passeng[j+1].destination=t;
			t=passeng[j].waiting_start_time;
			passeng[j].waiting_start_time=passeng[j+1].waiting_start_time;
			passeng[j+1].waiting_start_time=t;
			t=passeng[j].waiting_end_time;
			passeng[j].waiting_end_time=passeng[j+1].waiting_end_time;
			passeng[j+1].waiting_end_time=t;
			t=passeng[j].request_id;
			passeng[j].request_id=passeng[j+1].request_id;
			passeng[j+1].request_id=t;
									
			}
		}
	}
}

/* Sort the taxi in order of there current location considering 0th location as the smallest and 99th as the largest location */

void sort_taxi(int n)
{
int i,j,p,t=0,k=0;
for(p=0;p<n;p++)
	{
	for(j=p+1;j<n;j++)
		{
		if(taxi_available_location[p].location>taxi_available_location[j].location)
			{
			t=taxi_available_location[p].location;
			taxi_available_location[p].location=taxi_available_location[j].location;
			taxi_available_location[j].location=t;
			k=taxi_available_location[p].id;
			taxi_available_location[p].id=taxi_available_location[j].id;
			taxi_available_location[j].id=k;						
			}
		}
	}
}

/* This function keeps on updating the information of the passenger with the cab it has used for travelling */

void update_info_taxi(int j,int request)
{

/* update taxi, output structure, update the output with the request corresponding to a particular taxi*/ 

/* update revenue */

out_taxi[taxi_available_location[j].id].revenue+=shortest_path_city_location[passeng[request].source][passeng[request].destination];

/* request id processed strored within the taxi*/

out_taxi[taxi_available_location[j].id].request_served[out_taxi[taxi_available_location[j].id].t++]=passeng[request].request_id;

/* update time */

taxi[taxi_available_location[j].id].time=taxi_available_location[j].location*2+taxi[taxi_available_location[j].id].time;

/* update location to the source location of the request been served */

taxi[taxi_available_location[j].id].location=passeng[request].source;

/* store the source location of the current request */

taxi_data[taxi_available_location[j].id][out_taxi[taxi_available_location[j].id].capacity].source=passeng[request].source;

/* store the destination location of the current request */

taxi_data[taxi_available_location[j].id][out_taxi[taxi_available_location[j].id].capacity].destination=passeng[request].destination;

/* update the current capacity of the cab */

out_taxi[taxi_available_location[j].id].capacity++;

/* check if the cab has reached before the starting time of the request to be served */

if(passeng[request].waiting_start_time>taxi[taxi_available_location[j].id].time)
{

/* Update its time to the waiting start time of the  request */

	taxi[taxi_available_location[j].id].time=passeng[request].waiting_start_time;
}

}

/*............................................................End of project.............................................................*/


