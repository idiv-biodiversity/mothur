/*
 *  catchallcommand.cpp
 *  Mothur
 *
 *  Created by westcott on 5/11/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "catchallcommand.h"
#include "globaldata.hpp"

/**************************************************************************************/
CatchAllCommand::CatchAllCommand(string option)  {	
	try {
		globaldata = GlobalData::getInstance();
		abort = false;
		allLines = 0;
		
		//allow user to run help
		if(option == "help") { help(); abort = true; }
		
		else {
			//valid paramters for this command
			string Array[] =  {"sabund","label","inputdir","outputdir"};
			vector<string> myArray (Array, Array+(sizeof(Array)/sizeof(string)));
			
			OptionParser parser(option);
			map<string,string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string, string>::iterator it;
		
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.validFile(parameters, "inputdir", false);		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("sabund");
				//user has given a template file
				if(it != parameters.end()){ 
					path = hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["sabund"] = inputDir + it->second;		}
				}
			}

			//check for required parameters
			sabundfile = validParameter.validFile(parameters, "sabund", true);
			if (sabundfile == "not open") { sabundfile = ""; abort = true; }
			else if (sabundfile == "not found") { sabundfile = "";  m->mothurOut("You must provide either a sabund file for the catchall command."); m->mothurOutEndLine(); abort=true; }
			else { globaldata->setSabundFile(sabundfile); globaldata->setFormat("sabund"); }
			
			string label = validParameter.validFile(parameters, "label", false);			
			if (label == "not found") { label = ""; }
			else { 
				if(label != "all") {  splitAtDash(label, labels);  allLines = 0;  }
				else { allLines = 1;  }
			}
		

			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.validFile(parameters, "outputdir", false);		if (outputDir == "not found"){	outputDir = "./";	}
		}

	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "CatchAllCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

void CatchAllCommand::help(){
	try {
		m->mothurOut("The catchall command interfaces mothur with the catchall program written by ...citation goes here...\n");
		m->mothurOut("For more information about clearcut refer to http://catchall.cac.cornell.edu/ \n");
		m->mothurOut("The catchall executable must be in a folder called catchall in the same folder as your mothur executable, similar to mothur's requirements for using blast. \n");
		m->mothurOut("The catchall command parameters are sabund and label, sabund is required. \n");
		m->mothurOut("The label parameter is used to analyze specific labels in your input.\n");
		m->mothurOut("The catchall command should be in the following format: \n");
		m->mothurOut("catchall(sabund=yourSabundFile) \n");
		m->mothurOut("Example: catchall(sabund=abrecovery.fn.sabund) \n");	
	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "help");
		exit(1);
	}
}

