#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <time.h>

using namespace std;

/****************************** STRUCTS DECLARATION *******************************/
typedef struct node
{
    const char* string;
    node* next_Node;

}Node;

typedef struct header
{
    Node* first_Node;
    int size;

}Header;

typedef struct wrong_word
{
    int line;
    int column;
    string word;
}Wrong_word;

/****************************** STRUCTS DECLARATION *******************************/

const int buckets = 25609; //number of buckets used in hash table
int cont_correct = 0; //number of correct words
int cont_wrong=0; //number of wrong words
float total_time; //time calculated in proccess (used to print results)
clock_t initial_time, final_time; //time = final - initial
string words_list[307860]; //copy of dictionary words (used to generate hash table)
Header table[buckets]; //array of Headers that will save a linked list and the size
std::vector<Wrong_word> vector_wrong; //vector used to save the wrong words found in text



/****************************** HASH FUNCTIONS *******************************/
 int hash_lose(const char *string)
    {
    unsigned int hash = 0;
    int c;

    while (c = *string++)
        hash += c;

     hash = hash % buckets;
    return hash;
    }

 int hash_sdbm(const char* string)
 {

        unsigned long int hash = 0;
        int c;

        while (c = *string++)
            hash = c + (hash << 6) + (hash << 16) - hash;

        hash = hash % buckets;
        return hash;

 }
/****
 *Author: Daniel J. Bernstein

 * hash_djb2(): This is one of the best
* known hash functions for strings. Because it is both computed very
* fast and distributes very well.
*
* The magic of number 33, i.e. why it works better than many other
* constants, prime or not, has never been adequately explained by
* anyone.
 *
 * Parameters: a string to be converted (hashed)
 *
 * Return: A int number that represents the string hashed
 *
 ****/
int hash_djb2(const char* string)
{
    unsigned long int hash = 5381;
    int c;

        while(c = *string++)
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    hash = hash % buckets; //the buckets define the range
    return hash;
}



/****
 *Author: Diogo Dantas and Higor Anjos

 * to_lowerCase(): This function is used to convert a string to its equivalent lowercase form
 *
 * Parameters: a string to be converted to lowercase
 *
 * Return: a string in lowercase form
 *
 ****/

string to_lowerCase(string word)
{
    int i=0;

                if(word[0]!=tolower(word[0])) { //if the word starts with lowercase, isn't necessary to convert
                        while(word[i]!='\0'){
                            word[i]=tolower(word[i]); //converts to lower
                            i++;
                        }
                }

                return word;
}

/****
 *Author: Diogo Dantas and Higor Anjos

 * preprocessing(): This function does all preprocessing needed to use the spell-checker
 * creates the array of headers, the array of the dictionary words and generates the table
 *
 * Parameters: none
 *
 * Return: none
 *
 ****/
void preprocessing(){

    ifstream file;
    file.open ("dictionary.txt"); //opening file
    if (!file.is_open()) return; //if something gets wrong...

/******* CREATING AND PREPARING THE ARRAY ****/

    for (int i = 0; i < buckets; ++i) //setting all to 0 or NULL
    {
        Header * tmp = new Header;
        table[i] = *tmp;
        table[i].size = 0;
        table[i].first_Node = NULL;
    }
/******* CREATING AND PREPARING THE ARRAY ****/

/******* CONVERTING TO LOWERCASE AND SAVING IN THE ARRAY*****/

        string word;
        int j=0;

    while (file >> word) //jumps from word to word
        {

            words_list[j++] = to_lowerCase(word);  //converting


        }

file.close();

/******* CONVERTING TO LOWERCASE AND SAVING IN THE ARRAY*****/

/******* APPLYING HASH AND GENERATING TABLE *****/
    for (int i = 0; i < j; ++i)
    {
            int hashed = hash_sdbm(words_list[i].c_str()); //using the dictionary words to hash
            table[hashed].size++; //increasing the size of the linked list of this bucket
            Node* tmp = new Node;
                    if (!table[hashed].first_Node) //if first
                    {
                        table[hashed].first_Node = tmp;
                        table[hashed].first_Node->next_Node = NULL;
                        table[hashed].first_Node->string = words_list[i].c_str();

                    } else {
                        tmp->string = words_list[i].c_str();
                        tmp->next_Node =  table[hashed].first_Node;
                        table[hashed].first_Node = tmp;

                    }
    }
/******* APPLYING HASH AND GENERATING TABLE *****/

} //END OF preprocessing()



/****
 *Author: Diogo Dantas and Higor Anjos

 * compare(): This function sweeps a linked list comparing its nodes to a string
 *
 * Parameters: string to be compared
 *
 * Return: a boolean indicating if is equal or not
 *
 ****/

bool compare(string word)
{
    word = to_lowerCase(word);
    int hashed = hash_sdbm(word.c_str());


    Node* aux = table[hashed].first_Node;
        while(aux) //while is not in the end of the list
        {
            if (word == aux->string)
                return true;

            aux=aux->next_Node;
        }

    return false;

}



/****
 *Author: Diogo Dantas and Higor Anjos based on DVC (Diablo Valley College) parsing algorithm

 * parse_text(): This function sweeps an linked list comparing its nodes
 *
 * Parameters: none
 *
 * Return: none
 *
 ****/

