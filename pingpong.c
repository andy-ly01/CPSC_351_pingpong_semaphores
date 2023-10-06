#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

//---------------------------- DO NOT TOUCH THIS CODE (lines 8 to 55) -------
// ABSTRACTION LAYER for differences in semaphores between Linux and macOS
//---------------------------- DO NOT TOUCH THIS CODE (lines 8 to 53) -------
#if defined __APPLE__
  #include <dispatch/dispatch.h>    // macOS ONLY
#endif 

#if defined __APPLE__ 
  #define semaphore_t dispatch_semaphore_t
#else 
  #define semaphore_t sem_t
#endif

semaphore_t semPing, semPong, semPung, semPyung;

semaphore_t semaphore_create(semaphore_t* semaphore, unsigned int level) { 
  #if defined __APPLE__
    return dispatch_semaphore_create((long long)(level));
  #else 
    sem_init(semaphore, 0, level);
    return *semaphore;  
  #endif
}

void semaphore_wait(semaphore_t* semaphore) { 
  #if defined __APPLE__
    dispatch_semaphore_wait(*semaphore, DISPATCH_TIME_FOREVER);
  #else 
    sem_wait(semaphore);
  #endif
}

void semaphore_signal(semaphore_t* semaphore) { 
  #if defined __APPLE__
    dispatch_semaphore_signal(*semaphore);
  #else 
    sem_post(semaphore);
  #endif 
}

void semaphore_destroy(semaphore_t* semaphore) { 
  #if defined __APPLE__ 
    dispatch_release(*semaphore);
  #else 
    sem_destroy(semaphore);
  #endif
}
//---------------------------- DO NOT TOUCH THIS CODE (lines 8 to 55) -------

#define PINGS  5
#define PONGS PINGS 
#define PUNGS PINGS 
#define PYUNGS PINGS 



void* ping(void* x) {       // pinger thread runner (x is not used currently)
  int pings = PINGS;
  while (pings-- > 0) {
    // TODO: call semaphore_wait... done
    semaphore_wait(&semPing);
    printf("ping");
    semaphore_signal(&semPong);
    // TODO: call semaphore_signal... done
  }
  pthread_exit(NULL);
} 

void* pong(void* x) {        // ponger thread runner (x is not used currently)
  int pongs = PONGS * 2;     // ponger thread for task 3
  while (pongs-- > 0) {
    // TODO: call semaphore_wait...
    semaphore_wait(&semPong);
    printf("  pong"); 
    if (pongs % 2 == 0) {
      printf("\n");
      semaphore_signal(&semPing);
     } else {
      semaphore_signal(&semPung);
          // TODO: call semaphore_signal...
     }
  }
  pthread_exit(NULL);
}

void* pongT1(void* x) {        // ponger thread runner (x is not used currently)
  int pongs = PONGS;          // ponger thread for task 1
  while (pongs-- > 0) {
    // TODO: call semaphore_wait...
    semaphore_wait(&semPong);
    printf("  pong\n");
    semaphore_signal(&semPing);
    // TODO: call semaphore_signal...
  }
  pthread_exit(NULL);
}

void* pongT2(void* x) {        // ponger thread runner (x is not used currently)
  int pongs = PONGS;          // ponger thread for task 2
  while (pongs-- > 0) {
    // TODO: call semaphore_wait...
    semaphore_wait(&semPong);
    printf("  pong");
    semaphore_signal(&semPung);
    // TODO: call semaphore_signal...
  }
  pthread_exit(NULL);
}

void* pung(void* x) {        // punger thread runner (x is not used currently)
  int pungs = PUNGS * 2;     // punger thread for task 3
  while (pungs-- > 0) {
     // TODO: call semaphore_wait...
     semaphore_wait(&semPung);
     printf("  pung");
     if (pungs % 2 == 0 ){
      semaphore_signal(&semPong);
     } else{
      semaphore_signal(&semPyung);
      // TODO: call semaphore_signal...
     }
  }
  pthread_exit(NULL);
}