/**************************************************************************************/
int CatchAllCommand::execute() {	
	try {
		
		if (abort == true) { return 0; }
		
		vector<string> outputNames;
		
		//prepare full output directory
		outputDir = getFullPathName(outputDir);
		
		//get location of catchall
		GlobalData* globaldata = GlobalData::getInstance();
		string path = globaldata->argv;
		path = path.substr(0, (path.find_last_of('m')));
		path = getFullPathName(path);

		string catchAllCommandExe = ""; 
		#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux)
			catchAllCommandExe += "mono " + path + "catchall/CatchAllcmdL.exe ";
		#else
			catchAllCommandExe += path + "catchall/CatchAllcmdW.exe ";
		#endif
		
		read = new ReadOTUFile(sabundfile);	
		read->read(&*globaldata); 
		
		SAbundVector* sabund = globaldata->sabund;
		string lastLabel = sabund->getLabel();
		input = globaldata->ginput;
						
		set<string> processedLabels;
		set<string> userLabels = labels;
		
		//for each label the user selected
		while((sabund != NULL) && ((allLines == 1) || (userLabels.size() != 0))) {

					
			if(allLines == 1 || labels.count(sabund->getLabel()) == 1){
					m->mothurOut(sabund->getLabel());  m->mothurOutEndLine();
					
					//create list of output files catchall will make
					//datasetname_Analysis.csv 
					//datasetname_BestModelsAnalysis.csv
					//datasetname_BestModelsFits.csv 
					//datasetname_BubblePlot.csv 
					//datasetname_Poisson_fits.csv 
					//datasetname_SingleExp_fits.csv
					//datasetname_ThreeMixedExp_fits.csv
					//datasetname_TwoMixedExp_fits.csv

					//create catchall input file from mothur's inputfile
					string filename = process(sabund);
	sabund->print(cout);							
					//create system command
					string catchAllCommand = catchAllCommandExe + filename + " " + path;
cout << catchAllCommand << endl;
					//run catchall
					system(catchAllCommand.c_str());
										
					if (m->control_pressed) { delete read;  delete input; globaldata->ginput = NULL; delete sabund;  return 0; }

					processedLabels.insert(sabund->getLabel());
					userLabels.erase(sabund->getLabel());
			}
			
			if ((anyLabelsToProcess(sabund->getLabel(), userLabels, "") == true) && (processedLabels.count(lastLabel) != 1)) {
					string saveLabel = sabund->getLabel();
					
					delete sabund;		
					sabund = (input->getSAbundVector(lastLabel));
					
					m->mothurOut(sabund->getLabel());  m->mothurOutEndLine();
					
					//create list of output files catchall will make
					//datasetname_Analysis.csv 
					//datasetname_BestModelsAnalysis.csv
					//datasetname_BestModelsFits.csv 
					//datasetname_BubblePlot.csv 
					//datasetname_Poisson_fits.csv 
					//datasetname_SingleExp_fits.csv
					//datasetname_ThreeMixedExp_fits.csv
					//datasetname_TwoMixedExp_fits.csv

					
					//create catchall input file from mothur's inputfile
					string filename = process(sabund);
			
					//create system command
					string catchAllCommand = catchAllCommandExe + filename + " " + path;
cout << catchAllCommand << endl;
					//run catchall
					system(catchAllCommand.c_str());

					if (m->control_pressed) { delete read;  delete input; globaldata->ginput = NULL; delete sabund;  return 0; }

					processedLabels.insert(sabund->getLabel());
					userLabels.erase(sabund->getLabel());
					
					//restore real lastlabel to save below
					sabund->setLabel(saveLabel);
			}
			
			
			lastLabel = sabund->getLabel();	
			
			delete sabund;		
			sabund = (input->getSAbundVector());
		}
		
		//output error messages about any remaining user labels
		set<string>::iterator it;
		bool needToRun = false;
		for (it = userLabels.begin(); it != userLabels.end(); it++) {  
			m->mothurOut("Your file does not include the label " + *it); 
			if (processedLabels.count(lastLabel) != 1) {
				m->mothurOut(". I will use " + lastLabel + ".");  m->mothurOutEndLine();
				needToRun = true;
			}else {
				m->mothurOut(". Please refer to " + lastLabel + ".");  m->mothurOutEndLine();
			}
		}
		
		//run last label if you need to
		if (needToRun == true)  {
			if (sabund != NULL) {	delete sabund;	}
			sabund = (input->getSAbundVector(lastLabel));
			
			m->mothurOut(sabund->getLabel());  m->mothurOutEndLine();
			//create list of output files catchall will make
			//datasetname_Analysis.csv 
			//datasetname_BestModelsAnalysis.csv
			//datasetname_BestModelsFits.csv 
			//datasetname_BubblePlot.csv 
			//datasetname_Poisson_fits.csv 
			//datasetname_SingleExp_fits.csv
			//datasetname_ThreeMixedExp_fits.csv
			//datasetname_TwoMixedExp_fits.csv

			//create catchall input file from mothur's inputfile
			string filename = process(sabund);

			//create system command
			string catchAllCommand = catchAllCommandExe + filename + " " + path;
cout << catchAllCommand << endl;
			//run catchall
			system(catchAllCommand.c_str());
	
			
			delete sabund;
		}

		delete read;
		delete input; globaldata->ginput = NULL;
		
		m->mothurOutEndLine();
		m->mothurOut("Output File Names: "); m->mothurOutEndLine();
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i]); m->mothurOutEndLine();	}	
		m->mothurOutEndLine();
		

		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************
string CatchAllCommand::process(SAbundVector* sabund) {
	try {
		string filename = outputDir + getRootName(getSimpleName(sabundfile)) + sabund->getLabel() + ".catchall";
		filename = getFullPathName(filename);
	
		ofstream out;
		openOutputFile(filename, out);
		
		for (int i = 1; i <= sabund->getMaxRank(); i++) {
			int temp = sabund->get(i);
			
			if (temp != 0) {
				out << i << "," << temp << endl;
			}
		}
		out.close();
		
		return filename;
	
	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "process");
		exit(1);
	}
}
/**************************************************************************************/


