#include<bits/stdc++.h>
using namespace std;
class aprori
{
private:
    /* Data shall be private*/
    string input_file;
    string output_file;
    int support_per;
    int total_tranx;
    set<set<int>> old_freqItemset; 
public:
    /* Methods public*/ 
    aprori(string input , string output , int support_per)
    {
        this->input_file = input;
        this->output_file = output;
        this->support_per = support_per;
        this->total_tranx = 0;  
        this->old_freqItemset.clear();
    }

    void print_tranx()
    {
        ifstream file(this->input_file);
        if(file.is_open())
        {
            string trx;
            while(getline(file, trx))
                cout<< trx << endl;
        }
        file.close();
    }

    set<set<int>> gen_frequentItemset()
    {
        if(this->old_freqItemset.size() == 0)
        {
            /* No k-1 length freqItemset means we need to gen 1 len freqItemset*/
        }
    }

};


int main(int argc, char **argv)
{
    aprori * obj = new aprori(argv[1],argv[2],stoi(argv[3]));
    obj->print_tranx();
}
