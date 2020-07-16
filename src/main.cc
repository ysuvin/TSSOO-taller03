#include <global.h>
#include <checkArgs.hpp>


int main(int argc, char** argv){
	
	uint64_t totalElementos;
	uint32_t numThreads;
	uint32_t lim_inf;
	uint32_t lim_sup;
	uint32_t sum_serial = 0;
	uint32_t sum_hilos = 0;
	std::vector<std::thread *> threads;
	std::vector<uint32_t> array;
	std::vector<uint32_t> sump_hilo;
        std::mutex seguro;

//Captura de parametros
	auto argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);
	totalElementos = argumentos->getArgs().tamProblema;
	numThreads = argumentos->getArgs().numThreads;
	lim_inf = argumentos->getArgs().lim_inf;
	lim_sup = argumentos->getArgs().lim_sup;

//Creacion y llenado del arreglo Modulo 1
	// numeros aleatorios
		static thread_local std::random_device device;
		static thread_local std::mt19937 rng(device());
		std::uniform_int_distribution<uint32_t> nRandom(lim_inf, lim_sup);

	//LLenado y toma de tiempo
		auto  start = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < totalElementos; i++){
			seguro.lock();
	                array.push_back(nRandom(rng));
			sump_hilo.push_back(0);
	                seguro.unlock();
		}
	        auto end     = std::chrono::high_resolution_clock::now(); 
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		auto t_time0 = elapsed.count();

//Sumas Modulo 2
	
		start = std::chrono::high_resolution_clock::now();
		for (auto &numero : array){
			seguro.lock();
      	                sum_serial += numero;
       	                seguro.unlock();
		}

		auto sumaParcial = [](std::vector<uint32_t> &arreglo, std::vector<uint32_t> &sump_hilo, size_t pos, size_t Left, size_t Right) {
		for (size_t i = Left; i < Right; ++i){
			sump_hilo[pos] += arreglo[i];
		}
		return sump_hilo;
	};

        end     = std::chrono::high_resolution_clock::now(); 
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto t_time1 = elapsed.count();

        for (size_t i = 0; i < numThreads; ++i){
		seguro.lock();
                threads.push_back(new std::thread(sumaParcial, std::ref(array), std::ref(sump_hilo), i, i * (totalElementos) / numThreads, (i + 1) * (totalElementos) / numThreads));
                seguro.unlock();
	}
	for (auto &hilo : threads){
		hilo->join();
	}
        start = std::chrono::high_resolution_clock::now();
	for (auto &sum : sump_hilo){
		seguro.lock();
        	sum_hilos += sum;
        	seguro.unlock();
	}
        end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto t_time2 = elapsed.count();

//Resultados

    std::cout << "Total de Elementos: " << totalElementos << std::endl;
    std::cout << "Total de Hilos  : " << numThreads     << std::endl;


    std::cout << "===Llenado del arreglo===" << std::endl;
    std::cout << "Tiempo de llenado :"  << t_time0<< "[ms]" << std::endl;

    std::cout << "==========Sumas==========" << std::endl;
    std::cout << "Suma total serial:" << sum_serial << std::endl;
    std::cout << "Suma total paralela: " << sum_hilos << std::endl;

    std::cout << "=====Tiempos de Suma=====" << std::endl;
    std::cout << "Tiempo serial :"  << t_time1 << "[ms]" << std::endl;
    std::cout << "Tiempo paralelo :"  << t_time2 << "[ms]" << std::endl;
	
	
	return(EXIT_SUCCESS);
}


