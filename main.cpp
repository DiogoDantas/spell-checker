#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <time.h>

using namespace std;

const int buckets = 25609;
int cont_correct = 0;
int cont_wrong=0;
float tempo;
clock_t t_ini, t_fim;
string lista_palavras[307860];


/****************************** DECLARAÇÃO DE ESTRUTURAS *******************************/
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

Header table[buckets];
std::vector<Wrong_word> vector_wrong;

/****************************** DECLARAÇÃO DE ESTRUTURAS *******************************/

/****************************** HASHING FUNCTION *******************************/

int hash_djb2(const char* string)
{
    unsigned long int hash = 5381;
    int c;

        while(c = *string++)
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    hash = hash % buckets;
    return hash;
}

/****************************** HASHING FUNCTION *******************************/

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

void preprocessing(){

    ifstream file;
    file.open ("dictionary.txt");
    if (!file.is_open()) return;

/******* CRIANDO E PREPARANDO O ARRAY ****/



    for (int i = 0; i < buckets; ++i)
    {
        Header * tmp = new Header;
        table[i] = *tmp;
        table[i].size = 0;
        table[i].first_Node = NULL;
    }
/******* CRIANDO E PREPARANDO O ARRAY ****/

/******* CONVERTENDO PARA LOWERCASE E SALVANDO EM UM ARRAY*****/

        string word;
        int j=0;

    while (file >> word)
        {

            lista_palavras[j++] = to_lowerCase(word);


        }

file.close();

/******* CONVERTENDO PARA LOWERCASE E SALVANDO EM UM ARRAY *****/

/******* APLICANDO O HASH E GERANDO A TABLE *****/
for (int i = 0; i < j; ++i)
{
        int hashed = hash_djb2(lista_palavras[i].c_str());
        table[hashed].size++;
        Node* tmp = new Node;
                if (!table[hashed].first_Node) //if first
                {
                    table[hashed].first_Node = tmp;
                    table[hashed].first_Node->next_Node = NULL;
                    table[hashed].first_Node->string = lista_palavras[i].c_str();

                } else {
                    tmp->string = lista_palavras[i].c_str();
                    tmp->next_Node =  table[hashed].first_Node;
                    table[hashed].first_Node = tmp;

                }
}
/******* APLICANDO O HASH E GERANDO A TABLE *****/
}

bool compare(string word)
{
    word = to_lowerCase(word);
    int hashed = hash_djb2(word.c_str());


    Node* aux = table[hashed].first_Node;
        while(aux)
        {
            if (word == aux->string)
                return true;

            aux=aux->next_Node;
        }

    return false;

}

/************** PARSING TEXT *******************************/
void parse_text(){
    const int MAX_CHARS_PER_LINE = 512;
    const int MAX_TOKENS_PER_LINE = 100;
    const char* const DELIMITER = "  ,.:?;!'\"";
    int line_count = -1;

    // create a file-reading object
  ifstream file;
  file.open("teste.txt"); // open a file
  if (!file.good())
    cout<<"problem loading file"<<endl; // exit if file not found

  // read each line of the file
  while (!file.eof())
  {
    line_count++;
    // read an entire line into memory
    char buf[MAX_CHARS_PER_LINE];
    file.getline(buf, MAX_CHARS_PER_LINE);

    // parse the line into blank-delimited tokens
    int n = 0; // a for-loop index

    // array to store memory addresses of the tokens in buf
    const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0

    // parse the line
    token[0] = strtok(buf, DELIMITER); // first token
    if (token[0]) // zero if line is blank
    {
      for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
      {
        token[n] = strtok(0, DELIMITER); // subsequent tokens
        if (!token[n]) break; // no more tokens
      }
    }

    // process (print) the tokens
    for (int i = 0; i < n; i++){ // n = #of tokens

           if(compare(token[i])){
                cont_correct++;
            }else{
                cont_wrong++;
                Wrong_word * tmp = new Wrong_word;
                tmp->line = line_count;
                tmp->column = i;
                tmp->word = token[i];
                vector_wrong.push_back(*tmp);
                delete tmp;
            }

       }

  }

    file.close();


}
/************** PARSING TEXT *******************************/

/************** PRINTING RESULTS ******************************/
void print_results(){
cout<<endl;
cout<<"Número total de palavras do texto: "<<cont_correct+cont_wrong<<endl;
cout<<"Tempo de verificação: "<<tempo<<"ms."<<endl;
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

int  main(int argc, char const *argv[])
{
    preprocessing();

    t_ini = clock();
    parse_text();
    t_fim = clock();

    tempo = ((float)(t_fim - t_ini)/CLOCKS_PER_SEC)*1000;

    print_results();
    return 0;
}
