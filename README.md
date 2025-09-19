# Informe del Proyecto: Simulador MLFQ en C++

## Introducción
En el marco del curso de **Sistemas Operativos**, se desarrolló un simulador del algoritmo **Multilevel Feedback Queue (MLFQ)** utilizando C++.  
El objetivo principal es comprender cómo funciona la planificación multinivel con realimentación, y cómo varía el rendimiento dependiendo de los algoritmos asignados a cada cola.

---

## Descripción del Algoritmo
El algoritmo **MLFQ** utiliza varias colas de prioridad, cada una con un algoritmo de planificación específico.  
Las reglas principales son:
- Siempre se ejecuta la cola de mayor prioridad que tenga procesos listos.
- Si un proceso no termina dentro de su quantum, se transfiere a la cola de menor prioridad.
- Los algoritmos posibles son:
  - **R**: Round Robin (RR)
  - **F**: First Come First Served (FCFS)
  - **S**: Shortest Job First (SJF, no expropiativo)
  - **C**: Shortest Remaining Time (STCF, expropiativo)

---

## Implementación en C++

### 1. Clase `proc`
Representa un proceso con sus atributos principales:
- `id`: Identificador del proceso.
- `burst_time`: Tiempo total de ráfaga.
- `arrival_time`: Tiempo de llegada.
- `queue`: Cola inicial a la que pertenece.
- `priority`: Prioridad asignada.
- Métricas calculadas: `CT`, `TAT`, `WT`, `RT`.

### 2. Clase `queue`
Representa cada cola de planificación.  
- Almacena los procesos en un vector.  
- Aplica el algoritmo de planificación correspondiente.  
- Métodos principales: `push`, `pop`, y control del quantum en caso de RR.

### 3. Simulación (`main`)
- Se inicializa el sistema cargando procesos desde un archivo de entrada.  
- Se avanza el tiempo global **tick por tick**.  
- En cada unidad de tiempo:
  - Se insertan procesos que llegan en ese instante.  
  - Se ejecuta la cola de mayor prioridad disponible.  
  - Se actualizan tiempos de espera y ráfagas.  
  - Si un proceso termina, se calculan sus métricas y se mueve a la lista de terminados.  
- Al finalizar, se genera un archivo `output.txt` con los resultados individuales y los promedios.

---

## Ejemplo de Entrada
Archivo `Examples/example3.txt`:

```
A; 6; 0; 1; 5
B; 9; 0; 1; 4
C; 10; 0; 2; 3
D; 15; 0; 2; 3
E; 8; 0; 3; 2
```

Formato:  
`ID ; BurstTime ; ArrivalTime ; Queue ; Priority`

---

## Ejemplo de Salida
Archivo `output.txt` generado:

```
ID   BT   AT   Q   PR   CT   TAT   WT   RT
A    6    0    1   5    9    9    3    0
B    9    0    1   4   18   18    9    0
...

Average:   TAT: 12.2   WT: 5.6   RT: 3.4   CT: 15.4
```

---

## Conclusiones
- El proyecto permitió **comprender mejor la planificación MLFQ**, simulando colas con diferentes algoritmos.  
- La **orientación a objetos** facilitó el manejo de procesos y colas.  
- Se observaron diferencias en las métricas de rendimiento (WT, TAT, RT) según los algoritmos asignados a cada nivel de la jerarquía.  
- El simulador puede ampliarse para estudiar variaciones del MLFQ o comparar directamente con otros algoritmos de planificación.

