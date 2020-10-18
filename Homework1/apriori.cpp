#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;
typedef seconds timematric;
// typedef long long int  ll;
typedef int ll;
class aprori
{
private:
    /* Data private*/
    string input_file;
    string output_file;
    ll  support_per;
    ll  total_tranx;
    ll  total_item;
    ll  support;
    map<ll , set<set<ll > >  > freq_itemset;

    void print_tranx()
    {
        ifstream file(this->input_file.c_str());
        if(file.is_open())
        {
            string trx;
            while(getline(file, trx))
                cout<< trx << endl;
        }
        file.close();
    }

    /*candidate set generation*/
    void  candidate_gen(set<set<ll > >  &Fk_1, set<set<ll> > &Ck){
        for(set<set<ll > >::iterator it = Fk_1.begin(); it != Fk_1.end(); it++){
            set<set<ll > >::iterator jt = it;
            jt++;
            for(; jt != Fk_1.end(); jt++)
            {
                set<ll > temp;
                set<ll > left = *it, right = *jt;
                ll  small = *(left.rbegin());
                ll  large = *(right.rbegin());
                left.erase(small);
                right.erase(large);
                if(left == right)
                {
                    temp = left;
                    temp.insert(small);
                    temp.insert(large);
                    /* pruning the non freq itemset by subset checking*/
                    bool isbreak = false;
                    for(set<ll > ::iterator r = temp.begin(); r != temp.end(); r++)
                    {
                        ll  element = *r;
                        temp.erase(element);
                        if(Fk_1.find(temp) == Fk_1.end())
                        {
                                isbreak = true;
                                break;
                        }
                        temp.insert(element);
                    }
                    if(!isbreak)
                        Ck.insert(temp);
                }
                else
                {
                    /* as items are sorted in set once k-1 elements
                    are different no point of checking further */
                    break;
                }
            }
        }
   }
    void print(set< set<ll > > &Fk)
    {
        for(set<ll > i: Fk){
            for(int j: i){
                cout<<j<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
   void frequent_gen(set<set<ll > >  &Ck)
   {
       vector<set<ll> >  temp{Ck.begin(), Ck.end()};
       map<ll, ll > mp;
       ifstream file(this->input_file.c_str());
       if(file.is_open())
       {
           string trx;
           while(getline(file, trx))
           {
                stringstream itemstring(trx);
                ll  item;
                set<ll > transaction;
                while(itemstring>> item)
                {
                    transaction.insert(item);
                }
                for(ll i=0;i<temp.size();i++)
                // for(auto it = Ck.begin(); it != Ck.end(); it++)
                {
                    if(includes(transaction.begin(), transaction.end(), temp[i].begin(), temp[i].end()))
                    {
                        mp[i]++;
                    }
                }
           }
       }
       file.close();
        set< set<ll > > Fk;
        for(map<ll, ll>::iterator it = mp.begin() ; it != mp.end() ; it++)
        {
            if(it->second >= this->support)
            {
                Fk.insert(temp[it->first]);
            }

        }
        this->freq_itemset[Fk.begin()->size()] = Fk ;
    }


    void init()
    {
        /* init stores freq of all single itemset from database*/
        map<ll ,ll > mp; // itemset : freq
        ifstream file(this->input_file.c_str());
        if(file.is_open())
        {
            string trx;
            while(getline(file, trx))
            {
                stringstream itemstring(trx);
                ll  item ;
                while(itemstring>> item)
                {
                        mp[item]++;
                }
                this->total_tranx++;
            }

        }

        file.close();
        this->total_item = mp.size();
        this->support = ceil(( (float)support_per * (float)total_tranx )/100);

        set< set<ll > > Fk_one;
        for(auto it = mp.begin() ; it != mp.end() ; it++)
        {
            if(it->second >= this->support)
                Fk_one.insert({it->first});
        }
        this->freq_itemset[1] = Fk_one ;


        // /* test and comment-out */
        // cout<<"TEST N COMMENT \n";
        cout<<"total tranx " << this->total_tranx << endl;
        cout<<"total distinct items " << this->total_item << endl;
        cout<<"suppport " << this->support << endl;


    }
public:
    /* Methods public*/
    aprori(string input , string output , ll  support_per)
    {
        this->input_file = input;
        this->output_file = output;
        this->support_per = support_per;
        this->total_tranx = 0;
        this->total_item = 0;
    }

    /* Returns all the frequent itemset generated */
    map<ll , set<set<ll > >  > get_freq_itemset()
    {
        return this->freq_itemset;
    }

    void mine()
    {
        init();  // Initalize the 1 freq itemset from database

        ll  k = 1;
        cout<<k<<" freq itemset size "<< this->freq_itemset[1].size() <<endl;

        while(this->freq_itemset[k].size() != 0)
        {
            set< set<ll > > Ck_new;
            auto start = high_resolution_clock::now();
            candidate_gen(this->freq_itemset[k], Ck_new);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<timematric>(stop - start);
            cout << "Time taken by function candidate_gen: "<< duration.count() << " sec" << endl;
            start = high_resolution_clock::now();
            frequent_gen(Ck_new);
            stop = high_resolution_clock::now();
            duration = duration_cast<timematric>(stop - start);
            cout << "Time taken by function frequent_gen: "<< duration.count() << " sec" << endl;
            k++;
            // cout<<k<<" Candidate size "<< Ck_new.size() << endl;
            cout<<k<<" freq itemset size "<< this->freq_itemset[k].size() <<endl;
            // print(this->freq_itemset[k]);

        }
    }

    void outputFrequent()
    {
        ofstream fout;
        fout.open(this->output_file);
        for(auto it: this->freq_itemset)
        {
            for(auto s : it.second) // set
            {
                vector<string> ans;
                for(auto i : s) //set
                {
                    ans.push_back(to_string(i));
                }
                sort(ans.begin(), ans.end());
                for(string i : ans)
                    fout<< i << " ";
                fout << endl;
            }
        }
        fout.close();
    }

};


int  main(int  argc, char **argv)
{
    aprori * obj = new aprori(argv[1],argv[3],stoi(argv[2]));
    obj->mine();
    obj->outputFrequent();
}
