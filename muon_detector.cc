	#include <fstream>
	#include <string>
	#include <iostream>
	#include <cstdio>
	#include <vector>

	using namespace std;

	struct coinc{
		double hour, minute, second, microsec, nanosec;
		int cod_uni;
	};

	vector<coinc> ReadAll( const char* Filename ) {
		vector<coinc> data;
		ifstream f(Filename);

		if(!f){
			cerr <<"Error: cannot open file " <<endl;
			exit(0);
		}
		else{  
			for (;;){
					string val ;
					coinc var;					
					getline (f, val, ';');			
					getline (f, val, ':');
					var.hour = stod(val);
					getline (f, val, ':');
					var.minute = stod(val);
					getline (f, val, ':');
					var.second = stod(val);
					getline (f, val, ':');
					var.microsec = stod(val);
					getline (f, val, ':');
					var.nanosec = stod(val);
					getline (f, val);
								
					data.push_back( var );
					
					if(f.eof()){
						cout << "End of file reached. Data read: "<< data.size() << endl;
						break;
					}		
			}
		}
		f.close();	
		return data;
	}


	int main(int argc, char** argv){
		vector<coinc>	data = ReadAll(argv[1]);

		return 0;
	}
