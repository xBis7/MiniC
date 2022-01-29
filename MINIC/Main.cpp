#include <iostream>
#include <fstream>
#include "MiniC.tab.h"
extern FILE* yyin;
using namespace std;

int main(int argc, char** argv) {

	fopen_s(&yyin, "test.txt", "r");
	yy::parser* p = new yy::parser();
	p->parse();

	ofstream* f = new ofstream("STree.dot", std::ofstream::out);
	g_root->Visit_SyntaxTreePrinter(f, nullptr);

	g_root->Visit_Eval(nullptr);

	system("dot -Tgif STree.dot -o STree.gif");

	return 0;
}
