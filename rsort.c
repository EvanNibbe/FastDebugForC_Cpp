#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Performs radix sort on an arbitrary list
//Evan Nibbe
//Sept 22nd, 2021
//I am releasing this code under the GNU open source license, so any edit you make to this code must be published under the same share and share alike license.
struct list_item {
	void *data;
	long c; //the thing used to run radix sort on integer equivalents
};


#define MODULO 16
void rsort(void *list, size_t item_size, long getInt(void *thing), int n) {
	char *l=list;
	typedef struct list_item item; //prevents the namespace from getting weird.
	item *array=malloc(n*sizeof(item)*2 + n*item_size);
	item *sec=array+n; //secondary array.
	char *transfers=sec+n;
	int count[MODULO]; //the count array which will be initialized to 0 each time
	//set up initial array of items using getInt function
	for (int i=0; i<n; i++) {
		array[i].data=l + item_size*i;
		array[i].c=getInt((void*)(l+ item_size*i));
	}
	int power=0;
	int num_g_mod=1; //There is a number greater than MODULO^power
	while (num_g_mod) {
		//place 0s into count array to set indicies
		for (int i=0; i<MODULO; i++) {
			count[i]=0;
		}
		count[0]=-1;
		long divide_by=MODULO<<4*power;
		for (int i=0; i<n; i++) {
			count[(array[i].c/divide_by)%MODULO]++;
		}	
		//do the count array rolling sum
		int sum=0;
		for (int i=0; i<MODULO; i++) {
			int prev=count[i];
			count[i]+=sum;
			sum+=prev;
		}
		//place the values into sec at count indicies (sub 1 per thing at an index)
		for (int i=0; i<n; i++) {
			int ci=(array[n-1-i].c/divide_by)%MODULO;
			int index=count[ci];
			count[ci]--;
			sec[index].data=array[n-1-i].data;
			sec[index].c=array[n-1-i].c;
			
		}
		
		power++;
		//do it again starting from sec and putting in array
		//place 0s into count array to set indicies
		for (int i=0; i<MODULO; i++) {
			count[i]=0;
		}
		count[0]=-1;
		num_g_mod=0;
		divide_by=MODULO<<4*power;
		for (int i=0; i<n; i++) {
			num_g_mod+=(sec[i].c>divide_by);
			count[(sec[i].c/divide_by)%MODULO]++;
		}	
		//do the count array rolling sum
		sum=0;
		for (int i=0; i<MODULO; i++) {
			int prev=count[i];
			count[i]+=sum;
			sum+=prev;
		}
		//place the values into sec at count indicies (sub 1 per thing at an index)
		for (int i=0; i<n; i++) {
			int ci=(sec[n-1-i].c/divide_by)%MODULO;
			int index=count[ci];
			count[ci]--;
			array[index].data=sec[n-1-i].data;
			array[index].c=sec[n-1-i].c;
			
		}
		
		power++;
	}

	//final step: place items from array back into l in the new order
	for (int i=0; i<n; i++) {
		memcpy(transfers+item_size*i, array[i].data, item_size);
		//char *temp=array[i].data;
		//for (int j=0; j<item_size; j++) {
		//	l[item_size*i+j]=temp[j];
		//}
	}
	memcpy(l, transfers, n*item_size);
	free(array);
	//Now all items are sorted according to the number generated with getInt.
}
