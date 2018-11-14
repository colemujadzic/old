#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <pthread.h>
using namespace std;
#define BUFFERSIZE 100
#define MAXCHILDREN 64
#define PTHREAD_COND_INITIALIZER {_PTHREAD_COND_SIG_init, {0}}

static int max_weight;
static int current_weight;

static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

struct Vehicle
{
	char vehicle_id[20];
	int arrival_delay;
	int vehicle_weight;
	int crossing_time;
};

void enterBridge(int vehicle_weight)
{
  //pthread_mutex_lock(&mutex1);
  // checks to see if bridge's current load is greater than bridge's max load
  while (current_weight + vehicle_weight > max_weight)
  {
    // wait
    pthread_cond_wait(&condition, &mutex1);
  }
  // updates the current weight of the bridge to include the new car's weight
  current_weight = current_weight + vehicle_weight;
  //pthread_mutex_unlock(&mutex1);

  //pthread_mutex_lock(&mutex1);
  // signals to other threads that they can enter the while loop again
  pthread_cond_signal(&condition);
  //pthread_mutex_unlock(&mutex1);
}

void leaveBridge(int vehicle_weight)
{
  //pthread_mutex_lock(&mutex1);
  pthread_cond_signal(&condition);
  current_weight = current_weight - vehicle_weight;
  //pthread_mutex_unlock(&mutex1);
}

// declares a child function
void *vehicle(void *arg)
{
  Vehicle *current = static_cast<Vehicle*>(arg);

  cout << "Vehicle " << current->vehicle_id << " arrives at bridge." << endl;
  cout << "The current bridge load is " << current_weight << " tons." << endl;

  // if vehicle is too heavy for the bridge terminate its thread
  // pthread_mutex_lock(&mutex1);
  if(current->vehicle_weight > max_weight)
  {
    cout << "Vehicle " << current->vehicle_id << " exceeds maximum bridge load." << endl;
    pthread_exit(NULL);
  }
  // pthread_mutex_unlock(&mutex1);

  // checks to see if vehicle can enter the bridge, waits until it can
  pthread_mutex_lock(&mutex1);
  enterBridge(current->vehicle_weight);
  cout << "Vehicle " << current->vehicle_id << " goes on bridge." << endl;
  cout << "The current bridge load is " << current_weight << " tons." << endl;
  pthread_mutex_unlock(&mutex1);

  // sleeps for the time it takes a certain vehicle to cross the bridge
  sleep(current->crossing_time);

  // after vehicle's crossing time expires, vehicle leaves bridge and the current bridge weight is updated
  pthread_mutex_lock(&mutex1);
  leaveBridge(current->vehicle_weight);
  cout << "Vehicle " << current->vehicle_id << " leaves the bridge." << endl;
  cout << "The current bridge load is " << current_weight << " tons." << endl;
  pthread_mutex_unlock(&mutex1);

  // terminates thread
  pthread_exit(NULL);
}

int main(int argc, char **argv)
{
  // converts string value of argument into an integer
  max_weight = atoi(argv[1]);

  // used to check to see if command line argument is being read in correctly
  cout << "Maximum bridge load is " << max_weight << " tons." << endl;

  // declares varaibles to read text file entries into
  int thread_number;
  int error_check;
  char vehicle_id[20];
  int arrival_delay, vehicle_weight, crossing_time;

  // creates a buffer to read the text file into
  char buffer[BUFFERSIZE];

  // declares a thread ID
  vector<pthread_t> threads(MAXCHILDREN);

  while (fgets(buffer, BUFFERSIZE, stdin) != NULL)
  {
    //current = malloc(sizeof(*current));
    Vehicle* current = new Vehicle;

    // reads a line of input
    sscanf(buffer, "%s   %d   %d   %d", vehicle_id, &arrival_delay, &vehicle_weight, &crossing_time);

    // assign the values
    strcpy(current->vehicle_id, vehicle_id);
    current->arrival_delay = arrival_delay;
    current->vehicle_weight = vehicle_weight;
    current->crossing_time = crossing_time;

    // sleep for arrival_delay seconds
    sleep(current->arrival_delay);

    // creates a child thread
    error_check = pthread_create(&threads[thread_number], NULL, vehicle, (void *)current);

    // checks to see if thread failed to be created
    if (error_check) {
      cout << "Error: unable to create thread," << error_check << endl;
      exit(-1);
    }
    // counts the number of threads being created
    thread_number++;
  }

  // waits until all threads terminate
  for (int i = 0; i < MAXCHILDREN; i++)
  {
    pthread_join(threads[i], NULL);
  }

  // prints the total number of vehicles that traversed the bridge
  cout << "" << endl;
  cout << "Total number of vehicles: " << thread_number - 1 << endl;
  pthread_exit(NULL);
  return 0;
}
