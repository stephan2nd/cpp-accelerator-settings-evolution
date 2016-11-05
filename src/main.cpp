#include <iostream>
#include <random>
#include <fstream>
//#include <chrono>

#include "Genome.hpp"
#include "Population.hpp"

#include "Accelerator.hpp"
#include "DriftTube.hpp"
#include "HKick.hpp"
#include "VKick.hpp"
#include "QuadrupoleMagnet.hpp"
#include "Slit.hpp"
#include "Screen.hpp"
#include "Trafo.hpp"
#include "DipoleMagnet.hpp"
#include "RectangularDipoleMagnet.hpp"
#include "ProfileGrid.hpp"

using namespace std;

Accelerator acc;

Trafo* t0;
Trafo* t1;
Trafo* t2;
Trafo* t3;
Trafo* t4;
Trafo* t5;
Trafo* t6;
Trafo* t7;
Trafo* final_trafo;

double fitness_simple_acc(const vector<double>& genes)
{
	double sum_off_diff = 0;
	acc.setNormValues(genes);
 	acc.startSimulation(20000);
 	sum_off_diff += t1->getCounts() + 10*t2->getCounts() + 100*t3->getCounts() + 1000*t4->getCounts() + 100000*final_trafo->getCounts();
	return sum_off_diff;
}

void experiment_simple_acc()
{
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
 	acc.appendDevice(new Screen("Screen3", width, height, dpm)); 
 	acc.appendDevice(new DriftTube("Drift3", width, height, 4.0));
 	acc.appendDevice(t3); 	
 	acc.appendDevice(new Screen("Screen4", width, height, dpm)); 	
  	acc.appendDevice(new DipoleMagnet("BEND1", width, height, 2.0, 0.5));
  	acc.appendDevice(new DriftTube("Drift4", width, height, 2.0));	
 	acc.appendDevice(t4);  	
 	acc.appendDevice(new Screen("Screen5", width, height, dpm)); 	
 	acc.appendDevice(new QuadrupoleMagnet("QD3", width, height, 0.5, 0.0, 10));
 	acc.appendDevice(new QuadrupoleMagnet("QD4", width, height, 0.5, -10, 0.0)); 	
  	acc.appendDevice(new DriftTube("Drift5", width, height, 4.0));
 	acc.appendDevice(new Screen("Screen6", width, height, dpm)); 
 	acc.appendDevice(new HKick("HKick2", -0.1, 0.1));
   	acc.appendDevice(new DriftTube("Drift6", width, height, 2.0));	
 	acc.appendDevice(new HKick("HKick3", -0.1, 0.1));   	
   	acc.appendDevice(new DriftTube("Drift7", width, height, 2.0));	 	
 	acc.appendDevice(new Slit("Slit", -0.05, -0.04, -0.01, 0.01));
 	acc.appendDevice(new Screen("Screen7", width, height, dpm));
 	final_trafo = new Trafo("FinalTrafo");
 	acc.appendDevice(final_trafo);
 	
	acc.setScreenIgnore(true);

 	
	default_random_engine rnd(chrono::high_resolution_clock::now().time_since_epoch().count());

	int number_of_genes       = acc.settingSize();
	int number_of_genomes     = 10;
	int number_of_generations = 2000;

	EvolutionParameters ep;
	ep.n_keep                     = 2;
	ep.sigma_survive              = 0.3; // 0.3 scheint ein guter Wert zu sein // 0.1 wirft 60% weg, 0.2 wirft 40% weg
	ep.p_mutate_disturbe          = 0.7;
	ep.p_mutate_replace           = 0.05;
	ep.p_non_homologous_crossover = 0.10;
	ep.b_crossing_over            = true;
	ep.b_mutate_mutation_rate     = true;
	ep.n_min_genes_till_cross     = 1;
	ep.n_max_genes_till_cross     = number_of_genes/2;	
	     
	
    Population p(number_of_genomes, number_of_genes, rnd);
   	p.evaluate(fitness_simple_acc);    

    for( int i=0; i<number_of_generations; i++ ) {    
		p = p.createOffspring(ep, rnd);
    	p.evaluate(fitness_simple_acc);
	    cout << "Generation " << i << ":\t" << p.toString() << endl;    
    }
    
    cout << p.getBestGenome() << endl;    
    
	acc.setScreenIgnore(false);
    acc.setNormValues(p.getBestGenome().getGenes());
 	acc.startSimulation(1000000);
 	cout << acc.toString() << endl; 
    
	((Screen*) acc.getDeviceByName("Screen1"))->exportHistogram();
	((Screen*) acc.getDeviceByName("Screen2"))->exportHistogram();
	((Screen*) acc.getDeviceByName("Screen3"))->exportHistogram();
	((Screen*) acc.getDeviceByName("Screen4"))->exportHistogram();			
	((Screen*) acc.getDeviceByName("Screen5"))->exportHistogram();
	((Screen*) acc.getDeviceByName("Screen6"))->exportHistogram();
	((Screen*) acc.getDeviceByName("Screen7"))->exportHistogram();
}

