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
