# Dining Philosophers Problem

This project simulates the Dining Philosophers problem, a classic synchronization problem. The goal is to demonstrate the use of threads and mutexes to manage shared resources and prevent deadlock.

## Table of Contents

- [Dining Philosophers Problem](#dining-philosophers-problem)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Building the Project](#building-the-project)
    - [Running the Simulation](#running-the-simulation)
  - [Code Structure](#code-structure)
  - [Philosophers' Actions](#philosophers-actions)
  - [Monitor](#monitor)
  - [Output](#output)
  - [Contributing](#contributing)
  - [License](#license)

## Overview

The Dining Philosophers problem involves a certain number of philosophers sitting at a table. Each philosopher alternates between thinking, eating, and sleeping. Philosophers need two forks to eat, and there are only as many forks as philosophers, leading to the potential for deadlock if not properly managed.

## Getting Started

### Prerequisites

- GCC or any compatible C compiler
- POSIX Threads library (pthread)

### Building the Project

To build the project, run the following command in your terminal:

```sh
make

./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [optional: number_of_times_each_philosopher_must_eat]

./philo 5 800 200 200
