#ifndef GNUPLOT_H
#define GNUPLOT_H

// System Headers
#include <iostream>
#include <string>
using namespace std;

class Gnuplot{
public:
	Gnuplot();
	~Gnuplot();
	void operator() (const string & command);

protected:
	FILE *gnuplotpipe;
};

Gnuplot::Gnuplot()//default structure
{
	gnuplotpipe=popen("gnuplot -persist","w");
	if(!gnuplotpipe)
		cerr<<("Gnuplot not found !");
}
Gnuplot::~Gnuplot()//default destructure
{
	fprintf(gnuplotpipe,"exit\n");
	pclose(gnuplotpipe);
}
void Gnuplot::operator() (const string & command)
{
	fprintf(gnuplotpipe,"%s\n",command.c_str());
	fflush(gnuplotpipe);
}


#endif
