#include<bits/stdc++.h>
using namespace std;

// a pair representing a frequent itemset with its freqeuncy
typedef pair<vector<int>,int> p;

//
typedef pair<int,int> pi;

//Function to convert the transaction read as string into a vector of int(items)
vector<int> stringToTransaction(string& s)
{
    vector<int> transaction;
    stringstream itemstring(s);
    int item;
    while(itemstring>> item)
    {
        transaction.push_back(item);
    }

    // for(int i = 0; i <= s.size(); i++)
    // {
    //     if(i == s.size() || s[i] == ' ')
    //     {
    //         transaction.push_back(stoi(curr));
    //         curr = "";
    //     }
    //     else curr += s[i];
    // }

    return transaction;
}

//The class for the FP/Prefix tree
class FPTree
{
    private:
        //Note: All the private data members have been declared after the private function definitions

        //Class for a node of the FP tree
        class Node
        {
            public:
                //the children of the current node corresponding to the next item in the prefix
                unordered_map<int,Node*> CH;

                //parent of the current node
                Node* parent;

                //number occurrences of the node in the current prefix
                int cnt;

                //the value of the order of the node, where order is defined on the
                //support values of individual items
                int ordVal;

                //the value of the current item in the prefix
                int item;

                //Constructor
                Node(int item, int ordVal)
                {
                    parent = NULL;
                    cnt = 1;
                    this->item = item;
                    this->ordVal = ordVal;
                }
        } *root; //create a root node pointer for the tree

        //Function to create an orcering on items based on their support/frequency values in the dataset
        //The larget the support/frequency of an item, the smaller its order value
        void createOrder(string filename)
        {
            ifstream fin;
            fin.open(filename);
            unordered_map<int,int> support;

            string s;
            while(!fin.eof())
            {
                getline(fin, s);
                if(s.empty()) continue;
                vector<int> transaction = stringToTransaction(s);
                for(auto item: transaction)
                    support[item]++;
            }
            fin.close();

            vector<pi> genOrd;
            for(auto it: support)
                genOrd.push_back(pi(it.second, it.first));

            sort(genOrd.begin(), genOrd.end(), greater<pi>());

            for(int i = 0; i < genOrd.size(); i++)
                ordering[genOrd[i].second] = i;
        }

        //order the items in a given transaction according to the ordering created previously
        void order(vector<int>& transaction)
        {
            vector<pi> temp;
            for(auto item: transaction)
                temp.push_back(pi(ordering[item], item));

            sort(temp.begin(), temp.end());
            for(int i = 0; i < transaction.size(); i++)
                transaction[i] = temp[i].second;
        }

        //insert a transaction/prefix into the tree
        void insert(vector<int>& transaction)
        {
           Node* node = root;

           for(auto item: transaction)
           {
               if(!node->CH.count(item))
               {
                   node->CH[item] = new Node(item, ordering[item]);
                   node->CH[item]->parent = node;
                   headerTable[item].insert(node->CH[item]);
               }
               else node->CH[item]->cnt++;

               node = node->CH[item];
           }
        }

        //An ordering defined on the nodes of the FP tree, with a node with smaller ordering (of the corresponding item)
        //than another node is considered as the larger of the two nodes.
        //Created for the heap to be used in the next function
        class NodeOrder
        {
            public:
                bool operator() (Node* a, Node* b)
                {
                    return (a->ordVal < b->ordVal);
                }
        };

