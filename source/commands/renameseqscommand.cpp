//
//  renameseqscommand.cpp
//  Mothur
//
//  Created by SarahsWork on 5/28/13.
//  Copyright (c) 2013 Schloss Lab. All rights reserved.
//

#include "renameseqscommand.h"
#include "sequence.hpp"
#include "groupmap.h"
#include "counttable.h"
#include "qualityscores.h"

//**********************************************************************************************************************
vector<string> RenameSeqsCommand::setParameters(){
	try {
        CommandParameter pfile("file", "InputTypes", "", "", "fileFasta-file", "fileFasta", "none","fasta",false,false,true); parameters.push_back(pfile);
        CommandParameter pmap("map", "InputTypes", "", "", "none", "none", "none","fasta",false,false,true); parameters.push_back(pmap);
		CommandParameter pfasta("fasta", "InputTypes", "", "", "fileFasta-file", "fileFasta", "none","fasta",false,false,true); parameters.push_back(pfasta);
        CommandParameter pqfile("qfile", "InputTypes", "", "", "file", "none", "none","qfile",false,false,true); parameters.push_back(pqfile);
        CommandParameter pcontigsreport("contigsreport", "InputTypes", "", "", "file", "none", "none","contigsreport",false,false,true); parameters.push_back(pcontigsreport);
        CommandParameter pname("name", "InputTypes", "", "", "NameCount-file", "none", "none","name",false,false,true); parameters.push_back(pname);
        CommandParameter pcount("count", "InputTypes", "", "", "NameCount-CountGroup-file", "none", "none","count",false,false,true); parameters.push_back(pcount);
        CommandParameter pgroup("group", "InputTypes", "", "", "CountGroup-file", "none", "none","group",false,false,true); parameters.push_back(pgroup);
        CommandParameter pdelim("delim", "String", "", "_", "", "", "","",false,false); parameters.push_back(pdelim);
        CommandParameter pplacement("placement", "Multiple", "front-back", "back", "", "", "","",false,false); parameters.push_back(pplacement);
		CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "RenameSeqsCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string RenameSeqsCommand::getHelpString(){
	try {
		string helpString = "";
		helpString += "The rename.seqs command reads a fastafile or file file and renames the sequences using a count. Optionally you can add a groupfile or count file with an optional namefile. Then it will create files with the sequence names concatenated with the group.";
		helpString += "The rename.seqs command parameters are fasta, name, group, count, qfile, placement, contigsreport and delim. Fasta is required, unless a current file is available for both.\n";
        helpString += "The qfile allows you to provide an associated quality file.\n";
        helpString += "The contigsreport allows you to provide an associated contigsreport file.\n";
        helpString += "The file option allows you to provide a 2 or 3 column file. The first column contains the file type: fasta or qfile. The second column is the filename, and the optional third column can be a group name. If there is a third column, all sequences in the file will be assigned to that group.  This can be helpful when renaming data separated into samples. \n";
        helpString += "The placement parameter allows you to indicate whether you would like the group name appended to the front or back of the sequence number.  Options are front or back. Default=back.\n";
        helpString += "The delim parameter allow you to enter the character or characters you would like to separate the sequence number from the group name. Default='_'.\n";
        helpString += "The rename.seqs command should be in the following format: \n";
        helpString += "The rename.seqs command should be in the following format: \n";
		helpString += "rename.seqs(fasta=yourFastaFile, group=yourGroupFile) \n";
		helpString += "Example rename.seqs(fasta=abrecovery.unique.fasta, group=abrecovery.group).\n";
		helpString += "Note: No spaces between parameter labels (i.e. fasta), '=' and parameters (i.e.yourFasta).\n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "RenameSeqsCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string RenameSeqsCommand::getOutputPattern(string type) {
    try {
        string pattern = "";
        
        if (type == "fasta")                    {  pattern = "[filename],renamed,[extension]"; }
        else if (type == "name")                {  pattern = "[filename],renamed,[extension]"; }
        else if (type == "group")               {  pattern = "[filename],renamed,[extension]"; }
        else if (type == "count")               {  pattern = "[filename],renamed,[extension]"; }
        else if (type == "qfile")               {  pattern = "[filename],renamed,[extension]"; }
        else if (type == "contigsreport")       {  pattern = "[filename],renamed,[extension]"; }
        else if (type == "map")                 {  pattern = "[filename],renamed_map"; }
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->control_pressed = true;  }
        
        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "RenameSeqsCommand", "getOutputPattern");
        exit(1);
    }
}
//**********************************************************************************************************************
RenameSeqsCommand::RenameSeqsCommand(){
	try {
		abort = true; calledHelp = true;
		setParameters();
		vector<string> tempOutNames;
        outputTypes["fasta"] = tempOutNames;
        outputTypes["name"] = tempOutNames;
        outputTypes["group"] = tempOutNames;
        outputTypes["count"] = tempOutNames;
        outputTypes["map"] = tempOutNames;
        outputTypes["qfile"] = tempOutNames;
        outputTypes["contigsreport"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "RenameSeqsCommand", "RenameSeqsCommand");
		exit(1);
	}
}
/**************************************************************************************/
RenameSeqsCommand::RenameSeqsCommand(string option)  {
	try {
		abort = false; calledHelp = false;
		
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string,string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string, string>::iterator it;
            
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) {
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
			vector<string> tempOutNames;
            outputTypes["fasta"] = tempOutNames;
            outputTypes["name"] = tempOutNames;
            outputTypes["group"] = tempOutNames;
            outputTypes["count"] = tempOutNames;
            outputTypes["map"] = tempOutNames;
            outputTypes["qfile"] = tempOutNames;
            outputTypes["contigsreport"] = tempOutNames;
            
			//if the user changes the input directory command factory will send this info to us in the output parameter
			string inputDir = validParameter.validFile(parameters, "inputdir", false);
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("fasta");
				//user has given a template file
				if(it != parameters.end()){
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["fasta"] = inputDir + it->second;		}
				}
				
				it = parameters.find("name");
				//user has given a template file
				if(it != parameters.end()){
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["name"] = inputDir + it->second;		}
				}
                
                it = parameters.find("group");
				//user has given a template file
				if(it != parameters.end()){
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["group"] = inputDir + it->second;		}
				}
                
                it = parameters.find("count");
                //user has given a template file
                if(it != parameters.end()){
                    path = m->hasPath(it->second);
                    //if the user has not given a path then, add inputdir. else leave path alone.
                    if (path == "") {	parameters["count"] = inputDir + it->second;		}
                }
                
                it = parameters.find("qfile");
                //user has given a template file
                if(it != parameters.end()){
                    path = m->hasPath(it->second);
                    //if the user has not given a path then, add inputdir. else leave path alone.
                    if (path == "") {	parameters["qfile"] = inputDir + it->second;		}
                }
                
                it = parameters.find("contigsreport");
                //user has given a template file
                if(it != parameters.end()){
                    path = m->hasPath(it->second);
                    //if the user has not given a path then, add inputdir. else leave path alone.
                    if (path == "") {	parameters["contigsreport"] = inputDir + it->second;		}
                }
                
                it = parameters.find("map");
                //user has given a template file
                if(it != parameters.end()){
                    path = m->hasPath(it->second);
                    //if the user has not given a path then, add inputdir. else leave path alone.
                    if (path == "") {	parameters["map"] = inputDir + it->second;		}
                }
                
                it = parameters.find("file");
                //user has given a template file
                if(it != parameters.end()){
                    path = m->hasPath(it->second);
                    //if the user has not given a path then, add inputdir. else leave path alone.
                    if (path == "") {	parameters["file"] = inputDir + it->second;		}
                }
			}
            
            //if the user changes the output directory command factory will send this info to us in the output parameter
            outputDir = validParameter.validFile(parameters, "outputdir", false);		if (outputDir == "not found"){ outputDir = ""; }
			
			//check for required parameters
			fastaFile = validParameter.validFile(parameters, "fasta", true);
			if (fastaFile == "not open") { abort = true; }
			else if (fastaFile == "not found") { fastaFile = ""; }
			else { m->setFastaFile(fastaFile); }
            
            fileFile = validParameter.validFile(parameters, "file", true);
            if (fileFile == "not open") { abort = true; }
            else if (fileFile == "not found") { fileFile = ""; }
            else { m->setFileFile(fileFile); }
            
            if ((fastaFile == "") && (fileFile == "")) {
                fastaFile = m->getFastaFile();
                if (fastaFile != "") { m->mothurOut("Using " + fastaFile + " as input file for the fasta parameter."); m->mothurOutEndLine(); }
                else {
                    fileFile = m->getFileFile();
                    if (fileFile != "") { m->mothurOut("Using " + fileFile + " as input file for the file parameter."); m->mothurOutEndLine(); }
                    else {  m->mothurOut("You have no current fastafile or file file and the fasta or file parameter is required."); m->mothurOutEndLine(); abort = true; }
                }
            }
			
            groupfile = validParameter.validFile(parameters, "group", true);
            if (groupfile == "not open") { abort = true; }
            else if (groupfile == "not found") {  groupfile = ""; }
			else { m->setGroupFile(groupfile); }

            countfile = validParameter.validFile(parameters, "count", true);
            if (countfile == "not open") { countfile = ""; abort = true; }
            else if (countfile == "not found") { countfile = ""; }
            else {  m->setCountTableFile(countfile);  }
            
            nameFile = validParameter.validFile(parameters, "name", true);
            if (nameFile == "not open") { abort = true; }
            else if (nameFile == "not found"){ nameFile =""; }
            else { m->setNameFile(nameFile); }
            
            qualfile = validParameter.validFile(parameters, "qfile", true);
            if (qualfile == "not open") { abort = true; }
            else if (qualfile == "not found"){ qualfile =""; }
            else { m->setQualFile(qualfile); }
            
            mapFile = validParameter.validFile(parameters, "map", true);
            if (mapFile == "not open") { abort = true; }
            else if (mapFile == "not found"){ mapFile = ""; }
            
            contigsfile = validParameter.validFile(parameters, "contigsreport", true);
            if (contigsfile == "not open") { abort = true; }
            else if (contigsfile == "not found"){ contigsfile = ""; }
            
            if ((countfile != "") && (nameFile != "")) { m->mothurOut("[ERROR]: You must enter ONLY ONE of the following: count or name."); m->mothurOutEndLine(); abort = true; }
            
            if ((fileFile != "") && (fastaFile != "")) { m->mothurOut("[ERROR]: You must enter ONLY ONE of the following: file or fasta."); m->mothurOutEndLine(); abort = true; }
            
            if ((countfile != "") && (groupfile != "")) { m->mothurOut("[ERROR]: You must enter ONLY ONE of the following: count or group."); m->mothurOutEndLine(); abort = true; }
            
            if ((fileFile != "") && ((nameFile != "") || (groupfile != "") || (qualfile != "") || (contigsfile != "") || (countfile != "") || (fastaFile != "")) ) {
                m->mothurOut("[ERROR]: The file option cannot be used with any other files except the map file."); m->mothurOutEndLine(); abort = true;
            }

            placement = validParameter.validFile(parameters, "placement", false);		if (placement == "not found") { placement = "back"; }
            if ((placement == "front") || (placement == "back")) { }
            else { m->mothurOut("[ERROR]: " + placement + " is not a valid placement option.  Valid placement options are front or back.\n"); abort = true; }
            
            delim = validParameter.validFile(parameters, "delim", false);			if (delim == "not found") { delim = "_"; }
            
            if (countfile == "") {
                if (nameFile == "")  {
                    vector<string> files; files.push_back(fastaFile);
                    parser.getNameFile(files);
                }
            }
		}
        
	}
	catch(exception& e) {
		m->errorOut(e, "RenameSeqsCommand", "RenameSeqsCommand");
		exit(1);
	}
}
/**************************************************************************************/
int RenameSeqsCommand::execute() {
	try {
		
		if (abort == true) { if (calledHelp) { return 0; }  return 2;	}
        
        map<string, string> renameMap;
        if (mapFile != "") { readMapFile(renameMap);  }
        
        if (fileFile != "") {  processFile(renameMap); }
        else {
            
            //prepare filenames and open files
            string thisOutputDir = outputDir;
            if (outputDir == "") {  thisOutputDir += m->hasPath(fastaFile);  }
            string outFastaFile = thisOutputDir + m->getRootName(m->getSimpleName(fastaFile));
            map<string, string> variables;
            variables["[filename]"] = outFastaFile;
            variables["[extension]"] = m->getExtension(fastaFile);
            outFastaFile = getOutputFileName("fasta", variables);
            outputNames.push_back(outFastaFile); outputTypes["fasta"].push_back(outFastaFile);
            
            string outMapFile = thisOutputDir + m->getRootName(m->getSimpleName(fastaFile));
            variables["[filename]"] = outMapFile;
            outMapFile = getOutputFileName("map", variables);
            outputNames.push_back(outMapFile); outputTypes["map"].push_back(outMapFile);
            ofstream outMap; m->openOutputFile(outMapFile, outMap);
            
            ofstream outName;
            map<string, vector<string> > nameMap;
            if (nameFile != "") {
                thisOutputDir = outputDir;
                if (outputDir == "") {  thisOutputDir += m->hasPath(nameFile);  }
                string outNameFile = thisOutputDir + m->getRootName(m->getSimpleName(nameFile));
                variables["[filename]"] = outNameFile;
                variables["[extension]"] = m->getExtension(nameFile);
                outNameFile = getOutputFileName("name", variables);
                outputNames.push_back(outNameFile); outputTypes["name"].push_back(outNameFile);
                
                m->openOutputFile(outNameFile, outName);
                m->readNames(nameFile, nameMap);
            }
            
            ofstream outGroup;
            if (groupfile != "") {
                thisOutputDir = outputDir;
                if (outputDir == "") {  thisOutputDir += m->hasPath(groupfile);  }
                string outGroupFile = thisOutputDir + m->getRootName(m->getSimpleName(groupfile));
                variables["[filename]"] = outGroupFile;
                variables["[extension]"] = m->getExtension(groupfile);
                outGroupFile = getOutputFileName("group", variables);
                outputNames.push_back(outGroupFile); outputTypes["group"].push_back(outGroupFile);
                
                m->openOutputFile(outGroupFile, outGroup);
            }
            
            ofstream outFasta;
            m->openOutputFile(outFastaFile, outFasta);
            
            ifstream in;
            m->openInputFile(fastaFile, in);
            
            GroupMap* groupMap = NULL;
            CountTable* countTable = NULL;
            
            bool hasGroups = false;
            vector<string> allGroups; allGroups.push_back("");
            m->setAllGroups(allGroups);
            if (groupfile != "") {
                groupMap = new GroupMap(groupfile);
                
                int groupError = groupMap->readMap();
                if (groupError == 1) { delete groupMap; return 0; }
                allGroups = groupMap->getNamesOfGroups();
                m->setAllGroups(allGroups);
                hasGroups = true;
            }else if (countfile != "") {
                countTable = new CountTable();
                countTable->readTable(countfile, true, false);
                hasGroups = countTable->hasGroupInfo();
                if (hasGroups) {     allGroups = countTable->getNamesOfGroups(); allGroups.push_back("Multi"); m->setAllGroups(allGroups); }
            }
            
            //set up for reads
            map<string, int> counts; for (int i = 0; i < allGroups.size(); i++) {  counts[allGroups[i]] = 1; }
            map<string, string> old2NewNameMap;
            bool fillOld2NewNameMap = false;
            if ((qualfile != "") || (contigsfile != "")) { fillOld2NewNameMap = true; }
            
            while (!in.eof()) {
                if (m->control_pressed) { break; }
                
                Sequence seq(in); m->gobble(in);
                
                vector<string> dups;
                if (nameFile != "") {
                    map<string, vector<string> >::iterator it = nameMap.find(seq.getName());
                    if (it == nameMap.end()) {  m->mothurOut("[ERROR]: " + seq.getName() + " is not in your name file, please correct.\n"); m->control_pressed = true;  }
                    else { dups = it->second; }
                }else { dups.push_back(seq.getName()); }
                
                for (int i = 0; i < dups.size(); i++) {
                    string group = "";
                    
                    if (groupfile != "") {
                        group = groupMap->getGroup(dups[i]);
                    }else if (countfile != "") {
                        if (hasGroups) {
                            vector<string> groups = countTable->getGroups(seq.getName());
                            if (group.size() == 0)      {   group = "not found";    }
                            else if (group.size() == 1) {   group = groups[0];      }
                            else                        {   group = "Multi";        }
                        }
                    }
                    
                    if (group == "not found") {  m->mothurOut("[ERROR]: " + seq.getName() + " is not in your file, please correct.\n"); m->control_pressed = true; }
                    else {
                        //get new name
                        string newName = "";
                        if (mapFile != "") {
                            map<string, string>::iterator itMap = renameMap.find(dups[i]);
                            if (itMap == renameMap.end()) { m->mothurOut("[ERROR]: " + dups[i] + " is not in your map file, please correct.\n"); m->control_pressed = true;}
                            else { newName = itMap->second; }
                        }else {
                            newName = toString(counts[group]); counts[group]++;
                            if ((placement == "back") && (group != "")) { newName += delim + group; }
                            else if (group != "") { newName = group + delim + newName; }
                        }
                        
                        if (i == 0) {
                            seq.setName(newName);
                            seq.printSequence(outFasta);
                        }
                        
                        if (countfile != "") { countTable->renameSeq(seq.getName(), newName); }
                        if (groupfile != "") { outGroup << newName << '\t' << group << endl;  }
                        
                        if ((nameFile != "") && (i == 0))  { outName << newName << '\t' << newName;  }
                        else { outName << "," << newName;  }
                        
                        outMap << newName << '\t' << dups[i] << endl;
                        
                        if (fillOld2NewNameMap) {  old2NewNameMap[dups[i]] = newName; }
                    }
                }
                if (nameFile != "") { outName << endl;  }
            }
            in.close();
            outMap.close();
            if (nameFile != "") { outName.close(); }
            renameMap.clear();
            
            if (m->control_pressed) {  if (groupMap != NULL) { delete groupMap; } if (countTable != NULL) { delete countTable; } for (int i = 0; i < outputNames.size(); i++) { m->mothurRemove(outputNames[i]);  } return 0; }
            
            
            if (groupfile != "") {   outGroup.close(); }
            else if (countfile != "") {
                thisOutputDir = outputDir;
                if (outputDir == "") {  thisOutputDir += m->hasPath(countfile);  }
                string outCountFile = thisOutputDir + m->getRootName(m->getSimpleName(countfile));
                variables["[filename]"] = outCountFile;
                variables["[extension]"] = m->getExtension(countfile);
                outCountFile = getOutputFileName("count", variables);
                outputNames.push_back(outCountFile); outputTypes["count"].push_back(outCountFile);
                countTable->printTable(outCountFile);
            }
            
            if (groupMap != NULL) { delete groupMap; }
            if (countTable != NULL) { delete countTable; }
            
            if (qualfile != "")         { readQual(old2NewNameMap);     }
            if (contigsfile != "")      { readContigs(old2NewNameMap);  }
        }
        
        if (m->control_pressed) {  for (int i = 0; i < outputNames.size(); i++) { m->mothurRemove(outputNames[i]);  } return 0; }

        m->mothurOutEndLine();
        m->mothurOut("Output File Names: "); m->mothurOutEndLine();
        for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i]); m->mothurOutEndLine();	}
        m->mothurOutEndLine();
        
        //set fasta file as new current fastafile
        string current = "";
        itTypes = outputTypes.find("fasta");
        if (itTypes != outputTypes.end()) {
            if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setFastaFile(current); }
        }
        
        itTypes = outputTypes.find("name");
        if (itTypes != outputTypes.end()) {
            if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setNameFile(current); }
        }
        
        itTypes = outputTypes.find("group");
        if (itTypes != outputTypes.end()) {
            if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setGroupFile(current); }
        }
        
        itTypes = outputTypes.find("count");
        if (itTypes != outputTypes.end()) {
            if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setCountTableFile(current); }
        }
        
        itTypes = outputTypes.find("qfile");
        if (itTypes != outputTypes.end()) {
            if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setQualFile(current); }
        }
				
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "RenameSeqsCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************
int RenameSeqsCommand::readQual(map<string, string>& oldMap){
    try {
        string thisOutputDir = outputDir;
        if (outputDir == "") {  thisOutputDir += m->hasPath(qualfile);  }
        map<string, string> variables;
        variables["[filename]"] = thisOutputDir + m->getRootName(m->getSimpleName(qualfile));
        variables["[extension]"] = m->getExtension(qualfile);
        string outputFileName = getOutputFileName("qfile", variables);
        ofstream out;
        m->openOutputFile(outputFileName, out);
        outputNames.push_back(outputFileName); outputTypes["qfile"].push_back(outputFileName);

        ifstream in;
        m->openInputFile(qualfile, in);
        
        map<string, string>::iterator it;
        
        while(!in.eof()){
            if (m->control_pressed) { break; }
            
            QualityScores qual(in); m->gobble(in);
            
            it = oldMap.find(qual.getName());
            if (it == oldMap.end()) {
                m->mothurOut("[ERROR]: " + qual.getName() + " is not in your quality file, please correct.\n"); m->control_pressed = true;
            }else {
                qual.setName(it->second);
            }
            
            qual.printQScores(out);
        }
        in.close();
        out.close();
        
        return 0;
        
    }
    catch(exception& e) {
        m->errorOut(e, "RenameSeqsCommand", "readQual");
        exit(1);
    }
}
//**********************************************************************************************************************
int RenameSeqsCommand::readContigs(map<string, string>& oldMap){
    try {
        string thisOutputDir = outputDir;
        if (outputDir == "") {  thisOutputDir += m->hasPath(contigsfile);  }
        map<string, string> variables;
        variables["[filename]"] = thisOutputDir + m->getRootName(m->getSimpleName(contigsfile));
        variables["[extension]"] = m->getExtension(contigsfile);
        string outputFileName = getOutputFileName("contigsreport", variables);
        ofstream out;
        m->openOutputFile(outputFileName, out);
        outputNames.push_back(outputFileName); outputTypes["contigsreport"].push_back(outputFileName);

        ifstream in;
        m->openInputFile(contigsfile, in);
        out << (m->getline(in)) << endl;   //skip headers
        
        map<string, string>::iterator it;
        int length, OLength, thisOStart, thisOEnd, numMisMatches, numNs;
        string name;
        while (!in.eof()) {
            
            if (m->control_pressed) { break; }
            
            //seqname	start	end	nbases	ambigs	polymer	numSeqs
            in >> name >> length >> OLength >> thisOStart >> thisOEnd >> numMisMatches >> numNs; m->gobble(in);
            
            it = oldMap.find(name);
            if (it == oldMap.end()) {
                m->mothurOut("[ERROR]: " + name + " is not in your contigs report file, please correct.\n"); m->control_pressed = true;
            }else {
                name = it->second;
            }

            out << name << '\t' << length  << '\t' << OLength  << '\t' << thisOStart  << '\t' << thisOEnd  << '\t' << numMisMatches  << '\t' << numNs << endl;
        }
        in.close();
        out.close();

        
        return 0;
    }
    catch(exception& e) {
        m->errorOut(e, "RenameSeqsCommand", "readContigs");
        exit(1);
    }
}
//**********************************************************************************************************************
int RenameSeqsCommand::readFasta(string thisFastaFile, map<string, string>& oldMap){
    try {
        string thisOutputDir = outputDir;
        if (outputDir == "") {  thisOutputDir += m->hasPath(thisFastaFile);  }
        map<string, string> variables;
        variables["[filename]"] = thisOutputDir + m->getRootName(m->getSimpleName(thisFastaFile));
        variables["[extension]"] = m->getExtension(thisFastaFile);
        string outputFileName = getOutputFileName("fasta", variables);
        ofstream out;
        m->openOutputFile(outputFileName, out);
        outputNames.push_back(outputFileName); outputTypes["fasta"].push_back(outputFileName);
        
        ifstream in;
        m->openInputFile(thisFastaFile, in);
        
        map<string, string>::iterator it;
        while (!in.eof()) {
            
            if (m->control_pressed) { break; }
            
            Sequence seq(in); m->gobble(in);
            
            it = oldMap.find(seq.getName());
            if (it == oldMap.end()) {
                m->mothurOut("[ERROR]: " + seq.getName() + " is not in your fasta file, please correct.\n"); m->control_pressed = true;
            }else {
                seq.setName(it->second);
            }
            seq.printSequence(out);
        }
        in.close();
        out.close();
        
        return 0;
    }
    catch(exception& e) {
        m->errorOut(e, "RenameSeqsCommand", "readFasta");
        exit(1);
    }
}
//**********************************************************************************************************************
int RenameSeqsCommand::readMapFile(map<string, string>& readMap){
    try {
        ifstream in;
        m->openInputFile(mapFile, in);
        
        map<string, string>::iterator it;
        string oldname, newname;
        while (!in.eof()) {
            
            if (m->control_pressed) { break; }
            
            in >> oldname; m->gobble(in);
            in >> newname; m->gobble(in);
            
            it = readMap.find(oldname);
            if (it != readMap.end()) {
                m->mothurOut("[ERROR]: " + oldname + " is already in your map file. Sequence names must be unique, quitting.\n"); m->control_pressed = true;
            }else {
                readMap[oldname] = newname;
            }
            
        }
        in.close();
        
        return 0;
    }
    catch(exception& e) {
        m->errorOut(e, "RenameSeqsCommand", "readMapFile");
        exit(1);
    }
}
//**********************************************************************************************************************
int RenameSeqsCommand::processFile(map<string, string>& readMap){
    try {
        
        vector<map<string, string> > files = readFiles();
        
        for (int i = 0; i < files.size(); i++) {
            if (m->control_pressed) { break; }
            
            string thisFile = "";
            string thisFileType = "";
            string group = (files[i].begin())->second;
            string temp = (files[i].begin())->first;
            
            int pos = temp.find_first_of('-');
            if (pos == string::npos) {  m->mothurOut("[Opps]: I should never get here...\n");  }
            else {
                thisFileType = temp.substr(0, pos);
                thisFile = temp.substr(pos+1);
            }
            
            string thisOutputDir = outputDir;
            if (outputDir == "") {  thisOutputDir += m->hasPath(thisFile);  }
            string outMapFile = thisOutputDir + m->getSimpleName(thisFile);
            map<string, string> variables;
            variables["[filename]"] = outMapFile + ".";
            outMapFile = getOutputFileName("map", variables);
            outputNames.push_back(outMapFile); outputTypes["map"].push_back(outMapFile);
            ofstream outMap; m->openOutputFile(outMapFile, outMap);
            
            //prepare filenames and open files
            string outFile = thisOutputDir + m->getRootName(m->getSimpleName(thisFile));
            variables["[filename]"] = outFile;
            variables["[extension]"] = m->getExtension(thisFile);
            outFile = getOutputFileName(thisFileType, variables);
            outputNames.push_back(outFile); outputTypes[thisFileType].push_back(outFile);
            
            ofstream out;
            m->openOutputFile(outFile, out);
            
            ifstream in;
            m->openInputFile(thisFile, in);
            
            int count = 1;
            while (!in.eof()) {
                if (m->control_pressed) { break; }
                
                Sequence* seq;  QualityScores* qual;
                string name = "";
                if (thisFileType == "fasta")        {  seq = new Sequence(in);   name = seq->getName();        }
                else if (thisFileType == "qfile")   {  qual = new QualityScores(in);  name = qual->getName();  }
                else if (thisFileType == "group")   {  in >> name; m->gobble(in); in >> group;                 }
                m->gobble(in);
 
                //get new name
                string newName = "";
                if (mapFile != "") {
                    map<string, string>::iterator itMap = readMap.find(name);
                    if (itMap == readMap.end()) { m->mothurOut("[ERROR]: " + name + " is not in your map file, please correct.\n"); m->control_pressed = true;}
                    else { newName = itMap->second; }
                }else {
                    newName = toString(count); count++;
                    if ((placement == "back") && (group != "")) { newName += delim + group; }
                    else if (group != "") { newName = group + delim + newName; }
                }
                
                
                if (thisFileType == "fasta")        {  seq->setName(newName);  seq->printSequence(out); delete seq;     }
                if (thisFileType == "qfile")        {  qual->setName(newName);  qual->printQScores(out); delete qual;   }
                if (thisFileType == "group")        {  out << newName << '\t' << group << endl;                         }
                
                outMap << newName << '\t' << name << endl;
            }
            in.close();
            outMap.close();
            out.close();
        }
        
        
        return 0;
    }
    catch(exception& e) {
        m->errorOut(e, "RenameSeqsCommand", "processFile");
        exit(1);
    }
}
//**********************************************************************************************************************
vector<map<string, string> > RenameSeqsCommand::readFiles(){
    try {
        
        vector<map<string, string> > files;
        
        ifstream in;
        m->openInputFile(fileFile, in);
        
        while (!in.eof()) {
            if (m->control_pressed) { break; }
            
            string line = m->getline(in);  m->gobble(in);
            vector<string> pieces = m->splitWhiteSpace(line);
            
            string group = "";
            string thisFileName; thisFileName = "";
            if (pieces.size() == 2) {
                thisFileName = pieces[0]+"-"+pieces[1];
            }else if (pieces.size() == 3) {
                thisFileName = pieces[0]+"-"+pieces[1];
                group = pieces[2];
                m->checkGroupName(group);
            }else {
                m->mothurOut("[ERROR]: Your file contains " + toString(pieces.size()) + " columns. TThe file option allows you to provide a 2 or 3 column file. The first column contains the file type: fasta or qfile. The second column is the filename, and the optional third column can be a group name. If there is a third column, all sequences in the file will be assigned to that group.  This can be helpful when renaming data separated into samples.\n"); m->control_pressed = true;
            }
            
            map<string, string> temp; temp[thisFileName] = group;
            files.push_back(temp);
        }
        in.close();
        
        return files;
    }
    catch(exception& e) {
        m->errorOut(e, "RenameSeqsCommand", "readFiles");
        exit(1);
    }
}
/**************************************************************************************/

