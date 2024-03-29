#ifndef __FAKE_CONFIGURATION_STORAGE_H_
#define __FAKE_CONFIGURATION_STORAGE_H_

#include "SimpleIni.h"
#include <string>
#include <vector>
using namespace std;

/**
* This is a Basic Configuration Storage Interface, should be virtual pure to be a better interface, but
* since it's fake and its only for made the setup configuration independent until you get a 
* a working configurations storage system, I create with empty method to make things easy
**/

// define 3 exceptions for ConfigurationsStorage
class KeyNotFoundEx {};
class SectionNotFoundEx {};
class InvalidValueEx {};   // This should be raised if the key is tried to be read using the wrong format

class ConfigurationsStorage {
  public:
    // getters
    //   throws SectionNotFound Exception or KeyNotFound Exception
    virtual int getIntValue(string section,string key) = 0;
    virtual string getStringValue(string section,string key) = 0;
    virtual double getDoubleValue(string section,string key) = 0;
    virtual vector<int> getIntVector(string section,string keys)  = 0;   
    virtual vector<string> getStringVector(string section,string key) = 0;
    virtual vector<double> getDoubleVector(string section,string key) = 0;
    virtual vector<string> getKeys(string section) = 0;
    virtual vector<string> getSections() = 0;
    
    // setters
    //   throws SectionNotFound Exception or KeyNotFound Exception
    virtual void setIntValue(string section,string key,int value) = 0;
    virtual void setStringValue(string section,string key,string value)  = 0;
    virtual void setDoubleValue(string section,string key,double value) = 0;
    virtual void setIntVector(string section,string key, vector<int> values) = 0;
    virtual void setStringVector(string section,string key,vector<string> values) = 0;
    virtual void setDoubleVector(string section,string key,vector<double> values) = 0;
    
    
};

/** 
* This class partially implement ConfigurationsStorage interface
* in particular, it has the logic to manage it in a INI file, but we're not interesed in vectors...
**/
class Configurations:public ConfigurationsStorage {
    CSimpleIniA ini;
    string filename;
  public:
    Configurations(string configurationFile);
    // getters
    //   throws SectionNotFound Exception or KeyNotFound Exception
    virtual int getIntValue(string section,string key); 
    virtual string getStringValue(string section,string key);
    virtual double getDoubleValue(string section,string key);
    virtual vector<int> getIntVector(string section,string keys)   { vector<int> a; return a; }; // Dummy implementation                         
    virtual vector<string> getStringVector(string section,string key)  { vector<string> a; return a; }; // Dummy implementation 
    virtual vector<double> getDoubleVector(string section,string key)  { vector<double> a; return a; }; // Dummy implementation 
    virtual vector<string> getKeys(string section);
    virtual vector<string> getSections()  { vector<string> a; return a; }; // Dummy implementation ;

    // setters
    //   throws SectionNotFound Exception or KeyNotFound Exception
    virtual void setIntValue(string section,string key,int value);
    virtual void setStringValue(string section,string key,string value);
    virtual void setDoubleValue(string section,string key,double value);
    virtual void setIntVector(string section,string key, vector<int> values) {};  // Not implemented
    virtual void setStringVector(string section,string key,vector<string> values)  {};  // Not implemented
    virtual void setDoubleVector(string section,string key,vector<double> values)  {};  // Not implemented

};
#endif
