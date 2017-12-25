#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

/* (c) Larry Herman, 2017.  You are allowed to use this code yourself, but
 * not to provide it to anyone else.
 */

sem_t mutex;
int i, n;
FILE *fp;

static void *count_file(void *args);
/*static void free_string_list(char **strings);*/

static void *count_file(void *args) {

  int *count = malloc(sizeof(int));
  *count = 0;
  sem_wait(&mutex);
  
  fp= fopen(args, "r");

  /* silently ignore nonexistent files, or ones that there was an error
     trying to open */
  if (fp != NULL) {

    fscanf(fp, "%d", &n);

    while (!feof(fp)) {
      (*count) += n;
      fscanf(fp, "%d", &n);
    }

  }
  sem_post(&mutex);
  return count; 
}

int main(int argc, char *argv[]) {
  pthread_t *tids = malloc((argc - 1) * sizeof(pthread_t));
  void **ptrs = malloc((argc - 1) * sizeof(char *));
  /* char **ids = malloc((argc - 1) * sizeof(char *)); */
  int sum = 0;

  if (argc > 1) {

    sem_init(&mutex, 0, 1);
    
    for (i= 0; i < argc - 1; i++) {
      if (argc > 1) {
	/* ids[i] = malloc(strlen(argv[i + 1]) + 1); */
	/* strcpy(ids[i], argv[i + 1]); */
	pthread_create(&tids[i], NULL, count_file, argv[i + 1]);
	/* free(ids[i]); */
      }
    }

    for (i= 0; i < argc - 1; i++)
      pthread_join(tids[i], &ptrs[i]);

    for (i = 0; i < argc - 1; i++){
      sum += *((int *)ptrs[i]);
      free(ptrs[i]);
    }

    free(ptrs);
    /* free(ids); */
    free(tids);
    tids = NULL;
    
    printf("%d\n", sum);

    
  }
  return 0;
}

    #if 0
static void free_string_list(char **strings){
  char **temp = strings;

  /* while the pointer to the strings isnt NULL frees all the string pointers */
  while (*strings != NULL) {
    free(*strings);
    *strings = NULL;
    strings++;
  }
  
  free(temp);
  temp = NULL;
}

 #endif

