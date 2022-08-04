#include <string.h>
#include <stdio.h>
#include <unistd.h>
/* The standard allocator interface from stdlib.h.  These are the
 * functions you must implement, more information on each function is
 * found below. They are declared here in case you want to use one
 * function in the implementation of another. */
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
typedef struct Header{
    size_t size;
    struct Header *next;
}Header;
    
/* When requesting memory from the OS using sbrk(), request it in
 * increments of CHUNK_SIZE. */
#define PACK(size,alloc) ((size)|(alloc))   
#define CHUNK_SIZE (1<<12)


static Header **freep=NULL;




/*
 * This function, defined in bulk.c, allocates a contiguous memory
 * region of at least size bytes.  It MAY NOT BE USED as the allocator
 * for pool-allocated regions.  Memory allocated using bulk_alloc()
 * must be freed by bulk_free().
 *
 * This function will return NULL on failure.
 */
extern void *bulk_alloc(size_t size);

/*
 * This function is also defined in bulk.c, and it frees an allocation
 * created with bulk_alloc().  Note that the pointer passed to this
 * function MUST have been returned by bulk_alloc(), and the size MUST
 * be the same as the size passed to bulk_alloc() when that memory was
 * allocated.  Any other usage is likely to fail, and may crash your
 * program.
 */
extern void bulk_free(void *ptr, size_t size);

/*
 * This function computes the log base 2 of the allocation block size
 * for a given allocation.  To find the allocation block size from the
 * result of this function, use 1 << block_size(x).
 *
 * Note that its results are NOT meaningful for any
 * size > 4088!
 *
 * You do NOT need to understand how this function works.  If you are
 * curious, see the gcc info page and search for __builtin_clz; it
 * basically counts the number of leading binary zeroes in the value
 * passed as its argument.
 */
static inline __attribute__((unused)) int block_index(size_t x) {
    if (x <= 8) {
        return 5;
    } else {
        return 32 - __builtin_clz((unsigned int)x + 7);
    }
}

/*
 * You must implement malloc().  Your implementation of malloc() must be
 * the multi-pool allocator described in the project handout.
 */