void* pungT2(void* x) {        // ponger thread runner (x is not used currently)
  int pungs = PUNGS;          // ponger thread for task 2
  while (pungs-- > 0) {
    // TODO: call semaphore_wait...
    semaphore_wait(&semPung);
    printf("  pung\n");
    semaphore_signal(&semPing);
    // TODO: call semaphore_signal...
  }
  pthread_exit(NULL);
}

void* pyung(void* x) {        // pyunger thread runner (x is not used currently)
  int pyungs = PYUNGS;
  while (pyungs-- > 0) {
    // TODO: call semaphore_wait...
    semaphore_wait(&semPyung);
    printf("  pyung"); 
    semaphore_signal(&semPung);
    // TODO: call semaphore_signal...
  }
  pthread_exit(NULL);
}

semaphore_t semOne, semTwo, semThree, semFour, semOne2, semTwo2, semThree2, semFour2;

void* one(void* x) {
  int times = PINGS;
  while(times-- > 0) {
    semaphore_wait(&semOne);
    printf("1 ");
    semaphore_signal(&semTwo);
  }
  pthread_exit(NULL);
}

void* two(void* x) {
  int times = PINGS;
  while(times-- > 0) {
    semaphore_wait(&semTwo);
    printf("2 ");
    semaphore_signal(&semThree);
  }
  pthread_exit(NULL);
}

void* three(void* x) {
  int times = PINGS;
  while(times-- > 0) {
    semaphore_wait(&semThree);
    printf("3 ");
    semaphore_signal(&semFour);
  }
  pthread_exit(NULL);
}

void* four(void* x) {
  int times = PINGS;
  while(times-- > 0) {
    semaphore_wait(&semFour);
    printf("4 ");
    semaphore_signal(&semThree2);
    }
  pthread_exit(NULL);
}

void* three2(void* x) {
  int times = PINGS * 3; // Because 3 appears 3 times in the reverse sequence.
  while(times-- > 0) {
    semaphore_wait(&semThree2);
    printf("3 ");
    if (times % 3 == 0) {
      semaphore_signal(&semTwo2);
    } else {
      semaphore_signal(&semFour2);
      }
  }
  pthread_exit(NULL);
}

void* four2(void* x) {
  int times = PINGS * 2;
  while(times-- > 0) {
    semaphore_wait(&semFour2);
    printf("4 ");
    semaphore_signal(&semThree2);
  }
  pthread_exit(NULL);
}

void* two2(void* x) {
  int times = PINGS;
  while(times-- > 0) {
    semaphore_wait(&semTwo2);
    printf("2 ");
    semaphore_signal(&semOne2);
  }
  pthread_exit(NULL);
}

void* one2(void* x) {
  int times = PINGS;
  while(times-- > 0) {
    semaphore_wait(&semOne2);
    printf("1\n");
    semaphore_signal(&semOne);  // restart for the next sequence.
  }
  pthread_exit(NULL);
}

void task1() {
  pthread_t pinger, ponger;
    
  // Initialize semaphores for Task 1
  semPing = semaphore_create(&semPing, 1);
  semPong = semaphore_create(&semPong, 0);
    
  // Create and join threads
  pthread_create(&ponger, NULL, pongT1, NULL);
  pthread_create(&pinger, NULL, ping, NULL);
  pthread_join(pinger, NULL);
  pthread_join(ponger, NULL);
  
  // Destroy semaphores
  semaphore_destroy(&semPing);
  semaphore_destroy(&semPong);
    
  printf("\n");
}

void task2() {
  pthread_t pinger, ponger, punger;
  
  // Initialize semaphores for Task 2
  semPing = semaphore_create(&semPing, 1);
  semPong = semaphore_create(&semPong, 0);
  semPung = semaphore_create(&semPung, 0);
  
  // Create and join threads
  pthread_create(&punger, NULL, pungT2, NULL);
  pthread_create(&ponger, NULL, pongT2, NULL);
  pthread_create(&pinger, NULL, ping, NULL);
  pthread_join(pinger, NULL);
  pthread_join(ponger, NULL);
  pthread_join(punger, NULL);
    
  // Destroy semaphores
  semaphore_destroy(&semPing);
  semaphore_destroy(&semPong);
  semaphore_destroy(&semPung);
    
  printf("\n");
}

