#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "properties.h"

using namespace std;

map<string,string> properties;
map<string, string>::iterator finder;
int loadProperties(string);
void logProperties();

//int main()
//{
//	if(initializeProperties(PROP_FILE_PATH) == 0)
//	{
//		cout<<findPropertyValueByKey(VIDEO_DIR)<<endl;
//		cout<<findPropertyValueByKey(PROP_DELAY)<<endl;
//	}
//	return 0;
//}

int initializeProperties(string propFilePath)
{
	if(loadProperties(propFilePath) == 0)
	{
		logProperties();
		return 0;
	}
	else
	{
		return -1;
	}
}

string findPropertyValueByKey(string key)
{
	finder = properties.find(key);
	return finder->second;
}

int loadProperties(string propFilePath)
{
	cout << "--Properties initializing..." << endl;
		map<string,string>::iterator it = properties.begin();
		ifstream propertiesFile;
		propertiesFile.open(propFilePath.c_str());
		if (propertiesFile.is_open())
		{
			string line;
			while (getline(propertiesFile, line))
			{
				string output,name,value,firstChar;
				output=line;
				if(!output.empty())
				{
					firstChar = output.at(0);
					if(firstChar.compare("#"))
					{
						size_t found = output.find("=");
						name = output.substr(0,found);
						value = output.substr(found+1,output.size());
						properties.insert (it, pair<string,string>(name,value));
					}
				}
				line="";
			 }
			return 0;
		}
		else
		{
			cout<<"Property File Error"<<endl;
			return -1;
		}
		return 0;
}

void logProperties()
{
	vector<string> v;
	for(map<string,string>::iterator it = properties.begin(); it != properties.end(); ++it)
	{
	  v.push_back(it->first);
	  v.push_back(it->second);
	  cout << "--Key:"<< it->first.c_str() << ", Value:"<<it->second.c_str()<<endl;
	}
	cout << "--Properties initialized!" << endl;
}
