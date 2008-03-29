#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <string>
#include <vector>

using namespace std;

class Resource
{
public:
	Resource(const string identifier, const vector<unsigned char>& data);
	virtual ~Resource();
	
	string Identifier() const;
	const vector<unsigned char> * Data() const;
	
private:
	string m_Identifier;
	const auto_ptr<vector<unsigned char> > m_Data;
};

#endif /*RESOURCE_H_*/
