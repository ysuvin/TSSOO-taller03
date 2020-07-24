#include <global.h>
#include <checkArgs.hpp>


int main(int argc, char** argv){
	
	uint64_t totalElementos;
	uint32_t numThreads;
	uint32_t lim_inf;
	uint32_t lim_sup;
	//Arreglo OpenMP--------
	uint64_t *array_omp;
	//SUMA OpenMP-----------
	uint64_t sum_omp = 0;

//Captura de parametros
	auto argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);
	totalElementos = argumentos->getArgs().tamProblema;
	numThreads = argumentos->getArgs().numThreads;
	lim_inf = argumentos->getArgs().lim_inf;
	lim_sup = argumentos->getArgs().lim_sup;


//Llenado paralelo con openMP
	//generador de los numeros random
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<> nRandom(lim_inf, lim_sup);

	//Creacion y Llenado paralelo del arreglo con openMP
	array_omp = new uint64_t[totalElementos];
	auto start = std::chrono::system_clock::now();

	#pragma omp parallel for  num_threads(numThreads)
	for(size_t i = 0; i < totalElementos; ++i){	
		array_omp[i] = nRandom(rng);
	}
	
	auto end = std::chrono::system_clock::now(); 
	std::chrono::duration<float, std::milli> elapsed = end - start;
	auto t_time0 = elapsed.count();



///Suma Paralela con OpenMP
	
	auto start1 = std::chrono::system_clock::now();
	#pragma omp parallel for reduction(+:sum_omp) num_threads(numThreads)

	for(size_t i = 0; i < totalElementos; ++i){
		sum_omp += array_omp[i];
	}
	auto end1 = std::chrono::system_clock::now(); 
	std::chrono::duration<float, std::milli> elapsed1 = end1 - start1;
	auto t_time1 = elapsed1.count();

//Resultados por pantalla

    std::cout << "Total de Elementos : " << totalElementos << std::endl;
    std::cout << "Total de Hilos     : " << numThreads     << std::endl;

    std::cout << "====Llenado del arreglo con OpenMP====" << std::endl;
    std::cout << "Tiempo de llenado  :"  << t_time0<< "[ms]" << std::endl;
    
    std::cout << "===========Sumas con OpenMP===========" << std::endl;
    std::cout << "Suma total paralela: " << sum_omp << std::endl;

    std::cout << "============Tiempos de Suma===========" << std::endl;
    std::cout << "Tiempo paralelo    :"  << t_time1 << "[ms]" << std::endl;
	
	
	return(EXIT_SUCCESS);
}
