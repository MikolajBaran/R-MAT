#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int n, g;
int mat[2];
fstream file;

void print(int** a, int size) {

	file.open("rmat.txt", ios::out|ios::app);

	cout << "\nWygenerowany graf w postaci macierzy sasiedztwa:" << endl;

	for (int i = 0; i < size; i++) {
		cout << '\t';
		for(int j=0; j < size; j++){
			cout << a[i][j] << " ";
			file << a[i][j] << " ";
		}
		cout << endl;
		file << endl;
	}
	cout << endl;
	file << endl;
	file.close();
	
	cout << "Wygenerowany wynik zostal zapisany do pliku rmat.txt" << endl;
}

int generate_size(){
	int k = 2;
	int size = 1;
	while(size<n){
		size = 1;
		for(int i=1; i<k; i++){
			size *= 2;
		}
		k++;
	}

	return size;
}

void random_matrix(int* p){
	int pick =(rand()%100);
	if(pick<=p[0]){
		mat[0] = 0;
		mat[1] = 0;
	}
	else if(pick>p[0]&&pick<=p[0]+p[1]){
		mat[0] = 1;
		mat[1] = 0;
	}
	else if(pick>p[0]+p[1]&&pick<=p[0]+p[1]+p[2]){
		mat[0] = 0;
		mat[1] = 1;
	}
	else{
		mat[0] = 1;
		mat[1] = 1;
	}
}

int generate_density(int t, int* p){
	int emax = 0;
	if(t==1){
		emax = n*(n-1) + n;
	}
	else{
		emax = (n*(n-1))/2 + n;
	}
	
	int ctr = 0;
	for(int i=0; i<4; i++){
		if(p[i] == 0){
			ctr++;
		}
	}
	
	emax = emax - (ctr*emax)/4;
	
	int density;
	density = round(emax*g*0.01);
	return density;
} 

void generate_rmat(int* p, int edges, int s, int oriented){
	
	cout << "\tLiczba krawedzi w generowanym grafie wyniesie: " << edges << endl;
	cout << "---------------------------------" << endl;
	
	int **arr = new int*[s];
	for(int i = 0; i < s; ++i) {
    	arr[i] = new int[s];
	}
	
	for(int x=0; x<s; x++){
		for(int y=0; y<s; y++){
			if(x>n-1 || y>n-1){
				arr[x][y] = 8;
			}
			else{
				if(x == y){
					arr[x][y] = 1;
				}
				else{
					arr[x][y] = 0;
				}
			}
		}
	}
	
	srand(time(NULL));
	int min_x,max_x,min_y,max_y, temp_s;
	int e = 0;
	
	while(e < edges){
		min_x = 0;
		max_x = s-1;
		min_y = 0;
		max_y = s-1;
		temp_s = s/2;
		
		while(temp_s >= 1){
			random_matrix(p);
			if((mat[0]*temp_s+(temp_s-1)<n) && (mat[1]*temp_s+(temp_s-1)<n)){
				min_x += mat[0]*temp_s;
				max_x = min_x + temp_s - 1;
				min_y += mat[1]*temp_s;
				max_y = min_y + temp_s - 1;
				if(temp_s>1){
					temp_s/=2;
				}
				else{
					break;
				}
			}
		}
		if(arr[min_x][min_y] == 0){
			arr[min_x][min_y] = 1;
			if(oriented == 0){
				arr[min_y][min_x] = 1;
			}
			e++;
		}
	}
	
	print(arr, s);
	
	for(int i = 0; i < s; ++i) {
    	delete [] arr[i];
	}
	delete [] arr;
}

int main() {
	cout << "Program do generowania grafow R-MAT w postaci macierzy sasiedztwa." << endl;
	cout << "Wprowadz liczbe n wierzcholkow: ";
	cin >> n;
	while(!n || n<2){
		cout << "Wprowadzono niepoprawna wartosc. Liczba wierzcholkow n musi byc rowna co najmniej 2 do utworzenia grafu. Sprobuj jeszcze raz." << endl;
		cout << "Wprowadz liczbe n wierzcholkow: ";
		cin >> n;
	}
	cout << "Wprowadz oczekiwana gestosc generowanego grafu [wyrazona w procentach 0-100]: ";
	cin >> g;
	while(g<0||g>100){
		cout << "Wprowadzono niepoprawna wartosc. Gestosc musi zawierac sie w przedziale <0;100>. Sprobuj jeszcze raz." << endl;
		cout << "Wprowadz oczekiwana gestosc generowanego grafu [wyrazona w procentach 0-100]: ";
		cin >> g;
	}
	int probability[4];
	int sum = 0;
	for(int i=0; i<3; i++){
			cout << "Podaj prawdopodobienstwo wylosowania czesci " << i+1 << " grafu [wyrazona w procentach 0-"<< 100-sum << "]: ";
			cin >> probability[i];
			while(probability[i]<0||probability[i]>100-sum){
				cout << "Wprowadzono niepoprawna wartosc. Prawdopodobienstwo calkowite wszystkich 4 zbiorow musi zawierac sie w przedziale <0;100>. Sprobuj jeszcze raz." << endl;
				cout << "Podaj prawdopodobienstwo wylosowania czesci  " << i+1 << " grafu [wyrazona w procentach 0-"<< 100-sum << "]: ";
				cin >> probability[i];
			}
			sum+=probability[i];
	}
	probability[3] = 100 - sum;
	cout << "Prawdopodobienstwo wylosowania czesci 4 grafu [wyrazona w procentach] wyniesie zatem: " << probability[3] << endl;
	int type;
	cout << "Podaj rodzaj grafu. Wpisz 0 dla grafu nieskierowanego i 1 dla grafu skierowanego: ";
	cin >> type;
	while(type!=0&&type!=1){
		cout << "Wprowadzono niepoprawne dane. Sprobuj jeszcze raz." << endl;
		cout << "Podaj rodzaj grafu. Wpisz 0 dla grafu nieskierowanego i 1 dla grafu skierowanego: ";
		cin >> type;
	}
	
	cout << "\n---------------------------------\nDane wejsciowe programu" << endl;
	cout << "\tRodzaj grafu: ";
	if(type == 0){
		cout << "nieskierowany" << endl;
	}
	else{
		cout << "skierowany" << endl;
	}

	generate_rmat(probability,generate_density(type, probability),generate_size(),type);

	return 0;
}
