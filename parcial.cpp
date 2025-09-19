//Harold Guerrero Caicedo

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
// Definiciones de algoritmos
// R: Round Robin (RR)
// F: First Come First Served (FCFS)
// S: Shortest Job First (SJF)
// C: Shortest Remaining Time (STCF)
#define file_path "Examples/example3.txt" // Ruta del archivo de entrada
// Definición de algoritmos para cada cola
#define ALGO_Q1 'R' 
#define ALGO_Q2 'R' 
#define ALGO_Q3 'R' 
#define ALGO_Q4 'C'
// Definición de quantums para cada cola (en caso de usar Round Robin)
#define TIME_Q1 2   // Quantum para Round Robin
#define TIME_Q2 3   // Quantum para Round Robin
#define TIME_Q3 4   // Quantum para Round Robin
#define TIME_Q4 4   // Quantum para Round Robin

const vector<int> TIME_QUANTA = {TIME_Q1, TIME_Q2, TIME_Q3, TIME_Q4}; // Vector de quantums para cada cola

// Clase para representar un proceso
class proc {
    private:
        string id;  // Identificador del proceso
        unsigned int burst_time = 0; // Tiempo de ráfaga
        unsigned int arrival_time = 0; // Tiempo de llegada
        unsigned int queue = 0; // Cola a la que pertenece
        unsigned int priority = 0; // Prioridad del proceso
        unsigned int remaining_time = 0; // Tiempo restante
        unsigned int WT = 0; // Waiting Time
        unsigned int CT = 0; // Completion Time
        unsigned int RT = 0; // Response Time
        unsigned int TAT = 0; // Turn Around Time
    public:
        // Constructor
        proc(string i, unsigned int bt, unsigned int at, unsigned int q, unsigned int p) 
            : id(i), burst_time(bt), arrival_time(at), queue(q), priority(p), remaining_time(bt) {}

        // Getters
        string get_id() const { return id; }
        unsigned int get_burst_time() const { return burst_time; }
        unsigned int get_arrival_time() const { return arrival_time; }
        unsigned int get_queue() const { return queue; }
        unsigned int get_priority() const { return priority; }
        unsigned int get_remaining_time() const { return remaining_time; }
        unsigned int get_WT() const { return WT; }
        unsigned int get_CT() const { return CT; }
        unsigned int get_RT() const { return RT; }
        unsigned int get_TAT() const { return TAT; }

        // Setters
        void set_remaining_time(unsigned int rt) { remaining_time = rt; }
        void set_WT(unsigned int wt) { WT = wt; }
        void set_CT(unsigned int ct) { CT = ct; }
        void set_RT(unsigned int rt) { RT = rt; }
        void set_TAT(unsigned int tat) { TAT = tat; }
};

// Clase para representar una cola de procesos
class queue {
    private:
        unsigned int cola; // Identificador de la cola
        vector<proc> elements; // Elementos en la cola
        char algoritmo; // Algoritmo de planificación
        unsigned int quantum; // Solo se usa en Round Robin
        bool executing = false; // Indica si la cola está en ejecución, solo se usa en SJF
    public:
        // Constructor
        queue(unsigned qu, char alg, unsigned int q = 0) : cola(qu), algoritmo(alg), quantum(q) {}

        // Métodos
        unsigned int size() const { return elements.size(); } // Tamaño de la cola
        
        // Agrega un proceso a la cola según el algoritmo
        void push(const proc element) {
            // Si la cola está vacía, simplemente agrega el elemento
            if(elements.empty()) {
                elements.push_back(element); // si está vacía, agrega al final
                return;
            } else if(algoritmo == 'R' || algoritmo == 'F') {
                elements.push_back(element);    // agrega al final (ambos algoritmos son FIFO)
            } else if(algoritmo == 'S') {
                // Shortest Job First (SJF)
                if(executing)  // Si la cola está en ejecución, el primer elemento no se mueve porque no es expropiativo
                {
                    for(int i = 1; i < elements.size(); i++) {
                        if ((elements[i].get_remaining_time() > element.get_remaining_time())) {
                            elements.insert(elements.begin() + i, element);
                            return;
                        }
                    }
                } else // Si no está en ejecución, se considera desde el primer elemento
                {
                    for(int i = 0; i < elements.size(); i++) {
                        if ((elements[i].get_remaining_time() > element.get_remaining_time())) {
                            elements.insert(elements.begin() + i, element);
                            return;
                        }
                    }
                }
                elements.push_back(element);
            } else if(algoritmo == 'C') {
                // Shortest Time to Completition First (STCF)
                for(int i = 0; i < elements.size(); i++) {
                    if ((elements[i].get_remaining_time() > element.get_remaining_time())) {
                        elements.insert(elements.begin() + i, element);
                        return;
                    }
                }
                elements.push_back(element);
            }
        }

