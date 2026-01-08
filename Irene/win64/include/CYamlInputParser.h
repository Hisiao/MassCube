#include <vector>
#include <string>

using namespace std;

typedef vector<int> ivector;
typedef vector<double> dvector;
typedef vector<string> svector;

class YamlInputParser
{
  public:
    YamlInputParser();
    ~YamlInputParser();
    int parseInputFile( const string& strInputFile );
  private:
    int splitLine( string& strSource,
                   char cSplitOn,
                   svector& vstrValues );
    void trimWhitespace( string& strToken );
    int parseStringsToDoubles( const svector& vstrValues, 
                               dvector& vdValues,
                               const int& iExpect = 0 );
    int parseStringsToInts( const svector& vstrValues, 
                            ivector& viValues,
                            const int& iExpect = 0 );
    int validateKeyValues( const svector& vstrKey,
                           svector& vstrValues );
};
