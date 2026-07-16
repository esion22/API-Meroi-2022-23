# API-Meroi-2022-23

## Final Project of Algorithms and Data Structures 2022/2023 (Politecnico di Milano)

***Evaluation:*** 30/30

*Student:* Lorenzo Meroi  
*Professor:* Matteo Pradella  

### Project Challenges & Objectives
This is the final project for the **Algorithms and Data Structures** (*Algoritmi e Strutture Dati*) course at Politecnico di Milano (A.Y. 2022/2023). The objective is to design and implement an efficient C program to determine the optimal route (the route with the minimum number of stops/car rentals) between service stations along a highway.

The project presents several demanding algorithmic and system-level challenges:
- **Strict Time Complexity Constraints**: Performance is key. The maximum score requires solving large test cases containing millions of operations in less than 1 second.
- **Tight Memory Budget**: Memory usage must be minimized (down to 78 MiB). Constructing a dense static graph represents too much overhead; stations must be traversed and evaluated dynamically.
- **Dynamic Operations**: The highway network is not static. Stations and cars of varying ranges are constantly added, demolished, or scrapped at runtime.
- **Custom Tie-Breaking Rule**: If multiple paths exist with the same minimum number of stops, a specific tie-breaking rule must be satisfied. It requires selecting the path that favors stops closest to the start of the highway.
- **Pure C Implementation**: No external libraries are allowed. This mandates custom implementations of self-balancing Red-Black trees (for managing vehicles in station garages in $O(\log N)$ time) and custom Hash Maps (for fast $O(1)$ station insertions and lookups).

---

## Technical Specifications & Performance Constraints

The program must satisfy the following memory limits and CPU execution time targets:

| Evaluation | Memory  | Time  |       Outcome      |
|------------|--------:|------:|:------------------:|
| 18         | 128 MiB | 19 s  | :white_check_mark: |
| 21         | 118 MiB | 15 s  | :white_check_mark: |
| 24         | 108 MiB | 10 s  | :white_check_mark: |
| 27         |  98 MiB |  6 s  | :white_check_mark: |
| 30         |  88 MiB |  4 s  | :white_check_mark: |
| 30L        |  78 MiB |  1 s  |        :x:         |

### Design Choices
- **Highway Map**: Implemented as a custom hash map with linear probing to achieve $O(1)$ average complexity for insertions, searches, and removals.
- **Garage Management**: The fleet of vehicles at each station is managed using a Red-Black Tree. This guarantees $O(\log N)$ time complexity for renting, scrapping, and querying the maximum range.
- **Routing Engine**: Implemented via a Breadth-First Search (BFS) algorithm that dynamically generates the reachable state space on-the-fly rather than allocating a static highway graph.

---

## Assignment Specification Translation (2022_2023.pdf)

Consider a highway described as a sequence of service stations. Each service station is located at a distance from the beginning of the highway, represented by a non-negative integer (in kilometers). No two service stations share the same distance: thus, each service station is uniquely identified by its distance from the beginning of the highway.

Each service station is equipped with a fleet of electric vehicles for rent. Each vehicle is characterized by its range (maximum distance achievable on a full charge), expressed in kilometers as a positive integer. The vehicle fleet of a single station contains at most 512 vehicles. Renting a car from a station $s$, one can reach any station whose distance from $s$ is less than or equal to the car's range.

A journey is defined by a sequence of service stations where the driver makes a stop. It starts at one service station and ends at another, passing through zero or more intermediate stations. It is assumed that the driver cannot go backward during the journey and rents a new car every time they stop at a service station. Therefore, given two consecutive stops $s$ and $t$, $t$ must always be further away from the starting point than $s$, and $t$ must be reachable using one of the vehicles available at $s$.

The objective of the project is: given a pair of stations, plan the route with the fewest number of stops between them. In case there are multiple routes with the same minimum number of stops (i.e., a tie), the route that favors stops closer to the beginning of the highway must be chosen. More formally, consider the set of $n$ tied routes $P = \{p_1, p_2, \ldots, p_n\}$ where each route is a tuple of $m$ elements $p_i = \langle p_{i,1}, p_{i,2}, \ldots, p_{i,m} \rangle$ representing the distance from the beginning of the highway for each stop in chronological order of travel. The unique route $p_i$ must be chosen such that there is no other route $p_j$ with the same final $k$ stops preceded by a stop with a smaller distance, i.e., $\nexists j, k : \langle p_{i,m-k+1}, \ldots, p_{i,m} \rangle = \langle p_{j,m-k+1}, \ldots, p_{j,m} \rangle \land p_{j,m-k} < p_{i,m-k}$.

