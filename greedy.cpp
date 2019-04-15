#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <bits/stdc++.h>
#include <sys/time.h> 
using namespace std;
//Función que calcula el costo de una permutación. Costos son bidireccionales, contemplando instancias no-simétricas.
int calcularCosto(std::vector<int> solucion,std::vector<std::vector<int>> flujos,std::vector<std::vector<int>> distancias){
	int suma=0;
	for (int i = 0; i < (int)solucion.size(); ++i)
	{
		std::vector<int> fluSal,distSal;
		fluSal=flujos[i];
		distSal=distancias[solucion[i]];
		for (int j = 0; j < (int)solucion.size(); ++j)
		{
			if (i!=j)
			{
				suma+=fluSal[j]*distSal[solucion[j]];
			}
		}
	}
	return(suma);
}

int main(int argc, char const *argv[])
{
	//Se obtiene nombre de la instancia
	string instancia=argv[1];
	string archivo="instancias/"+instancia+".dat";
  	ifstream myfile (archivo);
  	bool first=true;
  	int nro=0, vacios=0;
  	string line;
  	//Se extraen las matrices de flujo y distancias
  	std::vector<vector <int>> flujos,distancias;
  	if (myfile.is_open())
  	{
  		while ( getline (myfile,line) )
	    {	
	    	if (first)
	  		{
	  			nro=stoi(line);
	  			first=false;
	  		}else{
	  			if ((int)line.length()==0)
	  			{
	  				vacios++;
	  			}else{
	  				string tempNro="";
	  				std::vector<int> fila;
		  			for (int i = 0; i < (int)line.length(); ++i)
		  			{
		  				
		  				if (line[i]==' ' || i==(int)line.length()-1)
		  				{
		  					if (i==(int)line.length()-1)
			  				{
			  					tempNro+=line[i];
			  				}	
		  					if (!tempNro.empty())
		  					{
			  					fila.push_back(stoi(tempNro));
			  					tempNro="";
		  					}
		  					else{
		  						continue;
		  					}
		  				}else{
		  					tempNro+=line[i];
		  				}
		  				
		  			}
		  			if (vacios==1)
		  			{
		  				flujos.push_back(fila);
		  			}else{

		  				distancias.push_back(fila);
	  				}
		  		}
	  		}
	    }
    myfile.close();
    
 	}
 	//Se cuenta el tiempo de ejecución
 	struct timeval tp;
 	long long t1,t2;
 	gettimeofday(&tp,NULL);
 	t1=tp.tv_sec*1000+tp.tv_usec/1000;
 	std::vector<int> solucion;
 	//Por cada fabrica existente, se inicializa con -1
 	for (int i = 0; i < nro; ++i)
 	{
 		solucion.push_back(-1);
 	}
 	//Indice de la solucion es la instalación
 	//Contenido es la localizacion asignada
 	for (int fabrica = 0; fabrica < nro; ++fabrica)
 	{
 		//Se buscan máximos en la matriz de flujos y mínimos en la matriz de distancias
 		int maximo=0,minimo=999999;
 		int flujoF,flujoC,distF,distC;
 		for (int fila = 0; fila < nro; ++fila)
 		{
 			for (int columna = 0; columna < nro; ++columna)
 			{
 				//Se ignoran costos autoreferentes (0 en todos los casos)
 				if (fila==columna)
	 				{
	 					continue;
	 				}
	 				if (flujos[fila][columna]>maximo)
	 				{
	 					//¿Ha sido el indice de la solucion asignado?
	 					if(solucion[fila]==-1){
			 				maximo=flujos[fila][columna];
		 					flujoF=fila;
		 					flujoC=columna;	
		 				}
	 				}
	 				if (distancias[fila][columna]<minimo)
	 				{
	 					bool localizacionAsignada=false;
	 					//¿Esta el valor de localizacion en la solucion?
	 					for (int i = 0; i < (int)solucion.size(); ++i)
			 			{
			 				if (solucion[i]==fila)
			 				{
			 					localizacionAsignada=true;
			 				}
			 			}
			 			if (!localizacionAsignada)
			 			{
			 				minimo=distancias[fila][columna];
		 					distF=fila;
		 					distC=columna;
			 			}

	 				}
 				
 			}
 		}
 		//En primer lugar se asignan dos fábricas que incurran al menor costo entre ambas
 		if (fabrica==0)
 		{
	 		solucion[flujoF]=distF;
	 		solucion[flujoC]=distC;
	 		++fabrica;
	 		//Luego se agregan fábricas que mantengan la solución factible y tengan el máximo
	 		//costo en la matriz de flujos y menor en la de distancias
 		}else{
 			solucion[flujoF]=distF;
 		}
 	}
 	gettimeofday(&tp,NULL);
 	t2=tp.tv_sec*1000+tp.tv_usec/1000;
 	//Se printean resultados
 	cout << "Instancia: "<<instancia<<"\n";
 	cout << "Solucion: (";
 	for (int i = 0; i < (int)solucion.size(); ++i)
 	{
 		cout << solucion[i] << " ";
 	}
 	cout << ")\n";
 	cout << "Costo: " << calcularCosto(solucion,flujos,distancias) << "\n";
 	cout << "Tiempo: " << t2-t1 << " ms\n";
	
 	return 0;
}