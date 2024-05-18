	#include <fstream>
	#include <string>
	#include <iostream>
	#include <cstdio>
	#include <vector>
    #include <math.h>
    #include <iomanip>
    #include "TH1F.h"
    #include "TCanvas.h"
    #include "TApplication.h"
    #include "TAxis.h"
    #include "TGraph.h"

	using namespace std;

	struct coinc{
		double hour, minute, second, millisec, microsec, nanosec;
		char cod_uni;
	};

    double time(coinc c){
        return  c.hour * 3600 + c.minute * 60 + c.second + c.millisec/1000. + c.microsec/1000000. + c.nanosec/1000000000.;
    }



	int main(int argc, char** argv){

		if ( argc < 3 ) {
    	    cout << "Code usage : " << argv[0] << " <input_filename> <time interval in microseconds>" << endl;
    	    return -1 ;
	    }

        ifstream f(argv[1]);
        double delta_t = atof(argv[2])/1000000.;
        vector<coinc> data(3);
        f >> data[0].hour >> data[0].minute >> data[0].second >> data[0].millisec >> data[0].microsec >> data[0].nanosec >> data[0].cod_uni;
        f >> data[1].hour >> data[1].minute >> data[1].second >> data[1].millisec >> data[1].microsec >> data[1].nanosec >> data[1].cod_uni;

        double t_0, t_1, t_2;
        bool M, P, C;
        int coincidence = 0;
        TH1F*h = new TH1F ("h", "Counts/s", 100, 0, 15);
      
        TH1F*h1 = new TH1F ("h1", "Time_interval", 300, 0, 800);
        TGraph*g = new TGraph();
        double cont=2;
        double cont2=2;
        int min=0;
        TApplication app("app", 0, 0);

        for(;;){
            f >> data[2].hour >> data[2].minute >> data[2].second >> data[2].millisec >> data[2].microsec >> data[2].nanosec >> data[2].cod_uni;
            t_0 = time(data[0]);
            t_1 = time(data[1]);
            t_2 = time(data[2]);
            if(data[0].cod_uni=='M' || data[1].cod_uni=='M' || data[2].cod_uni=='M' ) M =1;
            if(data[0].cod_uni=='P' || data[1].cod_uni=='P' || data[2].cod_uni=='P' ) P =1;
            if(data[0].cod_uni=='C' || data[1].cod_uni=='C' || data[2].cod_uni=='C' ) C =1;
            if( fabs(t_0-t_1) < delta_t && fabs(t_2-t_1) < delta_t && M*P*C == 1) coincidence++;
            
            if(data[2].second!=data[1].second){ h->Fill(cont);cont=0;}
            if(data[2].minute!=data[1].minute){ min++; g->SetPoint(min, min,cont2);cont2=0;}
            
            h1->Fill((t_1-t_0)*1e3);
           
            
            data[0] = data[1];
            data[1] = data[2];
            M = 0;
            P = 0;
            C = 0;   
            cont++;
            cont2++;   
           
            if(f.eof()){
						cout << "End of file reached " << endl;           
						break;
					}	
        }

        cout << "Number of simultaneous coincidence in the three sites: " << coincidence << endl;
		TCanvas c("muon", "muon");
		c.cd();
		h->Draw();
		h->GetXaxis()->SetTitle("Counts/s");
		TCanvas c1("muon", "muon");
		c1.cd();
		h1->Draw();
		h1->GetXaxis()->SetTitle("time_interval[us]");
		TCanvas c2("muon", "muon");
		c2.cd();
		g->SetTitle("Counts/min");
		g->GetXaxis()->SetTitle("Counts/min");
		g->Draw();
		f.close();
		
		app.Run();

		return 0;
	}