        //A function that recursively builds conditional FPT's and checks for frequent itemsets
        void FPGrowth(unordered_set<Node*>& itemsForCondFPT, vector<int>& currItemSet)
        {
            //I'll explain the logic behind the function here.
            //The main concept is that given a list of nodes corresponding to a particular item
            //in the FP-tree (say 'E', from sir's example), how can we ensure that from that list of E's,
            //we can generate all the nodes corresponding to the distinct items in the conditional FP-tree
            //of E's? That is, if you look at the conditional FP-tree of E in sir's example, how can we
            //obtain all the nodes corresponding to D (or C, B, A) in that tree, at once?

            //To achieve that, the ordering previously declared on the items has been leveraged.
            //In conditional FP-tree for any item, all the other items will have a lower order than that item,
            //and the order decreases monotonically as we go from a lower level to a higher level in the tree.
            //If we use a priority-queue/heap, we can ensure that the element extracted from the heap is of
            //the highest order (out of all the other elements in the heap). So for example, if we start with a list of
            //E's, and try to move upwards in the tree, then we're going to process all the D's in the tree next,
            //because 'D' is the item with the next highest order in the conditional FP-tree of E.

            //Once we obtain all the D's, and the support of DE is greater than threshold
            //we can then recursively call the FPGrowth function to find the conditional subtree of DE, and so on.

            //This also works because it ensures that elements of higher order (i.e. lower in the tree), will always
            //be processed before elements of lower order (i.e. higher in the tree), i.e. w.r.t Sir's example, 'A'
            //in the conditional FP-tree of 'E', will always be processed after 'D', 'C' and 'B', and if the frequencies
            //of E's in their corresponding prefixes have been correctly propagated upwards, then by the time we reach
            //A, it's frequency in the conditional FP-tree of 'E' will have been updated to 2, as in sir's example.

            priority_queue<Node*, vector<Node*>, NodeOrder> PQ;

            //A map to keep track of visited nodes and once the processing for a node has been completed,
            //restore its original frequency (since the original frequency was replaced by the frequency in the
            //current conditional FP-tree)
            unordered_map<Node*,int> visitedNodes;

            //Initially, obtain all the parents of the current list of nodes (for example, the current list of E's
            //in sir's example).
            for(auto node: itemsForCondFPT)
            {
                if(node->parent != root)
                {
                    if(visitedNodes.count(node->parent))
                        node->parent->cnt += node->cnt;
                    else
                    {
                        visitedNodes[node->parent] = node->parent->cnt;
                        node->parent->cnt = node->cnt;

                        PQ.push(node->parent);
                    }
                }
            }

            //maintain the list of nodes of the same item extracted from the heap so far.
            unordered_set<Node*> nextItems;
            while(!PQ.empty())
            {
                Node* par = PQ.top();
                PQ.pop();

                nextItems.insert(par);

                //Insert the parent of the extracted node into the heap if not already there
                //If already there, then update the frequency of the corresponding item
                if(par->parent != root)
                {
                    if(visitedNodes.count(par->parent))
                        par->parent->cnt += par->cnt;
                    else
                    {
                        visitedNodes[par->parent] = par->parent->cnt;
                        par->parent->cnt = par->cnt;
                        PQ.push(par->parent);
                    }
                }

                //If the item corresponding to the next node in the heap is different or the heap is empty
                //recurse on the current set of items (say, all the D's in the conditional FP-tree of E)
                //if the frequency is greater than threshold.
                if(PQ.empty() || par->item != PQ.top()->item)
                {
                    int cnt = 0;
                    //calculate the frequency and check whether to recurse further
                    for(auto item: nextItems)
                        cnt += item->cnt;

                    if(cnt >= supportThresh)
                    {
                        currItemSet.push_back(par->item);
                        freqItemSets.push_back(currItemSet);

                        FPGrowth(nextItems, currItemSet);

                        currItemSet.pop_back();
                    }

                    //restore the original values of these processed nodes
                    for(auto item: nextItems)
                        item->cnt = visitedNodes[item];

                    //clear the list for the next item (say, 'C' after 'D')
                    nextItems.clear();
                }
            }
        }

        //All the private data members
        unordered_map<int,int> ordering;
        unordered_map<int,unordered_set<Node*>> headerTable;
        vector<vector<int>> freqItemSets; //The answer which contains the frequent itemsets
        int supportThresh;

    public:

        //FP tree constructor
        FPTree(string filename, int threshold)
        {
            int supportPerc = threshold;
            root = new Node(-1,-1);

            createOrder(filename);

            ifstream fin;
            fin.open(filename);

            string s;
            int numtranx = 0;
            while(!fin.eof())
            {
                getline(fin, s);
                if(s.empty()) continue;
                vector<int> transaction = stringToTransaction(s);

                order(transaction);
                insert(transaction);
                numtranx++;
            }

            supportThresh = ceil(float(numtranx * supportPerc)/100.0);
            fin.close();
        }

        bool search(vector<int>& transaction)
        {
            Node* node = root;

            for(auto item: transaction)
            {
                if(!node->CH.count(item)) return false;
                node = node->CH[item];
            }

            return true;
        }


        //Function to generate the frequent itemsets in from the FP-tree
        void generateFrequent()
        {
            for(auto it: headerTable)
            {
                int item = it.first;
                auto nodes = it.second;

                int sum = 0;
                for(auto item: nodes)
                    sum += item->cnt;

                if(sum < supportThresh) continue;
                vector<int> curr{item};
                freqItemSets.push_back(curr);
                FPGrowth(nodes, curr);
            }
        }

        vector<vector<int>> getFrequent()
        {
            return freqItemSets;
        }

        void outputFrequent(string outfile)
        {
            ofstream fout;
            fout.open(outfile);
            for(auto freq: freqItemSets)
            {
                set<string> S;
                for(auto i: freq)
                    S.insert(to_string(i));
                for(auto i: S)
                    fout << i << " ";
                fout << endl;
                // cout << "Frequency: " << freq.second << endl << endl;
            }
        }

        void printOrdering()
        {
            for(auto it: ordering)
                cout << "Item: " << it.first << " Order:" << it.second << endl;
        }
};

int main(int argc, char* argv[])
{
    string filename = argv[1], outfile = argv[3];
    int support = stoi(argv[2]);
    time_t start, end, ast, aen;
    time(&ast);
    time(&start);
    FPTree *T = new FPTree(filename, support);
    time(&end);
    cout << "Tree created!" << endl;
    cout << "Time taken to create the tree is " << difftime(end,start) << " seconds." << endl;
    time(&start);
    T->generateFrequent();
    time(&end);
    cout << "Time taken to generate frequent itemsets is " << difftime(end,start) << " seconds." << endl;
    time(&start);
    T->outputFrequent(outfile);
    time(&end);
    time(&aen);
    cout << "Time taken write output to file is " << difftime(end,start) <<  " seconds." << endl;
    cout << "Total time taken is " << difftime(aen, ast) << " seconds." << endl;

    return 0;
}
