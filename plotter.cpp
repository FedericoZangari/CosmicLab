#include "TApplication.h"
#include "TH1D.h"
#include "TCanvas.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>

#define SKIPN 15

int main(int argc, char** argv){
	std::ifstream coincidence{argv[1], std::ios::in};
	std::string tempStr{};
	getline(coincidence, tempStr);
	getline(coincidence, tempStr);
	uint32_t minscount{};
	std::vector<uint32_t> count;
	int8_t prevmin{-1}, currmin{};
	TApplication app{"app",0,0};
	TCanvas *canv{new TCanvas("Coincidences Histograms", "Coincidences Histograms")};
	while(!(coincidence.eof())){
		getline(coincidence, tempStr);
		if(tempStr.size() > SKIPN){
			tempStr = tempStr.substr(SKIPN,3);
			if(tempStr[0] == ':'){
				currmin = std::atoi(tempStr.substr(1,3).c_str());
			}
			else{
				currmin = std::atoi(tempStr.substr(0,2).c_str());
			}
			if(currmin != prevmin){
				minscount++;
				count.push_back(0);
				prevmin = currmin;
			}
			else{
				count[minscount-1]++;
			}
		} 
	}
	TH1D *histo0{new TH1D("Histogram0", "Coincidences per minute", minscount-1, 0, minscount-1)}; 
	TH1D *histo1{new TH1D("Histogram1", "Coincidences per Minute Frequency", 550, 100, 350)};
	for(size_t i=0; i< minscount-1; i++){
		histo1->Fill(count[i]);
		histo0->AddBinContent(i, count[i]);
	}
	canv->Divide(1,2);
	canv->cd(1);
	histo0->SetStats(0);
	histo0->Draw();	
	canv->cd(2);
	histo1->Draw("bar");
	coincidence.close();
	std::cout << "Standard Deviation: " << histo1->GetMeanError() << "\n";
	std::cout << "Relative Error:\t" << histo1->GetMeanError()/histo1->GetMean() << "%\n";
	app.Run();
	return 0;
}
