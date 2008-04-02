#include "Resource.h"

Resource::Resource(const string identifier, const vector<unsigned char>& data) :
				   m_Data(new vector<unsigned char>(data))
{
	m_Identifier = identifier;
}

Resource::~Resource()
{
}

string Resource::identifier() const
{
	return m_Identifier;
}

const vector<unsigned char>* Resource::data() const
{
	return m_Data.get();
}