double fitness_m3_beamline(const vector<double>& genes)
{
	double f=0;
	acc.setNormValues(genes);
 	acc.startSimulation(80000);
 	f = t0->getCounts();
	f = f * (1 + t1->getCounts() - t1->getCounts()*(abs(((ProfileGrid*) acc.getDeviceByName("UMADG1g"))->centerX()) + abs(((ProfileGrid*) acc.getDeviceByName("UMADG1g"))->centerY())));
	f = f * (1 + t2->getCounts() - t2->getCounts()*(abs(((ProfileGrid*) acc.getDeviceByName("UMADG2g"))->centerX()) + abs(((ProfileGrid*) acc.getDeviceByName("UMADG2g"))->centerY())));
	f = f * (1 + t3->getCounts() - t3->getCounts()*(abs(((ProfileGrid*) acc.getDeviceByName("UMADG3g"))->centerX()) + abs(((ProfileGrid*) acc.getDeviceByName("UMADG3g"))->centerY())));
	f = f * (1 + t4->getCounts() - t4->getCounts()*(abs(((ProfileGrid*) acc.getDeviceByName("UMADG4g"))->centerX()) + abs(((ProfileGrid*) acc.getDeviceByName("UMADG4g"))->centerY())));
	f = f * (1 + t5->getCounts());
	f = f * (1 + t6->getCounts() - t6->getCounts()*(abs(((ProfileGrid*) acc.getDeviceByName("UM3DG6g"))->centerX()) + abs(((ProfileGrid*) acc.getDeviceByName("UM3DG6g"))->centerY())));
	f = f * (1 + t7->getCounts() - t7->getCounts()*(abs(((ProfileGrid*) acc.getDeviceByName("TARGET_SCREENag"))->centerX()) + abs(((ProfileGrid*) acc.getDeviceByName("TARGET_SCREENag"))->centerY())));
	f = f * (1 + t7->getCounts() - t7->getCounts()*(abs(((ProfileGrid*) acc.getDeviceByName("TARGET_SCREENbg"))->centerX()) + abs(((ProfileGrid*) acc.getDeviceByName("TARGET_SCREENbg"))->centerY())));
 	
 /*	f += t0->getCounts() * (1 + t1->getCounts()) * (1 + t2->getCounts()) * (1 + t3->getCounts()) * (1 + t4->getCounts()) * (1 + t5->getCounts()) * (1 + t6->getCounts()) * (1 + t7->getCounts());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("UMADG1g"))->centerX());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("UMADG1g"))->centerY());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("UMADG2g"))->centerX());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("UMADG2g"))->centerY());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("UMADG3g"))->centerX());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("UMADG3g"))->centerY());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("UMADG4g"))->centerX());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("UMADG4g"))->centerY());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("UM3DG6g"))->centerX());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("UM3DG6g"))->centerY());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("TARGET_SCREENag"))->centerX());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("TARGET_SCREENag"))->centerY());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("TARGET_SCREENbg"))->centerX());
 	f -= 1E6 * abs(((ProfileGrid*) acc.getDeviceByName("TARGET_SCREENbg"))->centerY());*/
	return f;
}

void experiment_m3_beamline()
{
	t0 = new Trafo("T0");
	t1 = new Trafo("T1");
	t2 = new Trafo("T2");
	t3 = new Trafo("T3");
	t4 = new Trafo("T4");
	t5 = new Trafo("T5");
	t6 = new Trafo("T6");
	t7 = new Trafo("T7");	
	
	IonSource ion_source(12, 6, 2, 1000, 0., 0.00012, 0., 0.00245, 0., 0.00063, 0., 0.00316, 0., 0., 0., 0.);
	//IonSource ion_source(12, 6, 2, 1000, 0., 0.00006, 0., 0.00150, 0., 0.00030, 0., 0.00212, 0., 0., 0., 0.);
    //IonSource ion_source(12, 6, 2, 1000, 0., 0.00003, 0., 0.00050, 0., 0.00015, 0., 0.00050, 0., 0., 0., 0.);
	acc.setIonSource(ion_source);
		
	double max_quad_strength = 7;
	double drift_width = 0.040;
	double grid_width = 0.040;
	double dpm = 5000;
	const double pi = 3.141592653589793;	
		
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 1.1 + 0.1));
  	acc.appendDevice(t0); 	
  	acc.appendDevice(new RectangularDipoleMagnet("UT1MK0", 0.080, 0.040, -10.*pi*-4.894/180., -10.*pi/180.));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.1 + 2.3385 + 0.124));
