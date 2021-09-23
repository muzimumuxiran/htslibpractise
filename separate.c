#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "htslib/sam.h"

#define bam_is_read1(b) (((b)->core.flag&BAM_FREAD1) != 0)

typedef struct Link{//存储出现过的qname
	char *qname;
	int tie;
	struct Link *next;
}Link,*LinkList;

int main(int argc,char **argv){
	samFile *in = sam_open(argv[1], "r");
	bam_hdr_t *header;
	bam1_t *aln = bam_init1();
	header = sam_hdr_read(in);	
	htsFile *outR1 = hts_open(argv[2], "wb");
    htsFile *outR2 = hts_open(argv[3], "wb");
    if (sam_hdr_write(outR1, header) < 0) {
        fprintf(stderr, "Error writing output.\n");
        exit(-1);
    }
    if (sam_hdr_write(outR2, header) < 0) {
        fprintf(stderr, "Error writing output.\n");
        exit(-1);
    }	
	Link *head = (Link *)malloc(sizeof(Link));	
    head->next = NULL; 
    Link *temp = head;
    Link *e = head;
    char *name;  
    srand((unsigned int)time(NULL));//time函数返回的时间戳作为srand随机数的生成器
    while(sam_read1(in, header, aln) >= 0){
    	
    	Link *t = head;
    	int sign = 0;
		name = bam_get_qname(aln);
    	while(t->next != NULL){//查询是否已经录入相同qname的read 
    		if(strcmp(name,t->next->qname) == 0){
    			if(t->next->tie == 1){
    				int a=sam_write1(outR1, header, aln);
    				sign = 1;
					break;	
				}else{
					int a=sam_write1(outR2, header, aln);
					sign = 1;
					break;
				}
			}
			else{
	    		t = t->next;
			} 	
		}
		if(sign == 0){//将新出现的qname录入 
			Link *b = (Link *)malloc(sizeof(Link));	
			b->qname =(char *)malloc(sizeof(char)*1000);
	    	b->qname = strcpy(b->qname,bam_get_qname(aln));		 	
			int i = rand()%10; 
    	    if(i%2!=0){
    	    	int a=sam_write1(outR1, header, aln);
				b->tie = 1;	
			}
		    else{
		    	int a=sam_write1(outR2, header, aln);
		    	b->tie = 0;
	    	}
	    	b->next = NULL;
	    	temp->next = b;
	    	temp = temp->next;	
		}
	}
	
	sam_close(in);
	sam_close(outR1);
	sam_close(outR2);
	
}