void *malloc(size_t size) {
    if(size<=0){
        return NULL;
    }
    static Header *base[13];
    void *rb;
    static Header *retval;
    int  x= block_index(size);
    
    /*fprintf(stderr,"%d",x );*/
    /*int allocsize2 = 1<<block_index(size);*/
    
   if(freep==NULL){ 
        for(int i=0;i<13;i++){
            size_t sz= 4096;
           
            void *sb=(void *)sbrk(CHUNK_SIZE);
            if(i==5){
                while(sz>0){
                    if(sz==32){
                    base[i]=(Header *)sb;
                    base[i]->size=24;
                    base[i]->size=base[i]->size|0x0;
                    sb=sb+32;
                    base[i]->next=NULL;
                    base[i]=base[i]->next;
                    sz=sz-32;
                    }
                    else{
                    
                    base[i]=(Header *)sb;
                    base[i]->size=24;
                    base[i]->size=base[i]->size|0x0;
                    sb=sb+32;
                    base[i]->next=(Header *)sb;
                    base[i]=base[i]->next;
                    sz=sz-32;
                    }
                }
                sb=sb-4096;
                base[i]=(Header *)sb;
            }
             if(i==6){
                while(sz>0){
                    if(sz==64){
                        base[i]=(Header *)sb;
                    base[i]->size=56;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+64;
                    base[i]->next=NULL;
                    base[i]=base[i]->next;
                    sz=sz-64; 
                    }
                    else{
                    base[i]=(Header *)sb;
                    base[i]->size=56;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+64;
                    base[i]->next=(Header *)sb;
                    base[i]=base[i]->next;
                    sz=sz-64;
                    }
                }
                sb=sb-4096;
                base[i]=(Header *)sb;
            }
              if(i==7){
                while(sz>0){
                    if(sz==128){
                       base[i]=(Header *)sb;
                    base[i]->size=120;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+128;
                    base[i]->next=NULL;
                    base[i]=base[i]->next;
                    sz=sz-128; 
                    }
                    else{
                    base[i]=(Header *)sb;
                    base[i]->size=120;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+128;
                    base[i]->next=(Header *)sb;
                    base[i]=base[i]->next;
                    sz=sz-128;
                    }
                }
                sb=sb-4096;
                base[i]=(Header *)sb;
            }
               if(i==8){
                while(sz>0){
                    if(sz==256){
                       base[i]=(Header *)sb;
                    base[i]->size=248;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+256;
                    base[i]->next=NULL;
                    base[i]=base[i]->next;
                    sz=sz-256; 
                    }
                    else{
                    base[i]=(Header *)sb;
                    base[i]->size=248;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+256;
                    base[i]->next=(Header *)sb;
                    base[i]=base[i]->next;
                    sz=sz-256;
                    }
                }
                sb=sb-4096;
                base[i]=(Header *)sb;
            }
                if(i==9){
                while(sz>0){
                    if(sz==512){
                     base[i]=(Header *)sb;
                    base[i]->size=504;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+512;
                    base[i]->next=NULL;
                    base[i]=base[i]->next;
                    sz=sz-512;
                    }else{
                    base[i]=(Header *)sb;
                    base[i]->size=504;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+512;
                    base[i]->next=(Header *)sb;
                    base[i]=base[i]->next;
                    sz=sz-512;
                    }
                }
                sb=sb-4096;
                base[i]=(Header *)sb;
            }
                 if(i==10){
                while(sz>0){
                    if(sz==1024){
                    base[i]=(Header *)sb;
                    base[i]->size=1016;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+1024;
                    base[i]->next=NULL;
                    base[i]=base[i]->next;
                    sz=sz-1024;
                    }
                    else{
                    base[i]=(Header *)sb;
                    base[i]->size=1016;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+1024;
                    base[i]->next=(Header *)sb;
                    base[i]=base[i]->next;
                    sz=sz-1024;
                    }
                }
                sb=sb-4096;
                base[i]=(Header *)sb;
            }
                  if(i==11){
                while(sz>0){
                    if(sz==2048){
                    base[i]=(Header *)sb;
                    base[i]->size=2040;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+2048;
                    base[i]->next=NULL;
                    base[i]=base[i]->next;
                    sz=sz-2048;
                    }
                    else{
                         base[i]=(Header *)sb;
                    base[i]->size=2040;
                     base[i]->size=base[i]->size|0x0;
                    sb=sb+2048;
                    base[i]->next=(Header *)sb;
                    base[i]=base[i]->next;
                    sz=sz-2048;
                    }
                   
                }
                sb=sb-4096;
                base[i]=(Header *)sb;
            }
                   if(i==12){
                while(sz>0){
                    
                     base[i]=(Header *)sb;
                    base[i]->size=4088;
                     base[i]->size=base[i]->size|0x0;
                     sb=sb+4096;
                    base[i]->next=NULL;
                    base[i]=base[i]->next;
                    sz=sz-4096;
                    
                }
                sb=sb-4096;
                base[i]=(Header *)sb;
            }
        }
        freep=base;
    }
   if((x>=5&&x<=12)||size<=4088){
       if(freep[x]==NULL){
           size_t sz= 4096;
           size_t allocsize = 1<<block_index(size);
           void *sb=(void *)sbrk(CHUNK_SIZE);
           /* while(sz>0){
                    
                    freep[x]=(Header *)sb;
                    freep[x]->size=allocsize;
                    freep[x]->size=freep[x]->size|0x0;
                    sb=sb+allocsize;
                    freep[x]->next=(Header *)sb;
                    freep[x]=freep[x]->next;
                    sz=sz-allocsize;
                }
                sb=sb-4096;
                freep[x]=(Header *)sb;*/
            while(sz>0){
                    if(sz==allocsize){
                    freep[x]=(Header *)sb;
                    freep[x]->size=allocsize-8;
                    freep[x]->size=freep[x]->size|0x0;
                    sb=sb+allocsize;
                    freep[x]->next=NULL;
                    freep[x]=freep[x]->next;
                    sz=sz-allocsize;
                    }
                    else{
                    freep[x]=(Header *)sb;
                    freep[x]->size=allocsize-8;
                     freep[x]->size=freep[x]->size|0x0;
                    sb=sb+allocsize;
                    freep[x]->next=(Header *)sb;
                    freep[x]=freep[x]->next;
                    sz=sz-allocsize;
                    }
                   
                }
            sb=sb-4096;
            freep[x]=(Header *)sb;

       }
       rb=freep[x];
       retval=freep[x];
       freep[x]=freep[x]->next;
       retval->size=retval->size;
       
       *(size_t*)rb=retval->size;
       /*int p=retval->size&~0x7;
         fprintf(stderr,"%i",p);*/
       
       rb=rb+sizeof(size_t);
       
   }
   else if(size>4088){
      
       size_t allocsize4= size+8;
       
      rb=bulk_alloc(allocsize4);
      size_t siz= allocsize4-8;
      
      *(size_t*)rb= siz;
      
      rb=rb+sizeof(size_t);
     
       
       
   }
   return rb;

}
/*
 * You must also implement calloc().  It should create allocations
 * compatible with those created by malloc().  In particular, any
 * allocations of a total size <= 4088 bytes must be pool allocated,
 * while larger allocations must use the bulk allocator.
 *
 * calloc() (see man 3 calloc) returns a cleared allocation large enough
 * to hold nmemb elements of size size.  It is cleared by setting every
 * byte of the allocation to 0.  You should use the function memset()
 * for this (see man 3 memset).
 */
