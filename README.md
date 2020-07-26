# Taller 02 - Taller de Sistemas Operativos
- Yian Vera Soto - yian.vera@alumnos.uv.cl


## Diseño de la Solución
El objetivo de este taller es llenar un arreglo con números generados con una función random y sumarlos así logrando la multitarea logrando un mayor desempeño y eficiencia en el procesamiento y el tiempo a traves del uso de OpenMP.

### Módulo 1
Este modulo pide la creacion y llenado de un arreglo de forma paralela utilizando OpemMP.

#### Creación del arreglo:
Para crear el arreglo se ultiliza la linea de codigo
~~~
uint64_t totalElementos;
uint64_t *array_omp;
array_omp = new uint64_t[totalElementos];
~~~
Todo esto dentro de la funcion ```main()``` siendo ```uint64_ *array_omp``` la variable que guarde el arreglo que usaremos con OpenMP y ```uint64_ totalElementos``` el total de  elementos que ingresamos por parametros.

#### LLenado del Arreglo:
El siguiente codigo se encargara de llenar el arreglo de forma paralela utilizando las funciones de OpenMP.
~~~
#pragma omp parallel for  num_threads(numThreads)
	for(size_t i = 0; i < totalElementos; ++i){	
		array_omp[i] = nRandom(rng);
	}
~~~

### Módulo 2
En este modulo se pide sumar todos los numeros que guarda el arreglo de forma paralela utilizando OpenMP.
En el siguiente codigo se utiliza la variable ```uint64_t sum_omp``` para que la suma que va sumando el for y la funcion de OpenMP se vaya guardando en esta y obtengamos el total.
~~~
uint64_t sum_omp = 0;
#pragma omp parallel for reduction(+:sum_omp) num_threads(numThreads)
	for(size_t i = 0; i < totalElementos; ++i){
		sum_omp += array_omp[i];
	}
~~~