void task3() {
  pthread_t pinger, ponger, punger, pyunger;
  
  // Initialize semaphores for Task 3
  semPing = semaphore_create(&semPing, 1);
  semPong = semaphore_create(&semPong, 0);
  semPung = semaphore_create(&semPung, 0);
  semPyung = semaphore_create(&semPyung, 0);

  // Create and join threads
  pthread_create(&pyunger, NULL, pyung, NULL);
  pthread_create(&punger, NULL, pung, NULL);
  pthread_create(&ponger, NULL, pong, NULL);    // can create ponger first -- it will wait
  pthread_create(&pinger, NULL, ping, NULL);
    
  pthread_join(pinger, NULL);
  pthread_join(ponger, NULL);
  pthread_join(punger, NULL);
  pthread_join(pyunger, NULL);

  // Destroy semaphores
  semaphore_destroy(&semPing);
  semaphore_destroy(&semPong);
  semaphore_destroy(&semPung);
  semaphore_destroy(&semPyung);
    
  printf("\n");
}

void task4() {
  pthread_t t_one, t_two, t_three, t_four, t_three2, t_four2, t_two2, t_one2;
    
  // Initialize semaphores for Task 4
  semOne = semaphore_create(&semOne, 1);
  semTwo = semaphore_create(&semTwo, 0);
  semThree = semaphore_create(&semThree, 0);
  semFour = semaphore_create(&semFour, 0);
  semThree2 = semaphore_create(&semThree2, 0);
  semFour2 = semaphore_create(&semFour2, 0);
  semTwo2 = semaphore_create(&semTwo2, 0);
  semOne2 = semaphore_create(&semOne2, 0);
    
  // Create threads
  pthread_create(&t_one, NULL, one, NULL);
  pthread_create(&t_two, NULL, two, NULL);
  pthread_create(&t_three, NULL, three, NULL);
  pthread_create(&t_four, NULL, four, NULL);
  pthread_create(&t_three2, NULL, three2, NULL);
  pthread_create(&t_four2, NULL, four2, NULL);
  pthread_create(&t_two2, NULL, two2, NULL);
  pthread_create(&t_one2, NULL, one2, NULL);
    
  // Join threads
  pthread_join(t_one, NULL);
  pthread_join(t_two, NULL);
  pthread_join(t_three, NULL);
  pthread_join(t_four, NULL);
  pthread_join(t_three2, NULL);
  pthread_join(t_four2, NULL);
  pthread_join(t_two2, NULL);
  pthread_join(t_one2, NULL);

  // Destroy semaphores
  semaphore_destroy(&semOne);
  semaphore_destroy(&semTwo);
  semaphore_destroy(&semThree);
  semaphore_destroy(&semFour);
  semaphore_destroy(&semThree2);
  semaphore_destroy(&semFour2);
  semaphore_destroy(&semTwo2);
  semaphore_destroy(&semOne2);

}


int main(int argc, const char * argv[]) {
  printf("Starting Task 1...\n");
  task1();
    
  printf("Starting Task 2...\n");
  task2();
    
  printf("Starting Task 3...\n");
  task3();

  printf("Starting Task 4...\n");
  task4();
    
  printf("            SUCCESS!\n"); 
  printf("\n\tdone...\n");

  return 0;
}
/*
Starting Task 1...
ping  pong
ping  pong
ping  pong
ping  pong
ping  pong

Starting Task 2...
ping  pong  pung
ping  pong  pung
ping  pong  pung
ping  pong  pung
ping  pong  pung

Starting Task 3...
ping  pong  pung  pyung  pung  pong
ping  pong  pung  pyung  pung  pong
ping  pong  pung  pyung  pung  pong
ping  pong  pung  pyung  pung  pong
ping  pong  pung  pyung  pung  pong

Starting Task 4...
1 2 3 4 3 4 3 4 3 2 1
1 2 3 4 3 4 3 4 3 2 1
1 2 3 4 3 4 3 4 3 2 1
1 2 3 4 3 4 3 4 3 2 1
1 2 3 4 3 4 3 4 3 2 1
            SUCCESS!

        done...
*/