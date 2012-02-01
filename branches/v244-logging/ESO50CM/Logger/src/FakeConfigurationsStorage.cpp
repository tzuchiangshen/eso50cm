#include "FakeConfigurationsStorage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "SimpleIni.h"   /// this is for read the INI files
#define SI_SUPPORT_IOSTREAMS
using namespace std;

//                                                        non-utf8,non-Multikey,multiline
Configurations::Configurations(string configurationFile):ini(false, false, true) {
    
    string swroot;
    ifstream configFile;
    size_t found;
    if (getenv("SWROOT")!=NULL) 
        swroot=getenv("SWROOT");
    else 
        swroot="/eso50cm/SWROOT";
   filename=swroot+string("/config/")+configurationFile;
   ini.LoadFile(filename.c_str());
};


int Configurations::getIntValue(string section,string key) {
   int i;
   const char * pVal = ini.GetValue(section.c_str(),key.c_str());
   if (pVal==NULL)
       throw KeyNotFoundEx();
   stringstream ss(pVal);
   try {
       ss >> i;
   } catch (...) {
       throw InvalidValueEx();
   }
   return i;
};

string Configurations::getStringValue(string section,string key) {
   const char * pVal = ini.GetValue(section.c_str(),key.c_str());
   if (pVal==NULL)
       throw KeyNotFoundEx();
   return string(pVal);
};


double Configurations::getDoubleValue(string section,string key) {
   double i;
   const char * pVal = ini.GetValue(section.c_str(),key.c_str());
   if (pVal==NULL)
       throw KeyNotFoundEx();
   stringstream ss(pVal);
   try {
       ss >> i;
   } catch (...) {
       throw InvalidValueEx();
   }
   return i;
};

void Configurations::setIntValue(string section,string key,int value) {
  stringstream ss;
  ss << value;
  ini.SetValue(section.c_str(),key.c_str(),ss.str().c_str());
  ini.SaveFile(filename.c_str());
}
void Configurations::setStringValue(string section,string key,string value) {
  stringstream ss;
  ss << value;
  ini.SetValue(section.c_str(),key.c_str(),ss.str().c_str());
  ini.SaveFile(filename.c_str());
}

void Configurations::setDoubleValue(string section,string key,double value) {
  stringstream ss;
  ss << value;
  ini.SetValue(section.c_str(),key.c_str(),ss.str().c_str());
  ini.SaveFile(filename.c_str());
}

vector<string> Configurations::getKeys(string section)
{
    vector<string>  keyList;    
     // get all keys in a section
    CSimpleIniA::TNamesDepend keys;
    if (ini.GetAllKeys(section.c_str(), keys)){
        CSimpleIniA::TNamesDepend::const_iterator i;
        for (i = keys.begin(); i != keys.end(); ++i) 
        	keyList.push_back(string(i->pItem));
    }    
    return keyList;
}


