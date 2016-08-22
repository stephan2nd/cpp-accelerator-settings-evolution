#include <iostream>
#include <random>

#include "Genome.hpp"
#include "Population.hpp"

#include "Accelerator.hpp"
#include "DriftTube.hpp"
#include "HKick.hpp"
#include "QuadrupoleMagnet.hpp"
#include "Slit.hpp"
#include "Screen.hpp"
#include "Trafo.hpp"
#include "DipoleMagnet.hpp"

using namespace std;

Accelerator acc;

Trafo* t1;
Trafo* t2;
Trafo* t3;
Trafo* t4;
Trafo* final_trafo;
Screen* final_screen;

double fitness_function(const vector<double>& genes)
{
	double sum_off_diff = 0;
	acc.setNormValues(genes);
 	acc.startSimulation(10000);
 	sum_off_diff += t1->getCounts() + 10*t2->getCounts() + 100*t3->getCounts() + 1000*t4->getCounts() + 10000*final_trafo->getCounts();
	return sum_off_diff;
}
 
 
int main(int argc , char *argv[])
{
	cout << "BRIENCHJEN!" << endl;
 	
	double width = 0.1;
	double height = 0.1; 
	
	t1 = new Trafo("T1");
	t2 = new Trafo("T2");
	t3 = new Trafo("T3");
	t4 = new Trafo("T4");
	int dpm = 1000;
 	acc.appendDevice(new DriftTube("Drift1", width, height, 0.25));
 	acc.appendDevice(t1);
 	acc.appendDevice(new Screen("Screen1", width, height, dpm));
 	acc.appendDevice(new HKick("HKick1", -0.1, 0.1));
 	acc.appendDevice(new DriftTube("Drift2", width, height, 0.25));
 	acc.appendDevice(t2);
 	acc.appendDevice(new Screen("Screen2", width, height, dpm));
 	acc.appendDevice(new QuadrupoleMagnet("QD1", width, height, 0.5, 0.0, 10));
 	acc.appendDevice(new QuadrupoleMagnet("QD2", width, height, 0.5, -10, 0.0));
 	acc.appendDevice(new DriftTube("Drift3", width, height, 4.0));
 	acc.appendDevice(t3); 	
 	acc.appendDevice(new Screen("Screen3", width, height, dpm)); 	
  	acc.appendDevice(new DipoleMagnet("BEND1", width, height, 2.0, 0.5));
  	acc.appendDevice(new DriftTube("Drift4", width, height, 2.0));	
 	acc.appendDevice(t4);  	
 	acc.appendDevice(new Screen("Screen4", width, height, dpm)); 	
 	acc.appendDevice(new QuadrupoleMagnet("QD3", width, height, 0.5, 0.0, 10));
 	acc.appendDevice(new QuadrupoleMagnet("QD4", width, height, 0.5, -10, 0.0)); 	
  	acc.appendDevice(new DriftTube("Drift5", width, height, 4.0));
 	acc.appendDevice(new Screen("Screen5", width, height, dpm)); 
 	acc.appendDevice(new HKick("HKick2", -0.1, 0.1));
   	acc.appendDevice(new DriftTube("Drift6", width, height, 2.0));	
 	acc.appendDevice(new HKick("HKick3", -0.1, 0.1));   	
   	acc.appendDevice(new DriftTube("Drift7", width, height, 2.0));	 	
 	acc.appendDevice(new Slit("Slit", -0.05, -0.04, -0.01, 0.01));
 	final_screen = new Screen("FinalScreen", width, height, dpm);
 	acc.appendDevice(final_screen);
 	final_trafo = new Trafo("FinalTrafo");
 	acc.appendDevice(final_trafo);
 	


 	
	default_random_engine rnd(chrono::high_resolution_clock::now().time_since_epoch().count());

	int number_of_genes       = acc.settingSize();
	int number_of_genomes     = 100;
	int number_of_generations = 50;

	EvolutionParameters ep;
	ep.n_keep                     = 2;
	ep.sigma_survive              = 0.3; // 0.3 scheint ein guter Wert zu sein // 0.1 wirft 60% weg, 0.2 wirft 40% weg
	ep.p_mutate_disturbe          = 0.7;
	ep.p_mutate_replace           = 0.05;
	ep.p_non_homologous_crossover = 0.10;
	ep.b_crossing_over            = true;
	ep.b_mutate_mutation_rate     = false;
	ep.n_min_genes_till_cross     = 1;
	ep.n_max_genes_till_cross     = number_of_genes/2;	
	     
	
    Population p(number_of_genomes, number_of_genes, rnd);
    
    for( int i=0; i<number_of_generations; i++ ) {
    
		p = p.createOffspring(fitness_function, ep, rnd);
    	p.evaluate(fitness_function);
	    cout << "Generation " << i << ":\t" << p.toString() << endl;    
	    

    }
    
    cout << p.getBestGenome() << endl;

    acc.setNormValues(p.getBestGenome().getGenes());
 	acc.startSimulation(100000);
 	cout << acc.toString() << endl; 
    
    final_screen->exportHistogram();
	((Screen*) acc.getDeviceByName("Screen1"))->exportHistogram();
	((Screen*) acc.getDeviceByName("Screen2"))->exportHistogram();
	((Screen*) acc.getDeviceByName("Screen3"))->exportHistogram();
	((Screen*) acc.getDeviceByName("Screen4"))->exportHistogram();			
	((Screen*) acc.getDeviceByName("Screen5"))->exportHistogram();		

    return 0;
}
