# Qualitative Clustering via Integer Linear Programming
Qualitative Clustering via Integer Linear Programming

[ILP.h](ILP.h)

The following 

Installation
------------

only depends on [C++ standard library](http://en.cppreference.com/w/cpp/header), `unistd.h` system header on unix and `windows.h` & `io.h` system headers on windows based systems. In other words, you don't need any 3rd party dependencies.


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

```c++
#include "LPCreator.h"

using namespace std;
using namespace qualitativeclustering;

int main()
{
    cout << "Plain old text"
         << style::bold << "Rang styled text!!"
         << style::reset << endl;
}
```
