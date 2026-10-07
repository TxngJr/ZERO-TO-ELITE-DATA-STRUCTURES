#include "ml_structures.h"
#include <stdio.h>
int main(void){MlDataset*d=ml_dataset_create(4,3);if(!d)return 1;float row[3]={1,2,3};for(size_t i=0;i<4;++i)if(!ml_dataset_set_row(d,i,row,(int)i))return 2;BatchPlan*p=batch_plan_create(4,2,123);if(!p)return 3;const size_t*idx;size_t count;bool has;if(!batch_plan_next(p,&idx,&count,&has)||!has)return 4;printf("batch_count=%zu first_index=%zu\n",count,idx[0]);batch_plan_free(p);ml_dataset_free(d);return 0;}
