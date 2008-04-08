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
	
	string identifier() const;
	const vector<unsigned char> * data() const;
	
private:
	string m_Identifier;
	const vector<unsigned char> m_Data;
};

#endif /*RESOURCE_H_*/
