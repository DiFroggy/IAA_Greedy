Autor: Felipe Vásquez
Rol: 201473504-9

Algoritmo greedy para resolución de problemas de QAP.

Para correr el programa usar los siguientes comandos:

make

./greedy.o path/al/archivo.dat

Las instancias deben estar en la carpeta "instancias", de manera que el path sea "instancias/__nombre_instancia__.dat". El programa asume que las instancias tienen el siguiente formato: nro de localizaciones/instalaciones, matriz de flujos y matriz de costos. Todos estos deben estar separados por un salto de línea y además las matrices deben ser cuadradas. Instancias no simétricas pueden ser probadas.

Si se desea eliminar los archivos .o generados, ejecutar el siguiente comando en el terminal:

make clean