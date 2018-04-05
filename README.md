# Qualitative Clustering via Integer Linear Programming
Qualitative Data Clustering via Integer Linear Programming

[ILP.h](ILP.h)

The following 

Installation
------------

[ILP.h](ILP.h) 

[LPCreator.h](LPCreator.h) only depends on [C++ standard library](http://en.cppreference.com/w/cpp/header), you don't need any 3rd party dependencies.


Example usage
-------------

```c++
#include "ILP.h"

using namespace std;
using namespace qualitativeclustering;

int main()
{
    cout << "Plain old text"
         << style::bold << "Rang styled text!!"
         << style::reset << endl;
}
```

You can create LP ([CPLEX LP format](https://www.ibm.com/support/knowledgecenter/SSSA5P_12.5.0/ilog.odms.cplex.help/CPLEX/FileFormats/topics/LP.html)) 

Example usage
-------------

[LPCreator.h](LPCreator.h) is a single header-only library.  

```c++
#include "LPCreator.h"

using namespace std;
using namespace qualitativeclustering;

int main()
{
    // Create GM model in LP format
    createLP("data/1-Lenses.txt", "1-Lenses-GM.lp", 0);
    
    // Create GM1 model in LP format
    createLP("data/1-Lenses.txt", "1-Lenses-GM1.lp", 1);
    
    // Create GM2 model in LP format
    createLP("data/1-Lenses.txt", "1-Lenses-GM2.lp", 2);
    
    // Create GM3 model in LP format
    createLP("data/1-Lenses.txt", "1-Lenses-GM3.lp", 3);
    
}
```
