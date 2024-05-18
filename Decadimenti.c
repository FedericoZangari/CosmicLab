#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TGaxis.h"
#include "TGraph.h"
#include "TRandom.h"
#include "TFile.h"
#include "TChain.h"
#include "TString.h"

// Definiamo una funzione esponenziale con 2 parametri
Double_t oneexp(Double_t *x, Double_t *par)
{   
   Double_t val = par[0]*TMath::Exp(x[0]*par[1])  ;
   return val;
}

// Definiamo una somma di due esponenziali 4 parametri
Double_t sumexp(Double_t *x, Double_t *par)
{   
   Double_t val = par[0]*TMath::Exp(x[0]*par[1]) + par[2]*TMath::Exp(x[0]*par[3]) ;
   return val;
}

// Definiamo una somma di tre esponenziali, con 6 parametri, piu un termine costante (che si dovrebbe misurare separatamente)
Double_t sumtexp(Double_t *x, Double_t *par)
{
   
   Double_t val = par[0]*TMath::Exp(x[0]*par[1]) + par[2]*TMath::Exp(x[0]*par[3])  + par[4]*TMath::Exp(x[0]*par[5]) + 35. ;
   return val;
}

void Decadimenti(){

	gROOT->Reset();
	gStyle->SetOptFit();

	ifstream in;
       
	Int_t tot = 10000;
	Int_t nbin = 0;

cout << "Sto studiando " << tot << " particelle." << endl;
cout << "nbin= " << nbin << endl;
  
gRandom = new TRandom2(); //


 int N=tot;
   float tau= 2450; //nanosecondi
   float dt = 1; // nanosecondi
   const int nvm = 5;
   Float_t tmax = nvm * tau;
   Float_t prob = 1./tau * dt;
   Float_t tdecay=0.;
   float x;
   //
   int nmaxcicli = tmax / dt * N;
   cout << "Numero massimo di iterazioni " << nmaxcicli << endl << " tmax = " << tmax << "  prob " << prob << endl;

   TH1F *h3 = new TH1F("h3","Distribuzione Tempi di decadimento simulazione",50,0.,6650);
   float binwidth = h3->GetBinWidth(3);
   cout << "bin width " << binwidth << " nanoseconds" <<endl;
   TH1F *h5 = new TH1F("h5","Distribuzione numeri casuali",101,-0.05,1.05);


int s=0;
   // Per ogni particella i-esima
   for (int i = 0; i < N; i++){
     // andiamo a vedere nell'intervalli successivi delta t se decade

     for (Float_t g = 0.; g < tmax ; g = g + dt){
       //la probabilita di decadere tra t e t+dt e` costante e proporzionale a dt
        x = gRandom->Uniform(1.);
	// vediamo se sono distributi uniformemente tra zero e uno
        h5->Fill(x);
        // se il valore del numero casuale tra zero e uno e` inferiore 
        // al valore della probabilita di decadere
	// dichiariamo la particella ufficilmente decaduta al tempo t e usciamo dal ciclo
        if ( x < prob ) {tdecay = g; break;}
     } // fine del ciclo sui tempi
     // Riempiamo l'istogramma con il tempo di decadimento della particella i-esima
     h3-> Fill(tdecay);
     
     s++;
   } // fine del ciclo sulle particelle
   cout << "controllo " << s;
   TCanvas *c2 = new TCanvas("c2");
   c2->cd();
   gPad->SetLogy();
   h3->Draw("E0");

   // Visualizza l'istogramma dei tempi di decadimento
   TCanvas *c1 = new TCanvas("c1");
   c1->cd();
   h3->SetMarkerStyle(20);
   h3->SetMarkerSize(1.2);
   h3->GetXaxis()->SetTitle("Tempo [ns]");
   h3->GetYaxis()->SetTitle("Decadimenti in 133 ns");

   // Fit dei dati simulati
   TFitResultPtr myFitPar1 = h3->Fit("expo","S");
   Double_t nchi2   = myFitPar1->Chi2();
   Double_t nconstt   = myFitPar1->Value(0);
   Double_t nlambda   = myFitPar1->Value(1);
   Double_t nDa   = myFitPar1->ParError(0);
   Double_t nDb   = myFitPar1->ParError(1);
   
   cout << " Fitted lifetime " << 1/nlambda << " Chi2 = " << nchi2 << endl;



//TCanvas *c1 = new TCanvas("c1");
   h3->SetMarkerStyle(20);
   h3->SetMarkerSize(1.2);
   h3->GetXaxis()->SetTitle("Tempo [ns]");
   h3->GetYaxis()->SetTitle("Decadimenti in 133 ns");


   // Fit dei dati sperimentali con una funzione esponenziale
   // Per scegliere il range del fit occorre definire una funzione esponenziale, con range
   TF1 *onexpo = new TF1("onexpo",oneexp,1450.,6000.,2);
   onexpo->SetParameter(0,float(tot/2.));
   onexpo->SetParameter(0,-0.002);
   TFitResultPtr myFitPar = h3->Fit("onexpo","RS"); // Opzione RS per fare un fit entro il range
   Double_t chi2     = myFitPar->Chi2();
   Double_t constt   = myFitPar->Value(0);
   Double_t lambda   = myFitPar->Value(1);
   Double_t Da       = myFitPar->ParError(0);
   Double_t Db       = myFitPar->ParError(1);
 
cout << " Fitted experimental lifetime " << 1/lambda << " Chi2 = " << chi2 << endl;
  gPad->SetLogy();
 h3->Draw("E0");
//h3->Draw("SAME");

c1->Print("SperimentaleUnEspo.pdf");

// Fit con piu` esponenziali
// Copiamo l'istogramma in uno nuovo
 TH1F *h4 = (TH1F*)h3->Clone("h3");
 h4->Sumw2();
 // Apriamo un nuovo spazio grafico
 TCanvas *c4 = new TCanvas ("fit con piu esponenziali");
 c4->cd();
 cout << tot << endl;
 // Plot dell'istogramma clonato in scala logaritmica
 gPad->SetLogy();
 h4->Draw("E0");
 // we define a new function sum of two exponential
 // f(x) = exp(p0+p1*x) +  exp(p2+p3*x)
   TF1 *twoexpo = new TF1("twoexpo",sumexp,1050.,6400.,4);

 // we define a new function sum of three exponential, within the range 0 to 6250 with 6 parameters
   TF1 *threexpo = new TF1("threexpo",sumtexp,180.,6000.,6);
   // We set the initial values
 threexpo->SetParameter(0,float(tot/2.));
 threexpo->SetParameter(2,float(tot/4.));
 threexpo->SetParameter(4,float(tot/4.));
 threexpo->SetParameter(1,-0.0027);
 threexpo->SetParameter(3,-0.00042);
 threexpo->SetParameter(5,-0.002);
 threexpo->SetParameter(6,50.);
 // we rename the parameters
 threexpo->SetParNames("Nfast","lifetime1","Nslow", "lifetime2","Nthree","lifetime3");
 // Fit 
 TFitResultPtr myFitPar4 = h4->Fit("threexpo","RS");

 //Accesso ai risultati del fit con la somma di tre esponenziali
   Double_t tchi2       = myFitPar4->Chi2();
   Double_t tconstt1    = myFitPar4->Value(0);
   Double_t tmu1        = myFitPar4->Value(1);
   Double_t tconstt2    = myFitPar4->Value(2);
   Double_t tmu2        = myFitPar4->Value(3);
   Double_t tconstt3    = myFitPar4->Value(4);
   Double_t tmu3        = myFitPar4->Value(5);
   Double_t tDmu1       = myFitPar4->ParError(1);
   Double_t tDmu2       = myFitPar4->ParError(3);
   Double_t tDmu3       = myFitPar4->ParError(5);
   // Propagazione degli errori
   Double_t tlambda1 = 1/tmu1;
   Double_t tlambda2 = 1/tmu2;
   Double_t tlambda3 = 1/tmu3;
   Double_t Dlambda1 = tDmu1/tmu1*tlambda1; 
   Double_t Dlambda2 = tDmu2/tmu2*tlambda2;
   Double_t Dlambda3 = tDmu3/tmu3*tlambda3;  
   const float muonlifetime = 2197.;
cout << " Chi2 = " << tchi2 << endl;
 cout << " Fitted experimental lifetimes: lambda1 = " << tlambda1 << " +/- " << Dlambda1 << " nanoseconds "<< endl;
 cout << "                              : lambda2 = " << tlambda2 << " +/- " << Dlambda2 << endl;
 cout << "                              : lambda3 = " << tlambda3 << " +/- " << Dlambda3 << endl;
 cout << " comparison with nominal muon lifetime : " << muonlifetime << " nanoseconds" << endl;
 Double_t diffmeas = (fabs(tlambda2) - muonlifetime ) /  Dlambda2;
 cout << " This measurement is about  " << diffmeas << " standard deviations from the actual value. Two sigma happens" << endl;
 cout << "Somma dei termini costanti: " << tconstt1 + tconstt2 +tconstt3 << " confronto con tot " << tot<<endl;

 // produciamo un file pdf che contiene il grafico
c4->Print("FitDati3Esponenziali.pdf");

 cout << "versione finale \n 'n" << endl;



}

