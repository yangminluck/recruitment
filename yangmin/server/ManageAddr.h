#ifndef _ManagerAddr_Head
#define _ManagerAddr_Head

const char separator=',';
struct entryaddr  //address entry data structure
{
    entryaddr *next;
    string name;
    string mobile;
    string address;
};
/*
class name:ManageAddr
funtion   :manage the address entries
interfaces:void AddEntry();
           void SearchEntry();
           void RemoveEntry();
*/
class ManageAddr
{
    private:
    entryaddr *data; //store temporary changed entries
    string filename; //a related file
    bool filechanged;// the state of file

    entryaddr* ReadFile();
    void WriteFile(entryaddr*,int);
    entryaddr* doremove(entryaddr*,string&,int ,int&);
    bool matching(string &dest,string &str);

    public:
    ManageAddr(char *s):data(NULL),filename(s),filechanged(false){}
    ~ManageAddr();

    int AddEntry(CommUnit *,const string &);
    int SearchEntry(CommUnit *,const string &);
    int RemoveEntry(CommUnit *,const string &);
};
#endif