        // Elimina el primer proceso de la cola, cuando termina la ejecución de un proceso, o se mueve a otra cola
        void pop() {
            if (!elements.empty()) {
                elements.erase(elements.begin());
            }
        }

        // Getters
        unsigned int get_cola() const { return cola; }
        char get_algoritmo() const { return algoritmo; }
        unsigned int get_quantum() const { return quantum; }
        proc get_process(unsigned int i) const { return elements[i]; }

        // Setters
        void set_quantum(unsigned int q) { quantum = q; }
        void set_remaining_time(unsigned int i, unsigned int rt) { elements[i].set_remaining_time(rt); }
        void set_WT(unsigned int i, unsigned int wt) { elements[i].set_WT(wt); }
        void set_CT(unsigned int i, unsigned int ct) { elements[i].set_CT(ct); }
        void set_RT(unsigned int i, unsigned int rt) { elements[i].set_RT(rt); }
        void set_TAT(unsigned int i, unsigned int tat) { elements[i].set_TAT(tat); }
        void set_executing(bool exec) { executing = exec; }
};

int main() {
    // Abrir el archivo
    ifstream file(file_path);
    string line;
    vector<vector<string>> data; // Vector para almacenar los datos

    // Extraer datos línea por línea
    while (getline(file, line)) {
        istringstream ss(line);
        string token;
        vector<string> tokens;

        // Separa por "; "
        while (getline(ss, token, ';')) {
            // Eliminar posibles espacios iniciales
            if (!token.empty() && token[0] == ' ') {
                token.erase(0, 1);
            }
            tokens.push_back(token);
        }
        data.push_back(tokens);
    }
    file.close();

    vector<proc> processes; // Vector para almacenar los procesos
    vector<proc> finished_processes; // Vector para almacenar los procesos terminados
    vector<queue> queues;   // Vector para las colas

    // Inicializar colas con sus respectivos algoritmos y quantums
    queues.emplace_back(1, ALGO_Q1, TIME_Q1);
    queues.emplace_back(2, ALGO_Q2, TIME_Q2);
    queues.emplace_back(3, ALGO_Q3, TIME_Q3);
    queues.emplace_back(4, ALGO_Q4, TIME_Q4);

    // Crear procesos a partir de los datos extraídos
    for (const auto& item : data) {
        if (item.size() == 5) {
            processes.emplace_back(item[0], stoi(item[1]), stoi(item[2]), stoi(item[3]), stoi(item[4]));
        }
    }

    unsigned int time = 0; // Reloj del sistema

    // Bucle principal de simulación
    while(processes.size() > 0 || queues[0].size() > 0 || queues[1].size() > 0 || queues[2].size() > 0 || queues[3].size() > 0) {
        // Añadir procesos que llegan en el tiempo actual a sus respectivas colas
        for (auto it = processes.begin(); it != processes.end(); ) {
            // comprobar si el tiempo de llegada de un proceso es igual al tiempo actual
            if (it->get_arrival_time() == time) {
                // agregar el proceso a la cola correspondiente
                for(auto& q : queues) {
                    if (q.get_cola() == it->get_queue()) {
                        q.push(*it);
                        break;
                    }
                }
                // eliminar el proceso de la lista de procesos
                processes.erase(it);
            } else {
                ++it; // solo incrementar el iterador si no se agrga el proceso
            }
        }

        // Ejecutar la primera cola que tenga procesos
        for(int i = 0; i < queues.size(); i++) {
            // Si la cola tiene procesos, ejecutarla
            if(queues[i].size() > 0)
            {
                queues[i].set_executing(true); // Indicar que la cola está en ejecución

                // Si es la primera vez que se ejecuta el proceso, establecer su tiempo de respuesta
                if(queues[i].get_process(0).get_burst_time() == queues[i].get_process(0).get_remaining_time())
                {
                    queues[i].set_RT(0, time);
                }
                // Ejecutar según el algoritmo de la cola
                if(queues[i].get_algoritmo() == 'R' && queues[i].get_process(0).get_remaining_time() > 0)
                {
                    queues[i].set_remaining_time(0, queues[i].get_process(0).get_remaining_time() - 1); // Ejecutar por 1 unidad de tiempo
                    queues[i].set_quantum(queues[i].get_quantum() - 1); // Decrementar el quantum
                    // Si el proceso ha terminado
                    if(queues[i].get_process(0).get_remaining_time() == 0)
                    {
                        queues[i].set_CT(0, time + 1); // El tiempo de finalización es el tiempo actual + 1
                        queues[i].set_TAT(0, queues[i].get_process(0).get_CT() - queues[i].get_process(0).get_arrival_time()); // TAT = CT - AT
                        queues[i].set_WT(0, queues[i].get_process(0).get_TAT() - queues[i].get_process(0).get_burst_time()); // WT = TAT - BT
                        finished_processes.push_back(queues[i].get_process(0)); // Mover a la lista de procesos terminados
                        queues[i].pop(); // Eliminar el proceso de la cola
                        queues[i].set_quantum(TIME_QUANTA[i]); // Resetear el quantum
                    } else if(queues[i].get_quantum() == 0)
                    {
                        // Si el quantum se ha agotado y el proceso no ha terminado, moverlo a la siguiente cola
                        proc temp = queues[i].get_process(0);
                        queues[i].pop();
                        if(i + 1 < queues.size()) queues[i+1].push(temp);   // Mover a la siguiente cola
                        else  queues[i].push(temp); // Si es la última cola, se queda ahí
                        queues[i].set_quantum(TIME_QUANTA[i]); // Resetear el quantum
                    }
                } else
                {
                    // Otros algoritmos (F, S, C) se ejecutan por 1 unidad de tiempo
                    queues[i].set_remaining_time(0, queues[i].get_process(0).get_remaining_time() - 1); // Ejecutar por 1 unidad de tiempo
                    if(queues[i].get_process(0).get_remaining_time() == 0)
                    {
                        queues[i].set_CT(0, time + 1); // El tiempo de finalización es el tiempo actual + 1
                        queues[i].set_TAT(0, queues[i].get_process(0).get_CT() - queues[i].get_process(0).get_arrival_time()); // TAT = CT - AT
                        queues[i].set_WT(0, queues[i].get_process(0).get_TAT() - queues[i].get_process(0).get_burst_time()); // WT = TAT - BT
                        finished_processes.push_back(queues[i].get_process(0)); // Mover a la lista de procesos terminados
                        queues[i].pop(); // Eliminar el proceso de la cola
                    }
                }
                queues[i].set_executing(false); // Indicar que la cola ha terminado su ejecución
                break; // Solo se ejecuta una cola a la vez
            }
        }
        time++;
    }

    // Calcular promedios
    double total_WT = 0;
    double total_TAT = 0;
    double total_RT = 0;
    double total_CT = 0;
    for (const auto& p : finished_processes) {
        total_WT += p.get_WT();
        total_TAT += p.get_TAT();
        total_RT += p.get_RT();
        total_CT += p.get_CT();
    }
    double avg_WT = total_WT / finished_processes.size();
    double avg_TAT = total_TAT / finished_processes.size();
    double avg_RT = total_RT / finished_processes.size();
    double avg_CT = total_CT / finished_processes.size();

    // Exportar resultados individuales y promedios a un archivo de texto
    ofstream output("output.txt");
    output << "ID\tBT\tAT\tQ\tPR\tCT\tTAT\tWT\tRT\n";
    for (const auto& p : finished_processes) {
        output << p.get_id() << "\t" 
               << p.get_burst_time() << "\t" 
               << p.get_arrival_time() << "\t" 
               << p.get_queue() << "\t" 
               << p.get_priority() << "\t" 
               << p.get_CT() << "\t" 
               << p.get_TAT() << "\t" 
               << p.get_WT() << "\t" 
               << p.get_RT() << "\n";
    }
    output << "\n\n\nAverage:\t" 
           << "TAT: " << avg_TAT << "\t" 
           << "WT: " <<avg_WT << "\t" 
           << "RT: " <<avg_RT << "\t"
           << "CT: " <<avg_CT << "\n";
}