Below is an example of a highway. In this example, the correct route between the station at distance 20 and the one at distance 50 is $20 \to 30 \to 50$ (and not $20 \to 45 \to 50$). Note that, conversely, $50 \to 30 \to 20$ is the correct route between the station at distance 50 and the one at distance 20 (i.e., in the right-to-left direction).

```
Vehicle range:
  [20] -> 5, 10, 15, 25
  [30] -> 40
  [45] -> 30
  [50] -> 20, 25

Highway Layout:
  Start --- 20 --- 30 --- 45 --- 50
```

### Input File Format and Expected Outputs

The input text file contains a sequence of commands, one per line, in the following format. All positive or zero integer values can be represented in 32 bits.

* **`aggiungi-stazione`** `distance` `num-cars` `car-range-1` ... `car-range-n`
  Adds a station located at the specified distance, equipped with `num-cars` cars having the specified ranges.
  *Example:*
  `aggiungi-stazione 10 3 100 200 300`
  adds a station at distance 10 from the beginning of the highway, with a fleet of three vehicles with ranges of 100, 200, and 300 km respectively. If a station already exists at the specified distance, the command does nothing.
  Expected output: `aggiunta` or `non aggiunta`.

* **`demolisci-stazione`** `distance`
  Removes the station located at the specified distance, if it exists.
  Expected output: `demolita` or `non demolita`.

* **`aggiungi-auto`** `station-distance` `car-range-to-add`
  If the station exists, adds a car to it. It is possible to have multiple cars with the same range.
  Expected output: `aggiunta` or `non aggiunta`.

* **`rottama-auto`** `station-distance` `car-range-to-scrap`
  Removes a car from the specified station, if the station exists and has at least one vehicle with the specified range.
  Expected output: `rottamata` or `non rottamata`.

* **`pianifica-percorso`** `start-station-distance` `destination-station-distance`
  Requests planning the path with the constraints described above.
  Expected output: the stops in chronological order of travel, represented by the distances of the stations from the beginning of the highway, separated by spaces, and followed by a newline at the end. The start and destination must be included; if they coincide, the station is printed only once. If the route does not exist, print `nessun percorso`. The planning action does not alter the stations or their vehicle fleets. The given stations are guaranteed to exist.

---

## Progetto di prova finale di algoritmi e strutture dati 2022/2023 del Politecnico di Milano A.A. 2022/2023.

***Valutazione:*** 30

*Studente:* Lorenzo Meroi

*Docente:* Pradella Matteo


## Obiettivo del progetto

Realizzare un programma in C per la ricerca del percorso ottimo tra stazioni di servizio di un'autostrada.

Il programma non solo deve produrre un output corretto, ma deve rispettare dei vincoli di memoria e tempo CPU come in tabella:

| Valutazione | Memoria | Tempo |       Esito        |
|-------------|--------:|------:|:------------------:|
| 18          | 128 MiB | 19 s  | :white_check_mark: |
| 21          | 118 MiB | 15 s  | :white_check_mark: |
| 24          | 108 MiB | 10 s  | :white_check_mark: |
| 27          |  98 MiB |  6 s  | :white_check_mark: |
| 30          |  88 MiB |  4 s  | :white_check_mark: |
| 30L         |  78 MiB |  1 s  |        :x:         |

## Scelte progettuali

**L'autostrada** è stata implementata come una hashmap per le sua complessità di inserimento, cancellazione e ricerca. La funzione di hashing utilizzata sfrutta l'ispezione lineare.

Ogni stazione dell'autostrada possiede un'autorimessa. **Le autorimesse** sono state implementate come alberi rosso neri vista la loro conveniente complessità temporale nella ricerca, nell'inserimento e nell'agigunta di autovetture.

Infine la funzione che permette di **trovare il percorso migliore** è stata implementata come un BFS che genera sul momento tutte le stazioni raggiungibili da quella considerata. 

Questa scelta è stata fatta in quanto generare l'intera autostrada come un grafo sarebbe stato molto sconveniente dal punto di vista della complessità temporale e soprattutto spaziale.
