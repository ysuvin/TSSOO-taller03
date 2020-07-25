#include <global.h>
#include <checkArgs.hpp>


int main(int argc, char** argv){

//Variables para los Parametros	
	uint64_t totalElementos;
	uint32_t numThreads;
	uint32_t lim_inf;
	uint32_t lim_sup;
//Variables del Arreglo y suma secuencial
	uint64_t *array_sec;
	uint64_t sum_sec = 0;
//Variables del Arreglo y suma OpenMP
	uint64_t *array_omp;
	uint64_t sum_omp = 0;


//Captura de parametros

	auto argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);
	totalElementos = argumentos->getArgs().tamProblema;
	numThreads = argumentos->getArgs().numThreads;
	lim_inf = argumentos->getArgs().lim_inf;
	lim_sup = argumentos->getArgs().lim_sup;


//Generador de Numeros Aleatorios

	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<> nRandom(lim_inf, lim_sup);


//Creacion y Llenado secuencial

	array_sec = new uint64_t[totalElementos];
	auto start2 = std::chrono::system_clock::now();

	for (size_t i = 0; i < totalElementos; ++i){
		array_sec[i] = nRandom(rng);
	}
	auto end2 = std::chrono::system_clock::now(); 
	std::chrono::duration<float, std::milli> elapsed2 = end2 - start2;
	auto t_time_array_sec = elapsed2.count();
	delete[] array_sec;

//Creacion y Llenado paralelo con openMP

	array_omp = new uint64_t[totalElementos];
	auto start = std::chrono::system_clock::now();

	#pragma omp parallel for  num_threads(numThreads)
	for(size_t i = 0; i < totalElementos; ++i){	
		array_omp[i] = nRandom(rng);
	}
	auto end = std::chrono::system_clock::now(); 
	std::chrono::duration<float, std::milli> elapsed = end - start;
	auto t_time_array_omp = elapsed.count();


// Suma secuencial

	auto start3 = std::chrono::system_clock::now();

	for (size_t i = 0; i < totalElementos; ++i){
		sum_sec += array_sec[i];
	}
	auto end3 = std::chrono::system_clock::now(); 
	std::chrono::duration<float, std::milli> elapsed3 = end3 - start3;
	auto t_time_sum_sec= elapsed3.count();


//Suma Paralela con OpenMP

	auto start1 = std::chrono::system_clock::now();
	#pragma omp parallel for reduction(+:sum_omp) num_threads(numThreads)

	for(size_t i = 0; i < totalElementos; ++i){
		sum_omp += array_omp[i];
	}
	auto end1 = std::chrono::system_clock::now(); 
	std::chrono::duration<float, std::milli> elapsed1 = end1 - start1;
	auto t_time_sum_omp = elapsed1.count();


//Resultados

    std::cout << "======== Condiciones Iniciales =======" << std::endl;
    std::cout << "Total de Elementos           : " << totalElementos << std::endl;
    std::cout << "Total de Hilos               : " << numThreads     << std::endl;

    std::cout << "========= Llenado del arreglo ========" << std::endl;
    std::cout << "Tiempo Secuencial            : " << t_time_array_sec<< "[ms]" << std::endl;
    std::cout << "Tiempo paralelo OpenMP       : " << t_time_array_omp<< "[ms]" << std::endl;
    
    std::cout << "=============== Sumas ================" << std::endl;
    std::cout << "Suma total secuencial        : " << sum_sec << std::endl;
    std::cout << "Suma total paralela OpenMP   : " << sum_omp << std::endl;

    std::cout << "=========== Tiempos de Suma ==========" << std::endl;
    std::cout << "Tiempo secuencial            : " << t_time_sum_sec << "[ms]" << std::endl;
    std::cout << "Tiempo paralelo OpenMP       : " << t_time_sum_omp << "[ms]" << std::endl;
	
	
	return(EXIT_SUCCESS);
}
