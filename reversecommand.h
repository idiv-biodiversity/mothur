#ifndef REVERSECOMMAND_H
#define REVERSECOMMAND_H

/*
 *  reversecommand.h
 *  Mothur
 *
 *  Created by Pat Schloss on 6/6/09.
 *  Copyright 2009 Patrick D. Schloss. All rights reserved.
 *
 */

#include "mothur.h"
#include "command.hpp"
#include "globaldata.hpp"

class ReverseSeqsCommand : public Command {
public:
	ReverseSeqsCommand();
	~ReverseSeqsCommand();
	int execute();
	
private:
	GlobalData* globaldata;	
	
};

#endif