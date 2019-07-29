#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <numeric>
using namespace std;
using namespace std::chrono;
//Función que calcula el costo de una permutación. Costos son bidireccionales, contemplando instancias no-simétricas.
int calcularCosto(std::vector<int> solucion,std::vector<std::vector<int> > flujos,std::vector<std::vector<int> > distancias){
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
//Chequea si un vector está dentro de una matriz. O vector de vectores
bool vectorIn(std::vector<int> elemento,std::vector<std::vector<int> > vectores){
	bool match=false;
	for (int i = 0; i < (int)vectores.size(); ++i)
	{
		if(elemento==vectores[i]){
			match=true;
		}
	}
	return(match);
}
//Chequea si un elemento esta en un vector
bool valueIn(int elemento,std::vector<int > vector){
	bool match=false;
	for (int i = 0; i < (int)vector.size(); ++i)
	{
		if(elemento==vector[i]){
			match=true;
		}
	}
	return(match);
}
//Movimiento básico. Cambia dos elementos en la solución
std::vector<int> swap(std::vector<int> solucion,int pos1,int pos2){
	int temp=solucion[pos1];
	solucion[pos1]=solucion[pos2];
	solucion[pos2]=temp;
	return(solucion);
}
//Función que retorna las posibles soluciones que están a un movimiento de 'distancia'
//Considera lista tabú

std::vector<std::vector<std::vector<int> > > obtenerVecindad(std::vector<int> solucion,std::vector<int> listaTabu){
	std::vector<std::vector<std::vector<int> > > vectorDeVectoresDeVectores;
	for (int i = 0; i < (int)solucion.size()-1; ++i)
	{
		for (int j = i+1; j < (int)solucion.size(); ++j)
		{
			//Si el vector en la vecindad está en la lista tabú, no se considera
			if ((int)listaTabu.size()>0)
			{
				//Lista tabu considera posiciones que no pueden ser swappeadas
				if(valueIn(i,listaTabu) || valueIn(j,listaTabu)){
					continue;
				}
			}
			std::vector<int> swapInfo,solucionConsiderada=swap(solucion,i,j);
			swapInfo.push_back(i);
			swapInfo.push_back(j);
			
			std::vector<std::vector<int> > info;
			info.push_back(solucionConsiderada);
			info.push_back(swapInfo);
			vectorDeVectoresDeVectores.push_back(info);
		}
	}
	return(vectorDeVectoresDeVectores);
}

//Funcion que agrega una solucion a la lista tabu hasta un máximo definido por el valor 'lenTabu'
//Pasado ese máximo, elimina a la solución más antigua
std::vector<int> actualizarTabu(std::vector<int > tabu,std::vector<int> elementos,int lenTabu){
	if ((int)tabu.size()<lenTabu)
	{
		tabu.push_back(elementos[0]);
		tabu.push_back(elementos[1]);
	}else{
		tabu.erase(tabu.begin());
		tabu.erase(tabu.begin());
		tabu.push_back(elementos[0]);
		tabu.push_back(elementos[1]);

	}
	return(tabu);
}


//Devuelve una solución inicial aleatoria, en caso de ser necesitada
std::vector<int> solucionInicialRandom(int largoSolucion){
	std::vector<int> posibles,solucion;
	
	for (int i = 0; i < largoSolucion; ++i)
	{
		posibles.push_back(i);
	}
	//De este se "popean" las posiciones en la solución inicial
	for (int i = 0; i < largoSolucion; ++i)
	{
		int random=rand()%(int)posibles.size();
		int numero=posibles[random];
		posibles.erase(posibles.begin()+random);
		solucion.push_back(numero);
	}
	return(solucion);
}
int main(int argc, char const *argv[])
{
	
	
	//Se obtiene nombre de la instancia
  	string archivo=argv[1];
  	ifstream myfile (archivo);
  	int nro=0;
  	string line;
  	int lineas=0;
  	//Se extraen las matrices de flujo y distancias
  	std::vector<vector <int> > flujos,distancias;
  	if (myfile.is_open())
  	{
  		while ( getline (myfile,line) )
	    {	
	    	lineas++;
	    	int largoString=(int)line.length();
	    	if (lineas==1)
	  		{
	  			nro=stoi(line);
	  			continue;
	  		}
	  		
	  		if (largoString==0)
  			{
  				continue;
  				
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
	  			if ((int)flujos.size()<nro)
	  			{
	  				flujos.push_back(fila);
	  			}else{

	  				distancias.push_back(fila);
  				}
	  		}
	    }

    myfile.close();
    
 	}

 	cout << "=========Greedy=========\n";
	
	//Se cuenta el tiempo de ejecución total de cada algoritmo
 	steady_clock::time_point t1,t2;
	 
 	
	t1=steady_clock::now();

	//Para llevar registro de desempeño, se usan vectores que compartan indices
	std::vector<int> costosTabu;
	std::vector<double> tiemposTabu;
 	
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
 	t2=steady_clock::now();
	//Se printean resultados
 	cout << "Archivo: "<<archivo<<"\n";
 	int costoGreedy=calcularCosto(solucion,flujos,distancias);
 	duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
	double tiempoGreedy=time_span.count();

	cout << "Costo: " << costoGreedy << "\n";
	cout << "Tiempo: " << tiempoGreedy << " s\n";
 	
	 std::vector<std::vector<int> > vecindad;
	std::vector<int> posibles,solucionActual,mejorSolucion,tabu;
	int lenTabu=0.4*nro,iteracionesTabu=stoi(argv[2]);
	long int costoActual,mejorCosto;

	cout << "=========Tabu Search=========\n";
	cout << "Tamanno instancia: " << nro << "\n";
	cout << "Tamanno lista tabu: " << lenTabu << "\n";
 	cout << "Iteraciones Tabu: " << iteracionesTabu << "\n";
	int repeticiones=0,maxRepeticiones=8;
	while(repeticiones<maxRepeticiones){
		
		//Se consigue una solucion aleatoria	 
		solucionActual=solucionInicialRandom(nro);

		//Se calcula el costo inicial
		costoActual=calcularCosto(solucionActual,flujos,distancias);
		mejorCosto=costoActual;
		mejorSolucion=solucionActual;

		//Inicia el loop
		t1=steady_clock::now();
		t2=t1;
		time_span = duration_cast<duration<double> >(t2 - t1);
			
		for (int l = 0; l < iteracionesTabu; l++)
		{
			std::vector<std::vector<std::vector<int> > > contenedor;
			//info tiene a la solución en posición 0, y el swap efectuado en 1
			contenedor=obtenerVecindad(solucionActual,tabu);
			int mejorVecLocal=0;
			long int mejorCostoLocal=2147483647,costoTemp;
			//Se busca en la vecindad al mejor vecino
			for (int j = 0; j < (int)contenedor.size(); ++j)
			{
				costoTemp=calcularCosto(contenedor[j][0],flujos,distancias);
				if (costoTemp<mejorCostoLocal)
				{
					mejorCostoLocal=costoTemp;
					mejorVecLocal=j;
				}
			}
			//Invariablemente se produce el movimiento
			
			solucionActual=contenedor[mejorVecLocal][0];
			costoActual=mejorCostoLocal;
			tabu=actualizarTabu(tabu,contenedor[mejorVecLocal][1],lenTabu);
		
			//Luego se comprueba si la solución actual es la mejor
			if (costoActual<mejorCosto)
			{
				mejorCosto=costoActual;
				mejorSolucion=solucionActual;
			}
			
			t2=steady_clock::now();
			time_span = duration_cast<duration<double> >(t2 - t1);
		}
		double tiempoTabu=time_span.count();
		
		repeticiones++;
		cout <<"-----Intento nro " << repeticiones << "-----\n";
		cout << "Costo: " <<mejorCosto << "\n";
		cout << "Tiempo de ejecucion: " << tiempoTabu<<"\n";

		tiemposTabu.push_back(tiempoTabu);
		costosTabu.push_back(mejorCosto);
	}
	cout << "=========Resultados=========\n";
	cout << "-------Greedy-------\n";
	cout << "Costo final: " << costoGreedy << "\n";
	cout << "Tiempo de ejecucion: " << tiempoGreedy << " s\n";
 	
	cout << "-------Tabu Search-------\n";
	float avg=0;
	for (int i = 0; i < (int)costosTabu.size(); i++)
	{
		avg+=costosTabu[i]/costosTabu.size();
	}
	cout << "Mejor costo promedio: " << std::fixed << avg << endl;
	cout << "Tiempo promedio de ejecucion: " << accumulate(tiemposTabu.begin(),tiemposTabu.end(), 0.0)/tiemposTabu.size() << "\n";
 	return 0;
}