void *calloc(size_t nmemb, size_t size) {
    if(nmemb<=0||size<=0){
        return NULL;
    }
    else{
    void *ptr = malloc(nmemb * size);
    memset(ptr, 0, nmemb * size);
    return ptr;

    }
       
    
   
}

/*
 * You must also implement realloc().  It should create allocations
 * compatible with those created by malloc(), honoring the pool
* alocation and bulk allocation rules.  It must move data from the
 * previously-allocated block to the newly-allocated block if it cannot
 * resize the given block directly.  See man 3 realloc for more
 * information on what this means.
 *
 * It is not possible to implement realloc() using bulk_alloc() without
 * additional metadata, so the given code is NOT a working
  * implementation!
 */
void *realloc(void *ptr, size_t size) {
    void *ret;
    if(ptr==NULL){
        ret=  malloc(size);
    }
    else if((ptr!=NULL)&&size==0){
        free(ptr);
        ret= NULL;
    }
    else{
        size_t allocsize;
         ptr=ptr-sizeof(size_t);
        size_t size1=*(size_t*)ptr;
       ptr= ptr+sizeof(size_t);
       if(size>4088){
           allocsize= size;       }
       else{
           allocsize=(1<<block_index(size))-8;
       }
  
       if(allocsize<=size1){
        ret= ptr;
        
       }
       else{
           
        ret=malloc(size);
        memcpy(ret,ptr,size1);
        free(ptr);
        
      }
    }
    
    
    return ret;
    
    
 
}

/*
 * You should implement a free() that can successfully free a region of
 * memory allocated by any of the above allocation routines, whether it
 * is a pool- or bulk-allocated region.
 *
 * The given implementation does nothing.
 * */
void free(void *ptr) {
    if(ptr!=NULL){
       ptr=ptr-sizeof(size_t);
    size_t size=*(size_t*)ptr;
    ptr= ptr+sizeof(size_t);
    
    

    static Header *retval;
    
    if(size<=4088){
        size_t allocsize= 1<<block_index(size);
        int i=block_index(size);
        retval=(Header *)(ptr-sizeof(size_t));
        retval->size=allocsize-8;
        retval->size=retval->size|0x0;
        retval->next=freep[i];
        freep[i]=retval;
    }
    else{
        ptr=ptr -sizeof(size_t);
        bulk_free(ptr,size+8);
        
    } 
    }
   
    
    
    return;
}
