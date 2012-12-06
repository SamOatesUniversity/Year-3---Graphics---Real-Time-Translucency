#include "ProFy.h"


ProFy::ProFy()
{

}

ProFy::~ProFy()
{

}

ProFy *ProFy::Instance()
{
	static ProFy instance;
	return &instance;
}
