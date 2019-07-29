Autor: Felipe Vásquez
Rol: 201473504-9

Algoritmos greedy y tabu search para resolución de problemas de QAP.

El makefile adjunto ahora hace referencia al algoritmo tabu search, sin embargo notar que en este archivo se incluye la ejecución del algoritmo greedy. Esto no tiene efecto en el resultado final pues tabu search se implementa con inicialización aleatoria, y resultados para ambos algoritmos son mostrados en pantalla.

Para correr el programa usar los siguientes comandos.

make

./tabu.o path/al/archivo.dat nroIteraciones

El apartado nroIteraciones corresponde al número de iteraciones efectuadas por el Tabu Search, el cuál varía conforme es indicado en el informe. El largo de la lista tabú es dependiente del tamaño de la instancia. Además, Tabu Search es ejecutado un total de 8 veces y se presenta el promedio de los mejores costos y tiempos de ejecución al finalizar.

Las instancias deben estar en la carpeta "instancias", de manera que el path sea "instancias/__nombre_instancia__.dat". El programa asume que las instancias tienen el siguiente formato: nro de localizaciones/instalaciones, matriz de flujos y matriz de costos. Todas las matrices de costo deben ser cuadradas, además ahora es posible probar con instancias no-simétricas.

Si se desea eliminar los archivos .o generados, ejecutar el siguiente comando en el terminal:

make clean
