#include <stdio.h>
#include <stdlib.h>
#include "htslib/sam.h"

#define bam_is_read1(b) (((b)->core.flag&BAM_FREAD1) != 0)
  
int main(int argc,char **argv)
{
    samFile *in = sam_open(argv[1], "r");
    htsFile *outR1 = hts_open(argv[2], "wb");
    htsFile *outR2 = hts_open(argv[3], "wb");
    
    bam_hdr_t *header;
    bam1_t *aln = bam_init1();
    header = sam_hdr_read(in);
    if (sam_hdr_write(outR1, header) < 0) {   //创建bam文件 
        fprintf(stderr, "Error writing output.\n");
        exit(-1);
    }
    if (sam_hdr_write(outR2, header) < 0) {
        fprintf(stderr, "Error writing output.\n");
        exit(-1);
    }
    
    while (sam_read1(in, header, aln) >= 0) {  //按照奇数偶数行将read写入不同的bam文件 
    	uint32_t flag = (aln->core).flag;
    	if (flag%2 != 0){
            int a=sam_write1(outR1, header, aln);
        }
		else{
			int a=sam_write1(outR2, header, aln);
		} 
    }
    sam_close(in);
    sam_close(outR1);
    sam_close(outR2);
}