//	acc.appendDevice(new Screen("PROBE", grid_width, grid_width, dpm));
	acc.appendDevice(new HKick("UMAMS1H", -0.1, 0.1));
 	acc.appendDevice(new VKick("UMAMS1V", -0.1, 0.1));
//	acc.appendDevice(new Screen("PROBE2", grid_width, grid_width, dpm));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.124 + 0.0935 + 0.0335));
//	acc.appendDevice(new Screen("PROBE3", grid_width, grid_width, dpm));
 	acc.appendDevice(new QuadrupoleMagnet("UMAQD11", 0.040, 0.040, 0.318, 0.0, max_quad_strength));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.0335 + 0.0335));
 	acc.appendDevice(new QuadrupoleMagnet("UMAQD12", 0.040, 0.040, 0.318, -max_quad_strength, 0.0));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.0335 + 0.100));
// 	acc.appendDevice(new DriftTube("DUMMY", drift_width, drift_width, 4)); 	
 	acc.appendDevice(new Screen("UMADG1", grid_width, grid_width, dpm)); 
	acc.appendDevice(new ProfileGrid("UMADG1g", grid_width, grid_width, 100));
 	acc.appendDevice(t1); 	
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.460));
  	acc.appendDevice(new RectangularDipoleMagnet("UMAMU1", 0.080, 0.040, -12.5*pi*-7.699/180., -12.5*pi/180.));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.465));
 	acc.appendDevice(new Screen("UMADG2", grid_width, grid_width, dpm)); 
	acc.appendDevice(new ProfileGrid("UMADG2g", grid_width, grid_width, 100));
 	acc.appendDevice(t2); 	
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.465)); 	
  	acc.appendDevice(new RectangularDipoleMagnet("UMAMU2a", 0.080, 0.040, -22.5*pi*-2.739/180., -22.5*pi/180.));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.3335));  	
 	acc.appendDevice(new QuadrupoleMagnet("UMAQD21", 0.040, 0.040, 0.318, 0.0, max_quad_strength));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.0335 + 0.0335));
 	acc.appendDevice(new QuadrupoleMagnet("UMAQD22", 0.040, 0.040, 0.318, -max_quad_strength, 0.0));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.3335));
  	acc.appendDevice(new RectangularDipoleMagnet("UMAMU2b", 0.080, 0.040, -22.5*pi*-2.739/180., -22.5*pi/180.));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.300));
 	acc.appendDevice(new Screen("UMADG3", grid_width, grid_width, dpm));
	acc.appendDevice(new ProfileGrid("UMADG3g", grid_width, grid_width, 100));
 	acc.appendDevice(t3); 	
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.300));
  	acc.appendDevice(new RectangularDipoleMagnet("UMAMU2c", 0.080, 0.040, -22.5*pi*-2.739/180., -22.5*pi/180.));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.3335));
 	acc.appendDevice(new QuadrupoleMagnet("UMAQD31", 0.040, 0.040, 0.318, 0.0, max_quad_strength));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.0335 + 0.0335));
 	acc.appendDevice(new QuadrupoleMagnet("UMAQD32", 0.040, 0.040, 0.318, -max_quad_strength, 0.0));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.491));
 	acc.appendDevice(new Screen("UMADG4", grid_width, grid_width, dpm));
	acc.appendDevice(new ProfileGrid("UMADG4g", grid_width, grid_width, 100));
 	acc.appendDevice(t4); 	
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.275));
 	acc.appendDevice(new HKick("UMAMS2H", -0.1, 0.1));
 	acc.appendDevice(new VKick("UMAMS2V", -0.1, 0.1));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.124 + 0.3435 + 0.100));
  	acc.appendDevice(new RectangularDipoleMagnet("UM2MU5", 0.080, 0.040, -22.5*pi*-2.739/180., -22.5*pi/180.));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.700));
  	acc.appendDevice(new RectangularDipoleMagnet("UM3MU6", 0.080, 0.040, -22.5*pi*-2.739/180., -22.5*pi/180.));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.0195 + 0.124));
 	acc.appendDevice(t5);
 	acc.appendDevice(new HKick("UM1MS3H", -0.1, 0.1));
 	acc.appendDevice(new VKick("UM1MS3V", -0.1, 0.1));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.124 + 0.3325 + 0.0335));
 	acc.appendDevice(new QuadrupoleMagnet("UM3QD41", 0.040, 0.040, 0.318, 0.0, max_quad_strength));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.0335 + 0.0335));
 	acc.appendDevice(new QuadrupoleMagnet("UM3QD42", 0.040, 0.040, 0.318, -max_quad_strength, 0.0));
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 0.249));
 	acc.appendDevice(new Screen("UM3DG6", grid_width, grid_width, dpm));
	acc.appendDevice(new ProfileGrid("UM3DG6g", grid_width, grid_width, 100));
 	acc.appendDevice(t6); 	
 	acc.appendDevice(new DriftTube("", drift_width, drift_width, 2.5845 + 2.400));

 	acc.appendDevice(new Screen("TARGET_SCREENa", grid_width, grid_width, dpm));
	acc.appendDevice(new ProfileGrid("TARGET_SCREENag", grid_width, grid_width, 100));
 	acc.appendDevice(new Slit("TARGET_SIZE", -0.002, 0.002, -0.002, 0.002));
 	acc.appendDevice(new Screen("TARGET_SCREENb", grid_width, grid_width, dpm));
	acc.appendDevice(new ProfileGrid("TARGET_SCREENbg", grid_width, grid_width, 100));
 	acc.appendDevice(t7);
	acc.setScreenIgnore(true);

