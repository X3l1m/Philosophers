# Philosophers

## Description
Philosophers is a concurrency and synchronization project that simulates the classic **Dining Philosophers Problem** using multithreading. The goal is to manage resource sharing (forks) between multiple philosophers without causing deadlocks or race conditions.

## Features
- Uses **threads** to simulate philosophers.
- Implements **mutexes** to manage fork access.
- Handles synchronization to prevent deadlocks and starvation.
- Configurable simulation parameters.

## Usage
### Compilation
To compile the program, run:
```bash
make
```
This will generate an executable named `philo`.

### Running the Simulation
Run the program with the following arguments:
```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```
Example:
```bash
./philo 5 800 200 200
```
- `number_of_philosophers`: The number of philosophers at the table.
- `time_to_die`: Time (in ms) before a philosopher dies if they don't eat.
- `time_to_eat`: Time (in ms) a philosopher spends eating.
- `time_to_sleep`: Time (in ms) a philosopher spends sleeping.
- `[number_of_times_each_philosopher_must_eat]` (optional): If provided, the simulation ends when all philosophers have eaten this many times.

## Implementation Details
- Each philosopher is represented as a separate **thread**.
- **Mutex locks** are used to control fork access.
- The simulation avoids **deadlocks** and **race conditions**.