/************** PARSING TEXT *******************************/
void parse_text(){
    const int CHARS_PARAGRAPH = 5250; //5250 characters per line * 7 lines (paragraph)
    const int WORDS_LINE = 1000; //1000 words in a line
    const char* const DELIMITER = "  ,.:?;!'\""; //these are ignorated as words
    int line_count = -1; //used to save the quantity of lines
  
  ifstream file;
  file.open("teste.txt");
  
  if (!file.good())
    cout<<"problem loading file"<<endl;

  while (!file.eof())
  {
    line_count++;
    // read an entire line into memory
    char buf[CHARS_PARAGRAPH];
    file.getline(buf, CHARS_PARAGRAPH);

    // parse the line into DELIMITER
    int n = 0;

    // array to store pointers of the words in buf
    const char* words[WORDS_LINE] = {}; // initialize to 0

    // parse the line
    words[0] = strtok(buf, DELIMITER); // first words
    if (words[0]) // zero if line is blank
    {
      for (n = 1; n < WORDS_LINE; n++)
      {
        words[n] = strtok(0, DELIMITER); // subsequent words
        if (!words[n]) break; // no more words
      }
    }

    // process (print) the words
    for (int i = 0; i < n; i++){ // n = number of words

           if(!compare(words[i])){
                cont_correct++; //if is equal just increase
            }else{
                cont_wrong++;
                Wrong_word * tmp = new Wrong_word;
                tmp->line = line_count; //in wich line the words is
                tmp->column = i; //in wich column the words is
                tmp->word = words[i];
                vector_wrong.push_back(*tmp); //adding to vector
                delete tmp;
            }

       }

  }

    file.close();


}
/************** PARSING TEXT *******************************/



/****
 *Author: Diogo Dantas and Higor Anjos

 * print_results(): This function prints the results of the program,
 * such as number of wrong words and time
 *
 * Parameters: none
 *
 * Return: none
 *
 ****/
/************** PRINTING RESULTS ******************************/
void print_results(){
cout<<endl;
cout<<"Número total de palavras do texto: "<<cont_correct+cont_wrong<<endl;
cout<<"time de verificação: "<<total_time<<"ms."<<endl;
cout<<"Número de palavras que falharam no spell check: "<<cont_wrong<<endl;
cout<<"Lista de palavras que falharam no spell check: "<<endl;
cout<<endl;
cout<<"Linha  -  Coluna  : Palavra "<<endl;
cout<<"------------------------------------------"<<endl;

for (int i = 0; i < cont_wrong; ++i)
    {
        cout<<vector_wrong[i].line+1<<" - "<<vector_wrong[i].column+1<<" : "<<vector_wrong[i].word<<endl;
    }

}
/************** PRINTING RESULTS **************************************/

/****
 *Author: Diogo Dantas and Higor Anjos

 * print_stats(): This function prints the statistics of the program. If the
 * hash function distriuted well and so on...
 *
 * Parameters: none
 *
 * Return: none
 *
 ****/
/************** PRINTING STATS ******************************/
void print_stats(){
char option;
int indexb, indexw, greater=0, lower=0, equal=0;
int average=307860/buckets;
int worst=0;
int best = table[0].size; //used for comparison
cout<<endl;


for (int i = 0; i < buckets; ++i)
{
	if(table[i].size > worst){
		worst = table[i].size;
		indexw = i;
	}
	
	if(table[i].size < best){
		best = table[i].size;
		indexb = i;
	}

	if(table[i].size > average){
		greater++;
	} else if(table[i].size < average){
		lower++;
	} else {
		equal++;
	}

}
cout<<"SOME STATISTICS"<<endl;
cout<<"-----------------------"<<endl;
cout<<"Number of buckets: "<<buckets<<endl;
cout<<"Average collision: "<<average<<" collisions"<<endl;
cout<<"Buckets above average: "<<greater<<endl;
cout<<"Buckets below average: "<<lower<<endl;
cout<<"Buckets equal average: "<<equal<<endl<<endl;

cout<<"'Best Bucket'"<<endl;
cout<<"Number ["<<indexb<<"]"<<endl;
cout<<best<<" collisions"<<endl<<endl;
cout<<"'Worst Bucket'"<<endl;
cout<<"Number ["<<indexw<<"]"<<endl;
cout<<worst<<" collisions"<<endl<<endl;
cout<<"-----------------------"<<endl;


cout<<endl;
cout<<"Want to print buckets list? [y/n]"<<endl;
cin>>option;

if (option=='y')
{
	for (int i = 0; i < buckets; ++i)
	{
		cout<<"Bucket "<<i<<" - "<<table[i].size<<" buckets"<<endl;
	} 

} else {
	cout << "Thanks for using this program!"<<endl;
}




}
/************** PRINTING STATS ******************************/

int  main(int argc, char const *argv[])
{
    preprocessing();

    initial_time = clock();
    parse_text();
    final_time = clock();

    // FINAL TIME - INITIAL TIME

    total_time = ((float)(final_time - initial_time)/CLOCKS_PER_SEC)*1000;
    
    //print_results();
    print_stats();
    return 0;
}
