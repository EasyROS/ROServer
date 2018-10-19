#include <iostream>
#include <TL.hpp>
#include <shell.hpp>

using namespace std;

class root:public sys{
public:
	root(){
		init("root");
		add();
	}
	void add(){
		
	}
};

int main(){
    cout << "Test" << endl;
    
    root *r = new root();
    cout << r->binding->n->name << endl;
    
    test();
    return 0;
}