// 	cout << acc.toString() << endl; 	
 	
	default_random_engine rnd(chrono::high_resolution_clock::now().time_since_epoch().count());

	int number_of_genes       = acc.settingSize();
	int number_of_genomes     = 30;
	int number_of_generations = 300;

	EvolutionParameters ep;
	ep.n_keep                     = 2;
	ep.sigma_survive              = 0.3; // 0.3 scheint ein guter Wert zu sein // 0.1 wirft 60% weg, 0.2 wirft 40% weg
	ep.p_mutate_disturbe          = 0.80; // 0.60
	ep.p_mutate_replace           = 0.05; // 0.05
	ep.p_non_homologous_crossover = 0.025; // 0.05
	ep.b_crossing_over            = true;
	ep.b_mutate_mutation_rate     = true;
	ep.n_min_genes_till_cross     = 2;
	ep.n_max_genes_till_cross     = number_of_genes/2;	
	     
	
	Population p(number_of_genomes, number_of_genes, rnd);
	p.evaluate(fitness_m3_beamline);    

        ofstream outfile;
        outfile.open("fitness.dat", ios::out | ios::trunc );
        outfile << "func\n";

	for( int i=0; i<number_of_generations; i++ ) {    
		p = p.createOffspring(ep, rnd);
		p.evaluate(fitness_m3_beamline);
		outfile << i << "," << p.getBestGenome().fitness() << "\n";
		cout << "Generation " << i << ":\t" << p.toString() << endl;    
	}
    
	outfile.close();
	cout << p.getBestGenome() << endl;    
    
	acc.setScreenIgnore(false);
	acc.setNormValues(p.getBestGenome().getGenes());
 	acc.startSimulation(10000000);
 	cout << acc.toString() << endl; 	
 	
	acc.writeMirkoMakro("mirko.mak");
 	
	((Screen*) acc.getDeviceByName("UMADG1"))->exportHistogram();
	((Screen*) acc.getDeviceByName("UMADG2"))->exportHistogram();
	((Screen*) acc.getDeviceByName("UMADG3"))->exportHistogram();
	((Screen*) acc.getDeviceByName("UMADG4"))->exportHistogram();			
	((Screen*) acc.getDeviceByName("UM3DG6"))->exportHistogram();
	((Screen*) acc.getDeviceByName("TARGET_SCREENa"))->exportHistogram();
	((Screen*) acc.getDeviceByName("TARGET_SCREENb"))->exportHistogram();
//	((Screen*) acc.getDeviceByName("PROBE"))->exportHistogram();
//	((Screen*) acc.getDeviceByName("PROBE2"))->exportHistogram();
//	((Screen*) acc.getDeviceByName("PROBE3"))->exportHistogram();
}

 
 
int main(int argc , char *argv[])
{
	cout << "BRIENCHJEN!" << endl;
	
	clock_t begin = clock();
	time_t start, finish;
	time(&start);	
 	
	//experiment_simple_acc();
	experiment_m3_beamline();

	time(&finish);
	float diff = (((float)clock()-(float)begin)/CLOCKS_PER_SEC);
	cout << "CPU-time: " << diff << " s" << endl;
	cout << "runtime: " << difftime(finish,start) << " s" << endl;

    return 0;
}






